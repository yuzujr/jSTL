#include <atomic>
#include <chrono>
#include <iostream>
#include <thread>

#include "test.h"
#include "thread.h"

using namespace jstl;
using namespace jstl::test;

// 全局变量用于测试
std::atomic<int> counter{0};
std::atomic<bool> flag{false};

// 测试函数
void simple_function() {
    counter++;
}

void function_with_args(int a, int b) {
    counter += a + b;
}

void sleep_function(int ms) {
    std::this_thread::sleep_for(std::chrono::milliseconds(ms));
    flag = true;
}

class Callable {
public:
    void operator()() {
        counter += 10;
    }
};

void test_default_constructor(TestFramework& framework) {
    framework.run_test("Default Constructor", []() {
        thread t;
        ASSERT_FALSE(t.joinable());
    });
}

void test_function_thread(TestFramework& framework) {
    framework.run_test("Thread with Function", []() {
        counter = 0;
        thread t(simple_function);
        ASSERT_TRUE(t.joinable());
        t.join();
        ASSERT_FALSE(t.joinable());
        ASSERT_EQ(counter.load(), 1);
    });
}

void test_function_with_arguments(TestFramework& framework) {
    framework.run_test("Thread with Arguments", []() {
        counter = 0;
        thread t(function_with_args, 5, 10);
        t.join();
        ASSERT_EQ(counter.load(), 15);
    });
}

void test_lambda_thread(TestFramework& framework) {
    framework.run_test("Thread with Lambda", []() {
        counter = 0;
        thread t([]() {
            counter = 42;
        });
        t.join();
        ASSERT_EQ(counter.load(), 42);
    });
}

void test_lambda_with_capture(TestFramework& framework) {
    framework.run_test("Thread with Lambda Capture", []() {
        int value = 100;
        counter = 0;
        thread t([value]() {
            counter = value;
        });
        t.join();
        ASSERT_EQ(counter.load(), 100);
    });
}

void test_callable_object(TestFramework& framework) {
    framework.run_test("Thread with Callable Object", []() {
        counter = 0;
        Callable callable;
        thread t(callable);
        t.join();
        ASSERT_EQ(counter.load(), 10);
    });
}

void test_move_constructor(TestFramework& framework) {
    framework.run_test("Move Constructor", []() {
        counter = 0;
        thread t1(simple_function);
        ASSERT_TRUE(t1.joinable());

        thread t2(std::move(t1));
        ASSERT_FALSE(t1.joinable());
        ASSERT_TRUE(t2.joinable());

        t2.join();
        ASSERT_EQ(counter.load(), 1);
    });
}

void test_move_assignment(TestFramework& framework) {
    framework.run_test("Move Assignment", []() {
        counter = 0;
        thread t1(simple_function);
        thread t2;

        ASSERT_TRUE(t1.joinable());
        ASSERT_FALSE(t2.joinable());

        t2 = std::move(t1);
        ASSERT_FALSE(t1.joinable());
        ASSERT_TRUE(t2.joinable());

        t2.join();
        ASSERT_EQ(counter.load(), 1);
    });
}

void test_detach(TestFramework& framework) {
    framework.run_test("Detach Thread", []() {
        flag = false;
        thread t(sleep_function, 100);
        ASSERT_TRUE(t.joinable());

        t.detach();
        ASSERT_FALSE(t.joinable());

        // 等待线程完成
        std::this_thread::sleep_for(std::chrono::milliseconds(200));
        ASSERT_TRUE(flag.load());
    });
}

void test_swap(TestFramework& framework) {
    framework.run_test("Swap Threads", []() {
        counter = 0;
        thread t1([]() {
            counter = 1;
        });
        thread t2([]() {
            counter = 2;
        });

        ASSERT_TRUE(t1.joinable());
        ASSERT_TRUE(t2.joinable());

        t1.swap(t2);

        ASSERT_TRUE(t1.joinable());
        ASSERT_TRUE(t2.joinable());

        t1.join();
        t2.join();
    });
}

void test_get_id(TestFramework& framework) {
    framework.run_test("Get Thread ID", []() {
        thread t([]() {
            std::this_thread::sleep_for(std::chrono::milliseconds(50));
        });

        thread_id_t id = t.get_id();
        ASSERT_NE(id, 0);

        t.join();
    });
}

void test_multiple_threads(TestFramework& framework) {
    framework.run_test("Multiple Threads", []() {
        counter = 0;
        const int num_threads = 10;
        std::vector<thread> threads;

        for (int i = 0; i < num_threads; ++i) {
            threads.emplace_back([]() {
                counter++;
            });
        }

        for (auto& t : threads) {
            t.join();
        }

        ASSERT_EQ(counter.load(), num_threads);
    });
}

void test_join_non_joinable_throws(TestFramework& framework) {
    framework.run_test("Join Non-Joinable Throws", []() {
        thread t;
        ASSERT_FALSE(t.joinable());

        // join一个不可join的线程应该抛出异常
        ASSERT_THROW(t.join(), std::runtime_error);
    });
}

void test_detach_non_joinable_throws(TestFramework& framework) {
    framework.run_test("Detach Non-Joinable Throws", []() {
        thread t;
        ASSERT_FALSE(t.joinable());

        // detach一个不可join的线程应该抛出异常
        ASSERT_THROW(t.detach(), std::runtime_error);
    });
}

void test_reference_arguments(TestFramework& framework) {
    framework.run_test("Thread with Reference Arguments", []() {
        int value = 0;
        // 注意：这里需要使用std::ref来传递引用
        thread t(
            [](int& val) {
                val = 99;
            },
            std::ref(value));
        t.join();
        ASSERT_EQ(value, 99);
    });
}

void test_thread_performance(TestFramework& framework) {
    framework.run_test("Thread Creation Performance", []() {
        auto start = std::chrono::high_resolution_clock::now();

        const int num_threads = 100;
        std::vector<thread> threads;

        for (int i = 0; i < num_threads; ++i) {
            threads.emplace_back([]() {
                // 最小工作量
            });
        }

        for (auto& t : threads) {
            t.join();
        }

        auto end = std::chrono::high_resolution_clock::now();
        auto duration =
            std::chrono::duration_cast<std::chrono::milliseconds>(end - start)
                .count();

        std::cout << "    Created and joined " << num_threads << " threads in "
                  << duration << " ms\n";
        ASSERT_TRUE(true);
    });
}

int main() {
    TestFramework framework;

    framework.begin_suite("jSTL Thread Tests");

    // 基础功能测试
    test_default_constructor(framework);
    test_function_thread(framework);
    test_function_with_arguments(framework);
    test_lambda_thread(framework);
    test_lambda_with_capture(framework);
    test_callable_object(framework);

    // 移动语义测试
    test_move_constructor(framework);
    test_move_assignment(framework);

    // 线程管理测试
    test_detach(framework);
    test_swap(framework);
    test_get_id(framework);

    // 多线程测试
    test_multiple_threads(framework);

    // 异常处理测试
    test_join_non_joinable_throws(framework);
    test_detach_non_joinable_throws(framework);

    // 高级功能测试
    test_reference_arguments(framework);
    test_thread_performance(framework);

    // 打印测试报告
    framework.print_summary();

    // 返回失败的测试数量
    return framework.get_stats().failed;
}
