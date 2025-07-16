#pragma once
#include "Object.h"
#include "HelperMacros.h"
#include <vector>

template <typename T>
using Vector = std::vector<T>;

class SceneObject : public Object
{
public:
    SceneObject(/* args */);
    virtual ~SceneObject();
    
    virtual void AddChild(const SPtr<SceneObject>& child);
    virtual void RemoveChild(const SPtr<SceneObject>& child);
protected:

    SPtr<SceneObject> SharedFromSelf()
    {
        return m_selfPtr.lock();
    }

    void SetSelfPtr(const SPtr<SceneObject>& self)
    {
        m_selfPtr = self;
    }

    WPtr<SceneObject> m_parent;
    Vector<SPtr<SceneObject>> m_children;
    WPtr<SceneObject> m_selfPtr;
    
public:
    template <typename T, typename... Args>
    static SPtr<T> CreateSceneObject(Args&& ...args);
};

//////////////////////////////////////////

template <typename T, typename... Args>
inline SPtr<T> SceneObject::CreateSceneObject(Args&& ...args)
{
    static_assert(std::is_base_of<SceneObject,T>::value, "T must be derived from SceneObject");

    auto node = make_shared<T>(std::forward<Args>(args)...);
    node->SetSelfPtr(node);

    return node;
}



