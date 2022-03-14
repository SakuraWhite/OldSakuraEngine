#pragma once//防止重复包含
#define CLOSE_DISABLE_DEBUG_INFOR 

#include "../simple_library/public/simple_library.h"//包含老师写好的库以后可以用
#include "Debug/Log/SimpleLog.h"

#include <vector>
#include <sstream>//关于字符串的操作

//如果要使用DX，都需要包含这些库
#pragma comment(lib, "d3dcompiler.lib") //对应的是下面的“D3Dcompiler.h”
#pragma comment(lib, "D3D12.lib")	   //对应的是"d3d12.h"
#pragma comment(lib, "dxgi.lib")		   //对应的是"dxgi1_4.h" 1.4版本

#include <wrl.h>				 //windows提供的一些智能指针与事件的库
#include <dxgi1_4.h>			 //提供了一些接口，比如DXGL适配器与DXGL接口
#include "Rendering/DirectX12/d3dx12.h"		//在本渲染器中文件中Rending的DX12文件夹中的额外包含的头文件，是微软自己写的扩展库
#include <D3Dcompiler.h>		 //用来编译着色代码的Direct3D库
#include <DirectXMath.h>		 //DirectX自己封装的Math库，用来进行一些计算,已经封装好常用的数学API
#include <DirectXPackedVector.h> //用作计算向量相关的库
#include <DirectXColors.h>		 //DirectX已经封装好的颜色库 可以直接拿来用
#include <DirectXCollision.h>	 //物理碰撞相关库
#include "Debug/EngineDebug.h"   //包含自己写的EngineDebug文件 因为许多函数都用得到

using namespace Microsoft::WRL;
using namespace std;
using namespace DirectX;
using namespace DirectX::PackedVector;

extern class CEngine* Engine;

