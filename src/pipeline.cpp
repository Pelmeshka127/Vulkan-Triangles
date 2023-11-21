#include "../inc/pipeline.hpp"

#include <fstream>

namespace Vulkan
{

//-------------------------------------------------------------------------------//

Pipeline::Pipeline(const std::string& VertPath, const std::string& FragPath)
{
    CreateGraphicsPipeline(VertPath, FragPath);
}

//-------------------------------------------------------------------------------//

std::vector<char> Pipeline::ReadFile(const std::string& filepath) 
{
    std::ifstream file{filepath, std::ios::ate | std::ios::binary};

    if (!file.is_open()) 
    {
        throw std::runtime_error("failed to open file: " + filepath);
    }

    size_t file_size = static_cast<size_t>(file.tellg());
    
    std::vector<char> buffer(file_size);

    file.seekg(0);
    
    file.read(buffer.data(), file_size);

    file.close();
    
    return buffer;
}

//-------------------------------------------------------------------------------//

void Pipeline::CreateGraphicsPipeline(const std::string& VertPath, const std::string& FragPath)
{
    auto vertCode = ReadFile(VertPath);
    
    auto fragCode = ReadFile(FragPath);

    std::cout << "Vertex Shader Code Size: " << vertCode.size() << '\n';
    
    std::cout << "Fragment Shader Code Size: " << fragCode.size() << '\n';
}

//-------------------------------------------------------------------------------//

} // end of Vulkan namespace