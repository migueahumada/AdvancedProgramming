#include <iostream>
#include "App.h"



int main()
{
    App& app = App::GetInstance();
        
    app.Initialize();
    app.Run();
    app.Shutdown();

}