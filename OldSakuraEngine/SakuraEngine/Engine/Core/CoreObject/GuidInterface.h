#pragma once
#include "../../EngineMinimal.h"

//提供一个对象的身份标识ID
class IGuidInterface //储存当前的身份
{
public:
	IGuidInterface();

	bool operator==(const IGuidInterface& InOther)
	{
		return guid_equal(&Guid, &InOther.Guid);
	}



	simple_c_guid GetGuid() { return Guid; }//获取到渲染上的Guid
private:
	simple_c_guid Guid;

};