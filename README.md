<a href="http://www.boost.org/LICENSE_1_0.txt" target="_blank">![Boost Licence](http://img.shields.io/badge/license-boost-blue.svg)</a>
<a href="https://github.com/boost-ext/ut2/releases" target="_blank">![Version](https://badge.fury.io/gh/boost-ext%2Fut2.svg)</a>
<a href="https://godbolt.org/z/cbshMjocd">![build](https://img.shields.io/badge/build-blue.svg)</a>
<a href="https://godbolt.org/z/9jbzh3h4W">![Try it online](https://img.shields.io/badge/try%20it-online-blue.svg)</a>

---------------------------------------

> "If you liked it then you `"should have put a"_test` on it", Beyonce rule

## C++20 minimal/compile-time first unit-testing library

> https://en.wikipedia.org/wiki/Unit_testing

### Features

- Single header (https://raw.githubusercontent.com/boost-ext/ut2/main/ut2)
    - Easy integration (see [FAQ](#faq))
- Compile-time first (executes tests at compile-time and/or run-time)
    - Detects memory leaks and UBs at compile-time*
- Explicit by design (no implicit conversions, narrowing, epsilon-less floating point comparisions, ...)
- Minimal [API](#api)
- Reflection integration (optional via https://github.com/boost-ext/reflect)
- Compiles cleanly with ([`-fno-exceptions -fno-rtti -Wall -Wextra -Werror -pedantic -pedantic-errors`](https://godbolt.org/z/cbshMjocd))
- Fast compilation times (see [compilation times](#comp))
- Verifies itself upon include (aka run all tests via static_asserts but it can be disabled - see [FAQ](#faq))

> Based on the `constexpr` ability of given compiler/standard

### Requirements

- C++20 ([gcc-12+, clang-16+](https://godbolt.org/z/cbshMjocd))
    - No STL required (optionally - iostream and cstdint)

---

### Examples

> Hello world (https://godbolt.org/z/9jbzh3h4W)

```cpp
#include <ut2>

constexpr auto sum(auto... args) { return (args + ...); }

int main() {
  "sum"_test = [] {
    expect(sum(1, 2, 3) == 6_i);
  };
}
```

```sh
$CXX example.cpp -std=c++20 -o example && ./example
PASSED: tests: 1 (1 passed, 0 failed, 1 compile-time), asserts: 1 (1 passed, 0 failed)
```

> Execution model (https://godbolt.org/z/Gx31rMP56)

```cpp
static_assert(("compile time only"_test = [] {
  expect(sum(1, 2, 3) == 6_i);
}));

int main() {
  "sum [compile-time and run-time]"_test = [] {   // default: compile-time and run-time
    expect(sum(1, 2, 3) == 5_i);                  // fails at compile-time and/or run-ime
    // error: expect.operator()<ut::eq<int, int>>({6, 5})
  };

  "sum [run-time only]"_test = [] mutable {       // mutable = run-time only
    expect(sum(1, 2, 3) == 6_i);                  // fails at run-time
  };

  "sum [compile-time only]"_test = [] consteval { // conteval = compile-time only (C++23)
    expect(sum(1, 2, 3) == 6_i);                  // fails at compile-time
  };
}
```

```sh
$CXX example.cpp -std=c++20 # -DUT_COMPILE_TIME_ONLY
ut2:156:25: error: static_assert((test(), "[FAILED]"));
example.cpp:13:44: note:"sum [compile-time and run-time]"_test
example.cpp:14:5:  note: in call to 'expect.operator()<ut::eq<int, int>>({6, 5})'
```

```sh
$CXX example.cpp -std=c++20 -o example && ./example
/app/example.cpp:14:FAILED:"sum [compile-time and run-time]": 6 == 5
FAILED: tests: 3 (2 passed, 1 failed, 0 compile-time), asserts: 2 (1 passed, 1 failed)
```

> Suites/Sub-tests (https://godbolt.org/z/a9nceoPKn)

```cpp
ut::suite test_suite = [] {
  "vector [sub-tests]"_test = [] {
    std::vector<int> v(5);
    expect(v.size() == 5_ul);
    expect(v.capacity() >= 5_ul);

    "resizing bigger changes size and capacity"_test = [=] {
      mut(v).resize(10);
      expect(v.size() == 10_ul);
      expect(v.capacity() >= 10_ul);
    };
  };
};

int main() { }
```

```sh
$CXX example.cpp -std=c++20 -o example && ./example
PASSED: tests: 2 (2 passed, 0 failed, 1 compile-time), asserts: 4 (4 passed, 0 failed)
```

> Expectations (https://godbolt.org/z/57KxPsTsE)

```cpp
int main() {
  "expect"_test = [] {
    "different ways"_test = [] {
      expect(42 == 42_i);
      expect(eq(42, 42)) << "same as expect(42 == 42_i)";
      expect(_i(42) == 42) << "same as expect(42_i == 42)";
    };

    "floating point"_test = [] {
      expect((4.2 == 4.2_d)(.01)) << "floating point comparison with .01 epsilon precision";
    };

    "fatal"_test = [] mutable { // at run-time
      std::vector<int> v{1};
      expect[v.size() > 1_ul] << "fatal, stops running tests further";
      expect(v[1] == 42_i); // not executed
    };

    "compile-time expression"_test = [] {
      expect(constant<42 == 42_i>) << "requires compile-time expression";
    };
  };
}
```

```sh
$CXX example.cpp -std=c++20 -o example && ./example
example.cpp:21:FAILED:"fatal": 1 > 1
FAILED: tests: 3 (2 passed, 1 failed, 3 compile-time), asserts: 5 (4 passed, 1 failed)
```

> Errors/Checks (https://godbolt.org/z/n48qPs8K9)

```cpp
int main() {
  "leak"_test = [] {
    new int; // compile-time error
  };

  "ub"_test = [] {
    int* i{};
    *i = 42; // compile-time error
  };

  "errors"_test = [] {
    expect(42_i == short(42)); // [ERROR] Comparision of different types is not allowed
    expect(42 == 42);          // [ERROR] Expression required: expect(42_i == 42)
    expect(4.2 == 4.2_d);      // [ERROR] Epsilon is required: expect((4.2 == 4.2_d)(.1))
  };
}
```

---

> Reflection integration (https://godbolt.org/z/5T51odcfP)

```cpp
int main() {
  struct foo { int a; int b; };
  struct bar { int a; int b; };

  "reflection"_test = [] {
    auto f = foo{.a=1, .b=2};
    expect(eq(foo{1, 2}, f));
    expect(members(foo{1, 2}) == members(f));
    expect(names(foo{}) == names(bar{}));
  };
};
```

```sh
$CXX example.cpp -std=c++20 -o example && ./example
PASSED: tests: 1 (1 passed, 0 failed, 1 compile-time), asserts: 3 (3 passed, 0 failed)
```

> Custom configuration (https://godbolt.org/z/oqfjesT6E)

```cpp
struct outputter {
  template<ut::events::mode Mode>
  constexpr auto on(const ut::events::test_begin<Mode>&) { }
  template<ut::events::mode Mode>
  constexpr auto on(const ut::events::test_end<Mode>&) { }
  template<class TExpr>
  constexpr auto on(const ut::events::assert_pass<TExpr>&) { }
  template<class TExpr>
  constexpr auto on(const ut::events::assert_fail<TExpr>&) { }
  constexpr auto on(const ut::events::fatal&) { }
  constexpr auto on(const ut::events::summary&) { }
  template<class TMsg>
  constexpr auto on(const ut::events::log<TMsg>&) { }
};

struct custom_config {
  ::outputter outputter{};
  ut::reporter<decltype(outputter)> reporter{outputter};
  ut::runner<decltype(reporter)> runner{reporter};
} config{};

template<class... Ts> auto& ut::cfg<ut::override, Ts...> = ::config;

int main() {
  "config"_test = []() mutable {
    expect(42 == 43_i); // no output
  };
};
```

```sh
$CXX example.cpp -std=c++20 -o example && ./example
echo $? # 139 # no output
```

---

<a name="comp"></a>
### Compilation times (no iostream)

> [include] https://raw.githubusercontent.com/boost-ext/reflect/main/reflect

```cpp
time g++-13.2 -x c++ -std=c++20 ut2 -c -DDISABLE_STATIC_ASSERT_TESTS   # 0.022s
time g++-13.2 -x c++ -std=c++20 ut2 -c                                 # 0.042s
```

```cpp
time clang++-17 -x c++ -std=c++20 ut2 -c -DDISABLE_STATIC_ASSERT_TESTS # 0.028s
time clang++-17 -x c++ -std=c++20 ut2 -c                               # 0.049s
```

---

### API

```cpp
inline constexpr struct {
  constexpr auto operator()(auto expr);
  constexpr auto operator[](auto expr); /// fatal
} expect{};
```

```cpp
struct suite;
```

```cpp
template<fixed_string Str>
[[nodiscard]] constexpr auto operator""_test();
```

```cpp
template<auto Expr>
inline constexpr auto constant;
```

```cpp
template<class T>
[[nodiscard]] constexpr auto& mut(const T& t);
```

```cpp
template<class TLhs, class TRhs> struct eq;  /// equal
template<class TLhs, class TRhs> struct neq; /// not equal
template<class TLhs, class TRhs> struct gt;  /// greater
template<class TLhs, class TRhs> struct ge;  /// greater equal
template<class TLhs, class TRhs> struct lt;  /// less
template<class TLhs, class TRhs> struct le;  /// less equal
template<class TLhs, class TRhs> struct nt;  /// not
```

```cpp
constexpr auto operator==(const auto& lhs, const auto& rhs) -> decltype(eq{lhs, rhs});
constexpr auto operator!=(const auto& lhs, const auto& rhs) -> decltype(neq{lhs, rhs});
constexpr auto operator> (const auto& lhs, const auto& rhs) -> decltype(gt{lhs, rhs});
constexpr auto operator>=(const auto& lhs, const auto& rhs) -> decltype(ge{lhs, rhs});
constexpr auto operator< (const auto& lhs, const auto& rhs) -> decltype(lt{lhs, rhs});
constexpr auto operator<=(const auto& lhs, const auto& rhs) -> decltype(le{lhs, rhs});
constexpr auto operator! (const auto& t)                    -> decltype(nt{t});
```

```cpp
struct _b;      /// bool (true_b = _b{true}, false_b = _b{false})
struct _c;      /// char
struct _sc;     /// signed char
struct _s;      /// short
struct _i;      /// int
struct _l;      /// long
struct _ll;     /// long long
struct _u;      /// unsigned
struct _uc;     /// unsigned char
struct _us;     /// unsigned short
struct _ul;     /// unsigned long
struct _ull;    /// unsigned long long
struct _f;      /// float
struct _d;      /// double
struct _ld;     /// long double
struct _i8;     /// int8_t
struct _i16;    /// int16_t
struct _i32;    /// int32_t
struct _i64;    /// int64_t
struct _u8;     /// uint8_t
struct _u16;    /// uint16_t
struct _u32;    /// uint32_t
struct _u64;    /// uint64_t
struct _string; /// const char*
```

```cpp
constexpr auto operator""_i(auto value)   -> decltype(_i(value));
constexpr auto operator""_s(auto value)   -> decltype(_s(value));
constexpr auto operator""_c(auto value)   -> decltype(_c(value));
constexpr auto operator""_sc(auto value)  -> decltype(_sc(value));
constexpr auto operator""_l(auto value)   -> decltype(_l(value));
constexpr auto operator""_ll(auto value)  -> decltype(_ll(value));
constexpr auto operator""_u(auto value)   -> decltype(_u(value));
constexpr auto operator""_uc(auto value)  -> decltype(_uc(value));
constexpr auto operator""_us(auto value)  -> decltype(_us(value));
constexpr auto operator""_ul(auto value)  -> decltype(_ul(value));
constexpr auto operator""_ull(auto value) -> decltype(_ull(value));
constexpr auto operator""_f(auto value)   -> decltype(_f(value));
constexpr auto operator""_d(auto value)   -> decltype(_d(value));
constexpr auto operator""_ld(auto value)  -> decltype(_ld(value));
constexpr auto operator""_i8(auto value)  -> decltype(_i8(value));
constexpr auto operator""_i16(auto value) -> decltype(_i16(value));
constexpr auto operator""_i32(auto value) -> decltype(_i32(value));
constexpr auto operator""_i64(auto value) -> decltype(_i64(value));
constexpr auto operator""_u8(auto value)  -> decltype(_u8(value));
constexpr auto operator""_u16(auto value) -> decltype(_u16(value));
constexpr auto operator""_u32(auto value) -> decltype(_u32(value));
constexpr auto operator""_u64(auto value) -> decltype(_u64(value));
```

```cpp
template<fixed_string Str>
[[nodiscard]] constexpr auto operator""_s() -> decltype(_string(Str));
```

> Configuration

```cpp
namespace events {
enum class mode {
  run_time,
  compile_time
};

template<mode Mode>
struct test_begin {
  const char* file_name{};
  int line{}; const char* name{};
};

template<mode Mode>
struct test_end {
  const char* file_name{};
  int line{};
  const char* name{};
  enum { FAILED, PASSED, COMPILE_TIME } result{};
};

template<class TExpr>
struct assert_pass {
  const char* file_name{};
  int line{};
  TExpr expr{};
};

template<class TExpr>
struct assert_fail {
  const char* file_name{};
  int line{};
  TExpr expr{};
};

struct fatal { };

template<class TMsg>
struct log {
  const TMsg& msg;
  bool result{};
};

struct summary {
  enum { FAILED, PASSED, COMPILE_TIME };
  unsigned asserts[2]{}; /* FAILED, PASSED */
  unsigned tests[3]{}; /* FAILED, PASSED, COMPILE_TIME */
};
} // namespace events
```

```cpp
struct outputter {
  template<events::mode Mode> constexpr auto on(const events::test_begin<Mode>&);
  constexpr auto on(const events::test_begin<events::mode::run_time>& event);
  template<events::mode Mode> constexpr auto on(const events::test_end<Mode>&);
  template<class TExpr> constexpr auto on(const events::assert_pass<TExpr>&);
  template<class TExpr> constexpr auto on(const events::assert_fail<TExpr>&);
  constexpr auto on(const events::fatal&);
  template<class TMsg> constexpr auto on(const events::log<TMsg>&);
  constexpr auto on(const events::summary& event);
};
```

```cpp
struct reporter {
  constexpr auto on(const events::test_begin<events::mode::run_time>&);
  constexpr auto on(const events::test_end<events::mode::run_time>&);
  constexpr auto on(const events::test_begin<events::mode::compile_time>&);
  constexpr auto on(const events::test_end<events::mode::compile_time>&);
  template<class TExpr> constexpr auto on(const events::assert_pass<TExpr>&);
  template<class TExpr> constexpr auto on(const events::assert_fail<TExpr>&);
  constexpr auto on(const events::fatal& event);
};
```

```cpp
struct runner {
  template<class Test> constexpr auto on(Test test) -> bool;
};
```

```cpp
/**
 * Customization point to override the default configuration
 *
 * @code
 * template<class... Ts> auto ut::cfg<ut::override, Ts...> = my_config{};
 * @endcode
 */
struct override { }; /// to override configuration by users
struct default_cfg;  /// default configuration
template <class...> inline auto cfg = default_cfg{};
```

```cpp
#define UT2 2'0'0                   // Current library version (SemVer)
#define UT_RUN_TIME_ONLY            // If defined tests will be executed
                                    // at run-time + static_assert tests
#define UT_COMPILE_TIME_ONLY        // If defined only compile-time tests
                                    // will be executed
#define DISABLE_STATIC_ASSERT_TESTS // If defined it disables running
                                    // static_asserts tests for the UT library
                                    // (user tests are not affected)
```

---

### FAQ

- How does UT2 compare to https://github.com/boost-ext/ut?

  > UT2 ideas are based on UT. UT2 aim is not to replace UT.
    UT2 is minimal (no STL required).
    UT2 has different execution model (can run tests at compile-time and/or run-time).

- Can I disable running tests at compile-time for faster compilation times?

    > When `DISABLE_STATIC_ASSERT_TESTS` is defined static_asserts tests won't be executed upon inclusion.
    Note: Use with caution as disabling tests means that there are no guarantees upon inclusion that the given compiler/env combination works as expected.

- How to integrate with CMake/CPM?

    ```
    CPMAddPackage(
      Name ut2
      GITHUB_REPOSITORY boost-ext/ut2
      GIT_TAG v2.0.0
    )
    add_library(ut2 INTERFACE)
    target_include_directories(ut2 SYSTEM INTERFACE ${reflect_SOURCE_DIR})
    add_library(ut2::ut2 ALIAS ut2)
    ```

    ```
    target_link_libraries(${PROJECT_NAME} ut2::ut2);
    ```

- Similar projects?
    > [ut](https://github.com/boost-ext/ut), [catch2](https://github.com/catchorg/Catch2), [googletest](https://github.com/google/googletest), [gunit](https://github.com/cpp-testing/GUnit), [boost.test](https://www.boost.org/doc/libs/latest/libs/test/doc/html/index.html)

---

**Disclaimer** `ut2` is not an official Boost library.
