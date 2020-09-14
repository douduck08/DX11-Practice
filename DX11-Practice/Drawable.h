#pragma once
#include "BindableResources.h"
#include <vector>
#include <memory>

class Drawable
{
public:
	Drawable() = default;
	Drawable(const Drawable&) = delete;
	virtual ~Drawable() = default;

	virtual void Draw(Graphics& graphics) {
		for (auto& b : binds)
		{
			b->Bind(graphics);
		}
		for (auto& b : sharedBinds)
		{
			b->Bind(graphics);
		}
		graphics.pContext->DrawIndexed(indexCount, 0u, 0u);
	}

protected:
	void AddBind(std::unique_ptr<Bindable> bind) 
	{
		binds.push_back(std::move(bind));
	}

	void AddSharedBind(std::shared_ptr<Bindable> bind)
	{
		sharedBinds.push_back(std::move(bind));
	}

	void AddIndexBuffer(std::unique_ptr<IndexBuffer> indexBuffer)
	{
		indexCount = indexBuffer.get()->GetCount();
		binds.push_back(std::move(indexBuffer));
	}

private:
	UINT indexCount = 0;
	std::vector<std::unique_ptr<Bindable>> binds;
	std::vector<std::shared_ptr<Bindable>> sharedBinds;
};