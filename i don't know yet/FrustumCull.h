#pragma once
#define _XM_NO_INTRINSICS_
#include <DirectXMath.h>


using namespace DirectX;
class FrustumCull
{
public:
	FrustumCull() = delete;
	FrustumCull(XMMATRIX& view,XMMATRIX&proj);
	~FrustumCull();
private:
	struct frustumPlane
	{
		float x, y, z, w;
	};
	XMMATRIX& view;
	XMMATRIX& proj;
	XMMATRIX viewProj;
	frustumPlane frustumPlanes[6];
public:
	void extractFrustum();
	bool containsBoundingBox(XMFLOAT3* ver);
};

