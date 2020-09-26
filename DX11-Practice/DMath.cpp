#include "DMath.h"
#include <DirectXMath.h>

static float PlaneDotPoint(const DirectX::XMFLOAT4& plane, float x, float y, float z)
{
	return plane.x * x + plane.y * y + plane.z *z + plane.w;
}

bool DMath::BoundsInFrustum(const Frustum& frustum, const Bounds& bounds)
{
	for (int i = 0; i < 6; i++)
	{
		int out = 0;
		out += (PlaneDotPoint(frustum.planes[i], bounds.minX, bounds.minY, bounds.minZ) < 0.0f) ? 1 : 0;
		out += (PlaneDotPoint(frustum.planes[i], bounds.maxX, bounds.minY, bounds.minZ) < 0.0f) ? 1 : 0;
		out += (PlaneDotPoint(frustum.planes[i], bounds.minX, bounds.maxY, bounds.minZ) < 0.0f) ? 1 : 0;
		out += (PlaneDotPoint(frustum.planes[i], bounds.maxX, bounds.maxY, bounds.minZ) < 0.0f) ? 1 : 0;
		out += (PlaneDotPoint(frustum.planes[i], bounds.minX, bounds.minY, bounds.maxZ) < 0.0f) ? 1 : 0;
		out += (PlaneDotPoint(frustum.planes[i], bounds.maxX, bounds.minY, bounds.maxZ) < 0.0f) ? 1 : 0;
		out += (PlaneDotPoint(frustum.planes[i], bounds.minX, bounds.maxY, bounds.maxZ) < 0.0f) ? 1 : 0;
		out += (PlaneDotPoint(frustum.planes[i], bounds.maxX, bounds.maxY, bounds.maxZ) < 0.0f) ? 1 : 0;
		if (out == 8) {
			return false;
		}
	}
	return true;
}
