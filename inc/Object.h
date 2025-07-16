#pragma once
#include <string>

class Object
{
    
public:
    Object();
    virtual ~Object();
    
    inline const std::string& getName(){
        return m_name;
    }

    inline const int& getID(){
        return m_id;
    }

protected:
    
    std::string m_name;
    int m_id;
};


