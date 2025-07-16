#include "SceneObject.h"
#include <iostream>

SceneObject::SceneObject()
{
}

SceneObject::~SceneObject()
{
}

void SceneObject::AddChild(const SPtr<SceneObject>& child)
{
  if (!child)
  {
    std::cerr << "Error: Trying to add a null child" << std::endl;
    return;
  }

  m_children.push_back(child);
  child->m_parent = SharedFromSelf();
  
}

void SceneObject::RemoveChild(const SPtr<SceneObject>& child)
{
  if (!child)
  {
    std::cerr << "Error: Trying to remove a null child" << std::endl;
    return;
  }

  auto it = std::remove(m_children.begin(), m_children.end(), child);
  if (it != m_children.end())
  {
    m_children.erase(it, m_children.end());
    child->m_parent.reset();
  }
  else
  {
    std::cerr << "Error: Child not found in SceneObject" << std::endl;
  }
  
}
