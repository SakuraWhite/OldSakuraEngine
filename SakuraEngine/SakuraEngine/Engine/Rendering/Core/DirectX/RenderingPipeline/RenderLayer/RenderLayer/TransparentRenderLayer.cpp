#include "TransparentRenderLayer.h"
#include "../../PipelineState/DirectXPipelineState.h"
#include "../../Geometry/GeometryMap.h"

FTransparentRenderLayer::FTransparentRenderLayer()
{
	//设定透明渲染层级
	RenderPriority = 3000;
}

void FTransparentRenderLayer::Draw(float DeltaTime)
{
	
	DirectXPipelineState->ResetPSO(Transparent);//重置管线状态对象PSO  重置为透明模式
	Super::Draw(DeltaTime);//渲染反射
}

void FTransparentRenderLayer::BuildShader()
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

void FTransparentRenderLayer::BuildPSO()
{
	//渲染目标混合状态描述
	D3D12_RENDER_TARGET_BLEND_DESC RenderTargetBlendDesc;

	RenderTargetBlendDesc.BlendEnable = true;//开启混合
	RenderTargetBlendDesc.LogicOpEnable = false;//如果混合开启 必须将LogicOP关掉

	RenderTargetBlendDesc.SrcBlend = D3D12_BLEND_SRC_ALPHA; //混合因子  这里是按照原像素alpha通道 src=原来的像素
	RenderTargetBlendDesc.DestBlend = D3D12_BLEND_INV_SRC_ALPHA;//混合因子  这里是反向的原像素alpha通道

	RenderTargetBlendDesc.BlendOp = D3D12_BLEND_OP_ADD; //混合运算  类似于正片叠底等
	RenderTargetBlendDesc.SrcBlendAlpha = D3D12_BLEND_ONE;//F(1,1,1)  混合因子
	RenderTargetBlendDesc.DestBlendAlpha = D3D12_BLEND_ZERO;//F(0,0,0) 混合因子
	RenderTargetBlendDesc.BlendOpAlpha = D3D12_BLEND_OP_ADD;//Alpha的混合运算， 类似于正片叠底等

	RenderTargetBlendDesc.LogicOp = D3D12_LOGIC_OP_NOOP;//逻辑操作符  这里设置的是保持原样
	RenderTargetBlendDesc.RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;//写入当前的遮罩 这里写入的是RGBA全部写入

	//设置渲染目标							
	DirectXPipelineState->SetRenderTarget(0, RenderTargetBlendDesc);
	//构建PSO管线状态对象状态
	DirectXPipelineState->Build(Transparent);//构建为透明模式
}
