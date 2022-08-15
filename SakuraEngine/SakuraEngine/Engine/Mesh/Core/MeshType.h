#pragma once//防止重复包含
#include "../../EngineMinimal.h"
#include "../../Math/EngineMath.h"

struct FVertex //顶点数据结构
{
	FVertex(const XMFLOAT3 &InPos,const XMFLOAT4 &InColor);//创建自定义的BOX的顶点位置与顶点颜色
	//添加			   顶点位置           顶点颜色                           法线                          UV
	FVertex(const XMFLOAT3& InPos, const XMFLOAT4& InColor, const XMFLOAT3 &InNormal, const XMFLOAT2& InTexCoord = XMFLOAT2(0.f, 0.f));

	XMFLOAT3 Position;//顶点位置  XYZ三通道
	XMFLOAT4 Color;//顶点颜色 RGBA四通道

	XMFLOAT3 Normal; //添加法线
	XMFLOAT3 UTangent;//添加切线  U方向切线

	XMFLOAT2 TexCoord;//添加纹理UV坐标
};

struct FMeshRenderingData //模型渲染数据
{
	vector<FVertex> VertexData; //储存顶点数据
	vector<uint16_t> IndexData; //模型的Index

public:
	//获取当前顶点的Size的字节
	UINT GetVertexSizeInBytes() { return VertexData.size() * sizeof(FVertex); }
	//获取当前Index的Size的字节
	UINT GetIndexSizeInBytes() { return IndexData.size() * sizeof(uint16_t); }
};

//Pyramid棱追体的边数
enum EPyramidNumberSides
{
	Pyramid_3 = 3,
	Pyramid_4,
	Pyramid_5,
};