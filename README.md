# a string library
==================

[![Build Status](https://travis-ci.org/wcy123/voba_str.svg)](https://travis-ci.org/wcy123/voba_str)

It is a string library.

why a new string library?

 1. I want to use the [A garbage collector for C and C++][]

[A garbage collector for C and C++]: http://www.hboehm.info/gc/

 2. I want to a simple data structure as below

 ~~~~~{.c}
 typedef struct voba_str_s {
    char *   data;
    uint32_t capacity;
    uint32_t len;
 } voba_str_t;
 ~~~~~

 3. it is a header only library.

see <http://wcy123.github.io/voba_str/index.html> for more details



