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
										const Vector2f& velocity = Vector2f(0.0f,0.0f),
										const Vector2f& acceleration = Vector2f(0.0f, 0.0f),
										float radius = 20.0f,
										float scaleX = 1.0f,
										float scaleY = 1.0f,
										float rotation = 0.0f,
										const sf::Color& color = sf::Color::Green);
	virtual void Update(float deltaTime) override;
	virtual void FixedUpdate() override;
	virtual void Render(sf::RenderWindow& window) override;

	virtual void OnSubscribeEvents(SPtr<InputEvents>& inputEvents) override;

	inline void SetVelocity(float xVel, float yVel) {
		m_velocity = { xVel, yVel };
	}

	inline void SetAcceleration(float xAccel, float yAccel) {
		m_acceleration = { xAccel, yAccel };
	}

	inline void SetTargetAcceleration(float xAccel, float yAccel) {
		m_targetAcceleration = { xAccel, yAccel };
	}

	inline void SetRadius(float newRadius) {
		m_radius = newRadius;
		m_circleShape.setRadius(newRadius);
	}

	inline Vector2f GetVelocity() const {
		return m_velocity;
	}

	inline Vector2f GetAcceleration() const {
		return m_acceleration;
	}

	inline Vector2f GetTargetAcceleration() const {
		return m_targetAcceleration;
	}

protected:
	
	virtual void OnMouseRelease(int button, int x, int y);
	virtual void OnKeyRelease(int key);
	virtual void OnKeyPress(int key);
	virtual void UpdateInputs();

	Vector2f m_velocity;
	Vector2f m_acceleration;
	Vector2f m_targetAcceleration;

	float m_eulerAccelerationRate{ 0.5f }; //El cambio de la aceleración
	float m_verletAccelerationRate{ 0.001f }; //Máximo para no acelerar infinitamente
	float m_eulerMaxAcceleration{ 200.0f }; //Máximo para no acelerar infinitamente
	float m_verletMaxAcceleration{ 10.0f }; //Máximo para no acelerar infinitamente


	float m_friction{ 0.5f }; //Fuerza contraria a la dirección
	float m_maxSpeed{ 10000.0f };

	const float m_eulerAcceleration {10000.0f};
	const float verletAcceleration{ 5.0f };

	void ConstantVelocityMovement(float deltaTime);
	void ConstantAccelerationMovement(float deltaTime);
	void EulerIntegration(float deltaTime);
	void VerletIntegration();

	sf::CircleShape m_circleShape;
	float m_radius{20.0f};
};

