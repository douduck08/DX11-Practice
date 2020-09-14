#pragma once
#include <memory>
#include <unordered_map>
#include "Graphics.h"
#include "Resource.h"

class ResourceManager
{
private:
    ResourceManager() = default;

public:
    ResourceManager(const ResourceManager&) = delete;
    ResourceManager(ResourceManager&&) = delete;
    ResourceManager& operator=(const ResourceManager&) = delete;
    ResourceManager& operator=(ResourceManager&&) = delete;

    template<class T, typename...Params>
    static std::shared_ptr<T> Resolve(Graphics& graphics, Params&&...p)
    {
        static_assert(std::is_base_of<Resource, T>::value, "Can only resolve classes derived from Bindable");
        return Instance().ResolveImp<T>(graphics, std::forward<Params>(p)...);
    }

private:
    static auto& Instance() {
        static ResourceManager resourceManager;
        return resourceManager;
    }

    template<class T, typename...Params>
    std::shared_ptr<T> ResolveImp(Graphics& graphics, Params&&...p)
    {
        const std::string key = T::GetUID(std::forward<Params>(p)...);
        const auto i = resources.find(key);
        if (i == resources.end())
        {
            auto bind = std::make_shared<T>(graphics, std::forward<Params>(p)...);
            resources[key] = bind;
            return bind;
        }
        else
        {
            return std::static_pointer_cast<T>(i->second);
        }
    }

private:
    std::unordered_map<std::string, std::shared_ptr<Resource>> resources;
};