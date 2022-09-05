#pragma once
#include "../../EngineMinimal.h"
#include "../../Interface/DirectXDeviceInterfece.h"

struct FRenderingTexture //渲染纹理描述结构
{
	FRenderingTexture()
		:RenderingTextureID(0)//初始化材质ID
	{}

	ComPtr<ID3D12Resource> UploadBuffer;//定义上传堆
	ComPtr<ID3D12Resource> Data;//创建的资源

	wstring Name;//贴图名称  'xxx.dds'
	wstring Filename;//贴图路径
	wstring AssetFilename;//资源路径	贴图的资源路径 Texture'Asset/Texture/xxx.xxx'
	wstring SimpleAssetFilename;//简单的资源路径	直接是资源路径 Asset/Texture/xxx.dds

	UINT RenderingTextureID;//纹理贴图ID

};

//创建常量缓冲区结构(贴图)  渲染纹理资源更新
class FRenderingTextureResourcesUpdate 
	:public enable_shared_from_this<FRenderingTextureResourcesUpdate>
	, public IDirectXDeviceInterfece//渲染内容的接口

{

public:
	FRenderingTextureResourcesUpdate();


	//加载贴图资源				从磁盘的某个位置导入贴图资源(路径)
	void LoadTextureResources(const wstring& InFilename);
	//构建纹理常数缓冲区							堆的接口           地址偏移
	void BuildTextureConstantBuffer(ID3D12DescriptorHeap* InHeap, int Offset = 0);
public:
	//构建当前的参数 纹理贴图类型参数(Shader资源视图描述里的参数)
	void BuildParam(); 

	//根据类型进行重置纹理贴图				      具体的传入的贴图资源
	void ResetTextureByType(std::unique_ptr<FRenderingTexture>* InTexture);

	//设置描述加载资源的维度(按类型重置纹理)  区分是2D还是立方体贴图 还是3D贴图
	void SetViewDimension(D3D12_SRV_DIMENSION InNewDimension);

public:
	//获取纹理贴图数量
	FORCEINLINE UINT Size() const { return TexturesMapping.size(); }

	//获取查找渲染纹理路径(字符串)  唯一指针
	std::unique_ptr<FRenderingTexture>* FindRenderingTexture(const std::string& InKey);
protected:
	//   有序map		   贴图叫什么名字			具体的传入的贴图资源			贴图映射
	std::map<std::wstring, std::unique_ptr<FRenderingTexture>> TexturesMapping;

	//构建Shader资源视图描述 用来描述当前资源是什么样的 格式是什么等等信息
	D3D12_SHADER_RESOURCE_VIEW_DESC ShaderResourceViewDesc;
};