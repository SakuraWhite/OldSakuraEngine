#pragma once//��ֹ�ظ�����
#define CLOSE_DISABLE_DEBUG_INFOR 

#include "../simple_library/public/simple_library.h"//������ʦд�õĿ��Ժ������
#include "Debug/Log/SimpleLog.h"

#include <vector>
#include <sstream>//�����ַ����Ĳ���

//���Ҫʹ��DX������Ҫ������Щ��
#pragma comment(lib, "d3dcompiler.lib") //��Ӧ��������ġ�D3Dcompiler.h��
#pragma comment(lib, "D3D12.lib")	   //��Ӧ����"d3d12.h"
#pragma comment(lib, "dxgi.lib")		   //��Ӧ����"dxgi1_4.h" 1.4�汾

#include <wrl.h>				 //windows�ṩ��һЩ����ָ�����¼��Ŀ�
#include <dxgi1_4.h>			 //�ṩ��һЩ�ӿڣ�����DXGL��������DXGL�ӿ�
#include "Rendering/DirectX12/d3dx12.h"		//�ڱ���Ⱦ�����ļ���Rending��DX12�ļ����еĶ��������ͷ�ļ�����΢���Լ�д����չ��
#include <D3Dcompiler.h>		 //����������ɫ�����Direct3D��
#include <DirectXMath.h>		 //DirectX�Լ���װ��Math�⣬��������һЩ����,�Ѿ���װ�ó��õ���ѧAPI
#include <DirectXPackedVector.h> //��������������صĿ�
#include <DirectXColors.h>		 //DirectX�Ѿ���װ�õ���ɫ�� ����ֱ��������
#include <DirectXCollision.h>	 //������ײ��ؿ�
#include "Debug/EngineDebug.h"   //�����Լ�д��EngineDebug�ļ� ��Ϊ��ຯ�����õõ�

using namespace Microsoft::WRL;
using namespace std;
using namespace DirectX;
using namespace DirectX::PackedVector;

extern class CEngine* Engine;

