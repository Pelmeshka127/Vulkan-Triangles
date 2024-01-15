#include "inc/app.hpp"
#include "inc/model.hpp"
#include <ctime>
#include <vector>
#include <fstream>
#include <set>
#include <iostream>
#include <list>

// #include "triangle.hpp"
// #include "octree.hpp"
// #include "coordinates.hpp"
// #include "intersection.hpp"

int main()
{
    size_t tr_numbers = 0;

    std::cin >> tr_numbers;

    Vulkan::Model::Builder builder{};

    glm::vec3 red_color = {1.0f, 0.f, 0.f};

    glm::vec3 blue_color = {0.0f, 0.f, 1.f};

    for (int i = 0; i < tr_numbers; i++)
    {
        double p1x = 0, p1y = 0, p1z = 0, p2x = 0, p2y = 0, p2z = 0, p3x = 0, p3y = 0, p3z = 0;

        std::cin >> p1x >> p1y >> p1z;

        builder.vertices.push_back({{p1x, p1y, p1z}, red_color});

        std::cin >> p2x >> p2y >> p2z;

        builder.vertices.push_back({{p2x, p2y, p2z}, red_color});

        std::cin >> p3x >> p3y >> p3z;

        builder.vertices.push_back({{p3x, p3y, p3z}, red_color});
    }

    for (uint32_t i = 0; i < tr_numbers * 3; i++)
        builder.indices.push_back(i);

    Vulkan::App app{builder};

    app.RunApplication();

    return 0;
}