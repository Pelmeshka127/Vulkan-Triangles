#include "inc/app.hpp"
#include "inc/model.hpp"
#include <ctime>
#include <vector>
#include <fstream>
#include <set>
#include <iostream>
#include <list>

glm::vec3 GetNormal(const double p1, const double p2, const double p3, 
                    const double p4, const double p5, const double p6,
                    const double p7, const double p8, const double p9);

// #include "triangle.hpp"
// #include "octree.hpp"
// #include "coordinates.hpp"
// #include "intersection.hpp"

int main()
{
    size_t tr_numbers = 0;

    std::cin >> tr_numbers;

    Vulkan::Model::Builder builder{};

    glm::vec3 red_color  = {1.0f, 0.f, 0.f};

    glm::vec3 blue_color = {0.0f, 0.f, 1.f};

    for (int i = 0; i < tr_numbers; i++)
    {
        double p1x = 0, p1y = 0, p1z = 0, p2x = 0, p2y = 0, p2z = 0, p3x = 0, p3y = 0, p3z = 0;

        std::cin >> p1x >> p1y >> p1z;

        // builder.vertices.push_back({{p1x, p1y, p1z}, red_color});

        std::cin >> p2x >> p2y >> p2z;

        // builder.vertices.push_back({{p2x, p2y, p2z}, red_color});

        std::cin >> p3x >> p3y >> p3z;

        // builder.vertices.push_back({{p3x, p3y, p3z}, red_color});

        builder.vertices.push_back({{p1x, p1y, p1z}, red_color, GetNormal(p1x, p1y, p1z, p2x, p2y, p2z, p3x, p3y, p3z)});

        builder.vertices.push_back({{p2x, p2y, p2z}, red_color, GetNormal(p1x, p1y, p1z, p2x, p2y, p2z, p3x, p3y, p3z)});

        builder.vertices.push_back({{p3x, p3y, p3z}, red_color, GetNormal(p1x, p1y, p1z, p2x, p2y, p2z, p3x, p3y, p3z)});
    }

    for (uint32_t i = 0; i < tr_numbers * 3; i++)
        builder.indices.push_back(i);

    Vulkan::App app{builder};

    app.RunApplication();

    return 0;
}

glm::vec3 GetNormal(const double p1, const double p2, const double p3, 
                    const double p4, const double p5, const double p6,
                    const double p7, const double p8, const double p9)
{
    glm::vec3 side1{}, side2{};

    side1.x = p4 - p1;
    side1.y = p5 - p2;
    side1.z = p6 - p3;

    side2.x = p7 - p1;
    side2.y = p8 - p2;
    side2.z = p9 - p3;

    return glm::normalize(glm::cross(side1, side2));
}