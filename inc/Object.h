#pragma once
#include <string>

class Object
{
    
public:
    Object() = default;
    virtual ~Object() = default;
    
    void SetName(const std::string& name){
        m_name = name;
    }

    void SetID(unsigned int id){
        m_id = id;
    }

    inline const std::string& GetName()const {
        return m_name;
    }

    inline const int& GetID() const {
        return m_id;
    }

protected:
    
    std::string m_name;
    unsigned int m_id;
};


