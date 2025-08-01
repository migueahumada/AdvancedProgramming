#include "Character.h"
#include "App.h"
#include "Event.h"

void Character::Init(float posX, float posY, float radius, 
                     float scaleX, float scaleY, float rotation,
                     sf::Color color)
{
  Actor::Init(posX,posY,scaleX,scaleY,rotation);

  m_radius = radius;
  m_circleShape.setRadius(m_radius);
  m_circleShape.setFillColor(color);
  m_circleShape.setOrigin({m_radius, m_radius});
  m_circleShape.setPosition(m_localPosition);

  m_velocity = {0.0f,0.0f};
  m_acceleration = {0.0f,0.0f};
  m_targetAcceleration = {0.0f,0.0f};
  m_lastLocalPosition = m_localPosition;
}

void Character::Update(float deltaTime)
{
 

  UpdateInputs();

  if (!m_useEulerIntegration)
  {
    return;
  }

  EulerIntegration(deltaTime);

  Actor::Update(deltaTime);
  
}

void Character::FixedUpdate()
{

  if (m_useEulerIntegration)
  {
    return;
  }

  VerletIntegration();

  Actor::FixedUpdate();
}

void Character::Render(sf::RenderWindow &window)
{
  m_circleShape.setPosition(m_globalPosition);
  m_circleShape.setRotation(sf::radians(m_globalRotation));
  m_circleShape.setScale({m_globalScale.x,m_globalScale.y});
  window.draw(m_circleShape);
}

void Character::OnSubscribeEvents(SPtr<InputEvents>& inputEvents)
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

void Character::OnMouseRelease(int button, int x, int y)
{
  if (button == 0)
  {
    SetPosition((float)x, (float)y);
    SetTargetAcceleration(0.0f, 0.0f);
    SetVelocity(0.0f, 0.0f);
    SetAcceleration(0.0f, 0.0f);
    m_lastLocalPosition = m_localPosition;
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

void Character::OnKeyRelease(int key)
{
  if (key == static_cast<int>(sf::Keyboard::Key::R))
  {
    SetTargetAcceleration(0.0f,0.0f);
    SetVelocity(0.0f,0.0f);
    SetPosition(640.0f,380.0f);
    SetAcceleration(0.0f,0.0f);
    m_lastLocalPosition = m_localPosition;
  }
  
}

void Character::OnKeyPress(int key)
{
  

}

void Character::UpdateInputs()
{
  App& app = App::GetInstance();

  float accel = m_useEulerIntegration ? eulerAcceleration : verletAcceleration;

  if (app.IsKeyPressed(static_cast<int>(sf::Keyboard::Key::A)))
  {
    m_targetAcceleration.x = -accel;
  }

  if (app.IsKeyPressed(static_cast<int>(sf::Keyboard::Key::D)))
  {
    m_targetAcceleration.x = accel;
  }

  if (app.IsKeyPressed(static_cast<int>(sf::Keyboard::Key::W)))
  {
    m_targetAcceleration.y = -accel;
  }

  if (app.IsKeyPressed(static_cast<int>(sf::Keyboard::Key::S)))
  {
    m_targetAcceleration.y = accel;
  }
  
}

void Character::EulerIntegration(float deltaTime)
{
  // F�rmula de tiro libre -> Euler
  // P =  v0 * t + a * t^2 / 2

  // V = P - P-1
  
  m_acceleration += (m_targetAcceleration - m_acceleration) * m_eulerAccelerationRate * deltaTime;

  float accelMagnitude = m_acceleration.length();
  if (accelMagnitude > m_eulerMaxAcceleration)
  {
    m_acceleration *= m_eulerMaxAcceleration / accelMagnitude;
  }

  m_velocity += m_acceleration * deltaTime;

  if (m_targetAcceleration.length() == 0.0f)
  {
    m_acceleration = { 0.0f,0.0f };
    m_velocity *= m_friction;
  }

  float speed = m_velocity.length();
  if (speed > m_maxSpeed)
  {
    m_velocity *= m_maxSpeed / speed;
  }

  m_localPosition += m_velocity * deltaTime;
  m_circleShape.setPosition(m_localPosition);

  m_targetAcceleration = { 0.0f,0.0f };
}

void Character::VerletIntegration()
{
  Vector2f currentPosition = m_localPosition;
  Vector2f deltaPosition;

  m_acceleration += (m_targetAcceleration - m_acceleration) * m_verletAccelerationRate;

  if (m_targetAcceleration.length() == 0.0f)
  {
    m_acceleration = {0.0f,0.0f};
  }

  float accelMagnitude = m_acceleration.length();
  if (accelMagnitude > m_verletMaxAcceleration)
  {
    m_acceleration *= m_verletMaxAcceleration / accelMagnitude;
  }

  deltaPosition = (m_friction * m_localPosition) - (m_friction * m_lastLocalPosition);
  m_localPosition += deltaPosition + m_acceleration;

  m_circleShape.setPosition(m_localPosition);
  m_lastLocalPosition = currentPosition;
  m_globalPosition = m_localPosition;
  m_targetAcceleration = {0.0f,0.0f};
}
