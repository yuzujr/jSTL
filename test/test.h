#ifndef _JSTL_TEST_H
#define _JSTL_TEST_H

#include <chrono>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

namespace jstl {
namespace test {

// 测试结果
struct TestResult {
    std::string name;
    bool passed;
    std::string message;
    double duration_ms;
};

// 测试统计
struct TestStats {
    int total = 0;
    int passed = 0;
    int failed = 0;
    double total_time_ms = 0;
};

// 测试框架类
class TestFramework {
private:
    std::vector<TestResult> results_;
    TestStats stats_;
    std::string current_suite_;

public:
    TestFramework() = default;

    // 开始一个测试套件
    void begin_suite(const std::string& suite_name) {
        current_suite_ = suite_name;
        std::cout << "\n========================================\n";
        std::cout << "Test Suite: " << suite_name << "\n";
        std::cout << "========================================\n";
    }

    // 运行单个测试
    template <typename Func>
    void run_test(const std::string& test_name, Func&& test_func) {
        std::cout << "Running: " << test_name << " ... ";
        std::cout.flush();

        auto start = std::chrono::high_resolution_clock::now();
        bool passed = false;
        std::string message;

        try {
            test_func();
            passed = true;
            message = "OK";
        } catch (const std::exception& e) {
            passed = false;
            message = std::string("Exception: ") + e.what();
        } catch (...) {
            passed = false;
            message = "Unknown exception";
        }

        auto end = std::chrono::high_resolution_clock::now();
        double duration_ms = std::chrono::duration<double, std::milli>(end - start).count();

        results_.push_back({test_name, passed, message, duration_ms});
        stats_.total++;
        if (passed) {
            stats_.passed++;
            std::cout << "✓ PASSED";
        } else {
            stats_.failed++;
            std::cout << "✗ FAILED";
        }
        std::cout << " (" << duration_ms << " ms)\n";

        if (!passed) {
            std::cout << "  Error: " << message << "\n";
        }
    }

    // 打印测试报告
    void print_summary() {
        std::cout << "\n========================================\n";
        std::cout << "Test Summary\n";
        std::cout << "========================================\n";
        std::cout << "Total:  " << stats_.total << "\n";
        std::cout << "Passed: " << stats_.passed << " ✓\n";
        std::cout << "Failed: " << stats_.failed << " ✗\n";

        if (stats_.failed > 0) {
            std::cout << "\nFailed Tests:\n";
            for (const auto& result : results_) {
                if (!result.passed) {
                    std::cout << "  - " << result.name << ": " << result.message << "\n";
                }
            }
        }

        std::cout << "\nAll tests completed.\n";
        std::cout << "========================================\n";
    }

    // 获取统计信息
    const TestStats& get_stats() const {
        return stats_;
    }

    // 清除结果
    void reset() {
        results_.clear();
        stats_ = TestStats{};
        current_suite_.clear();
    }
};

// 断言宏
#define ASSERT_TRUE(condition)                                                    \
    do {                                                                          \
        if (!(condition)) {                                                       \
            std::ostringstream oss;                                               \
            oss << "Assertion failed: " << #condition << " at line " << __LINE__; \
            throw std::runtime_error(oss.str());                                  \
        }                                                                         \
    } while (0)

#define ASSERT_FALSE(condition) ASSERT_TRUE(!(condition))

#define ASSERT_EQ(a, b)                                                                   \
    do {                                                                                  \
        if (!((a) == (b))) {                                                              \
            std::ostringstream oss;                                                       \
            oss << "Assertion failed: " << #a << " == " << #b << " at line " << __LINE__; \
            throw std::runtime_error(oss.str());                                          \
        }                                                                                 \
    } while (0)

#define ASSERT_NE(a, b)                                                                   \
    do {                                                                                  \
        if (!((a) != (b))) {                                                              \
            std::ostringstream oss;                                                       \
            oss << "Assertion failed: " << #a << " != " << #b << " at line " << __LINE__; \
            throw std::runtime_error(oss.str());                                          \
        }                                                                                 \
    } while (0)

#define ASSERT_THROW(expression, exception_type)                                                   \
    do {                                                                                           \
        bool caught = false;                                                                       \
        try {                                                                                      \
            expression;                                                                            \
        } catch (const exception_type&) {                                                          \
            caught = true;                                                                         \
        }                                                                                          \
        if (!caught) {                                                                             \
            std::ostringstream oss;                                                                \
            oss << "Expected exception " << #exception_type << " not thrown at line " << __LINE__; \
            throw std::runtime_error(oss.str());                                                   \
        }                                                                                          \
    } while (0)

}  // namespace test
}  // namespace jstl

#endif  // _JSTL_TEST_H
