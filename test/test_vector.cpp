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

int main() {
    TestFramework framework;
    framework.begin_suite("jSTL Vector Tests");

    test_sizeof_vector(framework);

    framework.print_summary();
    return framework.get_stats().failed;
}