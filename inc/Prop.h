#pragma once
#include "Actor.h"

class Prop : public Actor
{
public:
  Prop() = default;
  virtual ~Prop() = default;

  virtual void Init(float posX,
                    float posY,
                    float width,
                    float height,
                    float scaleX = 1.0f ,
                    float scaleY = 1.0f ,
                    float rotation = 0.0f ,
                    sf::Color color = sf::Color::Green);
  virtual void Render(sf::RenderWindow& window) override;

protected:
  float m_width {0.0f};
  float m_height {0.0f};
  sf::RectangleShape m_rectangleShape;
};


