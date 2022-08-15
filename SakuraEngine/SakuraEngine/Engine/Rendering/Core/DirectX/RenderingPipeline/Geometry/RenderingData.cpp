#include "RenderingData.h"
#include "../../../../../Mesh/Core/MeshType.h"
FRenderingData::FRenderingData()//初始化
	: IndexSize(0) //Index数量初始化
	, VertexSize(0) //顶点数量初始化
	, IndexOffsetPosition(0) //Index偏移的位置初始化
	, VertexOffsetPosition(0) //顶点偏移的位置初始化
	, VertexTypeSize(sizeof(FVertex)) //顶点类型字符大小初始化
	, IndexTypeSize(sizeof(uint16_t)) //Index类型字符大小初始化
	, MeshHash(0) //mesh哈希初始化
	, MeshObjectIndex(0)//模型对象索引初始化
	, GeometryKey(0)//几何key初始化
	, IndexFormat(DXGI_FORMAT_R16_UINT)//Index格式初始化
	, WorldMatrix(EngineMath::IdentityMatrix4x4())//空间坐标单位化
	, TextureTransform(EngineMath::IdentityMatrix4x4())//单位化纹理变换矩阵
	, Mesh(NULL)
{

}
