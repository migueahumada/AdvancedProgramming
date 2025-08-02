#include "World.h"
#include "Actor.h"
#include "Event.h"
#include "App.h"
#include "SpawnBall.h"
#include "Character.h"
#include <iostream>
#include <random>

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
    CheckCollisions();
}
void World::FixedUpdate()
{
    for (auto& actor : m_actors)
    {
        actor->FixedUpdate();
    }
    CheckCollisions();
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

    inputEvents->keyReleasedEvent.Subscribe([this](int key)
      {
        OnKeyRelease(key);
      });
}


void World::OnKeyRelease(int key)
{
  if (key == static_cast<int>(sf::Keyboard::Key::E))
  {
    std::random_device randomDevice;
    std::mt19937 generator(randomDevice());
    std::uniform_real_distribution<float> distX(-1.0f, std::nextafter(1.0f, FLT_MAX));
    std::uniform_real_distribution<float> distY(0.0f, std::nextafter(1.0f, FLT_MAX));

    std::uniform_int_distribution<> distColor{ 0,255 };

    sf::Vector2f force{ distX(generator),-distY(generator) };

    force = force.normalized();

    float radius = 20.0f;

    auto newBall = SpawnActor<SpawnBall>(nullptr,
      App::GetInstance().getScreenSize().x * 0.5,
      App::GetInstance().getScreenSize().y - radius,
      radius,
      1.0f,
      1.0f,
      0.0f,
      sf::Color(distColor(generator),
        distColor(generator),
        distColor(generator)));

    if (!newBall)
    {
      std::cerr << "Failed to spawn new character." << std::endl;
      return;
    }

    auto spawnBall = std::reinterpret_pointer_cast<SpawnBall>(newBall);

    const float power = spawnBall->GetUseEulerAcceleration() ? m_eulerCannonPower : m_verletCannonPower;

    force.y *= power;
    force.x *= power;


    spawnBall->AddForce(force);

    std::cout << "Velocity: "<<spawnBall->GetVelocity().x << ", " << spawnBall->GetVelocity().y << std::endl;
    std::cout << "Acceleration: " << spawnBall->GetAcceleration().x << ", " << spawnBall->GetAcceleration().y << std::endl;

  }
  
}

void World::CheckCollisions()
{
  for(auto& actor : m_actors)
  {
    if (!actor)
    {
      continue;
    }

    auto spawnBall = std::dynamic_pointer_cast<SpawnBall>(actor);
    
    if (!spawnBall)
    {
      continue;
    }

    if (spawnBall->GetUseEulerAcceleration())
    {
      CheckBorderCollisionEuler(spawnBall);
    }else
    {
      CheckBorderCollisionVerlet(spawnBall);
    }
    

    
    //Colisión con los bordes -> en otro es OCTree
    
    //Colisión de uno con los otros
    for (auto& otherActor : m_actors )
    {
      if (actor == otherActor)
      {
        continue;
      }

      /*if (spawnBall->GetUseEulerAcceleration())
      {
        CheckActorsCollisionsEuler(spawnBall, otherActor);
      }
      else
      {
        CheckActorsCollisionsVerlet(spawnBall, otherActor);
      }*/
    }
  }
}

void World::CheckCollisionsWithBorders(SPtr<Actor> actor)
{

  //Esto es de Verlet -> la aceleración
  //Para euler tiene que ver la velocidad!!!!!!!!!!!
  auto SB = std::dynamic_pointer_cast<SpawnBall>(actor);

  if (SB)
  {
    sf::Vector2f currentPosition = SB->GetLocalPosition();
    sf::Vector2f currentVelocity = SB->GetVelocity();

    const float& radius = SB->GetRadius();

    if (SB->GetLocalPosition().x >= App::GetInstance().getScreenSize().x)
    {

      SB->setColor(sf::Color::Red);
      SB->SetVelocity(-3000.0F, -30000.0F);
      SB->SetTargetAcceleration(-30000.0F, -30000.0F);

    }

    if(SB->GetLocalPosition().y >= App::GetInstance().getScreenSize().y)
    {
      printf("Chocó en y\n");

      SB->setColor(sf::Color::Red);
      SB->SetVelocity(-3000.0F, -30000.0F);
      SB->SetTargetAcceleration(-30000.0F, -30000.0F);
    }

    if (SB->GetLocalPosition().y <= 0.0F)
    {
      SB->setColor(sf::Color::Green);
      SB->SetVelocity(3000.0F, 30000.0F);
      SB->SetTargetAcceleration(30000.0F, 30000.0F);
    }
  }
}

