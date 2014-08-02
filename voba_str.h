#pragma once
#include <stdint.h>
typedef struct voba_str_s {
    char *   data;
    uint32_t capacity;
    uint32_t len;
} voba_str_t;
static inline voba_str_t * voba_str_empty();
static inline voba_str_t * voba_str_from_char(char c, uint32_t len);
static inline voba_str_t * voba_str_fmt_pointer(const void * p);
static inline voba_str_t * voba_str_fmt_int8_t(int8_t x, int8_t base);
static inline voba_str_t * voba_str_fmt_uint8_t(uint8_t x, uint8_t base);
static inline voba_str_t * voba_str_fmt_int16_t(int16_t x, int16_t base);
static inline voba_str_t * voba_str_fmt_uint16_t(uint16_t x, uint16_t base);
static inline voba_str_t * voba_str_fmt_int32_t(int32_t x, int32_t base);
static inline voba_str_t * voba_str_fmt_uint32_t(uint32_t x, uint32_t base);
static inline voba_str_t * voba_str_fmt_int64_t(int64_t x, int64_t base);
static inline voba_str_t * voba_str_fmt_uint64_t(uint64_t x, uint64_t base);
static inline voba_str_t * voba_str_fmt_float(float);
static inline voba_str_t * voba_str_from_cstr(const char * str);
static inline const char * voba_str_to_cstr(voba_str_t *s );
static inline char * voba_str_to_str(voba_str_t *s );
static inline voba_str_t * voba_str_from_data(const void * p, uint32_t len );
static inline voba_str_t * voba_str_cat(voba_str_t * s1, const voba_str_t * s2);
static inline voba_str_t * voba_str_cat_cstr(voba_str_t * s1, const char * s2);
static inline voba_str_t * voba_str_cat_char(voba_str_t * s1, const char c);
static inline voba_str_t * voba_str_cat_data(voba_str_t * s1, const char * data, uint32_t len);
static inline voba_str_t * voba_str_clear(voba_str_t * s1);
static inline voba_str_t * voba_str_copy(const voba_str_t * s1);
static inline voba_str_t * voba_str_assign(voba_str_t * s1, voba_str_t * s2);
static inline voba_str_t * voba_str_assign_cstr(voba_str_t * s1, const char * s2);
static inline voba_str_t * voba_str_assign_data(voba_str_t * s1, const void * data, uint32_t len);
static inline voba_str_t * voba_str_toupper(voba_str_t * s1);
static inline voba_str_t * voba_str_tolower(voba_str_t * s1);
static inline int         voba_str_eq(const voba_str_t* s1, const voba_str_t * s2);
static inline uint32_t    voba_str_size(const voba_str_t * s1);
static inline voba_str_t * voba_str_substr(voba_str_t * s1, uint32_t from, uint32_t to);


//---------------------------------------------------------------------------------
#include "voba_str.c"




