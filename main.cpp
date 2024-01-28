// #include <ctime>
#include <vector>
#include <fstream>
#include <set>

#include "triangle.hpp"
#include "octree.hpp"
#include "coordinates.hpp"
#include "intersection.hpp"
#include "app.hpp"
#include "model.hpp"

//-------------------------------------------------------------------------------//

int main()
{
    size_t tr_numbers = 0;

    std::cin >> tr_numbers;

    std::list<Triangle> triangles;

    double BoundingBoxX = 0, BoundingBoxY = 0, BoundingBoxZ = 0;

    for (int i = 0; i < tr_numbers; i++)
    {
        double p1x = 0, p1y = 0, p1z = 0, p2x = 0, p2y = 0, p2z = 0, p3x = 0, p3y = 0, p3z = 0;

        std::cin >> p1x >> p1y >> p1z;
        Point p1(p1x, p1y, p1z);

        std::cin >> p2x >> p2y >> p2z;
        Point p2(p2x, p2y, p2z);

        std::cin >> p3x >> p3y >> p3z;
        Point p3(p3x, p3y, p3z);

        Triangle t(p1, p2, p3);

        t.number = i;

        triangles.push_back(t);

        if (BoundingBoxX < p1.MaxCoordinate(p2, p3, cords::X))
            BoundingBoxX = p1.MaxCoordinate(p2, p3, cords::X);

        if (BoundingBoxY < p1.MaxCoordinate(p2, p3, cords::Y))
            BoundingBoxY = p1.MaxCoordinate(p2, p3, cords::Y);

        if (BoundingBoxZ < p1.MaxCoordinate(p1, p2, cords::Z))
            BoundingBoxZ = p1.MaxCoordinate(p1, p2, cords::Z);
    }

    Point bounding_box(BoundingBoxX, BoundingBoxY, BoundingBoxZ);

    OctreeNew oct(triangles, bounding_box);

    bool *FlagArray = new bool [tr_numbers]{};

    // std::clock_t start = clock();

    FindIntersectionsInNode(oct.root_, FlagArray);

    Vulkan::Model::Builder builder{};

    glm::vec3 red_color     = {1.f, 0.f, 0.f};

    glm::vec3 blue_color    = {0.f, 0.f, 1.f};

    glm::vec3 color         = {0.f, 0.f, 0.f};

    auto triangle = triangles.begin();

    for (size_t i = 0; i < tr_numbers; i++, triangle++)
    {
        if (FlagArray[i])
        {
            color = red_color;

            std::cout << i << std::endl;
        }

        else
            color = blue_color;

        builder.vertices.push_back({Vulkan::GetGlmVector(triangle->P1()), color, Vulkan::GetNormal(*triangle)});

        builder.vertices.push_back({Vulkan::GetGlmVector(triangle->P2()), color, Vulkan::GetNormal(*triangle)});

        builder.vertices.push_back({Vulkan::GetGlmVector(triangle->P3()), color, Vulkan::GetNormal(*triangle)});
    }

    for (uint32_t i = 0; i < tr_numbers * 3; i++)
        builder.indices.push_back(i);

    Vulkan::App app{builder};

    app.RunApplication();

    return 0;
}

// -------------------------------------------------------------------------------//