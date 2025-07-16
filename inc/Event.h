#pragma once

#include <vector>
#include <functional>

using std::vector;
using std::function;

//Se necesita esta fwd declaration porque no se puede especializar
//Templates si antes tener la definición genérica.
template<typename T>
class Event;

template<typename ReturnType, typename... Args>
class Event<ReturnType(Args...)>
{
public:
  using Callback = function<ReturnType(Args...)>;
  //using Callback = ReturnType(*)(Args...);

  void Subscribe(Callback callback)
  {
    m_callbacks.push_back(callback);
  }

  void operator()(Args... args)
  {
    for (auto& callback : m_callbacks)
    {
      callback(args...);
    }
  }

  void Clear()
  {
    m_callbacks.clear();
  }
  
private:
  vector<Callback> m_callbacks;
};

struct InputEvents
{
  Event<void(int)> keyPressedEvent;
  Event<void(int)> keyReleasedEvent;
  Event<void(int, int)> mouseMoveEvent;
  Event<void(int, int, int)> mouseButtonPressedEvent;
  Event<void(int, int, int)> mouseButtonReleasedEvent;
};

