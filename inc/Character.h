#pragma once
#include <SFML/Graphics.hpp>
#include "Actor.h"
#include "HelperMacros.h"
class Character : public Actor
{

public:
    Character();
    virtual ~Character();

    // void Init() override;
    // void Update(float deltaTime) override;
    // void FixedUpdate() override;
    // void Render(sf::RenderWindow& window) override;

private:
    sf::CircleShape m_circle;
    float m_radius = 20.0f;

    sf::Vector2f m_position;
    sf::Vector2f m_velocity;
};


