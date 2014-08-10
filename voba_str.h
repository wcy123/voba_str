#pragma once
#include <stdint.h>
#ifdef __cplusplus
extern "C" {
#endif
    
typedef struct voba_str_s {
    char *   data;
    uint32_t capacity;
    uint32_t len;
} voba_str_t;
static inline voba_str_t * voba_str_empty();
static inline voba_str_t * voba_mkstr(char c, uint32_t len);
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
static inline char       * voba_str_to_str(voba_str_t *s );
static inline voba_str_t * voba_str_from_data(const void * p, uint32_t len );
static inline voba_str_t * voba_strcat(voba_str_t * s1, const voba_str_t * s2);
static inline voba_str_t * voba_strcat_cstr(voba_str_t * s1, const char * s2);
static inline voba_str_t * voba_strcat_char(voba_str_t * s1, const char c);
static inline voba_str_t * voba_strcat_data(voba_str_t * s1, const char * data, uint32_t len);
static inline voba_str_t * voba_strclr(voba_str_t * s1);
static inline voba_str_t * voba_strdup(const voba_str_t * s1);
static inline voba_str_t * voba_strcpy(voba_str_t * s1, voba_str_t * s2);
static inline voba_str_t * voba_strcpy_cstr(voba_str_t * s1, const char * s2);
static inline voba_str_t * voba_strcpy_data(voba_str_t * s1, const void * data, uint32_t len);
static inline voba_str_t * voba_toupper(voba_str_t * s1);
static inline voba_str_t * voba_tolower(voba_str_t * s1);
static inline int          voba_strcmp(const voba_str_t* s1, const voba_str_t * s2);
static inline uint32_t     voba_strlen(const voba_str_t * s1);
static inline voba_str_t * voba_substr(voba_str_t * s1, uint32_t from, uint32_t to);


//---------------------------------------------------------------------------------
#include "voba_str.c"

#ifdef __cplusplus
}
#endif



