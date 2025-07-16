#include "World.h"
#include "Actor.h"
#include "Event.h"

World::World()
{

}

World::~World()
{
}

void World::Init()
{
    for (auto &actor : m_actors)
    {
        actor->Init(20.0f,20.0f,20.0f,sf::Color::Green);
    }
    
}
void World::Update(float deltaTime)
{
    for (auto &actor : m_actors)
    {
        actor->Update(deltaTime);
    }
}
void World::FixedUpdate()
{
    for (auto &actor : m_actors)
    {
        actor->FixedUpdate();
    }
}
void World::Render(sf::RenderWindow& window)
{
    for (auto &actor : m_actors)
    {
        actor->Render(window);
    }
}

SPtr<Actor> World::SpawnActor(float x, float y, float radius, const sf::Color &color)
{
    SPtr<Actor> pActor = make_shared<Actor>();
    pActor->Init(x, y, radius, color);
    pActor->OnSubscribeEvents(m_inputEvents);
    m_actors.push_back(pActor);

    return pActor;
}

void World::AddActor(SPtr<Actor> actor)
{
    m_actors.push_back(actor);
}

void World::RemoveActor(SPtr<Actor> actor)
{
    
    auto it = std::remove(m_actors.begin(), m_actors.end(), actor);

    if (it != m_actors.end())
    {
        m_actors.erase(it, m_actors.end());
    }
    
}

void World::OnSubscribeEvents(SPtr<InputEvents>& inputEvents)
{
    m_inputEvents = inputEvents;
}
