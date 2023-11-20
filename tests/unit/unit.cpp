#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <fstream>
#include <vector>
#include <list>

#include "triangle.hpp"
#include "double_numbers.hpp"
#include "octree.hpp"
#include "intersection.hpp"

//================================================================================//

void EteTestRunner(const char* src, const char* ans)
{
    std::ifstream test_file{};

    test_file.open(src);

    size_t tr_numbers = 0;

    test_file >> tr_numbers;

    const std::clock_t start = clock();

    std::list<Triangle> triangles;

    double BoundingBoxX = 0, BoundingBoxY = 0, BoundingBoxZ = 0;

    for (int i = 0; i < tr_numbers; i++)
    {
        double p1x = 0, p1y = 0, p1z = 0, p2x = 0, p2y = 0, p2z = 0, p3x = 0, p3y = 0, p3z = 0;

        test_file >> p1x >> p1y >> p1z;
        Point p1(p1x, p1y, p1z);

        test_file >> p2x >> p2y >> p2z;
        Point p2(p2x, p2y, p2z);

        test_file >> p3x >> p3y >> p3z;
        Point p3(p3x, p3y, p3z);

        Triangle t(p1, p2, p3);

        t.number = i;

        triangles.push_back(t);

        if (BoundingBoxX < std::max(std::abs(p1.X()), std::max(std::abs(p2.X()), std::abs(p3.X()))))
            BoundingBoxX = std::max(std::abs(p1.X()), std::max(std::abs(p2.X()), std::abs(p3.X())));

        if (BoundingBoxY < std::max(std::abs(p1.Y()), std::max(std::abs(p2.Y()), std::abs(p3.Y()))))
            BoundingBoxY = std::max(std::abs(p1.Y()), std::max(std::abs(p2.Y()), std::abs(p3.Y())));

        if (BoundingBoxZ < std::max(std::abs(p1.Z()), std::max(std::abs(p2.Z()), std::abs(p3.Z()))))
            BoundingBoxZ = std::max(std::abs(p1.Z()), std::max(std::abs(p2.Z()), std::abs(p3.Z())));
    }

    Point bounding_box(BoundingBoxX, BoundingBoxY, BoundingBoxZ);

    OctreeNew oct(triangles, bounding_box);

    bool *FlagArray = new bool [tr_numbers]{};

    FindIntersectionsInNode(oct.root_, FlagArray);

    std::vector<int> intersecting_triangles{};

    for (size_t i = 0; i < tr_numbers; i++)
    {
        if (FlagArray[i])
            intersecting_triangles.push_back(i);
    }

    test_file.close();

    std::ifstream ans_file{};

    ans_file.open(ans);

    std::vector<int> answers{};

    while(ans_file.good())
    {
        int elem = 0;

        ans_file >> elem;

        answers.push_back(elem);
    }

    ans_file.close();

    for (int i = 0; i < intersecting_triangles.size(); i++)
        ASSERT_EQ(intersecting_triangles[i], answers[i]);

}

//================================================================================//

TEST(EteTest, test1)
{
    EteTestRunner("../../../tests/ete/001.dat", "../../../tests/ete/001.answer");
}

//================================================================================//

TEST(EteTest, test2)
{
    EteTestRunner("../../../tests/ete/002.dat", "../../../tests/ete/002.answer");
}

//================================================================================//

TEST(EteTest, test3)
{
    EteTestRunner("../../../tests/ete/003.dat", "../../../tests/ete/003.answer");
}

//================================================================================//

TEST(EteTest, test4)
{
    EteTestRunner("../../../tests/ete/004.dat", "../../../tests/ete/004.answer");
}

//================================================================================//

TEST(EteTest, test5)
{
    EteTestRunner("../../../tests/ete/005.dat", "../../../tests/ete/005.answer");
}

//================================================================================//

TEST(EteTest, test6)
{
    EteTestRunner("../../../tests/ete/006.dat", "../../../tests/ete/006.answer");
}

//================================================================================//

TEST(EteTest, test7)
{
    EteTestRunner("../../../tests/ete/007.dat", "../../../tests/ete/007.answer");
}

//================================================================================//

TEST(EteTest, test8)
{
    EteTestRunner("../../../tests/ete/008.dat", "../../../tests/ete/008.answer");
}

//================================================================================//

TEST(EteTest, test9)
{
    EteTestRunner("../../../tests/ete/009.dat", "../../../tests/ete/009.answer");
}

//================================================================================//

TEST(EteTest, test10)
{
    EteTestRunner("../../../tests/ete/0010.dat", "../../../tests/ete/0010.answer");
}

//================================================================================//

TEST(EteTest, test11)
{
    EteTestRunner("../../../tests/ete/011.dat", "../../../tests/ete/011.answer");
}

//================================================================================//

TEST(EteTest, test12)
{
    EteTestRunner("../../../tests/ete/012.dat", "../../../tests/ete/012.answer");
}

//================================================================================//

TEST(EteTest, test13)
{
    EteTestRunner("../../../tests/ete/013.dat", "../../../tests/ete/013.answer");
}

//================================================================================//

TEST(EteTest, test15)
{
    EteTestRunner("../../../tests/ete/015.dat", "../../../tests/ete/015.answer");
}

//================================================================================//

TEST(EteTest, test16)
{
    EteTestRunner("../../../tests/ete/016.dat", "../../../tests/ete/016.answer");
}

//================================================================================//

TEST(EteTest, test17)
{
    EteTestRunner("../../../tests/ete/017.dat", "../../../tests/ete/017.answer");
}

//================================================================================//

TEST(EteTest, test18)
{
    EteTestRunner("../../../tests/ete/018.dat", "../../../tests/ete/018.answer");
}

//================================================================================//

TEST(EteTest, test19)
{
    EteTestRunner("../../../tests/ete/019.dat", "../../../tests/ete/019.answer");
}

//================================================================================//

TEST(EteTest, test20)
{
    EteTestRunner("../../../tests/ete/020.dat", "../../../tests/ete/020.answer");
}

//================================================================================//

TEST(EteTest, test21)
{
    EteTestRunner("../../../tests/ete/021.dat", "../../../tests/ete/021.answer");
}

//================================================================================//

int main(int argc, char** argv)
{
    ::testing::InitGoogleTest(&argc, argv);

    return RUN_ALL_TESTS();
}

//================================================================================//