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

  virtual void Init(  float posX, 
                      float posY,
                      float scaleX = 1.0f, 
                      float scaleY = 1.0f, 
                      float rotation = 0.0f);
  virtual void Update(float deltaTime);
  virtual void FixedUpdate();
  virtual void Render(sf::RenderWindow& window);

  inline void SetLocalPosition(float xPos, float yPos){
      m_localPosition = {xPos, yPos};
  }

  inline void SetScale(float xScale, float yScale){
      m_localScale = {xScale, yScale};
  }

  inline void SetRotation(float newRot){
      m_localRotation = newRot;
  }

  inline void SetLastLocalPosition(float xPos, float yPos) {
    m_lastLocalPosition = { xPos, yPos };
  }

  inline Vector2f GetLocalPosition() const{
      return m_localPosition;
  }

  inline Vector2f GetLocalScale() const{
      return m_localScale;
  }

  inline float GetLocalRotation() const{
      return m_localRotation;
  }

  inline Vector2f GetGlobalPosition() const{
      return m_globalPosition;
  }

  inline Vector2f GetGlobalScale() const{
      return m_globalScale;
  }

  inline float GetGlobalRotation() const{
      return m_globalRotation;
  }

  inline Vector2f GetLastLocalPosition() const{
    return m_lastLocalPosition;
  }
  
  virtual void OnSubscribeEvents(SPtr<InputEvents>& inputEvents);

protected:
  void UpdateTransformations();

  //Local
  Vector2f m_localPosition  {0.0f,0.0f};
  Vector2f m_lastLocalPosition{ 0.0f,0.0f };
  Vector2f m_localScale     {1.0f,1.0f};
  float m_localRotation     {0.0f};
  
  //Global
  Vector2f m_globalPosition {0.0f,0.0f};
  Vector2f m_lastGlobalPosition{ 0.0f,0.0f };
  Vector2f m_globalScale    {1.0f,1.0f};
  float m_globalRotation    {0.0f};
    

    
};


