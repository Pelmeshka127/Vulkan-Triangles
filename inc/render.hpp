#ifndef RENDER_HPP_
#define RENDER_HPP_

#include "object.hpp"
#include "swap_chain.hpp"
#include "window.hpp"

#include <memory>
#include <vector>
#include <cassert>

namespace Vulkan 
{

//-------------------------------------------------------------------------------//

class Render 
{
    public:
    
        Render(Window& window, Device& device);
        
        ~Render();

        Render(const Render &) = delete;
        
        Render &operator=(const Render &) = delete;


        VkRenderPass GetSwapChainRenderPass() const { return swap_chain_->GetRenderPass(); }

        bool IsFrameInProgress() const { return is_frame_started_; }

        VkCommandBuffer GetCurrentCommandBuffer() const 
        {   
            assert(is_frame_started_);

            return command_buffers_[current_image_index_]; 
        }


        VkCommandBuffer BeginFrame();

        void EndFrame();

        void BeginSwapChainRenderPass(VkCommandBuffer commandBuffer);

        void EndSwapChainRenderPass(VkCommandBuffer commandBuffer);

    private:
    
        void CreateCommandBuffers();

        void FreeCommandBuffers();
        
        void RecreateSwapChain();

        Window&                         window_;
        
        Device&                         device_;
        
        std::unique_ptr<SwapChain>      swap_chain_;
        
        std::vector<VkCommandBuffer>    command_buffers_;

        uint32_t                        current_image_index_;

        bool                            is_frame_started_ = false;
};

//-------------------------------------------------------------------------------//

}  // end of Vulkan namespace 

#endif