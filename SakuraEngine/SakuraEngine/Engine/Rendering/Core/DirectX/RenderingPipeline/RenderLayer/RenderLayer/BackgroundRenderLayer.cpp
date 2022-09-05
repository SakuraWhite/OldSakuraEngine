#include "BackgroundRenderLayer.h"
#include "../../Geometry/GeometryMap.h"
#include "../../PipelineState/DirectXPipelineState.h"

FBackgroundRenderLayer::FBackgroundRenderLayer()
{
	//设定背景渲染层级
	RenderPriority = 1000;
}

void FBackgroundRenderLayer::BuildShader()
{
	//构建Shader HLSL

	
	vector<ShaderType::FShaderMacro> ShaderMacro;//提取自己写的Shader宏
	BuildShaderMacro (ShaderMacro);//创建宏实例

	vector<D3D_SHADER_MACRO> D3DShaderMacro;//shader宏实例 D3D的宏
	ShaderType::ToD3DShaderMacro(ShaderMacro, D3DShaderMacro);//转换为D3D的shader宏
	

	VertexShader.BuildShaders(//构建顶点着色器
		L"../SakuraEngine/Shader/Sky.hlsl",//寻找外部的hlsl着色器语言文件
		"VertexShaderMain",//入口函数名
		"vs_5_1",//使用的版本
		D3DShaderMacro.data());//指定容器  D3D的Shader宏数据
	PixelShader.BuildShaders(			//构建像素着色器
		L"../SakuraEngine/Shader/Sky.hlsl",//寻找外部的hlsl着色器语言文件
		"PixelShaderMain", //入口函数名
		"ps_5_1",//构建像素着色器 
		D3DShaderMacro.data());//指定容器 D3D的Shader宏数据
	DirectXPipelineState->BindShader(VertexShader, PixelShader);//将Shader绑定在渲染管线状态上

	//输入布局 构建层 位置颜色法线等等信息
	InputElementDesc =
	{
		//构建位置信息
		//指定位置 元素语义  指定格式占12个字节    插入槽  内存中的偏移		 输入槽类			当前的位置
		{"POSITION",0,DXGI_FORMAT_R32G32B32_FLOAT,  0 , 0, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,0},

		//构建法线信息
		//指定法线 元素语义  指定格式				插入槽 内存中的偏移	 输入槽类					当前的位置
		{"NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0},

		//构建UV坐标
		//指定UV坐标		指定R32位G32位格式			内存中偏移24字节	
		{"TEXCOORD", 0,DXGI_FORMAT_R32G32_FLOAT, 0 , 24,  D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0}
	};
	DirectXPipelineState->BindInputLayout(InputElementDesc.data(), InputElementDesc.size());////将输入布局绑定在渲染管线状态上

}

void FBackgroundRenderLayer::BuildPSO()
{
	Super::BuildPSO();//反射 执行构建管线状态对象父类

	//设置光栅化状态
	CD3DX12_RASTERIZER_DESC RasterizerDesc(D3D12_DEFAULT); //光栅化描述
	RasterizerDesc.FillMode = D3D12_FILL_MODE_SOLID; //默认以实体模式显示
	RasterizerDesc.CullMode = D3D12_CULL_MODE_NONE;  //设置裁剪 天空球不需要裁剪
	DirectXPipelineState->SetRasterizerState(RasterizerDesc);

	//设置深度模板测试状态
	CD3DX12_DEPTH_STENCIL_DESC DepthStencilDesc = CD3DX12_DEPTH_STENCIL_DESC(D3D12_DEFAULT);//深度模板描述
	DepthStencilDesc.DepthFunc = D3D12_COMPARISON_FUNC_LESS_EQUAL; //裁剪模式为小于等于， 防止深度测试中失败
	DirectXPipelineState->SetDepthStencilState(DepthStencilDesc);

	//构建PSO管线状态对象
	DirectXPipelineState->Build(Background);//背景模式

}

void FBackgroundRenderLayer::Draw(float DeltaTime)
{

	DirectXPipelineState->ResetPSO(Background);//重置管线状态对象PSO  重置为背景模式
	Super::Draw(DeltaTime);//渲染反射
}



