#ifndef APP_HPP_
#define APP_HPP_

#include "device.hpp"
#include "object.hpp"
#include "window.hpp"
#include "render.hpp"

#include <memory>
#include <vector>

namespace Vulkan 
{

//-------------------------------------------------------------------------------//

class App 
{
    public:
    
        static const int WIDTH = 800;

        static const int HEIGHT = 600;

        App();
        
        ~App();

        App(const App &) = delete;
        
        App &operator=(const App &) = delete;

        void RunApplication();

    private:
    
        void LoadObjects();
        
        Window                          window_{WIDTH, HEIGHT, "Hello Vulkan!"};
        
        Device                          device_{window_};

        Render                          render_{window_, device_};
        
        std::vector<Object>             objects_;
};

//-------------------------------------------------------------------------------//

}  // end of Vulkan namespace 

#endif