void World::CheckBorderCollisionEuler(SPtr<SpawnBall> spawnBall)
{
  sf::Vector2f currentPosition = spawnBall->GetLocalPosition();
  sf::Vector2f currentVelocity = spawnBall->GetVelocity();
  const float& radius = spawnBall->GetRadius();

  bool hasCollision = false;
  sf::Vector2f correctedPosition = currentPosition;
  sf::Vector2f bounceVelocity = currentVelocity;

  //Check left border
  if (currentPosition.x - radius <= 0.0f)
  {
    correctedPosition.x = radius;
    bounceVelocity.x = -bounceVelocity.x * m_collisionRestitution;
    hasCollision = true;
  }
  //Check right border
  else if (currentPosition.x + radius >= App::GetInstance().getScreenSize().x)
  {
    correctedPosition.x = App::GetInstance().getScreenSize().x - radius;
    bounceVelocity.x = -bounceVelocity.x * m_collisionRestitution;
    hasCollision = true;
  }

  //Check top border
  if (currentPosition.y - radius <= 0.0f)
  {
    correctedPosition.y = radius;
    bounceVelocity.y = -bounceVelocity.y * m_collisionRestitution;
    hasCollision = true;
  }
  //check bot border
  else if (currentPosition.y + radius >= App::GetInstance().getScreenSize().y)
  {
    correctedPosition.y = App::GetInstance().getScreenSize().y - radius;
    bounceVelocity.y = -bounceVelocity.y * m_collisionRestitution;
    hasCollision = true;
  }

  if (hasCollision)
  {
    spawnBall->SetLocalPosition(correctedPosition.x, correctedPosition.y);
    spawnBall->SetVelocity(bounceVelocity.x, bounceVelocity.y);
  }
}

void World::CheckBorderCollisionVerlet(SPtr<SpawnBall> spawnBall)
{

    sf::Vector2f currentPosition = spawnBall->GetLocalPosition();
    sf::Vector2f lastPosition = spawnBall->GetLastLocalPosition();
    float radius = spawnBall->GetRadius();

    sf::Vector2f correctedPosition = currentPosition;
    sf::Vector2f impliedVelocity = currentPosition - lastPosition;
    sf::Vector2f reflectedVelocity = impliedVelocity;

    bool hasCollision = false;

    sf::Vector2u screenSize = App::GetInstance().getScreenSize();

    // Reflect X if needed
    if (currentPosition.x - radius < 0.0f)
    {
      correctedPosition.x = radius;
      reflectedVelocity.x = -impliedVelocity.x * m_collisionRestitution;
      hasCollision = true;
    }
    else if (currentPosition.x + radius > screenSize.x)
    {
      correctedPosition.x = screenSize.x - radius;
      reflectedVelocity.x = -impliedVelocity.x * m_collisionRestitution;
      hasCollision = true;
    }

    // Reflect Y if needed
    if (currentPosition.y - radius < 0.0f)
    {
      correctedPosition.y = radius;
      reflectedVelocity.y = -impliedVelocity.y * m_collisionRestitution;
      hasCollision = true;
    }
    else if (currentPosition.y + radius > screenSize.y)
    {
      correctedPosition.y = screenSize.y - radius;
      reflectedVelocity.y = -impliedVelocity.y * m_collisionRestitution;
      hasCollision = true;
    }

    if (hasCollision)
    {
      sf::Vector2f adjustedLastPosition = correctedPosition - reflectedVelocity;
      spawnBall->SetLocalPosition(correctedPosition.x, correctedPosition.y);
      spawnBall->SetLastLocalPosition(adjustedLastPosition.x, adjustedLastPosition.y);
    }
}

void World::CheckActorsCollisionsEuler(SPtr<SpawnBall> spawnBall, SPtr<SpawnBall> other)
{
  sf::Vector2f currentSpawnBallPosition = spawnBall->GetLocalPosition();
  sf::Vector2f currentOtherPosition = other->GetLocalPosition();
  const float& actorRadius = spawnBall->GetRadius();
}

void World::CheckActorsCollisionsVerlet(SPtr<SpawnBall> spawnBall, SPtr<SpawnBall> other)
{
}

/* Ley de Hooke -> Sistema de resortes en el que x es el desplazamiento del resorte
*  desde su posición de reposo.
* -------------
* F = -k * x
* --------------
* F = fuerza resultante
* k = valor escalar constante
* x = desplazamiento del resorte
*/

sf::Vector2f g_restPosition;
float g_restDistance;
float g_springStiffness = 0.3f;

void World::HookeSetup()
{
  float anchorRadius = 10.0f;

  auto anchor = SpawnActor<Character>(nullptr, 
                                      App::GetInstance().getScreenSize().x * 0.5,
                                      anchorRadius * 100,
                                      anchorRadius,
                                      1.0f,
                                      1.0f,
                                      sf::Color::Red);
  if (anchor)
  {
    
  }
}

/*
* Ancla, peso
* Dos o varios elementos adjuntos.
*/
void World::HookeUpdate(float deltaTime)
{

}
