#pragma once
#include <Windows.h>
#include <d3d11.h>
#include <wrl/client.h>
#include "Graphics.h"
#include "Geometry.h"

using Microsoft::WRL::ComPtr;

class Model
{
public:
	Model() = default;
	~Model() = default;

	void SetMesh(Graphics& graphics, std::vector<Geometry::Vertex>& vertices, UINT vertexCount, std::vector<unsigned short>& indices, UINT indexCount);
	void Draw(Graphics& graphics, float angle);

private:
	ComPtr<ID3D11Buffer> pVertexBuffer = nullptr;
	ComPtr<ID3D11Buffer> pIndexBuffer = nullptr;
	UINT indexCount = 0;
};