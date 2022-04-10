#include "Rendering.h"
#include "../../Debug/EngineDebug.h"
#include "../../Platform/Windows/WindowsEngine.h"
#include "../../Rendering/Enigne/DurectX/Core/DirectXRenderingEngine.h"

//vector<IRenderingInterface*> IRenderingInterface::RenderingInterface;//.h里的静态函数 需要声明

IRenderingInterface::IRenderingInterface()
{

	 
	//RenderingInterface.push_back(this);
}

IRenderingInterface::~IRenderingInterface()
{
	/*
	for (vector<IRenderingInterface*>::const_iterator Iter = RenderingInterface.begin();
		Iter != RenderingInterface.end(); 
		++Iter)     //使用遍历的方式来判断什么时候结束
	{
		if (*Iter == this) 
		{
			RenderingInterface.erase(Iter);//移除
			break;
		}
	}*/
}

void IRenderingInterface::Init()
{
}

void IRenderingInterface::PreDraw(float DeltaTime)//预渲染
{

	//重置我们的命令列表
//	ANALYSIS_HRESULT(GetGraphicsCommandList()->Reset(GetCommandAllocator().Get(), NULL));

}

void IRenderingInterface::Draw(float DeltaTime) //渲染进行
{

}

void IRenderingInterface::PostDraw(float DeltaTime)//渲染交付
{
}

