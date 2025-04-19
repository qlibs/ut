// <!--
// The MIT License (MIT)
//
// Copyright (c) 2024 Kris Jusiak <kris@jusiak.net>
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.
//
module;
#include "ut"
export module ut;

export namespace ut {
  using ut::cfg;
  using ut::override;

  using ut::constant;
  using ut::mut;

  using ut::suite;
  using ut::operator""_test;

  using ut::expect;
  using ut::eq;
  using ut::neq;
  using ut::gt;
  using ut::ge;
  using ut::lt;
  using ut::le;
  using ut::nt;

  inline namespace dsl {
    using ut::operator==;
    using ut::operator!=;
    using ut::operator>;
    using ut::operator>=;
    using ut::operator<;
    using ut::operator<=;
    using ut::operator!;
  } // namespace dsl

  using ut::_b;
  using ut::_c;
  using ut::_sc;
  using ut::_s;
  using ut::_i;
  using ut::_l;
  using ut::_ll;
  using ut::_u;
  using ut::_uc;
  using ut::_us;
  using ut::_ul;
  using ut::_ull;
  using ut::_f;
  using ut::_d;
  using ut::_ld;
  using ut::_i8;
  using ut::_i16;
  using ut::_i32;
  using ut::_i64;
  using ut::_u8;
  using ut::_u16;
  using ut::_u32;
  using ut::_u64;
  using ut::_string;

  using ut::operator""_s;
  using ut::operator""_i;
  using ut::operator""_s;
  using ut::operator""_c;
  using ut::operator""_sc;
  using ut::operator""_l;
  using ut::operator""_ll;
  using ut::operator""_u;
  using ut::operator""_uc;
  using ut::operator""_us;
  using ut::operator""_ul;
  using ut::operator""_ull;
  using ut::operator""_f;
  using ut::operator""_d;
  using ut::operator""_ld;
  using ut::operator""_i8;
  using ut::operator""_i16;
  using ut::operator""_i32;
  using ut::operator""_i64;
  using ut::operator""_u8;
  using ut::operator""_u16;
  using ut::operator""_u32;
  using ut::operator""_u64;
} // namespace ut

// -*- mode: c++; -*-
// vim: set filetype=cpp:
