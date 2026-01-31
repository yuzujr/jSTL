# converting ctor

A converting constructor is a special type of constructor in C++ that allows an object of one type to be converted into an object of another type. This is typically done by defining a constructor in the target class that takes an object of the source class as a parameter.
In JSTL, you may see ctor like this:

```cpp
template<class U,
         enable_if_t<!is_same_v<U, T> && is_convertible_v<U, T>, int> = 0>
reverse_iterator(const reverse_iterator<U>& u)
    : current(u.base()) {}
```

This constructor allows for the creation of a `reverse_iterator<T>` from a `reverse_iterator<U>`, provided that `U` is convertible to `T`. The `enable_if_t` is used here to ensure that this constructor is only enabled when the conditions are met, preventing ambiguity with the **default copy constructor**.