#include "Actor.h"
#include <SFML/Graphics.hpp>
#include "Event.h"
#include "App.h"

void Actor::Init( float posX, 
                  float posY,
                  float scaleX, 
                  float scaleY,
                  float rotation)
{
  m_localPosition = {posX,posY};
  m_localScale = {scaleX,scaleY};
  m_localRotation = rotation;

  UpdateTransformations();
}

void Actor::Update(float deltaTime)
{
  UpdateTransformations();
}

void Actor::FixedUpdate()
{
  UpdateTransformations();
}

void Actor::Render(sf::RenderWindow& window)
{
  
}

void Actor::OnSubscribeEvents(SPtr<InputEvents>& inputEvents)
{
}

void Actor::UpdateTransformations()
{
  if (!m_parent.expired())
  {
    //RTTI
    SPtr<Actor> parent = std::static_pointer_cast<Actor>(m_parent.lock());

    float parentRotation = parent->GetGlobalRotation();
    sf::Vector2f rotatedPosition;

    rotatedPosition.x = std::cos(parentRotation) * m_localPosition.x - std::sin(parentRotation) * m_localPosition.y;
    rotatedPosition.y = std::sin(parentRotation) * m_localPosition.x + std::cos(parentRotation) * m_localPosition.y;
    
    m_globalPosition = parent->GetGlobalPosition() + rotatedPosition;
    m_globalRotation = parent->GetGlobalRotation() + m_localRotation;

    m_globalScale.x = parent->GetGlobalScale().x * m_localScale.x;
    m_globalScale.y = parent->GetGlobalScale().y * m_localScale.y;
  }
  else
  {
    m_globalPosition = m_localPosition;
    m_globalRotation = m_localRotation;
    m_globalScale = m_localScale;
  }
}
