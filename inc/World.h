#pragma once
#include <string>
#include <vector>
#include "HelperMacros.h"
#include <SFML/Graphics.hpp>

using String = std::string;

template <typename T>
using Vector = std::vector<T>;

class Actor;
struct InputEvents;

class World
{
    
public:
    World();
    virtual ~World();

    void Init();
    void Update(float deltaTime);
    void FixedUpdate();
    void Render(sf::RenderWindow& window);

    SPtr<Actor> SpawnActor(float x, float y, float radius, const sf::Color& color);

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
};


