#include "App.h"
#include "World.h"
#include "Actor.h"

#include <iostream>

App::App()
{

}

App::~App()
{
}

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
    m_window->setFramerateLimit(m_frameRateLimit);

    m_world = make_unique<World>();
    m_world->Init();
    m_world->OnSubscribeEvents(m_inputEvents);
    m_myActor = m_world->SpawnActor(20.0f,120.0f,20.0f,sf::Color::Magenta);
}

void App::Run()
{
    
    while (m_window->isOpen())
    {

        float deltaTime = m_clock.restart().asSeconds();

        //printf("Delta Time: %f\n",deltaTime);
        ProcessEvents();

        Update(deltaTime);
        FixedUpdate();
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
    m_myActor->Update(deltaTime);
}

void App::FixedUpdate()
{
    m_world->FixedUpdate();
}

void App::Render()
{
    m_window->clear();
    
    m_world->Render(*m_window);
    m_myActor->Render(*m_window);
    
    m_window->display();
}
