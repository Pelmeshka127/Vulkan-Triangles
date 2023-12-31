#ifndef APP_HPP_
#define APP_HPP_

#include "device.hpp"
#include "model.hpp"
#include "pipeline.hpp"
#include "swap_chain.hpp"
#include "window.hpp"

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
    
        void LoadModels();
        
        void CreatePipelineLayout();
        
        void CreatePipeline();
        
        void CreateCommandBuffers();

        void FreeCommandBuffers();
        
        void DrawFrame();

        void RecreateSwapChain();

        void RecordCommandBuffer(int ImageIndex);

        Window                          window_{WIDTH, HEIGHT, "Hello Vulkan!"};
        
        Device                          device_{window_};
        
        std::unique_ptr<SwapChain>      swap_chain_;
        
        std::unique_ptr<Pipeline>       pipeline_;
        
        VkPipelineLayout                pipeline_layout_;
        
        std::vector<VkCommandBuffer>    commandBuffers;
        
        std::unique_ptr<Model>          model_;
};

//-------------------------------------------------------------------------------//

}  // end of Vulkan namespace 

#endif