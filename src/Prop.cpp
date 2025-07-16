#include "Prop.h"

void Prop::Init(float posX, float posY, float width, float height, float scaleX, float scaleY, float rotation, sf::Color color)
{
  Actor::Init(posX,posY, scaleX,scaleY,rotation);
  m_width = width;
  m_height = height;
  m_rectangleShape.setSize({m_width, m_height});
  m_rectangleShape.setFillColor(color);
  m_rectangleShape.setOrigin({m_width/2.0f,m_height/2.0f});
  m_rectangleShape.setPosition(m_localPosition);
}

void Prop::Render(sf::RenderWindow& window)
{
  m_rectangleShape.setPosition(m_globalPosition);
  m_rectangleShape.setRotation(sf::radians(m_globalRotation));
  m_rectangleShape.setScale({ m_globalScale.x,m_globalScale.y });
  window.draw(m_rectangleShape);
}
