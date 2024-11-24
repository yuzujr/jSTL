// todo:尽可能地更换设施，这不是jSTL的一部分

#include <process.h>
#include <windows.h>

#include <iostream>
#include <memory>
#include <stdexcept>
#include <utility>

#include "utility.h"
#include "functional.h"

// todo:jSTL化

namespace my {

using thread_id_t = unsigned int;

namespace this_thread {
inline thread_id_t get_id() noexcept {
    return GetCurrentThreadId();
}
}  // namespace this_thread

class thread {
    struct thread_t {
        void* _handle;    // 线程句柄
        thread_id_t _id;  // 线程id
    };
    thread_t _thr;

public:
    // 默认构造
    thread() noexcept : _thr{} {}

    // 移动构造
    thread(thread&& _other) noexcept : _thr(exchange(_other._thr, {})) {}
    thread& operator=(thread&& t) noexcept {
        if (joinable()) {
            terminate();
        }
        swap(t);
        return *this;
    }

    // 禁用拷贝构造
    thread(const thread&) = delete;
    thread& operator=(const thread&) = delete;

    // 有参构造
    template <
        class Fn, class... Args,
        std::enable_if_t<!std::is_same_v<std::remove_cvref_t<Fn>, my::thread>,
                         int> = 0>  // Fn不能是thread类型
    explicit thread(Fn&& fn, Args&&... args) {
        _start(
            std::forward<Fn>(fn),
            std::forward<Args>(args)...);  // 完美转发到_start函数完成真正的工作
    }

    // 析构函数
    ~thread() {
        if (joinable()) {
            terminate();
        }
    }

    void swap(thread& t) noexcept {
        std::swap(_thr, t._thr);
    }

    bool joinable() const noexcept {
        return !(_thr._handle == nullptr);
    }

    void join() {
        if (!joinable()) {
            throw std::runtime_error("Thread is not joinable");
        }
        WaitForSingleObject(_thr._handle, INFINITE);
        CloseHandle(_thr._handle);
        _thr = {};
    }

    void detach() {
        if (!joinable()) {
            throw std::runtime_error("Thread is not joinable");
        }
        CloseHandle(_thr._handle);
        _thr = {};
    }

    thread_id_t get_id() const noexcept;

    friend bool operator==(const thread& left, const thread& right) noexcept;

    template <class Ch, class Tr>
    friend auto operator<<(std::basic_ostream<Ch, Tr>& stream,
                           const thread& th);

private:
    template <class _tuple, size_t... _indices>
    static unsigned int _invoke(void* _rawVals) noexcept {
        const std::unique_ptr<_tuple> _fnVals{static_cast<_tuple*>(_rawVals)};
        _tuple& _tup = *_fnVals.get();
        invoke(std::move(std::get<_indices>(_tup))...);
        return 0;
    }

    template <class _tuple, size_t... _indices>
    static constexpr auto _get_invoke(
        std::index_sequence<_indices...>) noexcept {
        return &_invoke<_tuple, _indices...>;
    }

    template <class Fn, class... Args>
    void _start(Fn&& fn, Args&&... args) {
        using _tuple = std::tuple<std::decay_t<Fn>, std::decay_t<Args>...>;

        std::unique_ptr<_tuple> _decay_copied = std::make_unique<_tuple>(
            std::forward<Fn>(fn), std::forward<Args>(args)...);

        constexpr auto _invoker_proc = this->_get_invoke<_tuple>(
            std::make_index_sequence<1 + sizeof...(Args)>{});

        _thr._handle = reinterpret_cast<void*>(_beginthreadex(
            nullptr, 0, _invoker_proc, _decay_copied.get(), 0, &_thr._id));

        if (_thr._handle) {
            (void)_decay_copied.release();
        } else {
            _thr._id = 0;
            std::cerr << "Error creating thread: " << _thr._handle << std::endl;
            throw std::runtime_error("Error creating thread");
        }
    }
};

inline thread_id_t thread::get_id() const noexcept {
    return _thr._id;
}

inline bool operator==(const thread& left, const thread& right) noexcept {
    return left._thr._id == right._thr._id;
}

template <class Ch, class Tr>
inline auto operator<<(std::basic_ostream<Ch, Tr>& stream, const thread& t) {
    stream << t._thr._id;
    return stream;
}

}  // namespace my