#ifndef PIPELINE_HPP_
#define PIPELINE_HPP_

#include <vector>
#include <string>
#include <iostream>

namespace Vulkan
{

//-------------------------------------------------------------------------------//

class Pipeline
{

public:

    Pipeline(const std::string& VertFilepath, const std::string& FragFilepath);

    ~Pipeline();

    Pipeline(const Pipeline& rhs) = delete;

    Pipeline& operator=(const Pipeline& rhs) = delete;

private:

    static std::vector<char> ReadFile(const std::string& filepath);

    void CreateGraphicsPipeline(const std::string& VertFilepath, const std::string& FragFilepath);

};

//-------------------------------------------------------------------------------//

} // end of Vulkan namespace

#endif