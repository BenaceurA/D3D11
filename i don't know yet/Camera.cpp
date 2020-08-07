#include "Camera.h"

Camera::Camera()
{
}

Camera::~Camera()
{
}

void Camera::moveForward(float speed)
{
	forward = XMVector3TransformCoord(DEFAULT_FORWARD, CamRotationMatrix);
	pos += forward * speed;
}

void Camera::moveBackward(float speed)
{
	backward = XMVector3TransformCoord(DEFAULT_BACKWARD, CamRotationMatrix);
	pos += backward * speed;
}

void Camera::rotateCamera(float x, float y, float z)
{
	CamRotationMatrix = XMMatrixRotationRollPitchYaw(rot.x, rot.y, rot.z);
	rot.x = x;
	rot.y = y;
	rot.z = z;
}

XMMATRIX Camera::updateViewMatrix()
{	
	target = XMVector3TransformCoord(DEFAULT_FORWARD, CamRotationMatrix);
	//target need to be always infront of camera(freelook)
	target += pos;

	viewMatrix = XMMatrixLookAtLH(pos, target, DEFAULT_UP);
	return viewMatrix;
}
