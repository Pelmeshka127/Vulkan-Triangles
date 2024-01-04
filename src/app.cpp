#include "../inc/app.hpp"

#include <array>
#include <stdexcept>

namespace Vulkan 
{

//-------------------------------------------------------------------------------//

App::App() 
{
    LoadModels();
    
    CreatePipelineLayout();
    
    RecreateSwapChain();
    
    CreateCommandBuffers();
}

//-------------------------------------------------------------------------------//

App::~App() 
{ 
    vkDestroyPipelineLayout(device_.device(), pipeline_layout_, nullptr); 
}

//-------------------------------------------------------------------------------//

void App::RunApplication() 
{
    while (!window_.ShouldClose()) 
    {
        glfwPollEvents();
        
        DrawFrame();
    }

    vkDeviceWaitIdle(device_.device());
}

//-------------------------------------------------------------------------------//

void App::LoadModels() 
{
    std::vector<Model::Vertex> vertices{
        {{-0.5f, -0.5f}, {0.0f, 0.0f, 1.0f}}, 
        {{0.5f, 0.5f},  {0.0f, 0.0f, 1.0f}},
        {{-0.5f, 0.5f}, {0.0f, 0.0f, 1.0f}},

        {{0.5f, 0.5f}, {1.0f, 0.0f, 0.0f}}, 
        {{-0.5f, 0.5f},  {1.0f, 0.0f, 0.0f}},
        {{0.5f, -0.5f}, {1.0f, 0.0f, 0.0f}},
    };
    
    model_ = std::make_unique<Model>(device_, vertices);
}

//-------------------------------------------------------------------------------//

void App::CreatePipelineLayout() 
{
    VkPipelineLayoutCreateInfo pipelineLayoutInfo{};
    
    pipelineLayoutInfo.sType                    = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
    pipelineLayoutInfo.setLayoutCount           = 0;
    pipelineLayoutInfo.pSetLayouts              = nullptr;
    pipelineLayoutInfo.pushConstantRangeCount   = 0;
    pipelineLayoutInfo.pPushConstantRanges      = nullptr;
    
    if (vkCreatePipelineLayout(device_.device(), &pipelineLayoutInfo, nullptr, &pipeline_layout_) !=
        VK_SUCCESS) 
    {
        throw std::runtime_error("failed to create pipeline layout!");
    }
}

//-------------------------------------------------------------------------------//

void App::CreatePipeline() 
{
    assert(swap_chain_ != nullptr && "Cannot create pipeline before swap chain");
    assert(pipeline_layout_ != nullptr && "Cannot create pipeline before pipeline layout");

    PipelineConfigInfo pipelineConfig{};
    
    Pipeline::DefaultPipelineConfigInfo(
        pipelineConfig);
    
    pipelineConfig.renderPass       = swap_chain_->GetRenderPass();
    
    pipelineConfig.pipelineLayout   = pipeline_layout_;
    
    pipeline_ = std::make_unique<Pipeline>(
        device_,
        "shaders/simple_shader.vert.spv",
        "shaders/simple_shader.frag.spv",
        pipelineConfig);
}

//-------------------------------------------------------------------------------//

void App::RecreateSwapChain()
{
    auto extent = window_.GetExtent();

    while (extent.width == 0 || extent.height == 0)
    {
        extent = window_.GetExtent();

        glfwWaitEvents();
    }

    vkDeviceWaitIdle(device_.device());

    // swap_chain_ = nullptr; // important!

    if (swap_chain_ == nullptr)
        swap_chain_ = std::make_unique<SwapChain>(device_, extent);

    else
    {
        swap_chain_ = std::make_unique<SwapChain>(device_, extent, std::move(swap_chain_));
        
        if (swap_chain_->ImageCount() != commandBuffers.size())
        {
            FreeCommandBuffers();

            CreateCommandBuffers();
        }
    }

    CreatePipeline();
}

//-------------------------------------------------------------------------------//

void App::CreateCommandBuffers() 
{
    commandBuffers.resize(swap_chain_->ImageCount());

    VkCommandBufferAllocateInfo allocInfo{};
    
    allocInfo.sType               = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
    allocInfo.level               = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
    allocInfo.commandPool         = device_.getCommandPool();
    allocInfo.commandBufferCount  = static_cast<uint32_t>(commandBuffers.size());

    if (vkAllocateCommandBuffers(device_.device(), &allocInfo, commandBuffers.data()) !=
        VK_SUCCESS) 
    {
        throw std::runtime_error("failed to allocate command buffers!");
    }
}

//-------------------------------------------------------------------------------//

void App::FreeCommandBuffers()
{
    vkFreeCommandBuffers(
        device_.device(), 
        device_.getCommandPool(),
        static_cast<float>(commandBuffers.size()),
        commandBuffers.data());

    commandBuffers.clear();
}

//-------------------------------------------------------------------------------//

void App::RecordCommandBuffer(int ImageIndex)
{
    VkCommandBufferBeginInfo beginInfo{};
    
    beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;

    if (vkBeginCommandBuffer(commandBuffers[ImageIndex], &beginInfo) != VK_SUCCESS) 
        throw std::runtime_error("failed to begin recording command buffer!");

    VkRenderPassBeginInfo renderPassInfo{};
    
    renderPassInfo.sType        = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
    renderPassInfo.renderPass   = swap_chain_->GetRenderPass();
    renderPassInfo.framebuffer  = swap_chain_->GetFrameBuffer(ImageIndex);

    renderPassInfo.renderArea.offset = {0, 0};
    renderPassInfo.renderArea.extent = swap_chain_->GetSwapChainExtent();

    std::array<VkClearValue, 2> clearValues{};
    
    clearValues[0].color            = {1.0f, 1.0f, 1.0f, 1.0f};
    clearValues[1].depthStencil     = {1.0f, 0};
    renderPassInfo.clearValueCount  = static_cast<uint32_t>(clearValues.size());
    renderPassInfo.pClearValues     = clearValues.data();

    vkCmdBeginRenderPass(commandBuffers[ImageIndex], &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);

    VkViewport viewport{};
    viewport.x = 0.0f;
    viewport.y = 0.0f;
    viewport.width = static_cast<float>(swap_chain_->GetSwapChainExtent().width);
    viewport.height = static_cast<float>(swap_chain_->GetSwapChainExtent().height);
    viewport.minDepth = 0.0f;
    viewport.maxDepth = 1.0f;
    VkRect2D scissor{{0, 0}, swap_chain_->GetSwapChainExtent()};
    vkCmdSetViewport(commandBuffers[ImageIndex], 0, 1, &viewport);
    vkCmdSetScissor(commandBuffers[ImageIndex], 0, 1, &scissor);

    pipeline_->Bind(commandBuffers[ImageIndex]);
    model_->Bind(commandBuffers[ImageIndex]);
    model_->Draw(commandBuffers[ImageIndex]);

    vkCmdEndRenderPass(commandBuffers[ImageIndex]);
    
    if (vkEndCommandBuffer(commandBuffers[ImageIndex]) != VK_SUCCESS) 
        throw std::runtime_error("failed to record command buffer!");
}

//-------------------------------------------------------------------------------//

void App::DrawFrame() 
{
    uint32_t imageIndex;

    auto result = swap_chain_->AcquireNextImage(&imageIndex);

    if (result == VK_ERROR_OUT_OF_DATE_KHR)
    {
        RecreateSwapChain();

        return;
    }

    if (result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR) 
        throw std::runtime_error("failed to acquire swap chain image!");

    RecordCommandBuffer(imageIndex);

    result = swap_chain_->SubmitCommandBuffers(&commandBuffers[imageIndex], &imageIndex);

    if (result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR || window_.WasWindowResized())
    {
        window_.ResetWindowResizeFlag();

        RecreateSwapChain();

        return;
    }

    if (result != VK_SUCCESS) 
        throw std::runtime_error("failed to present swap chain image!");
}

//-------------------------------------------------------------------------------//

} // end of Vulkan namespace