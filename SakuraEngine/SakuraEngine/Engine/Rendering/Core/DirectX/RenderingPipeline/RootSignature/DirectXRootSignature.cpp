#include "DirectXRootSignature.h"

FDirectXRootSignature::FDirectXRootSignature()
{


}

void FDirectXRootSignature::BuildRootSignature()
{
	//根签名构建  //根签名：部署Shader环境 根签名可以帮助着色器提供渲染流水线里面的所有资源
	CD3DX12_ROOT_PARAMETER RootParam[2];//现在是两个根签名其中一个是世界空间矩阵 一个是视口投影矩阵


	//对象的CBV描述表
	CD3DX12_DESCRIPTOR_RANGE DescriptorRangeObjCBV;//用来描述对象CBV的表
	//Param初始化
	DescriptorRangeObjCBV.Init(
		D3D12_DESCRIPTOR_RANGE_TYPE_CBV, //指定范围为CBV   CBV=常量缓冲区视图
		1, 0);//第二个参数是有多少个描述符  第三个参数是基于哪一个着色器的寄存器 这里是0号


	//投影矩阵的CBV描述表
	CD3DX12_DESCRIPTOR_RANGE DescriptorRangeViewportCBV;//用来描述对象CBV的表
	//Param初始化
	DescriptorRangeViewportCBV.Init(
		D3D12_DESCRIPTOR_RANGE_TYPE_CBV, //指定范围为CBV   CBV=常量缓冲区视图
		1, 1);//第二个参数是有多少个描述符  第三个参数是基于哪一个着色器的寄存器 这里是1号

	RootParam[0].InitAsDescriptorTable(1, &DescriptorRangeObjCBV);//第一个参数是当前的范围 第二个指定相应的内容
	RootParam[1].InitAsDescriptorTable(1, &DescriptorRangeViewportCBV);//第一个参数是当前的范围 第二个指定相应的内容

	CD3DX12_ROOT_SIGNATURE_DESC RootSignatureDesc(
		2,//指定有多少个参数
		RootParam,//指定的参数
		0,//有多少个静态采样
		nullptr,//指定静态采样实例
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
