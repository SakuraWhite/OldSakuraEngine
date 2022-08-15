#include "RenderingTextureResourcesUpdate.h"


//封装字符串 
const wchar_t DDS[] = L".dds";
const wchar_t Asset[] = L"/Asset/";
const wchar_t Project[] = L"/Project/";

void FRenderingTextureResourcesUpdate::LoadTextureResources(const wstring& InFilename)
{
	//创建全局唯一指针
	unique_ptr<FRenderingTexture> MyTexture = std::make_unique<FRenderingTexture>();
	//记录文件名
	MyTexture->Filename = InFilename;


	//记录贴图名
	//输入一个文件的路径比如 c:/a/b/c/xxx.jpg 然后提取文件名xxx.jpg
	wchar_t Filename[1024] = { 0 };//C语言
	get_path_clean_filename_w(Filename, MyTexture->Filename.c_str());//转成C语言的string
	wremove_string_start(Filename, DDS);//移除文件名后缀的.dds 

	MyTexture->Name = Filename;//记录贴图名字

	//读取DDS贴图数据
	CreateDDSTextureFromFile12(			//创建DDS贴图(支持dx12)
		GetD3dDevice().Get(),			//驱动 指向D3D驱动设备
		GetGraphicsCommandList().Get(), //命令队列(列表)，用于提交GPU命令
		MyTexture->Filename.c_str(),	//给定当前文件的路径以及文件名 记得转换类型转为宽字符
		MyTexture->Data,				//文件数据 资源
		MyTexture->UploadBuffer);		//上传缓冲区

	//自动分步ID   通过容器中的数量 来分配纹理贴图ID有多少
	MyTexture->RenderingTextureID = TexturesMapping.size();


	//拼接字符串 "Texture'Asset/Texture/xxx.xxx'"
	//给贴图的资源路径搜索赋值
	wchar_t AssetFilenameBuff[1024] = { 0 };//转换为宽字符
	{
		wchar_t AssetFilenameBuff1[1024] = { 0 };//资源名字缓冲区1
		wchar_t* AssetFilenamePtr = const_cast<wchar_t*>(MyTexture->Filename.c_str());//获取资源指针

		int Pos = wfind_string(AssetFilenamePtr, Asset);//寻找资源中的字符串 L"/Asset/"

		//切割字符串						资源指针				资源名字缓冲  在位置切割    字符串长度
		wchar_t* Value = wstring_mid(AssetFilenamePtr, AssetFilenameBuff1, Pos, wcslen(AssetFilenamePtr));//切割后变成了'Asset/Texture/xxx.xxx'

		//开始替换字符串
		wreplace_string_inline(Value, Asset, Project);//从切割字符串的位置开始 替换L"/Asset/"  L"/Project/"
		//替换第二层
		wreplace_string_inline(Value, DDS, (L"." + MyTexture->Name).c_str());//把L".dds"与文件名进行合并 这样只要搜XXX.XXX 不用XXX.dds

		//打印字符串
		wget_printf_s(AssetFilenameBuff, L"Texture'%s'", Value);//把字符串结构变为"Texture'Asset/Texture/xxx.xxx'" 
	}
	//讲上面拼接好的字符串拷贝给资源名
	MyTexture->AssetFilename = AssetFilenameBuff;

	//将贴图名放入到容器里
	TexturesMapping[MyTexture->Name] = std::move(MyTexture);//因为是唯一指针  只能通过move来移动贴图名
}

void FRenderingTextureResourcesUpdate::BuildTextureConstantBuffer(ID3D12DescriptorHeap* InHeap, int Offset)
{
	//描述符偏移量
	UINT DescriptorOffset = GetD3dDevice()->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
	//DX12 CPU描述符句柄
	CD3DX12_CPU_DESCRIPTOR_HANDLE Handle(InHeap->GetCPUDescriptorHandleForHeapStart());
	//描述符句柄偏移
	Handle.Offset(Offset, DescriptorOffset);


	//构建Shader资源视图描述 用来描述当前资源是什么样的 格式是什么等等信息
	D3D12_SHADER_RESOURCE_VIEW_DESC ShaderResourceViewDesc = {};
	ShaderResourceViewDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;//默认着色器组件映射  采样图片RGB像素时可以交换分量 比如RG交换 或者GB交换等
	ShaderResourceViewDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;//描述当前shader视图格式信息
	ShaderResourceViewDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;//描述加载资源的维度 这里是2D纹理格式
	ShaderResourceViewDesc.Texture2D.MostDetailedMip = 0;//当前mipmap的索引 
	ShaderResourceViewDesc.Texture2D.MipLevels = 1;      //mipmap的层级数量
	ShaderResourceViewDesc.Texture2D.ResourceMinLODClamp = 0.f;//mipmap层级限制 指定最小的mipmap层级 设置为0可以访问所有mipmap层级

	for (auto &Tmp:TexturesMapping)//遍历资源
	{
		ShaderResourceViewDesc.Format = Tmp.second->Data->GetDesc().Format;//通过资源数据获取描述  再通过描述获取格式
		ShaderResourceViewDesc.Texture2D.MipLevels = Tmp.second->Data->GetDesc().MipLevels;//通过资源数据获取描述  从资源描述获取mipmap的层级
		GetD3dDevice()->CreateShaderResourceView(//通过D3D驱动创建shader资源视图
			Tmp.second->Data.Get(),		//获得资源数据
			&ShaderResourceViewDesc,	//资源描述 
			Handle);					//地址偏移
		Handle.Offset(1, DescriptorOffset);//每次创建一个shader资源视图 地址继续偏移1个位置
	}



}

std::unique_ptr<FRenderingTexture>* FRenderingTextureResourcesUpdate::FindRenderingTexture(const std::string& InKey)
{
	if (!InKey.empty())//判断路径是否为空  如果不为空
	{

		//第一层寻找路径
		//将路径转化宽字符
		const char* InString = InKey.c_str();
		wchar_t TexturePath[1024] = { 0 };
		char_to_wchar_t(TexturePath, 1024, InString); //转换换字符 输入宽字符  宽字符数量 当前字符串

		if (TexturesMapping.find(TexturePath) != TexturesMapping.end())//通过路径kay判断是否能找到结构    如果等于代表没有找到
		{
			//如果找到结构中贴图路径 则返回  （唯一指针）
			return &TexturesMapping[TexturePath];//key索引路径

		}
		else
		{
			//如果没有找到结构中的贴图路径  则采用另外一种方式 遍历
			for (auto& Tmp : TexturesMapping)//遍历 比较低效
			{
				if (Tmp.second->Filename == TexturePath)//判断文件名
				{
					//返回路径名
					return &Tmp.second;
				}

				if (Tmp.second->AssetFilename == TexturePath)//判断资源路径
				{
					//返回路径名
					return &Tmp.second;
				}

				if (Tmp.second->SimpleAssetFilename == TexturePath)//判断简单的资源路径
				{
					//返回路径名
					return &Tmp.second;
				}
			}
		}
	}

	return NULL;
}
