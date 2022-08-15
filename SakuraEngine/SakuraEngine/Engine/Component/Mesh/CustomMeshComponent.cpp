#include "CustomMeshComponent.h"
#include "../../Mesh/Core/MeshType.h"

CCustomMeshComponent::CCustomMeshComponent()
{

}

void CCustomMeshComponent::CreateMesh(FMeshRenderingData& MeshData, string& InPath)
{
	//�ж��Ƿ�ΪOBJ��ʽ

		//�õ��ļ���С
	unsigned int FileSize = get_file_size_by_filename(InPath.c_str()); //�жϵ�����ļ��ж��
	//�����ļ���С����BUFF
	char* Buff = new char[FileSize + 1];//����BUFF �ǵ�+1 ��Ȼ�����
	//��ʼ��buff
	memset(Buff, 0, FileSize + 1); //�ǵ�+1 ��Ȼ�����

	//��ȡBUFF
	get_file_buf(InPath.c_str(), Buff);//�õ��ļ�


	if (!LoadObjFromBuff(Buff, FileSize, MeshData))
	{

	}

	delete Buff;//����buff
}

bool CCustomMeshComponent::LoadObjFromBuff(char* InBuff, uint32_t InBuffSize, FMeshRenderingData& MeshData)
{

	if (InBuffSize > 0)//�ļ��Ĵ�Сһ��Ҫ���������û������
	{
		stringstream BuffStream(InBuff); //����Buff�ļ�������
		char TmpLine[256] = { 0 };//��ȡ����ʱ����
		string MidTmp; //��Ϊ�м�ֵ

		for (; !BuffStream.eof();)  //   "!BuffStream.eof()"�����Ƿ�����˱���
		{
			memset(TmpLine, 0, 256);//�´α�����Ҫ���

			//�ж϶�ȡ��ÿһ������
			BuffStream.getline(TmpLine, 256);//һ��һ�еĶ�
			if (strlen(TmpLine) > 0)//�����������ݴ������������
			{
				if (TmpLine[0] == 'v')//��ȡ��ÿ�е����ݿ�ͷ �Ƿ���V��ͷ ����VT F�ȵ�
				{
					stringstream LineStream(TmpLine);//�ٴα���
					LineStream >> MidTmp;//��Ϊ�м�ֵ
					if (TmpLine[1] == 'n') //�жϵڶ����ַ��Ƿ�Ϊn  n���Ƿ���
					{

					}
					else if (TmpLine[1] == 't')//�����Ϊ�������ж��Ƿ�Ϊt  t������ͼUV
					{

					}
					else  //���û�еڶ����ַ� ��ô˵���Ƕ���
					{
						//Ȼ��Ѷ��㱣��Ȼ�����  �����һ��
						MeshData.VertexData.push_back(FVertex(
							XMFLOAT3(), XMFLOAT4(Colors::White)));

						//�õ���Ӻ��λ��
						int TopIndex = MeshData.VertexData.size() - 1;//�õ�����
						XMFLOAT3& Float3InPos = MeshData.VertexData[TopIndex].Position;//֮��ȷ�����λ��

						//����λ��
						LineStream >> Float3InPos.x;//ת�Ƴɶ�Ӧ������
						LineStream >> Float3InPos.y;//ת�Ƴɶ�Ӧ������
						LineStream >> Float3InPos.z;//ת�Ƴɶ�Ӧ������
					}

				}

				else if (TmpLine[0] == 'f')  //�������v��ͷ ��ô���п����ǻ���ָ�� �ж��Ƿ��ǻ���ָ�f�� 
				{
					stringstream LineStream(TmpLine);//�ٴα��� ����������
					LineStream >> MidTmp;//��ȡ����ĸ��f�� �൱�ڱ�ǩ

					char SaveLineString[256] = { 0 };
					char TmpBuff[256] = { 0 };

					for (size_t i = 0; i < 3; i++)
					{
						memset(SaveLineString, 0, 256);

						//����һ������
						LineStream >> SaveLineString;//�����Ե������뵽׼���õ�������
						//�ҵ�����λ��
						int StringPosA = find_string(SaveLineString, "/", 0);//Ѱ���ַ� ��/��
						memset(TmpBuff, 0, 256);//��TmpBuff����
						//��������ȡ
						char* VPosIndex = string_mid(SaveLineString, TmpBuff, 0, StringPosA);
						//��������������
						MeshData.IndexData.push_back(atoi(VPosIndex) - 1);

						//�ҵ��ڶ�������λ�� Ҳ������������index
						int StringPosB = find_string(SaveLineString, "/", StringPosA + 1);//Ѱ���ַ� ��/��
						memset(TmpBuff, 0, 256);//��TmpBuff����
						//��ȡ����Index����
						char* TexcoordIndex = string_mid(SaveLineString, TmpBuff, StringPosA + 1, StringPosB - (StringPosA + 1));

						//����Index��ȡ
						memset(TmpBuff, 0, 256);//��TmpBuff����
						//��ȡ����Index����
						char* NormalIndex = string_mid(SaveLineString, TmpBuff, StringPosB + 1, strlen(SaveLineString) - (StringPosB + 1));

					}

				}

			}

		}

		return true;
	}


	return false;
}


//������ϣkey
void CCustomMeshComponent::BuildKey(size_t& OutHashKey, std::string& InPath)
{
	std::hash<string> FloatHash;

	OutHashKey = 3;
	OutHashKey += FloatHash(InPath);
}