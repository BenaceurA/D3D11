#pragma once
#include <DirectXMath.h>

using namespace DirectX;

class Camera
{
public:
	
	Camera();
	~Camera();

public:
	void moveForward(float speed);
	void moveBackward(float speed);
	void rotateCamera(float x, float y, float z);
	XMMATRIX updateViewMatrix();
private:
	XMVECTOR pos = { 0.0f,0.0f,-5.0f };
	XMVECTOR target;
	XMFLOAT3 rot;		

	XMMATRIX CamRotationMatrix; 
	XMMATRIX viewMatrix;
	
	XMVECTOR DEFAULT_FORWARD = { 0.0f,0.0f,1.0f };
	XMVECTOR DEFAULT_BACKWARD = { 0.0f,0.0f,-1.0f };
	XMVECTOR DEFAULT_RIGHT = { 1.0f,0.0f,0.0f };
	XMVECTOR DEFAULT_LEFT = { -1.0f,0.0f,0.0f };
	XMVECTOR DEFAULT_UP = { 0.0f,1.0f,0.0f };
	XMVECTOR DEFAULT_DOWN = { 0.0f,-1.0f,0.0f };

	XMVECTOR forward;
	XMVECTOR backward;
	XMVECTOR right;
	XMVECTOR left;
	

};

