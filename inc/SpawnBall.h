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
										const sf::Color& color = sf::Color::Green);
	virtual void Update(float deltaTime) override;
	virtual void FixedUpdate() override;
	virtual void Render(sf::RenderWindow& window) override;

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

	inline void setColor(sf::Color newColor){
		m_color = newColor;
	}

	inline float GetRadius() const
	{
		return m_radius;
	}

	inline bool GetUseEulerAcceleration() const
	{
		return m_bUseEulerIntegration;
	}

	void AddForce(float forceX, float forceY);
	void AddForce(const Vector2f& force);
	

protected:

	
	Vector2f m_acceleration;
	Vector2f m_velocity;
	Vector2f m_targetAcceleration;

	float m_eulerAccelerationRate{ 0.5f }; //El cambio de la aceleración
	float m_verletAccelerationRate{ 0.001f }; //Máximo para no acelerar infinitamente
	float m_eulerMaxAcceleration{ 100000.0f }; //Máximo para no acelerar infinitamente
	float m_verletMaxAcceleration{ 50.0f }; //Máximo para no acelerar infinitamente

	float m_friction{ 0.99f }; //Fuerza contraria a la dirección
	float m_maxSpeed{ 100000.0f };

	const float m_eulerAcceleration {10000.0f};
	const float m_verletAcceleration{ 5.0f };

	bool m_bUseEulerIntegration{true};

	void ConstantVelocityMovement(float deltaTime);
	void ConstantAccelerationMovement(float deltaTime);
	void EulerIntegration(float deltaTime);
	void VerletIntegration();

	sf::CircleShape m_circleShape;
	sf::Color m_color;
	float m_radius{20.0f};
};

