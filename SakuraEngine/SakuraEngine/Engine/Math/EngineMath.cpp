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
}