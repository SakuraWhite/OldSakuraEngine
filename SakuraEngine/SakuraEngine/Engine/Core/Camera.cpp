#pragma once//��ֹ�ظ�����
#include "Camera.h"
#include "CameraType.h"
#include "../Component/InputComponent.h"
#include "../Component/TransformationComponent.h"


GCamera::GCamera()
	:GActorObject()
{
	InputComponent = CreateObject<CInputComponent>(new CInputComponent());
	//����������
	MouseSensitivity = 1.f;

	//�����ʼ����ʱ��Ĭ��Ϊ�������ģʽ
	CmeraType = ECmeraType::CameraRoaming;

	//���������ʼ��
	Radius = 10.f;     //��������뾶
	A = XM_PI;   //��������Ħ� ��ʼ��Ϊ�� = 180��
	B = XM_PI;   //��������Ħ� ��ʼ��Ϊ�� = 180��
}

void GCamera::BeginInit()
{
	
	ViewportInit();//ִ�г�ʼ��  ��ʼ��ͶӰ���� ���

	//�������
	InputComponent->CaptureKeyboardInforDelegate.Bind(this, &GCamera::ExecuteKeyboard);
	InputComponent->OnMouseButtonDownDelegate.Bind(this, &GCamera::OnMouseButtonDown);
	InputComponent->OnMouseButtonUpDelegate.Bind(this, &GCamera::OnMouseButtonUp);
	InputComponent->OnMouseMoveDelegate.Bind(this, &GCamera::OnMouseMove);
	InputComponent->OnMouseWheelDelegate.Bind(this, &GCamera::OnMouseWheel);
}

void GCamera::Tick(float DeltaTime)
{
	//ִ�й�����Ļ�ռ�
	BuildVewMatrix(DeltaTime);

}

void GCamera::ExecuteKeyboard(const FInputKey& InputKey)
{
	if (InputKey.KeyName == "W") //�ж�����
	{
		MoveForward(1.f);//����ж�����ΪW ��ô����ǰ���ƶ�Ϊ ��1  Ϊǰ��

	}
	else if (InputKey.KeyName == "S")
	{
		MoveForward(-1.f);//����ж�����ΪS ��ô����ǰ���ƶ�Ϊ ��1  Ϊ����
	}
	else if (InputKey.KeyName == "A")
	{
		MoveRight(-1.f);//����ж�����ΪA ��ô���������ƶ�Ϊ ��1  Ϊ�����ƶ�
	}
	else if (InputKey.KeyName == "D")
	{
		MoveRight(1.f);//����ж�����ΪA ��ô���������ƶ�Ϊ ��1  Ϊ�����ƶ�
	}
	else if (InputKey.KeyName == "Q")
	{
		CmeraType = ECmeraType::ObservationObject;//�����������Q ��ô������ͻᱻ�趨Ϊ�������۲�ģʽ
	}
	else if (InputKey.KeyName == "E")
	{
		CmeraType = ECmeraType::CameraRoaming;//�����������E ��ô������ͻᱻ�趨Ϊ�������ģʽ
	}
}

