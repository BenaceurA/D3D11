#include "FrustumCull.h"

FrustumCull::FrustumCull(XMMATRIX & view, XMMATRIX & proj)
	:
	view(view),
	proj(proj)
{
	
	
}


FrustumCull::~FrustumCull()
{
}

void FrustumCull::extractFrustum()
{
	viewProj = view * proj;

	// Left Frustum Plane
	frustumPlanes[0].x = viewProj._14 + viewProj._11;
	frustumPlanes[0].y = viewProj._24 + viewProj._21;
	frustumPlanes[0].z = viewProj._34 + viewProj._31;
	frustumPlanes[0].w = viewProj._44 + viewProj._41;

	// Right Frustum Plane
	frustumPlanes[1].x = viewProj._14 - viewProj._11;
	frustumPlanes[1].y = viewProj._24 - viewProj._21;
	frustumPlanes[1].z = viewProj._34 - viewProj._31;
	frustumPlanes[1].w = viewProj._44 - viewProj._41;

	// Top Frustum Plane
	frustumPlanes[2].x = viewProj._14 - viewProj._12;
	frustumPlanes[2].y = viewProj._24 - viewProj._22;
	frustumPlanes[2].z = viewProj._34 - viewProj._32;
	frustumPlanes[2].w = viewProj._44 - viewProj._42;

	// Bottom Frustum Plane
	frustumPlanes[3].x = viewProj._14 + viewProj._12;
	frustumPlanes[3].y = viewProj._24 + viewProj._22;
	frustumPlanes[3].z = viewProj._34 + viewProj._32;
	frustumPlanes[3].w = viewProj._44 + viewProj._42;

	// Near Frustum Plane
	frustumPlanes[4].x = viewProj._13;
	frustumPlanes[4].y = viewProj._23;
	frustumPlanes[4].z = viewProj._33;
	frustumPlanes[4].w = viewProj._43;

	// Far Frustum Plane
	frustumPlanes[5].x = viewProj._14 - viewProj._13;
	frustumPlanes[5].y = viewProj._24 - viewProj._23;
	frustumPlanes[5].z = viewProj._34 - viewProj._33;
	frustumPlanes[5].w = viewProj._44 - viewProj._43;

	for each (frustumPlane plane in frustumPlanes)
	{
		float t = sqrt(plane.x * plane.x + plane.y * plane.y + plane.z * plane.z);
		plane.x /= t;
		plane.y /= t;
		plane.z /= t;
		plane.w /= t;
	}
}

bool FrustumCull::containsBoundingBox(XMFLOAT3* ver)
{
	for (size_t i = 0; i < 6; i++)
	{
		XMVECTOR planeNormal = XMVectorSet(frustumPlanes[i].x, frustumPlanes[i].y, frustumPlanes[i].z, 0.0f);
		float planeConstant = frustumPlanes[i].w;
		int p = 0;
		for (size_t j = 0; j < 8; j++)
		{		
			if (XMVectorGetX(XMVector3Dot(planeNormal, XMLoadFloat3(&ver[j]))) + planeConstant > 0.0f)
			{
				p++;
			}
		}
		if (p == 0) return false;
	}
	return true;
}
