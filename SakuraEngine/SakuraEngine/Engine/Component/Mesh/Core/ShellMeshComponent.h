#pragma once
#include "MeshComponent.h"


//��������ģ�����							�̳���Mesh�������
/// <summary>
/// �̳���Mesh�������
/// </summary>
class CShellMeshComponent :public CMeshComponent
{

public:
	CShellMeshComponent();//����

protected:
	void DrawQuadrilateral(FMeshRenderingData& MeshData, const fvector_4id& InDrawPoint, bool bReversal = false);

	//ר����������ӵ��row��colum���Ե�
	//������������ʱ���Ȼ���һȦrow ����һ���ٻ���һȦrow
	//GroupLayer ������˼�Ļ���һȦrow�������Ƕ���
	//Offset ������˼���ǲ����ڻ������м��ʽ�Ļ���
	fvector_4id GetQuadrilateralDrawPointTypeA(int InRowsand, int InColumns, int GroupLayer, int Offset = 1);
};