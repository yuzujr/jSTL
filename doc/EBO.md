# EBO (Empty Base Optimization)

EBO is a compiler technique that optimizes memory layout by allowing derived classes to inherit from empty base classes without consuming additional space.

An empty class is a class that contains no non-static data members.

In JSTL, `jstl::allocator` is an empty class. it does not occupy any space in the `jstl::vector` object, thus reducing memory overhead.

## Details
```cpp
private:
    pointer __begin_ = nullptr;
    pointer __end_ = nullptr;
    __compressed_pair<pointer, allocator_type> __cap_and_alloc_;
```
if allocator_type is an empty class, `__compressed_pair` will optimize the memory layout to avoid wasting space for the empty allocator.

## Test
```cpp
jstl::vector<int> vec_int;
jstl::vector<double> vec_double;

ASSERT_EQ(sizeof(vec_int), sizeof(void*) * 3);
ASSERT_EQ(sizeof(vec_double), sizeof(void*) * 3);
```
