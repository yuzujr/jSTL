#ifndef _JSTL_THREAD_H
#define _JSTL_THREAD_H

#ifdef _WIN32
#include <process.h>
#include <windows.h>
#else
#include <pthread.h>
#endif

#include <iostream>
#include <memory>
#include <cstddef>

#include "functional/invoke.h"
#include "type_traits/decay.h"
#include "type_traits/enable_if.h"
#include "type_traits/is_same.h"
#include "type_traits/remove_cvref.h"
#include "utility/exchange.h"
#include "utility/forward.h"
#include "utility/move.h"

namespace jstl {

using thread_id_t = unsigned long;

namespace this_thread {
thread_id_t get_id() noexcept;
}

class thread {
    struct thread_t {
#ifdef _WIN32
        void* _handle;  // 线程句柄
#else
        pthread_t _handle;  // 线程句柄
#endif
        thread_id_t _id;  // 线程id
        bool _joinable;
    };
    thread_t _thr;

public:
    // 默认构造
    thread() noexcept : _thr{} {}

    // 移动构造
    thread(thread&& _other) noexcept : _thr(exchange(_other._thr, {})) {}
    thread& operator=(thread&& t) noexcept {
        if (joinable()) {
            std::terminate();
        }
        swap(t);
        return *this;
    }

    // 禁用拷贝构造
    thread(const thread&) = delete;
    thread& operator=(const thread&) = delete;

    // 有参构造
    template <class Fn, class... Args,
              enable_if_t<!is_same_v<remove_cvref_t<Fn>, jstl::thread>,
                          int> = 0>  // Fn不能是thread类型
    explicit thread(Fn&& fn, Args&&... args) {
        _start(forward<Fn>(fn),
               jstl::forward<Args>(
                   args)...);  // 完美转发到_start函数完成真正的工作
    }

    // 析构函数
    ~thread() {
        if (joinable()) {
            std::terminate();
        }
    }

    void swap(thread& t) noexcept {
        std::swap(_thr, t._thr);
    }

    bool joinable() const noexcept {
        return _thr._joinable;
    }

    void join() {
        if (!joinable()) {
            throw std::runtime_error("Thread is not joinable");
        }
        _join();
#ifdef _WIN32
        CloseHandle(_thr._handle);
#endif
        _thr = {};
    }

    void detach() {
        if (!joinable()) {
            throw std::runtime_error("Thread is not joinable");
        }
#ifdef _WIN32
        CloseHandle(_thr._handle);
#else
        pthread_detach(_thr._handle);
#endif
        _thr = {};
    }

    thread_id_t get_id() const noexcept;

    friend bool operator==(const thread& left, const thread& right) noexcept;

    template <class Ch, class Tr>
    friend auto operator<<(std::basic_ostream<Ch, Tr>& stream,
                           const thread& th);

private:
#ifdef _WIN32
    template <class _tuple, size_t... _indices>
    static unsigned int _invoke(void* _rawVals) {
        const std::unique_ptr<_tuple> _fnVals{static_cast<_tuple*>(_rawVals)};
        _tuple& _tup = *_fnVals.get();
        try {
            invoke(jstl::move(std::get<_indices>(_tup))...);
        } catch (...) {
            std::terminate();
        }
        return 0;
    }

    template <class _tuple, size_t... _indices>
    static auto _get_invoke(std::index_sequence<_indices...>) noexcept {
        return &_invoke<_tuple, _indices...>;
    }
#else
    template <class _tuple, size_t... _indices>
    static void* _invoke(void* _rawVals) {
        const std::unique_ptr<_tuple> _fnVals{static_cast<_tuple*>(_rawVals)};
        _tuple& _tup = *_fnVals.get();
        try {
            invoke(jstl::move(std::get<_indices>(_tup))...);
        } catch (...) {
            std::terminate();
        }
        return nullptr;
    }

    template <class _tuple, size_t... _indices>
    static auto _get_invoke(std::index_sequence<_indices...>) noexcept {
        return &_invoke<_tuple, _indices...>;
    }
#endif

    template <class Fn, class... Args>
    void _start(Fn&& fn, Args&&... args) {
        using _tuple = std::tuple<decay_t<Fn>, decay_t<Args>...>;

        std::unique_ptr<_tuple> _decay_copied = std::make_unique<_tuple>(
            jstl::forward<Fn>(fn), jstl::forward<Args>(args)...);

        auto _invoker_proc = _get_invoke<_tuple>(
            std::make_index_sequence<1 + sizeof...(Args)>{});

        bool created = false;
#ifdef _WIN32
        _thr._handle = reinterpret_cast<void*>(_beginthreadex(
            nullptr, 0, _invoker_proc, _decay_copied.get(), 0, &_thr._id));
        created = (_thr._handle != nullptr);
#else
        pthread_t tid;
        int result =
            pthread_create(&tid, nullptr, _invoker_proc, _decay_copied.get());
        if (result == 0) {
            _thr._handle = tid;
            _thr._id = static_cast<thread_id_t>(tid);
            created = true;
        } else {
            _thr = {};
        }
#endif

        if (created) {
            (void)_decay_copied.release();
            _thr._joinable = true;
        } else {
            _thr._id = 0;
            _thr._joinable = false;
            std::cerr << "Error creating thread" << std::endl;
            throw std::runtime_error("Error creating thread");
        }
    }

    void _join() noexcept {
#ifdef _WIN32
        WaitForSingleObject(static_cast<HANDLE>(_thr._handle), INFINITE);
#else
        pthread_join(_thr._handle, nullptr);
#endif
    }
};

}  // namespace jstl

#endif
