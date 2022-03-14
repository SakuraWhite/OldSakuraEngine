//枚举  做键盘鼠标输入的使用

#pragma  once
#include "../../EngineMinimal.h"

enum EPressState
{
	//键盘摁下的状态 松开还是摁下
	Press, //摁下
	Release , //松开
};



struct  FInputKey  //结构体输入枚举
{
	FInputKey();


	string KeyName;//键盘输入的名字  
	EPressState PressState;   //摁键状态

};