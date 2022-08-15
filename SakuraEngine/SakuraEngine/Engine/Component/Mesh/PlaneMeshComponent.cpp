#include "PlaneMeshComponent.h"
#include "../../Mesh/Core/MeshType.h"

CPlaneMeshComponent::CPlaneMeshComponent()
{

}

void CPlaneMeshComponent::CreateMesh(FMeshRenderingData& MeshData, float InHeight, float InWidth, uint32_t InHeightSubdivide, uint32_t InWidthSubdivide)
{

	auto SubdivideValue = [&](float InValue, uint32_t  InSubdivideValue)->float  //�ж�ϸ��ֵ ��ֹ���㵼�³���
	{
		if (InSubdivideValue <= 1)
		{
			return InValue;
		}

		return InValue / ((float)InSubdivideValue - 1);
	};

	//����ģ�����ĵ�
	float CHeight = 0.5f * InHeight;//����ƽ��߶����ĵ�
	float CWidth = 0.5f * InWidth;//����ƽ�������ĵ�
	//����ģ�͵�ϸ��
	float HeightSubdivideValie = SubdivideValue(InHeight, InHeightSubdivide);//ƽ��ĸ߶�ϸ�ּ���
	float WidthSubdivideValie = SubdivideValue(InWidth, InWidthSubdivide);//ƽ��Ŀ��ϸ�ּ���
	//����ģ�ͺ�������ϸ�ֵ�ƽ��
	float HorizontalAverageSubdivision = 1.f / ((float)InWidthSubdivide - 1.f);//ˮƽϸ�ֵ�ֵ ΪUV��׼��
	float VerticalAverageSubdivision = 1.f / ((float)InHeightSubdivide - 1.f);//��ֱϸ�ֵ�ֵ ΪUV��׼��


	//��������
	for (uint32_t i = 0; i < InHeightSubdivide; ++i)
	{
		float Z = CHeight - i * HeightSubdivideValie;//����Z��

		for (uint32_t j = 0; j < InWidthSubdivide; ++j)
		{
			float X = CWidth - j * WidthSubdivideValie;//����Z�� �������Ʒ���
			MeshData.VertexData.push_back(FVertex(
				XMFLOAT3(
					X, //x
					0.f, //y
					Z),//z					
				XMFLOAT4(Colors::Gray), //������ɫ
				XMFLOAT3(0.f, 1.f, 0.f),//����
				XMFLOAT2(VerticalAverageSubdivision * i, HorizontalAverageSubdivision * j)));//���������ˮƽ��ֱϸ����ֵ���Զ�չ��UV



		}
	}


	//������Index
	for (uint32_t i = 0; i < InHeightSubdivide - 1; ++i)
	{
		for (uint32_t j = 0; j < InWidthSubdivide - 1; ++j)
		{
			//�����ı���
			/*���߷�ת
			//������1
				MeshData.IndexData.push_back(i * InWidthSubdivide + j);
			MeshData.IndexData.push_back(i * InWidthSubdivide + j + 1);
			MeshData.IndexData.push_back((i + 1) * InWidthSubdivide + j);
			//������2
			MeshData.IndexData.push_back((i + 1) * InWidthSubdivide + j);
			MeshData.IndexData.push_back(i * InWidthSubdivide + j + 1);
			MeshData.IndexData.push_back((i + 1) * InWidthSubdivide + j + 1);
			*/
			//������1
			MeshData.IndexData.push_back((i + 1) * InWidthSubdivide + j);
			MeshData.IndexData.push_back(i * InWidthSubdivide + j + 1);
			MeshData.IndexData.push_back(i * InWidthSubdivide + j);
			//������2
			MeshData.IndexData.push_back((i + 1) * InWidthSubdivide + j + 1);
			MeshData.IndexData.push_back(i * InWidthSubdivide + j + 1);
			MeshData.IndexData.push_back((i + 1) * InWidthSubdivide + j);
		}
	}
}


//������ϣkey
void CPlaneMeshComponent::BuildKey(size_t& OutHashKey, float InHeight, float InWidth, uint32_t InHeightSubdivide, uint32_t InWidthSubdivide)
{
	std::hash<float> FloatHash;
	std::hash<int> IntHash;

	OutHashKey = 6;
	OutHashKey += FloatHash(InHeight);
	OutHashKey += FloatHash(InWidth);

	OutHashKey += IntHash._Do_hash(InHeightSubdivide);
	OutHashKey += IntHash._Do_hash(InWidthSubdivide);
}