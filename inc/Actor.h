#pragma once 
#include "SceneObject.h"
#include <SFML/Graphics.hpp>
#include "HelperMacros.h"

using sf::Vector2f;
class sf::RenderWindow;
struct InputEvents;

class Actor : public SceneObject
{
    
public:
    Actor() = default;
    virtual ~Actor() = default;

    virtual void Init(float posX, float posY, float radius, sf::Color color);
    virtual void Update(float deltaTime);
    virtual void FixedUpdate();
    virtual void Render(sf::RenderWindow& window);

    inline void SetVelocity(float xVel, float yVel){
        m_velocity = {xVel, yVel};
    }

    inline void SetPosition(float xPos, float yPos){
        m_position = {xPos, yPos};
        m_circleShape.setPosition(m_position);
    }

    inline void SetAcceleration(float xAccel, float yAccel){
        m_acceleration = {xAccel, yAccel};
    }

     inline void SetTargetAcceleration(float xAccel, float yAccel){
        m_targetAcceleration = {xAccel, yAccel};
    }

    inline void SetRadius(float newRadius){
        m_radius = newRadius;
        m_circleShape.setRadius(newRadius);
    }

    inline Vector2f GetVelocity() const{
        return m_velocity;
    }

    inline Vector2f GetPosition() const{
        return m_position;
    }

    inline Vector2f GetAcceleration() const{
        return m_acceleration;
    }

    inline Vector2f GetTargetAcceleration() const{
        return m_targetAcceleration;
    }

    inline float GetRadius() const{
        return m_radius;
    }

    virtual void OnSubscribeEvents(SPtr<InputEvents>& inputEvents);

protected:
    virtual void OnMouseRelease(int button, int x, int y);
    virtual void OnKeyRelease(int key);
    virtual void OnKeyPress(int key);
    virtual void UpdateInputs();

    Vector2f m_position; //Posición
    Vector2f m_velocity; //Velocidad
    Vector2f m_acceleration; // Aceleración que tenemos
    Vector2f m_targetAcceleration; //Aceleración a la que queremos llegar

    float m_accelerationRate {0.5f}; //El cambio de la aceleración
    float m_maxAcceleration {10000.0f}; //Máximo para no acelerar infinitamente
    float m_friction {0.98f}; //Fuerza contraria a la dirección
    float m_radius {20.0f};
    float m_maxSpeed {500.0f};

    const float acceleration {50000.0f};

    sf::CircleShape m_circleShape;
};


