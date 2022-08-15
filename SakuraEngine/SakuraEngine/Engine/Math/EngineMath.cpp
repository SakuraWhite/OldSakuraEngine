#include "EngineMath.h"


namespace EngineMath
{
	XMFLOAT4X4 IdentityMatrix4x4()
	{
		return XMFLOAT4X4(
			1.0f, 0.0f, 0.0f, 0.0f,
			0.0f, 1.0f, 0.0f, 0.0f,
			0.0f, 0.0f, 1.0f, 0.0f,
			0.0f, 0.0f, 0.0f, 1.0f);//4x4µ•Œªæÿ’Û
	}

	XMFLOAT3 ToFloat3(const fvector_3d& InV3d)
	{
		return XMFLOAT3(InV3d.x, InV3d.y, InV3d.z);
	}
}