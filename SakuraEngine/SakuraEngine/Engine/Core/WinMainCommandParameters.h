#pragma once//��ֹ�ظ�����


#if defined(_WIN32) //�ж��Ƿ���windowsƽ̨ ���������win32����ô����winƽ̨
#include "../EngineMinimal.h"//����ͷ�ļ�
class FWinMainCommandParameters //������
{
public:
	FWinMainCommandParameters(HINSTANCE InhInstance, HINSTANCE InprevInstance, PSTR IncmdLine, int InshowCmd);//������Ϣ��Ҫ����ͷ�ļ�
	
	HINSTANCE HInstance;//������� �����洫��������Ϣ�����֣�����ĸ��д
	HINSTANCE PrevInstance;//ͬ��
	PSTR CmdLine;//ͬ��
	int ShowCmd;//ͬ��
};
#elif 0

#endif // 0

