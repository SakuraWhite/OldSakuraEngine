#pragma once
#include "../../EngineMinimal.h"

//�ṩһ���������ݱ�ʶID
class IGuidInterface //���浱ǰ�����
{
public:
	IGuidInterface();

	bool operator==(const IGuidInterface& InOther)
	{
		return guid_equal(&Guid, &InOther.Guid);
	}



	simple_c_guid GetGuid() { return Guid; }//��ȡ����Ⱦ�ϵ�Guid
private:
	simple_c_guid Guid;

};