#include "SpawnBall.h"
#include "Event.h"
#include "App.h"
#include <cstdlib>
#include "World.h"

void SpawnBall::Init(float posX, 
                     float posY, 
                     float radius, 
                     float scaleX, 
                     float scaleY, 
                     float rotation, 
                     const sf::Color& color)
{

  Actor::Init(posX,posY,scaleX,scaleY,rotation);

  m_radius = radius;
  m_color = color;
  m_velocity = { 0.0f,0.0f };
  m_acceleration = { 0.0f,0.0f };
  m_targetAcceleration = { 0.0f,0.0f };
  
  m_circleShape.setRadius(m_radius);
  m_circleShape.setFillColor(m_color);
  m_circleShape.setOrigin({m_radius,m_radius});
  m_circleShape.setPosition(m_localPosition);

  m_lastLocalPosition = m_localPosition;

}

void SpawnBall::Update(float deltaTime)
{
  

  if (!m_bUseEulerIntegration)
  {
    return;
  }
  
  EulerIntegration(deltaTime);

  Actor::Update(deltaTime);

}

void SpawnBall::FixedUpdate()
{
  
  if (m_bUseEulerIntegration)
  {
    return;
  }

  VerletIntegration();

  Actor::FixedUpdate();
  
}

void SpawnBall::Render(sf::RenderWindow& window)
{

  m_circleShape.setFillColor(m_color);
  window.draw(m_circleShape);
}


void SpawnBall::AddForce(float forceX, float forceY)
{
  SetTargetAcceleration(m_targetAcceleration.x + forceX, 
                        m_targetAcceleration.y + forceY);
  SetAcceleration(m_acceleration.x + forceX,
                  m_acceleration.y + forceY);
}

void SpawnBall::AddForce(const Vector2f& force)
{
  AddForce(force.x,force.y);
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

  //m_targetAcceleration = { 0.0f,0.0f };
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

  m_targetAcceleration = { 0.0f,0.0f };
}