#include "Actor.h"
#include <SFML/Graphics.hpp>
#include "Event.h"
#include "App.h"

void Actor::Init(float posX, float posY, float radius, sf::Color color)
{
  m_position = {posX,posY};
  m_velocity = {0.0f,0.0f};
  m_acceleration = {0.0f,0.0f};
  m_targetAcceleration = {0.0f,0.0f};
  m_radius = radius;
  m_circleShape.setRadius(m_radius);
  m_circleShape.setFillColor(color);
  m_circleShape.setPosition(m_position);
  m_circleShape.setOrigin({m_radius,m_radius});
}

void Actor::Update(float deltaTime)
{
  UpdateInputs();

  m_acceleration += (m_targetAcceleration - m_acceleration) * m_accelerationRate * deltaTime;

  float accelMagnitude = m_acceleration.length();
  if (accelMagnitude > m_maxAcceleration)
  {
    m_acceleration *= m_maxAcceleration / accelMagnitude;
  }

  m_velocity += m_acceleration * deltaTime;

  if (m_targetAcceleration.length() == 0.0f)
  {
    m_acceleration = {0.0f,0.0f};
    m_velocity *= m_friction;
  }
  
  float speed = m_velocity.length();
  if (speed > m_maxSpeed)
  {
    m_velocity *= m_maxSpeed / speed;
  }

  m_position += m_velocity * deltaTime;
  m_circleShape.setPosition(m_position);

  m_targetAcceleration = {0.0f,0.0f};
    
}

void Actor::FixedUpdate()
{
    
}

void Actor::Render(sf::RenderWindow& window)
{
  window.draw(m_circleShape);
}

void Actor::OnSubscribeEvents(SPtr<InputEvents>& inputEvents)
{
  inputEvents->keyPressedEvent.Subscribe([this](int key)
  {
    OnKeyPress(key);
  });
  inputEvents->keyReleasedEvent.Subscribe([this](int key)
  {
    OnKeyRelease(key);
  });

  inputEvents->mouseButtonReleasedEvent.Subscribe([this](int button, int x, int y)
  {
    OnMouseRelease(button, x, y);
  });
}

void Actor::OnMouseRelease(int button, int x, int y)
{
  if (button == 0)
  {
    SetPosition((float)x, (float)y);
    SetTargetAcceleration(0.0f, 0.0f);
    SetVelocity(0.0f, 0.0f);
    SetAcceleration(0.0f, 0.0f);
  }

  if (button == 1)
  {
    sf::Color color = m_circleShape.getFillColor();
    if (color == sf::Color::Red)
    {
      color = sf::Color::Green;
    }
    else if (color == sf::Color::Green)
    {
      color = sf::Color::Blue;
    }
    else if (color == sf::Color::Blue)
    {
      color = sf::Color::Red;
    }
    m_circleShape.setFillColor(color);
  }
  
  
}

void Actor::OnKeyRelease(int key)
{
  if (key == static_cast<int>(sf::Keyboard::Key::R))
  {
    SetTargetAcceleration(0.0f,0.0f);
    SetVelocity(0.0f,0.0f);
    SetPosition(640.0f,380.0f);
    SetAcceleration(0.0f,0.0f);
  }
  
}

void Actor::OnKeyPress(int key)
{
  

}

void Actor::UpdateInputs()
{
  App& app = App::GetInstance();

  if (app.IsKeyPressed(static_cast<int>(sf::Keyboard::Key::A)))
  {
    m_targetAcceleration.x = -acceleration;
  }

  if (app.IsKeyPressed(static_cast<int>(sf::Keyboard::Key::D)))
  {
    m_targetAcceleration.x = acceleration;
  }

  if (app.IsKeyPressed(static_cast<int>(sf::Keyboard::Key::W)))
  {
    m_targetAcceleration.y = -acceleration;
  }

  if (app.IsKeyPressed(static_cast<int>(sf::Keyboard::Key::S)))
  {
    m_targetAcceleration.y = acceleration;
  }
  
}