void GCamera::BuildVewMatrix(float DeltaTime)
{
	//���¾���
	//���ģʽ�Ŀ��� ѡ���������ξ���������۲�ģʽ����
	switch (CmeraType)
	{
		case CameraRoaming: //�������ξ���
		{

			//��������ռ�ת������Ļ�ռ�����
			GetTransformationComponent()->CorrectionVector();
			//���㰴�������ƶ���ͼ
			fvector_3d V3;	// fvector_3d�Ƿ�װ�õ���ѧ����ļ���
			//��ô�����ռ�ת������Ļ�ռ��Ľ���λ��
			GetTransformationComponent()->GetCorrectionPosition(V3);

			//������Ļ�ռ�VewMatrix
			XMFLOAT3 RightVector = GetTransformationComponent()->GetRightVector();//��ȡ�ҳ�����
			XMFLOAT3 UPVector = GetTransformationComponent()->GetUPVector();//��ȡ�ϳ�����
			XMFLOAT3 ForwardVector = GetTransformationComponent()->GetForwardVector();//��ȡ��������

			//�����������ʽ  ��Ļ�ռ����
			ViewMatrix = {
				RightVector.x,	UPVector.x,	ForwardVector.x,	0.f,
				RightVector.y,	UPVector.y,	ForwardVector.y,	0.f,
				RightVector.z,	UPVector.z,	ForwardVector.z,	0.f,
				V3.x,			V3.y,		V3.z,				1.f };

			break;
		}	
		case ObservationObject: //�������۲�ģʽ
		{
			//������������ ��ʵ���������۲�ģʽ


			XMFLOAT3& CameraPos = GetTransformationComponent()->GetPosition();//�����õ������λ��
			//�õ������λ��֮�� �������XYZ������������   
		       
			CameraPos.x = Radius * sinf(B) * cosf(A);//x = r * sin�� * cos��
			CameraPos.z = Radius * sinf(B) * sinf(A);//z = r * sin�� * sin��
			CameraPos.y = Radius * cosf(B);//y = r * cos��
			
			//��Ļ�ӿ����
			XMVECTOR Pos = XMVectorSet(CameraPos.x, CameraPos.y, CameraPos.z, 1.0f); //�����������
			XMVECTOR ViewTarget = XMVectorZero();//��Ļ�ܿ�����Ŀ��  ������0�㣬��û�а취�õ�ģ�͵�����
			XMVECTOR ViewUp = XMVectorSet(0.f, 1.0f, 0.f, 0.f);//ͷ��ָ���Ϸ�

			XMMATRIX ViewLookAt = XMMatrixLookAtLH(Pos, ViewTarget, ViewUp);//��Ļ�����ķ��� ����
			XMStoreFloat4x4(const_cast<XMFLOAT4X4*>(&ViewMatrix), ViewLookAt);//ת��Ϊ��Ļ�ռ�
			break;
		}

	}
	
}


void GCamera::OnMouseButtonDown(int X, int Y)
{
	//��������ʱ�� ����ж�Ϊtrue
	bLeftMouseDown = true;

	//��¼��һ���������λ��
	LastMousePosition.x = X;
	LastMousePosition.y = Y;

	SetCapture(GetMianWindowsHandle());//��������

}

void GCamera::OnMouseButtonUp(int X, int Y)
{
	//�ɿ�����ʱ�� ����ж�Ϊfalse
	bLeftMouseDown = false;


	ReleaseCapture();//�ɿ��ͷ���������

	LastMousePosition.x = X;
	LastMousePosition.y = Y;

}

void GCamera::OnMouseMove(int X, int Y)
{
	if (bLeftMouseDown)//�ж��Ƿ�����ƶ�
	{
		//���㻡��       DX12API������ת�ɻ���   ��ǰ��Xֵ��ȥ����϶�����һ֡��ֵ   ����������
		float XRadians = XMConvertToRadians((float)(X - LastMousePosition.x) * MouseSensitivity);
		float YRadians = XMConvertToRadians((float)(Y - LastMousePosition.y) * MouseSensitivity);

		switch (CmeraType)//��������ģʽ������۲�ģʽ����
		{
			case CameraRoaming: //���Ϊ����ģʽ
			{
				RotateAroundYAxis(YRadians);//����Y��
				RotateAroundZAxis(XRadians);//����Z��
				break;
			}

			case ObservationObject://���Ϊ����۲�ģʽ
			{
				A += (-XRadians);//�æȼ���Y����
				B += YRadians;//�æռ�ȥX����

				B = math_libray::Clamp(B, 0.f, XM_2PI * 2);//������ת�Ƕȵ�360

				break;
			}

		}
	}

	LastMousePosition.x = X;
	LastMousePosition.y = Y;

}

void GCamera::OnMouseWheel(int X, int Y, float InDelta)
{
	if (CmeraType == ECmeraType::ObservationObject)//�ж��Ƿ���������۲�ģʽ
	{
		Radius += (-InDelta / 80.f);//����

		//������һ���ķ�Χ��
		Radius = math_libray::Clamp(Radius, 7.f, 40.f);
	}


}

void GCamera::MoveForward(float InValue)
{
	
	//ǰ���ƶ�
	//�ж��Ƿ�Ϊ����ģʽ
	if (CmeraType == ECmeraType::CameraRoaming)
	{
		//λ��
		XMFLOAT3 AT3Position = GetTransformationComponent()->GetPosition();
		//ǰ������
		XMFLOAT3 AT3ForwardVector = GetTransformationComponent()->GetForwardVector();

		//ת���ṹ
		XMVECTOR AmountMovement = XMVectorReplicate(InValue * 0.5f);//����16�ֽڶ���
		//���ǰ�����λ��
		XMVECTOR Forward = XMLoadFloat3(&AT3ForwardVector);
		//λ��
		XMVECTOR Position = XMLoadFloat3(&AT3Position);

		//�������泯�������ƫ���ƶ�
		XMStoreFloat3(&AT3Position, XMVectorMultiplyAdd(AmountMovement, Forward, Position));
		//�������������ɺ���ȥ�������ǵ�λ��
		GetTransformationComponent()->SetPosition(AT3Position);
	}
}

