#include "World.h"
#include "Actor.h"
#include "Event.h"



void World::Init()
{
    m_root = SceneObject::CreateSceneObject<Actor>();
    m_root->Init(0.0f,0.0f,1.0f,1.0f,0.0f);
}
void World::Update(float deltaTime)
{
    for (auto& actor : m_actors)
    {
        actor->Update(deltaTime);
    }
}
void World::FixedUpdate()
{
    for (auto& actor : m_actors)
    {
        actor->FixedUpdate();
    }
}
void World::Render(sf::RenderWindow& window)
{
    for (auto& actor : m_actors)
    {
        actor->Render(window);
    }
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
