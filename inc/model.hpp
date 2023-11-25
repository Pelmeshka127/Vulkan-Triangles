#ifndef MODEL_HPP_
#define MODEL_HPP_

#include "device.hpp"

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>

#include <vector>

namespace Vulkan 
{

//-------------------------------------------------------------------------------//

class Model 
{
    public:    
    
        struct Vertex {
            glm::vec2 position;
            glm::vec3 color;

            static std::vector<VkVertexInputBindingDescription> GetBindingDescriptions();
            static std::vector<VkVertexInputAttributeDescription> GetAttributeDescriptions();
        };

        Model(Device &device, const std::vector<Vertex> &vertices);
        
        ~Model();

        Model(const Model &) = delete;
        
        Model &operator=(const Model &) = delete;

        void Bind(VkCommandBuffer commandBuffer);
        
        void Draw(VkCommandBuffer commandBuffer);

    private:
        
        void CreateVertexBuffers(const std::vector<Vertex> &vertices);

        Device          &Device_;
        
        VkBuffer        vertexBuffer_;
        
        VkDeviceMemory  vertexBufferMemory_;
        
        uint32_t        vertexCount_;
};

//-------------------------------------------------------------------------------//

}  // end of Vulkan namespace 

#endif