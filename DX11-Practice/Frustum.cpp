#include "Frustum.h"

Frustum::Frustum(float fovY, float aspectRatio, float nearZ, float farZ)
    : fovY(fovY), aspectRatio(aspectRatio), nearZ(nearZ), farZ(farZ)
{
}

void Frustum::UpdatePlanes(const DirectX::XMFLOAT4X4& viewProj)
{
    // Left Frustum Plane
    // Add first column of the matrix to the fourth column
    planes[0].x = viewProj._14 + viewProj._11;
    planes[0].y = viewProj._24 + viewProj._21;
    planes[0].z = viewProj._34 + viewProj._31;
    planes[0].w = viewProj._44 + viewProj._41;

    // Right Frustum Plane
    // Subtract first column of matrix from the fourth column
    planes[1].x = viewProj._14 - viewProj._11;
    planes[1].y = viewProj._24 - viewProj._21;
    planes[1].z = viewProj._34 - viewProj._31;
    planes[1].w = viewProj._44 - viewProj._41;

    // Top Frustum Plane
    // Subtract second column of matrix from the fourth column
    planes[2].x = viewProj._14 - viewProj._12;
    planes[2].y = viewProj._24 - viewProj._22;
    planes[2].z = viewProj._34 - viewProj._32;
    planes[2].w = viewProj._44 - viewProj._42;

    // Bottom Frustum Plane
    // Add second column of the matrix to the fourth column
    planes[3].x = viewProj._14 + viewProj._12;
    planes[3].y = viewProj._24 + viewProj._22;
    planes[3].z = viewProj._34 + viewProj._32;
    planes[3].w = viewProj._44 + viewProj._42;

    // Near Frustum Plane
    // We could add the third column to the fourth column to get the near plane,
    // but we don't have to do this because the third column IS the near plane
    planes[4].x = viewProj._13;
    planes[4].y = viewProj._23;
    planes[4].z = viewProj._33;
    planes[4].w = viewProj._43;

    // Far Frustum Plane
    // Subtract third column of matrix from the fourth column
    planes[5].x = viewProj._14 - viewProj._13;
    planes[5].y = viewProj._24 - viewProj._23;
    planes[5].z = viewProj._34 - viewProj._33;
    planes[5].w = viewProj._44 - viewProj._43;

    for (int i = 0; i < 6; i++)
    {
        double x = planes[i].x;
        double y = planes[i].y;
        double z = planes[i].z;
        double length = sqrt(x * x + y * y + z * z);
        planes[i].x /= length;
        planes[i].y /= length;
        planes[i].z /= length;
        planes[i].w /= length;
    }
}
