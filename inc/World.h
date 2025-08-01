#pragma once
#include <string>
#include <vector>
#include "HelperMacros.h"
#include <SFML/Graphics.hpp>
#include "Actor.h"

using String = std::string;

template <typename T>
using Vector = std::vector<T>;

struct InputEvents;
class SceneObject;

class World
{
    
public:
    World() = default;
    virtual ~World() = default;

    void Init();
    void Update(float deltaTime);
    void FixedUpdate();
    void Render(sf::RenderWindow& window);

    template<typename T, typename ... Args>
    SPtr<Actor> SpawnActor(const SPtr<SceneObject>& parent, Args&&... args);

    void AddActor(SPtr<Actor> actor);
    void RemoveActor(SPtr<Actor> actor);

    void OnSubscribeEvents(SPtr<InputEvents>& inputEvents);

    inline const String& getName(){
        return m_name;
    }
protected:

    String m_name;
    SPtr<InputEvents> m_inputEvents;
    Vector<SPtr<Actor>> m_actors;
    SPtr<Actor> m_root;
};

template<typename T, typename ... Args>
SPtr<Actor> World::SpawnActor(const SPtr<SceneObject>& parent, Args&&... args)
{
    static_assert(std::is_base_of<Actor,T>::value, "T must be derived from Actor");

    SPtr<T> pActor = SceneObject::CreateSceneObject<T>();
    pActor->Init(std::forward<Args>(args)...);
    pActor->OnSubscribeEvents(m_inputEvents);
    m_actors.push_back(pActor);

    if (parent)
    {
        parent->AddChild(pActor);
    }
    else
    {
        m_root->AddChild(pActor);
    }

    return pActor;
}