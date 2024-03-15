<a href="http://www.boost.org/LICENSE_1_0.txt" target="_blank">![Boost Licence](http://img.shields.io/badge/license-boost-blue.svg)</a>
<a href="https://github.com/boost-ext/ut2/releases" target="_blank">![Version](https://badge.fury.io/gh/boost-ext%2Fut2.svg)</a>
<a href="">![build](https://img.shields.io/badge/build-blue.svg)</a>
<a href="">![Try it online](https://img.shields.io/badge/try%20it-online-blue.svg)</a>

---------------------------------------

> "If you liked it then you `"should have put a"_test` on it", Beyonce rule

## C++20 minimal/compile-time first unit-testing library

> https://en.wikipedia.org/wiki/Unit_testing

### Features

- Single header (https://raw.githubusercontent.com/boost-ext/ut2/main/ut2)
    - Easy integration (see [FAQ](#faq))
- Compile-time first (executes tests at compile-time and/or run-time)
    - Detects memory leaks and UBs*
- Explicit by design (no implicit conversions, narrowing, epsilon-less floating point comparisions, etc.)
- Minimal [API](#api)
- Optionally integrates with reflection (via https://github.com/boost-ext/reflect)
- Compiles cleanly with ([`-fno-exceptions -fno-rtti -Wall -Wextra -Werror -pedantic -pedantic-errors | /W4 /WX`](https://godbolt.org/z/M747ocGfx))
- Fast compilation times (see [compilation times](#comp))
- Verifies itself upon include (aka run all tests via static_asserts but it can be disabled - see [FAQ](#faq))

### Requirements

- C++20 ([gcc-12+, clang-15+](https://godbolt.org/z/xPc19Moef))
    - No STL required (optionally - iostream and cstdint)

---

### Hello world ()

```cpp
```

---

### Examples

- [feature] Reflection integration -

---

### API

> Configuration

```cpp
#define UT2 2'0'0                   // Current library version (SemVer)
#define UT_RUNTIME_ONLY             // If defined only run-time tests will be executed + static_assert tests
#define UT_COMPILE_TIME_ONLY        // If defined only compile-time tests will be executed
#define DISABLE_STATIC_ASSERT_TESTS // Disables running static_asserts tests
                                    // Not enabled by default (use with caution)
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
