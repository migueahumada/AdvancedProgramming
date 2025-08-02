#pragma once
#include <SFML/Graphics.hpp>
#include "Actor.h"
#include "HelperMacros.h"
class Character : public Actor
{

public:
    Character() = default;
    virtual ~Character() = default;

    virtual void Init(  float posX, 
                        float posY, 
                        float radius = 20.0f,                
                        float scaleX = 1.0f, 
                        float scaleY = 1.0f, 
                        float rotation = 0.0f, 
                        sf::Color color = sf::Color::Red);
    virtual void Update(float deltaTime) override;
    virtual void FixedUpdate() override;
    virtual void Render(sf::RenderWindow& window) override;

    virtual void OnSubscribeEvents(SPtr<InputEvents>& inputEvents) override;

    inline void SetVelocity(float xVel, float yVel){
        m_velocity = {xVel, yVel};
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

    inline Vector2f GetAcceleration() const{
        return m_acceleration;
    }

    inline Vector2f GetTargetAcceleration() const{
        return m_targetAcceleration;
    }

    inline float GetRadius() const{
        return m_radius;
    }

    inline bool GetIsStatic() const{
      return m_isStatic;
    }

    inline void SetIsStatic(bool _isStatic){
       m_isStatic = _isStatic;
    }
private:
  //Skills
  void Teleport(int key);

protected:
    Vector2f m_velocity; //Velocidad
    Vector2f m_acceleration; // Aceleración que tenemos
    Vector2f m_targetAcceleration; //Aceleración a la que queremos llegar

    virtual void OnMouseRelease(int button, int x, int y);
    virtual void OnKeyRelease(int key);
    virtual void OnKeyPress(int key);
    virtual void UpdateInputs();

    void EulerIntegration(float deltaTime);
    void VerletIntegration();

    float m_eulerAccelerationRate {0.5f}; //El cambio de la aceleración
    float m_verletAccelerationRate{0.001f}; //Máximo para no acelerar infinitamente
    float m_eulerMaxAcceleration{ 10000.0f }; //Máximo para no acelerar infinitamente
    float m_verletMaxAcceleration{ 10.0f }; //Máximo para no acelerar infinitamente
    

    float m_friction {0.99f}; //Fuerza contraria a la dirección
    float m_radius {20.0f};
    float m_maxSpeed {50000.0f};

    const float eulerAcceleration {10000.0f};
    const float verletAcceleration{ 5.0f };

    bool m_useEulerIntegration{true};
    bool m_isStatic{false};

    sf::CircleShape m_circleShape;
};


