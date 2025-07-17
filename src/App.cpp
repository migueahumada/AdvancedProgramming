#include "App.h"
#include "World.h"
#include "Actor.h"
#include "Character.h"
#include "Prop.h"

#include <iostream>

const float App::FixedDeltaTime = 1.0f/60.0f;

void App::Initialize()
{
    m_inputEvents = make_shared<InputEvents>();

    m_inputEvents->keyPressedEvent.Subscribe([this](int key)
    {
        std::cout << "Key Pressed: " << static_cast<char>(key + 'A') << std::endl;
    });

    m_inputEvents->keyReleasedEvent.Subscribe([this](int key)
    {
        std::cout << "Key Released: " << static_cast<char>(key + 'A') << std::endl;
    });

    m_inputEvents->mouseMoveEvent.Subscribe([this](int x, int y)
    {
        std::cout << "Mouse Moved: " << x << ", "<< y << std::endl;
    });

    m_inputEvents->mouseButtonPressedEvent.Subscribe([this](int button, int x, int y)
    {
        std::cout << "Mouse Button Pressed: " << button << ", "<< x << ", "<< y << std::endl;
    });

    m_inputEvents->mouseButtonReleasedEvent.Subscribe([this](int button, int x, int y)
    {
        std::cout << "Mouse Button Released: " << button << ", "<< x << ", "<< y << std::endl;
    });

    m_window = make_unique<sf::RenderWindow>();
    m_window->create(sf::VideoMode(m_screenSize), "Programación Avanzada");
    //m_window->setFramerateLimit(m_frameRateLimit);

    m_world = make_shared<World>();
    m_world->Init();
    m_world->OnSubscribeEvents(m_inputEvents);

    m_myActor = m_world->SpawnActor<Character>(nullptr,0.0f,0.0f);
    auto propOne = m_world->SpawnActor<Prop>(m_myActor.lock(), 100.0f, 100.0f,10.0f,10.0f,1.0f,1.0f,0.0f,sf::Color::Blue);
}

void App::Run()
{
  
  while (m_window->isOpen())
  {
    /*
    * FDT |-----|-----|-----|
    * DT  ^  ^^ ^        ^  ^
    */

    float deltaTime = m_clock.restart().asSeconds(); //20ms || 18ms
    accumulatedTime += deltaTime; //20ms || 4ms + 18ms = 22ms
        
    ProcessEvents();

    while (accumulatedTime >= FixedDeltaTime) // 20ms >= 16ms -> YES || 4ms >= 16ms -> NO || 22ms >= 16ms -> YES
    {
      std::cout << "Fixed Updated\n" << std::endl;
      FixedUpdate();
      accumulatedTime -= FixedDeltaTime; // 20ms - 16ms = 4ms || 22ms - 16ms = 6ms
    }

    Update(deltaTime);     
    Render();
  }
}

void App::Shutdown()
{
}

void App::ProcessEvents()
{
    while (const std::optional<sf::Event> sfmlEvent = m_window->pollEvent())
    {
        if (sfmlEvent->is<sf::Event::Closed>())
            m_window->close();
        
        if (auto* keyEvent = sfmlEvent->getIf<sf::Event::KeyPressed>())
        {
            m_keyStates[static_cast<int>(keyEvent->code)] = true;
            m_inputEvents->keyPressedEvent(static_cast<int>(keyEvent->code));
        }

        if (auto* keyEvent = sfmlEvent->getIf<sf::Event::KeyReleased>())
        {
            m_keyStates[static_cast<int>(keyEvent->code)] = false;
            m_inputEvents->keyReleasedEvent(static_cast<int>(keyEvent->code));
        }

        if (auto* mouseEvent = sfmlEvent->getIf<sf::Event::MouseButtonPressed>())
        {
           m_inputEvents->mouseButtonPressedEvent(static_cast<int>(mouseEvent->button), mouseEvent->position.x, mouseEvent->position.y);
        }

        if (auto* mouseEvent = sfmlEvent->getIf<sf::Event::MouseButtonReleased>())
        {
           m_inputEvents->mouseButtonReleasedEvent(static_cast<int>(mouseEvent->button), mouseEvent->position.x, mouseEvent->position.y);
        }

        if (auto* mouseEvent = sfmlEvent->getIf<sf::Event::MouseMoved>())
        {
           m_inputEvents->mouseMoveEvent(mouseEvent->position.x, mouseEvent->position.y);
        }
    }
}

void App::Update(float deltaTime)
{
    m_world->Update(deltaTime);
    printf("Update!!!\n");
}

void App::FixedUpdate()
{
    m_world->FixedUpdate();
}

void App::Render()
{
    m_window->clear();
    
    m_world->Render(*m_window);
    
    m_window->display();
}
