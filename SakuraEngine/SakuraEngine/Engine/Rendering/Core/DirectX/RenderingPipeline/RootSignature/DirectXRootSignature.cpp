#include "DirectXRootSignature.h"

FDirectXRootSignature::FDirectXRootSignature()
{


}

void FDirectXRootSignature::BuildRootSignature(UINT InTextureNum)
{
	//根签名构建  //根签名：部署Shader环境 根签名可以帮助着色器提供渲染流水线里面的所有资源
	//现在是6个根签名分别是 对象世界空间cbv描述、视口投影cbv描述、材质cbv描述、灯光cbv描述，纹理的SRV描述 立方体贴图SRV描述
	CD3DX12_ROOT_PARAMETER RootParam[7];


	////object对象的CBV描述表
	//CD3DX12_DESCRIPTOR_RANGE DescriptorRangeObjCBV;//用来描述对象CBV的表
	////Param初始化
	//DescriptorRangeObjCBV.Init(
	//	D3D12_DESCRIPTOR_RANGE_TYPE_CBV, //指定范围为CBV   CBV=常量缓冲区视图
	//	1, 0);//第二个参数是有多少个描述符  第三个参数是基于哪一个着色器的寄存器 这里是0号
	//
	//
	////Viewport投影矩阵的CBV描述表
	//CD3DX12_DESCRIPTOR_RANGE DescriptorRangeViewportCBV;//用来描述对象CBV的表
	////Param初始化
	//DescriptorRangeViewportCBV.Init(
	//	D3D12_DESCRIPTOR_RANGE_TYPE_CBV, //指定范围为CBV   CBV=常量缓冲区视图
	//	1, 1);//第二个参数是有多少个描述符  第三个参数是基于哪一个着色器的寄存器 这里是1号
	//
	////Light灯光的CBV描述表
	//CD3DX12_DESCRIPTOR_RANGE DescriptorRangeLightCBV;//用来描述对象CBV的表
	////Param初始化
	//DescriptorRangeLightCBV.Init(
	//	D3D12_DESCRIPTOR_RANGE_TYPE_CBV, //指定范围为CBV   CBV=常量缓冲区视图
	//	1, 2);//第二个参数是有多少个描述符  第三个参数是基于哪一个着色器的寄存器 这里是2号

	//Texture纹理的SRV描述表
	CD3DX12_DESCRIPTOR_RANGE DescriptorRangeTextureSRV;//用来描述对象SRV的表  
	//Param初始化
	DescriptorRangeTextureSRV.Init(
		D3D12_DESCRIPTOR_RANGE_TYPE_SRV, //指定范围为SRV   SRV=shaderRendering视图
		InTextureNum, 1);//第二个参数是有多少个描述符  第三个参数是基于哪一个着色器的寄存器 这里是1号
	//立方体贴图纹理的SRV描述表
	CD3DX12_DESCRIPTOR_RANGE DescriptorRangeCubeMapSRV;//用来描述对象SRV的表  
	//Param初始化
	DescriptorRangeCubeMapSRV.Init(
		D3D12_DESCRIPTOR_RANGE_TYPE_SRV, 1, 0);//第二个参数是有多少个描述符  第三个参数是基于哪一个着色器的寄存器 这里是0号


	//将跟参数表装到常量缓冲区
	RootParam[0].InitAsConstantBufferView(0);//object对象的CBV
	RootParam[1].InitAsConstantBufferView(1);//Viewport投影矩阵的CBV
	RootParam[2].InitAsConstantBufferView(2);//Light灯光的CBV
	RootParam[3].InitAsConstantBufferView(3);//Fog雾的CBV
	//材质
	RootParam[4].InitAsShaderResourceView(0, 1);//初始化为着色器资源视图  第一个是寄存器位置 第二个是寄存器空间

	//2D贴图
	RootParam[5].InitAsDescriptorTable(1, &DescriptorRangeTextureSRV, D3D12_SHADER_VISIBILITY_PIXEL);//Texture纹理的SRV  最后一个系数为像素
	//CUBE立方体贴图
	RootParam[6].InitAsDescriptorTable(1, &DescriptorRangeCubeMapSRV, D3D12_SHADER_VISIBILITY_PIXEL);

	//构建静态采样对象
	StaticSamplerObject.BuildStaticSampler();



	CD3DX12_ROOT_SIGNATURE_DESC RootSignatureDesc(
		7,//指定有多少个参数
		RootParam,//指定的参数
		StaticSamplerObject.GetSize(),//静态采样数量
		StaticSamplerObject.GetData(),//静态采样实例(数据 PTR)
		D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT);//根签名的布局选项

	//创建
	//二进制序列化
	ComPtr<ID3DBlob> SerializeRootSignature;
	ComPtr<ID3DBlob> ErrorBlob;//序列化报错
	//序列化根签名
	D3D12SerializeRootSignature(
		&RootSignatureDesc, //根描述
		D3D_ROOT_SIGNATURE_VERSION_1,//根版本
		SerializeRootSignature.GetAddressOf(),//指定二进制序列化的内容
		ErrorBlob.GetAddressOf());//指定二进制报错

	//判断报错的签名是否是空的
	if (ErrorBlob)
	{
		Engine_Log_Error("%s", (char*)ErrorBlob->GetBufferPointer());//如果报错不是空的 那么打印报错
	}

	//通过驱动创建根签名
	GetD3dDevice()->CreateRootSignature(
		0,//这里对单个GPU进行操作
		SerializeRootSignature->GetBufferPointer(),//指定序列化的获取Buffer的指针
		SerializeRootSignature->GetBufferSize(),////指定序列化的获取Buffer的Size
		IID_PPV_ARGS(&RootSignature));//创建根签名

}

void FDirectXRootSignature::PreDraw(float DeltaTime)
{

	//设置根签名
	GetGraphicsCommandList()->SetGraphicsRootSignature(RootSignature.Get());//根签名是每帧都需要设置的
}

void FDirectXRootSignature::Draw(float DeltaTime)
{
}

void FDirectXRootSignature::PostDraw(float DeltaTime)
{
}
