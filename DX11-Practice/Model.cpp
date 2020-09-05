#include "Model.h"
#include "SlotConfig.h"

#pragma comment(lib,"d3d11.lib")
#pragma comment(lib,"D3DCompiler.lib")

void Model::SetGeometry(Graphics& graphics, Geometry& geometry)
{
	AddBind(std::make_unique<VertexBuffer>(graphics, geometry.vertices));
	AddIndexBuffer(std::make_unique<IndexBuffer>(graphics, geometry.indices));

	auto pt = std::make_unique<VertexConstantBuffer<ModelTransform>>(graphics, TRANSFORM_CBUFFER_SLOT);
	pTransformbuffer = pt.get();
	AddBind(std::move(pt));
}

void Model::SetShader(Graphics& graphics, const std::wstring& vsFile, const std::wstring& psFile)
{
	AddBind(std::make_unique<VertexShader>(graphics, vsFile));
	AddBind(std::make_unique<PixelShader>(graphics, psFile));
}

void Model::Draw(Graphics& graphics)
{
	pTransformbuffer->Update(graphics, transform);
	Drawable::Draw(graphics);
}

void Model::SetPositionAndRotation(float x, float y, float z, float pitch, float yaw, float roll)
{
	transform.objectToWorld =
		DirectX::XMMatrixRotationRollPitchYaw(pitch, yaw, roll) *
		DirectX::XMMatrixTranslation(x, y, z);
}
