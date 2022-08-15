#include "Viewport.h"
#include "../../Mesh/Core/ObjectTransformation.h"
#include "../../Config/EngineRenderConfig.h"
#include "../../Math/EngineMath.h"

FViewport::FViewport() 
	:ViewMatrix(EngineMath::IdentityMatrix4x4())//��ʼ�������λ�� ��Ļ�ռ����굥λ��
	,ProjectMatrix(EngineMath::IdentityMatrix4x4())//��ʼ�����ӷ�Χ ����ռ����굥λ��
{

}

void FViewport::ViewportInit()
{
	//��ǰ��Ļ�ĺ��ݱ� һ����16��9
	//�����Ƕ��������ӿڵ���Ļ�Ŀ������Ļ�ӿڵĸ� ͬʱ�ǵô�INT����ת��ΪFloat����
	float AspectRatio = (float)FEngineRenderConfig::GetRenderConfig()->ScrrenWidth / (float)FEngineRenderConfig::GetRenderConfig()->ScrrenHight;
	//(1,1,0) (-1,1,0) (-1,-1,0) (1,-1,0) (1,1,1) (-1,1,1) (-1,-1,1) (1,-1,1)

	//ͶӰ����
	//���ǽ�3D�ռ��ת����2D����ת����3D  ���ǻ�����Ұ��������͸��ͶӰ����
	XMMATRIX Project = XMMatrixPerspectiveFovLH(
		0.25f * XM_PI, //�Ի���Ϊ��λ�����϶��µ��ӳ��ǡ�������45��
		AspectRatio,//��ͼor��Ļ�ռ� X:Y ���ݺ�ȡ�
		1.0f,		//��������ƽ��ľ��� ���������
		10000.f		//������ƽ�浽Զ������ľ��� ��
	);

	XMStoreFloat4x4(&ProjectMatrix, Project);//����ռ��ת��
}
