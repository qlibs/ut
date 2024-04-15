<a href="http://www.boost.org/LICENSE_1_0.txt" target="_blank">![Boost Licence](http://img.shields.io/badge/license-boost-blue.svg)</a>
<a href="https://github.com/boost-ext/ut2/releases" target="_blank">![Version](https://badge.fury.io/gh/boost-ext%2Fut2.svg)</a>
<a href="https://godbolt.org/z/cbshMjocd">![build](https://img.shields.io/badge/build-blue.svg)</a>
<a href="">![Try it online](https://img.shields.io/badge/try%20it-online-blue.svg)</a>

---------------------------------------

> "If you liked it then you `"should have put a"_test` on it", Beyonce rule

## C++20 minimal/compile-time first unit-testing library

> https://en.wikipedia.org/wiki/Unit_testing

### Features

- Single header (https://raw.githubusercontent.com/boost-ext/ut2/main/ut2)
    - Easy integration (see [FAQ](#faq))
- Compile-time first (executes tests at compile-time and/or run-time)
    - Detects memory leaks and UBs at compile-time*
- Explicit by design (no implicit conversions, narrowing, epsilon-less floating point comparisions, etc)
- Minimal [API](#api)
- Optionally integrates with reflection (via https://github.com/boost-ext/reflect)
- Compiles cleanly with ([`-fno-exceptions -fno-rtti -Wall -Wextra -Werror -pedantic -pedantic-errors`](https://godbolt.org/z/M747ocGfx))
- Fast compilation times (see [compilation times](#comp))
- Verifies itself upon include (aka run all tests via static_asserts but it can be disabled - see [FAQ](#faq))

> Based on the `constexpr` ability of given compiler/standard

### Requirements

- C++20 ([gcc-12+, clang-16+](https://godbolt.org/z/xPc19Moef))
    - No STL required (optionally - iostream and cstdint)

---

### Hello world (https://godbolt.org/z/ree76dTdE)

```cpp
#include <ut2>

constexpr auto sum(auto... args) { return (args + ...); }

int main() {
  using namespace ut;

  "sum"_test = [] {
    expect(sum(0) == 0_i);
    expect(sum(1, 2) == 3_i);
    expect(sum(1, 2) > 0_i);
  };
}
```

```sh
---> PASSED: tests: 1 (1 passed, 0 failed, 1 compile-time), asserts: 3 (3 passed, 0 failed)
```

---

### Examples

- [feature] Reflection integration - https://godbolt.org/z/5T51odcfP
- [feature] Custom configuration   - https://godbolt.org/z/oqfjesT6E

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
#define UT_RUN_TIME_ONLY            // If defined only run-time tests will be executed + static_assert tests
#define UT_COMPILE_TIME_ONLY        // If defined only compile-time tests will be executed
#define DISABLE_STATIC_ASSERT_TESTS // If defined it disables running static_asserts tests for the UT library (user tests are not affected)
```

---

### FAQ

- Can I disable running tests at compile-time for faster compilation times?

    > When `DISABLE_STATIC_ASSERT_TESTS` is defined static_asserts tests won't be executed upon inclusion.
    Note: Use with caution as disabling tests means that there are no guarantees upon inclusion that the given compiler/env combination works as expected.

- How to integrate with CMake/CPM?

    ```
    CPMAddPackage(
      Name ut2
      GITHUB_REPOSITORY boost-ext/ut2
      GIT_TAG v1.1.1
    )
    add_library(ut2 INTERFACE)
    target_include_directories(ut2 SYSTEM INTERFACE ${reflect_SOURCE_DIR})
    add_library(ut2::ut2 ALIAS ut2)
    ```

    ```
    target_link_libraries(${PROJECT_NAME} ut2::ut2);
    ```

- Similar projects?
    > [ut](https://github.com/boost-ext/ut), [Catch2](https://github.com/catchorg/Catch2), [googltest](https://github.com/google/googletest), [GUnit](https://github.com/cpp-testing/GUnit), [boost.test](https://www.boost.org/doc/libs/latest/libs/test/doc/html/index.html)

---

**Disclaimer** `ut2` is not an official Boost library.
