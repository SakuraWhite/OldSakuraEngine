
//cbuffer是常量BUFF声明后不能修改  声明ConstBuffer常量BUFF
cbuffer  ConstBuffer : register(b0)//从常量寄存器中读出来 寄存器一共有15个  从b0到b14
{	
	float4x4 World;//从封装的常量缓冲区读出来XMFLOAT4X4 World
}

struct MeshVertexIn //Mash的顶点 顶点输入IN
{
	float3 Position : POSITION;//在MeshType.h中有对应 顶点位置 XYZ三通道 最后指定为POSITION位置
	float4 Color : COLOR;//在MeshType.h中有对应 顶点颜色 RGBA四通道 最后指定为COLOR颜色
};

struct MeshVertexOut //Mash的顶点输出 out
{
	float4 Position : SV_POSITION;  //顶点输出必须要有齐次坐标，所以是float4 XYZW
	float4 Color : COLOR;
};


MeshVertexOut VertexShaderMain(MeshVertexIn MV)  //入口 在Mash.cpp中VertexShaderMain入口 顶点着色器   传入MeshVertex中定义的POSITION与COLOR含义
{
	MeshVertexOut Out;

//从世界空间变换到齐次裁剪空间
	Out.Position = mul(float4(MV.Position, 1.f), World);
	Out.Color = MV.Color;

	return Out;
}

float4 PixelShaderMain(MeshVertexOut MVOut) : SV_TARGET //入口 在Mash.cpp中的PixelShaderMain入口  像素着色器 传入MeshVertex中定义的COLOR含义
{
	return MVOut.Color;
}