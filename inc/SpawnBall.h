#pragma once
#include "Actor.h"
#include "Event.h"


using namespace sf;

struct InputEvents;

class SpawnBall : public Actor
{
public:
	SpawnBall() = default;
	virtual ~SpawnBall() = default;

	virtual void Init(float posX,
										float posY,
										float radius = 20.0f,
										float scaleX = 1.0f,
										float scaleY = 1.0f,
										float rotation = 0.0f,
										const sf::Color& color = sf::Color::White);
	virtual void Update(float deltaTime) override;
	virtual void FixedUpdate() override;
	virtual void Render(sf::RenderWindow& window) override;

	virtual void OnSubscribeEvents(SPtr<InputEvents>& inputEvents) override;

protected:
	
	virtual void OnMouseRelease(int button, int x, int y);
	virtual void OnKeyRelease(int key);
	virtual void OnKeyPress(int key);
	virtual void UpdateInputs();

	Vector2f m_velocity;
	Vector2f m_acceleration;
	Vector2f m_targetAcceleration;

	float m_eulerAccelerationRate{ 0.5f }; //El cambio de la aceleración
	//float m_verletAccelerationRate{ 0.001f }; //Máximo para no acelerar infinitamente
	float m_eulerMaxAcceleration{ 200.0f }; //Máximo para no acelerar infinitamente
	//float m_verletMaxAcceleration{ 10.0f }; //Máximo para no acelerar infinitamente


	float m_friction{ 0.50f }; //Fuerza contraria a la dirección
	float m_maxSpeed{ 10000.0f };

	const float m_eulerAcceleration {10000.0f};


	void ConstantVelocityMovement(float deltaTime);
	void ConstantAccelerationMovement(float deltaTime);
	void EulerIntegration(float deltaTime);
	void RandomParameters();



	sf::CircleShape m_circleShape;
	float m_radius{20.0f};
};

