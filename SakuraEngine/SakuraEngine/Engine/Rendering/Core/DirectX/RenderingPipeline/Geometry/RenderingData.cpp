#include "RenderingData.h"
#include "../../../../../Mesh/Core/MeshType.h"
FRenderingData::FRenderingData()//初始化
	: IndexSize(0) //Index数量初始化
	, VertexSize(0) //顶点数量初始化
	, IndexOffsetPosition(0) //Index偏移的位置初始化
	, VertexOffsetPosition(0) //顶点偏移的位置初始化
	, VertexTypeSize(sizeof(FVertex)) //顶点类型字符大小初始化
	, IndexTypeSize(sizeof(uint16_t)) //Index类型字符大小初始化
	, IndexFormat(DXGI_FORMAT_R16_UINT)//Index格式初始化
	, WorldMatrix(EngineMath::IdentityMatrix4x4())//空间坐标单位化
	, Mesh(NULL)
{

}
