#pragma once
#include <string>
#include <SFML/Graphics.hpp>
#include <memory>
#include "Event.h"
#include "HelperMacros.h"
#include <unordered_map>


class World;
class Actor;

template <typename K, typename V>
using UMap = std::unordered_map<K,V>;

class App{
public:
    App(){}
    ~App(){}

    void Initialize();
    void Run();
    void Shutdown();
    
    bool IsKeyPressed(int key)const
    {
        auto it = m_keyStates.find(key);
        return (it != m_keyStates.end() && it->second);
    }

    static App& GetInstance()
    {
        static App instance;
        return instance;
    }

    inline const std::string& getTitle(){
        return m_title;
    }
    
    static const float FixedDeltaTime;

private:

    void ProcessEvents();
    void Update(float deltaTime);
    void FixedUpdate();
    void Render();

protected:
    const std::string m_title {"First Project"};
    UPtr<sf::RenderWindow> m_window;
    sf::Vector2u m_screenSize {1280,720};
    bool isOpen{true};
    SPtr<InputEvents> m_inputEvents;
    WPtr<Actor> m_myActor;
    SPtr<World> m_world;
    sf::Clock m_clock;
    
    unsigned int m_frameRateLimit {60};
    float accumulatedTime {0.0f};

    UMap<int, bool> m_keyStates;
    
};