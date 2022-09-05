#pragma once//��ֹ�ظ�����

enum EMaterialType //ö�ٲ�������
{
	//�ֲ�����ģ��
	Lambert = 0,		 //������		ֽ�� �ֲڱ���
	HalfLambert,		 //��������	�ֲڱ��� Ҳ������Ƥ��
	Phong,				 //��			�����߹�
	BinnPhong,			 //���ַ�		�����߹�
	Wrap,				 //WrapLight ��������ģ��Ƥ���Ĳ���
	Minnaert,			 //Minnaert Lighting   ����ģ�Ⲽ�� ˿�� ��ά��
	Banded,				 //������ͨЧ��
	GradualBanded,		 //���н���Ч���Ŀ�ͨ����
	FinalBanded,		 //�Զ���Ŀ�ͨ���� �ں��˺ܶ�Ч���Ŀ�ͨ����
	BackLight,			 //͸��  �α���ɢ�����
	AnisotropyKajiyaKay, //GDC �������� 2004��KajiyaKay ͷ����Ⱦģ��
	OrenNayar,			 //GDC �ֲڱ������  ��ɳĮ

	//����Ч������  
	BaseColor,			 //������ɫģ�� ֻ����ɫ ���ܹ���
	Normal,				 //��ʾ����
	WorldNormal,		 //��ʾ���編��	WorldspaceNormal;

	Transparency = 15,		//͸��

	Fresnel = 100,		 //������  ������Ч��  �������ǲ���
	Max,				 //���

};


enum EMaterialDisplayStatusType //ö�ٲ���״̬��ʾ  �߿�/�����л�
{
	DefaultDisplay = 0, //Ĭ����ʾ(������ʾ��
	PointDisplay = 1,//��ʾ����
	WireframeDisplay = 2,//�߿���ʾ
	TriangleDisplay = 4 ,//��������ʾ
};