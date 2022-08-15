#include "OpaqueRenderLayer.h"
#include "../../Geometry/GeometryMap.h"
#include "../../PipelineState/DirectXPipelineState.h"

FOpaqueRenderLayer::FOpaqueRenderLayer()
{
	//设定不透明渲染层级
	RenderPriority = 2000;
}

void FOpaqueRenderLayer::Draw(float DeltaTime)
{
	Super::Draw(DeltaTime);
}

void FOpaqueRenderLayer::BuildShader()
{
	//构建Shader HLSL

	char TextureNumBuff[10] = { 0 };//申请一个纹理贴图数量缓冲区
	D3D_SHADER_MACRO ShaderMacro[] =  //shader宏 容器 储存贴图
	{
		"TEXTURE2D_MAP_NUM", //定义当前的宏 这里是贴图的数量
		_itoa(GeometryMap->GetDrawTextureResourcesNumber(),TextureNumBuff,10), //将纹理贴图缓冲区里的贴图数量取出然后将整形转换为字符串(10进制) 然后输入给容器里 动态的输入
		NULL,NULL,   //这个容器结尾必须是两个null
	};

	VertexShader.BuildShaders(//构建顶点着色器
		L"../SakuraEngine/Shader/HelloHLSL.hlsl",//寻找外部的hlsl着色器语言文件
		"VertexShaderMain",//入口函数名
		"vs_5_1",//使用的版本
		ShaderMacro);//指定容器
	PixelShader.BuildShaders(			//构建像素着色器
		L"../SakuraEngine/Shader/HelloHLSL.hlsl",//寻找外部的hlsl着色器语言文件
		"PixelShaderMain", //入口函数名
		"ps_5_1",//构建像素着色器
		ShaderMacro);//指定容器
	DirectXPipelineState->BindShader(VertexShader, PixelShader);//将Shader绑定在渲染管线状态上

	//输入布局 构建层 位置颜色法线等等信息
	InputElementDesc =
	{
		//构建位置信息
		//指定位置 元素语义  指定格式占12个字节    插入槽  内存中的偏移		 输入槽类			当前的位置
		{"POSITION",0,DXGI_FORMAT_R32G32B32_FLOAT,  0 , 0, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,0},

		//构建颜色信息
		//指定颜色 元素语义  指定格式占16个字节	插入槽 内存中的偏移	 输入槽类					当前的位置
		{"COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 12, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0},

		//构建法线信息
		//指定法线 元素语义  指定格式				插入槽 内存中的偏移	 输入槽类					当前的位置
		{"NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 28, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0},

		//构建切线信息
		//指定切线
		{"TANGENT", 0,DXGI_FORMAT_R32G32B32_FLOAT, 0 , 40,  D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0},

		//构建UV坐标
		//指定UV坐标		指定R32位G32位格式			内存中偏移52字节	
		{"TEXCOORD", 0,DXGI_FORMAT_R32G32_FLOAT, 0 , 52,  D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0}
	};
	DirectXPipelineState->BindInputLayout(InputElementDesc.data(), InputElementDesc.size());////将输入布局绑定在渲染管线状态上

}

void FOpaqueRenderLayer::BuildPSO()
{
	//构建PSO管线状态对象
	//管线状态					线框模式
	DirectXPipelineState->Build(Wireframe);
	//设置模型填充模式(线框/灰模)
	DirectXPipelineState->SetFillMode(false);
	//管线状态				灰度模型
	DirectXPipelineState->Build(GrayModel);

}
