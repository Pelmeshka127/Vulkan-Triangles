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
    
    CreatePipeline();
    
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
    std::vector<Model::Vertex> vertices{{{0.0f, -0.5f}}, {{0.5f, 0.5f}}, {{-0.5f, 0.5f}}};
    
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
    PipelineConfigInfo pipelineConfig{};
    
    Pipeline::DefaultPipelineConfigInfo(
        pipelineConfig,
        swap_chain_.Width(),
        swap_chain_.Height());
    
    pipelineConfig.renderPass       = swap_chain_.GetRenderPass();
    
    pipelineConfig.pipelineLayout   = pipeline_layout_;
    
    pipeline_ = std::make_unique<Pipeline>(
        device_,
        "shaders/simple_shader.vert.spv",
        "shaders/simple_shader.frag.spv",
        pipelineConfig);
}

//-------------------------------------------------------------------------------//

void App::CreateCommandBuffers() 
{
    commandBuffers.resize(swap_chain_.ImageCount());

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

    for (int i = 0; i < commandBuffers.size(); i++) 
    {
        VkCommandBufferBeginInfo beginInfo{};
        
        beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;

        if (vkBeginCommandBuffer(commandBuffers[i], &beginInfo) != VK_SUCCESS) 
            throw std::runtime_error("failed to begin recording command buffer!");

        VkRenderPassBeginInfo renderPassInfo{};
        
        renderPassInfo.sType        = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
        renderPassInfo.renderPass   = swap_chain_.GetRenderPass();
        renderPassInfo.framebuffer  = swap_chain_.GetFrameBuffer(i);

        renderPassInfo.renderArea.offset = {0, 0};
        renderPassInfo.renderArea.extent = swap_chain_.GetSwapChainExtent();

        std::array<VkClearValue, 2> clearValues{};
        
        clearValues[0].color            = {0.1f, 0.1f, 0.1f, 1.0f};
        clearValues[1].depthStencil     = {1.0f, 0};
        renderPassInfo.clearValueCount  = static_cast<uint32_t>(clearValues.size());
        renderPassInfo.pClearValues     = clearValues.data();

        vkCmdBeginRenderPass(commandBuffers[i], &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);

        pipeline_->Bind(commandBuffers[i]);
        model_->Bind(commandBuffers[i]);
        model_->Draw(commandBuffers[i]);

        vkCmdEndRenderPass(commandBuffers[i]);
        
        if (vkEndCommandBuffer(commandBuffers[i]) != VK_SUCCESS) 
            throw std::runtime_error("failed to record command buffer!");
    }
}

//-------------------------------------------------------------------------------//

void App::DrawFrame() 
{
    uint32_t imageIndex;
    
    auto result = swap_chain_.AcquireNextImage(&imageIndex);
    if (result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR) 
        throw std::runtime_error("failed to acquire swap chain image!");

    result = swap_chain_.SubmitCommandBuffers(&commandBuffers[imageIndex], &imageIndex);
    if (result != VK_SUCCESS) 
        throw std::runtime_error("failed to present swap chain image!");
}

//-------------------------------------------------------------------------------//

} // end of Vulkan namespace