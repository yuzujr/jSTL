# SFINAE

SFINAE (Substitution Failure Is Not An Error) is a C++ template metaprogramming technique that allows the compiler to ignore certain template instantiations that would result in errors, enabling more flexible and powerful code. It is commonly used for type traits, function overloading, and template specialization.

Here is a typical example of SFINAE in JSTL:

```cpp
template <class Default, class AlwaysVoid, template <class...> class Op,
          class... Args>
struct detected_or {
    using type = Default;
};

// only when `Op<Args...>` is well-formed
template <class Default, template <class...> class Op, class... Args>
struct detected_or<Default, void_t<Op<Args...>>, Op, Args...> {
    using type = Op<Args...>;
};

template <class Default, template <class...> class Op, class... Args>
using detected_or_t = typename detected_or<Default, void, Op, Args...>::type;
```

This code defines a `detected_or` template that checks if a certain operation `Op<Args...>` is well-formed. If it is, it uses the result of that operation; otherwise, it falls back to a default type. This is useful for creating type traits that can adapt based on the capabilities of the types involved.

Note the complex parameter pack expansion in the template.
`Op` is a template template parameter that can take multiple type parameters, and `Args...` is a parameter pack that can represent zero or more types. This allows `detected_or` to work with a wide variety of operations and type combinations.