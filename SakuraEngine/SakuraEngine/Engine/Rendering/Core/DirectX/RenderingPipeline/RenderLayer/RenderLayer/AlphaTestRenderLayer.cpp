#include "AlphaTestRenderLayer.h"
#include "../../Geometry/GeometryMap.h"
#include "../../PipelineState/DirectXPipelineState.h"

FAlphaTestRenderLayer::FAlphaTestRenderLayer()
{
	//设定Alpha渲染层级
	RenderPriority = 2450;
}

void FAlphaTestRenderLayer::BuildShader()
{
	//构建Shader HLSL

	vector<ShaderType::FShaderMacro> ShaderMacro;//提取自己写的Shader宏
	BuildShaderMacro(ShaderMacro);//创建宏实例

	vector<D3D_SHADER_MACRO> D3DShaderMacro;//shader宏实例 D3D的宏
	ShaderType::ToD3DShaderMacro(ShaderMacro, D3DShaderMacro);//转换为D3D的shader宏

	VertexShader.BuildShaders(//构建顶点着色器
		L"../SakuraEngine/Shader/HelloHLSL.hlsl",//寻找外部的hlsl着色器语言文件
		"VertexShaderMain",//入口函数名
		"vs_5_1",//使用的版本
		D3DShaderMacro.data());//指定容器
	PixelShader.BuildShaders(			//构建像素着色器
		L"../SakuraEngine/Shader/HelloHLSL.hlsl",//寻找外部的hlsl着色器语言文件
		"PixelShaderMain", //入口函数名
		"ps_5_1",//构建像素着色器
		D3DShaderMacro.data());//指定容器
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

void FAlphaTestRenderLayer::BuildPSO()
{
	Super::BuildPSO();//反射 执行构建管线状态对象父类
	//构建PSO管线状态对象
	DirectXPipelineState->Build(AlphaTest);//Alpha测试模式

}

void FAlphaTestRenderLayer::Draw(float DeltaTime)
{

	DirectXPipelineState->ResetPSO(AlphaTest);//重置管线状态对象PSO  重置为Alpha测试模式
	Super::Draw(DeltaTime);//渲染反射
}