#include "SpawnBall.h"
#include "Event.h"
#include "App.h"
#include <cstdlib>
#include "World.h"

void SpawnBall::Init(float posX, 
                     float posY, 
                     const Vector2f& velocity,
                     const Vector2f& acceleration,
                     float radius, 
                     float scaleX, 
                     float scaleY, 
                     float rotation, 
                     const sf::Color& color)
{

  Actor::Init(posX,posY,scaleX,scaleY,rotation);

  m_radius = radius;
  m_velocity = { 0.0f,0.0f };
  m_acceleration = { 0.0f,0.0f };
  m_targetAcceleration = { 0.0f,0.0f };
  
  m_circleShape.setRadius(m_radius);
  m_circleShape.setFillColor(color);
  m_circleShape.setOrigin({m_radius,m_radius});
  m_circleShape.setPosition(m_localPosition);
  //RandomParameters();
  m_lastLocalPosition = m_localPosition;
}

void SpawnBall::Update(float deltaTime)
{
  Actor::Update(deltaTime);


    m_targetAcceleration.x = verletAcceleration;





  

  /*if (m_localPosition.y > App::GetInstance().getScreenSize().y)
  {
    m_targetAcceleration.y = -m_eulerAcceleration;
  }

  if (m_localPosition.x < App::GetInstance().getScreenSize().x)
  {
    m_targetAcceleration.x = m_eulerAcceleration;
  }

  if (m_localPosition.y < App::GetInstance().getScreenSize().y)
  {
    m_targetAcceleration.y = m_eulerAcceleration;
  }*/






  //EulerIntegration(deltaTime);
  
}

void SpawnBall::FixedUpdate()
{
  Actor::FixedUpdate();

  VerletIntegration();
}

void SpawnBall::Render(sf::RenderWindow& window)
{
  m_circleShape.setPosition(m_globalPosition);
  m_circleShape.setRotation(sf::radians(m_globalRotation));
  m_circleShape.setScale({ m_globalScale.x,m_globalScale.y });
  window.draw(m_circleShape);
}

void SpawnBall::OnSubscribeEvents(SPtr<InputEvents>& inputEvents)
{
  
}

void SpawnBall::OnMouseRelease(int button, int x, int y)
{
}

void SpawnBall::OnKeyRelease(int key)
{
  
}

void SpawnBall::OnKeyPress(int key)
{
}

void SpawnBall::UpdateInputs()
{
 

}

void SpawnBall::ConstantVelocityMovement(float deltaTime)
{
  m_acceleration = { 0.0f, 0.0f };

  m_localPosition += m_velocity * deltaTime;

  m_circleShape.setPosition(m_localPosition);
}
void SpawnBall::ConstantAccelerationMovement(float deltaTime)
{

  m_velocity += m_acceleration * deltaTime;

  m_localPosition += m_velocity * deltaTime;

  m_circleShape.setPosition(m_localPosition);
}

void SpawnBall::EulerIntegration(float deltaTime)
{
  // F = m*a -> a = F/m

  // Se le suma poco a poco la aceleración para que sea gradual.
  // Se resta la aceleración objectivo menos la aceleración actual
  // Luego se mutiplica por un factor entre 0-1,una razón de cambio
  // Finalmente se usa el deltaTime

  //LERP -> 1D interpolation
  m_acceleration += (m_targetAcceleration - m_acceleration) * m_eulerAccelerationRate * deltaTime;

  //Si la magnitud de la aceleración  es mayor a la máxima aceleración.
  //La aceleración se ve disminuida por
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

  float speed = m_velocity.length(); //Speed es la magnitud de la velocidad
  if (speed > m_maxSpeed)
  {
    m_velocity *= m_maxSpeed / speed;
  }

  m_localPosition += m_velocity * deltaTime;
  m_circleShape.setPosition(m_localPosition);

  m_targetAcceleration = { 0.0f,0.0f };
}

void SpawnBall::VerletIntegration()
{
  Vector2f currentPosition = m_localPosition;
  Vector2f deltaPosition;

  m_acceleration += (m_targetAcceleration - m_acceleration) * m_verletAccelerationRate;

  if (m_targetAcceleration.length() == 0.0f)
  {
    m_acceleration = { 0.0f,0.0f };
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
  m_targetAcceleration = { 0.0f,0.0f };
}