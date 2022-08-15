#include "MeshType.h"
#include "MeshManage.h"

FVertex::FVertex(const XMFLOAT3& InPos, const XMFLOAT4& InColor)
	:Position(InPos) //初始化位置
	,Color(InColor)  //初始化顶点颜色
{
}

FVertex::FVertex(const XMFLOAT3& InPos, const XMFLOAT4& InColor, const XMFLOAT3& InNormal, const XMFLOAT2& InTexCoord)
	: Position(InPos) //初始化位置
	, Color(InColor)  //初始化顶点颜色
	, Normal(InNormal) //初始化法线
	, TexCoord(InTexCoord)//初始化UV坐标

{
}