void GCamera::MoveRight(float InValue)
{

	//�����ƶ�
	//�ж��Ƿ�Ϊ����ģʽ
	if (CmeraType == ECmeraType::CameraRoaming)
	{
		//λ��
		XMFLOAT3 AT3Position = GetTransformationComponent()->GetPosition();
		//��������
		XMFLOAT3 AT3RightVevtor = GetTransformationComponent()->GetRightVector();

		//ת���ṹ
		XMVECTOR AmountMovement = XMVectorReplicate(InValue * 0.5f);//����16�ֽڶ���
		//����������λ��
		XMVECTOR Right = XMLoadFloat3(&AT3RightVevtor);
		//λ��
		XMVECTOR Position = XMLoadFloat3(&AT3Position);

		//�������泯�������ƫ���ƶ�
		XMStoreFloat3(&AT3Position, XMVectorMultiplyAdd(AmountMovement, Right, Position));
		//�������������ɺ���ȥ�������ǵ�λ��
		GetTransformationComponent()->SetPosition(AT3Position);
	}

}

void GCamera::RotateAroundYAxis(FLOAT InRotateDegrees)
{
	//����תһ�����ʱ�� ��������Ӧ��Ӧ����ת
	//��ȡ��������� ���� ���ҵķ���
	XMFLOAT3 RightVector = GetTransformationComponent()->GetRightVector();
	XMFLOAT3 UPVector = GetTransformationComponent()->GetUPVector();
	XMFLOAT3 ForwardVector = GetTransformationComponent()->GetForwardVector();

	//�õ�����X����ת����  ʹ������������X�����������        ָ������������Y��
	XMMATRIX RotationX = XMMatrixRotationAxis(XMLoadFloat3(&GetTransformationComponent()->GetRightVector()), InRotateDegrees);


	//�����������Ͱ���Z����ת�������Ч��
	//ע��Y�� ��ֹӰ��
// 	XMStoreFloat3(//���渡������API		
// 		&TransformationComponent->GetRightVector(), //����Ķ�����ֵ����ת����
// 		XMVector3TransformNormal(XMLoadFloat3(&RightVector),//DXAP�˷�Ȼ��λ��
// 		RotationY));//���������ת������ Ȼ��ȥ�����õ���Z�����
	XMStoreFloat3(&GetTransformationComponent()->GetUPVector(), XMVector3TransformNormal(XMLoadFloat3(&UPVector), RotationX));
	XMStoreFloat3(&GetTransformationComponent()->GetForwardVector(), XMVector3TransformNormal(XMLoadFloat3(&ForwardVector), RotationX));

}

void GCamera::RotateAroundZAxis(FLOAT InRotateDegrees)
{
	//����תһ�����ʱ�� ��������Ӧ��Ӧ����ת
	//��ȡ��������� ���� ���ҵķ���
	XMFLOAT3 RightVector = GetTransformationComponent()->GetRightVector();
	XMFLOAT3 UPVector = GetTransformationComponent()->GetUPVector();
	XMFLOAT3 ForwardVector = GetTransformationComponent()->GetForwardVector();

	//�õ�����Y����ת����		�����Y����ת����
	XMMATRIX RotationY = XMMatrixRotationY(InRotateDegrees);

	//�����������Ͱ���Z����ת�������Ч��
	XMStoreFloat3(//���渡������API		
		&GetTransformationComponent()->GetRightVector(), //����Ķ�����ֵ����ת����
		XMVector3TransformNormal(XMLoadFloat3(&RightVector),//DXAP�˷�Ȼ��λ��
			RotationY));//���������ת������ Ȼ��ȥ�����õ���Z�����
	XMStoreFloat3(&GetTransformationComponent()->GetUPVector(), XMVector3TransformNormal(XMLoadFloat3(&UPVector), RotationY));
	XMStoreFloat3(&GetTransformationComponent()->GetForwardVector(), XMVector3TransformNormal(XMLoadFloat3(&ForwardVector), RotationY));
}
