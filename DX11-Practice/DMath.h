#pragma once
#include "Frustum.h"
#include "Bounds.h"

class DMath
{
public:
	static bool BoundsInFrustum(const Frustum& frustum, const Bounds& bounds);
};