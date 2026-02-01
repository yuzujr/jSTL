#include <string>

#include "test.h"
#include "vector/vector.h"

using namespace jstl;
using namespace jstl::test;

void test_sizeof_vector(TestFramework& framework) {
    framework.run_test("Size of Vector", []() {
        jstl::vector<int> vec_int;
        jstl::vector<double> vec_double;

        ASSERT_EQ(sizeof(vec_int), sizeof(void*) * 3);
        ASSERT_EQ(sizeof(vec_double), sizeof(void*) * 3);

        std::cout << "\nEBO applied: Size of vector<int> is " << sizeof(vec_int)
                  << ", Size of vector<double> is " << sizeof(vec_double) << "\n";
    });
}

void test_reserve_and_capacity(TestFramework& framework) {
    framework.run_test("Reserve and Capacity", []() {
        jstl::vector<int> vec;
        ASSERT_EQ(vec.capacity(), 0);

        vec.reserve(10);
        ASSERT_EQ(vec.capacity(), 10);
        ASSERT_EQ(vec.size(), 0);

        vec.push_back(1);
        ASSERT_EQ(vec.size(), 1);
        ASSERT_EQ(vec.capacity(), 10);
    });
}

void test_shrink_to_fit(TestFramework& framework) {
    framework.run_test("Shrink to Fit", []() {
        jstl::vector<int> vec;
        vec.reserve(100);
        vec.push_back(1);
        vec.push_back(2);

        ASSERT_EQ(vec.size(), 2);
        ASSERT_EQ(vec.capacity(), 100);

        vec.shrink_to_fit();
        ASSERT_EQ(vec.size(), 2);
        ASSERT_EQ(vec[0], 1);
        ASSERT_EQ(vec[1], 2);
    });
}

void test_insert(TestFramework& framework) {
    framework.run_test("Insert Operations", []() {
        jstl::vector<int> vec;
        vec.push_back(1);
        vec.push_back(3);

        // Insert single element
        auto it = vec.insert(vec.begin() + 1, 2);
        ASSERT_EQ(vec.size(), 3);
        ASSERT_EQ(vec[0], 1);
        ASSERT_EQ(vec[1], 2);
        ASSERT_EQ(vec[2], 3);
        ASSERT_EQ(*it, 2);

        // Insert multiple elements
        vec.insert(vec.begin(), 2, 0);
        ASSERT_EQ(vec.size(), 5);
        ASSERT_EQ(vec[0], 0);
        ASSERT_EQ(vec[1], 0);
        ASSERT_EQ(vec[2], 1);

        // Insert range
        jstl::vector<int> src = {10, 20, 30};
        vec.insert(vec.end(), src.begin(), src.end());
        ASSERT_EQ(vec.size(), 8);
        ASSERT_EQ(vec[5], 10);
        ASSERT_EQ(vec[6], 20);
        ASSERT_EQ(vec[7], 30);
    });
}

void test_erase(TestFramework& framework) {
    framework.run_test("Erase Operations", []() {
        jstl::vector<int> vec = {1, 2, 3, 4, 5};

        // Erase single element
        auto it = vec.erase(vec.begin() + 2);
        ASSERT_EQ(vec.size(), 4);
        ASSERT_EQ(vec[0], 1);
        ASSERT_EQ(vec[1], 2);
        ASSERT_EQ(vec[2], 4);
        ASSERT_EQ(vec[3], 5);
        ASSERT_EQ(*it, 4);

        // Erase range
        vec.erase(vec.begin() + 1, vec.begin() + 3);
        ASSERT_EQ(vec.size(), 2);
        ASSERT_EQ(vec[0], 1);
        ASSERT_EQ(vec[1], 5);
    });
}

void test_emplace(TestFramework& framework) {
    framework.run_test("Emplace Operations", []() {
        jstl::vector<std::string> vec;
        vec.push_back("a");
        vec.push_back("c");

        // Emplace in middle
        auto it = vec.emplace(vec.begin() + 1, "b");
        ASSERT_EQ(vec.size(), 3);
        ASSERT_EQ(vec[0], "a");
        ASSERT_EQ(vec[1], "b");
        ASSERT_EQ(vec[2], "c");
        ASSERT_EQ(*it, "b");

        // Emplace at end
        vec.emplace(vec.end(), "d");
        ASSERT_EQ(vec.size(), 4);
        ASSERT_EQ(vec[3], "d");
    });
}

void test_swap(TestFramework& framework) {
    framework.run_test("Swap Operations", []() {
        jstl::vector<int> vec1 = {1, 2, 3};
        jstl::vector<int> vec2 = {4, 5};

        vec1.swap(vec2);

        ASSERT_EQ(vec1.size(), 2);
        ASSERT_EQ(vec1[0], 4);
        ASSERT_EQ(vec1[1], 5);

        ASSERT_EQ(vec2.size(), 3);
        ASSERT_EQ(vec2[0], 1);
        ASSERT_EQ(vec2[1], 2);
        ASSERT_EQ(vec2[2], 3);
    });
}

void test_assign(TestFramework& framework) {
    framework.run_test("Assign Operations", []() {
        jstl::vector<int> vec = {1, 2, 3};

        // Assign with count and value
        vec.assign(5, 10);
        ASSERT_EQ(vec.size(), 5);
        for (int i = 0; i < 5; ++i) {
            ASSERT_EQ(vec[i], 10);
        }

        // Assign with range
        jstl::vector<int> src = {1, 2, 3, 4};
        vec.assign(src.begin(), src.end());
        ASSERT_EQ(vec.size(), 4);
        ASSERT_EQ(vec[0], 1);
        ASSERT_EQ(vec[1], 2);
        ASSERT_EQ(vec[2], 3);
        ASSERT_EQ(vec[3], 4);
    });
}

int main() {
    TestFramework framework;
    framework.begin_suite("jSTL Vector Tests");

    test_sizeof_vector(framework);
    test_reserve_and_capacity(framework);
    test_shrink_to_fit(framework);
    test_insert(framework);
    test_erase(framework);
    test_emplace(framework);
    test_swap(framework);
    test_assign(framework);

    framework.print_summary();
    return framework.get_stats().failed;
}
