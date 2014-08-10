#include <stdio.h>
#include <assert.h>
#include <string.h>
#include <gc.h>
#include <ctype.h>

#include "voba_str.h"



static inline
int v__is_valid(const voba_str_t * s1)
{
    if (s1 == NULL || s1->data == NULL ||
        (s1->capacity != 0 && s1->capacity < s1->len + 1)){
        return 0;
    }
    return 1;
}
static inline
uint32_t v__len_to_capacity(uint32_t s) 
{
    int a = 0;
#if ((__GNUC__ >= 4) || ((__GNUC__ == 3) && (__GNUC_MINOR__ >= 4)))
    a = __builtin_clz(s);
#else
#error("TODO implement clzl")
#endif
    return (1<<(32 - a));
}
static inline
char * v__new_data(uint32_t capacity, uint32_t len, const char * data) 
{
    // initialized with ending '\0'
    char *  p  = (char*) GC_MALLOC_ATOMIC(capacity);
    assert(p);
    if(len) memcpy(p, data, len);
    p[len] = '\0';
    return p;
}
static inline
voba_str_t * v__new(uint32_t capacity, uint32_t len, const char * data, int copy_data)
{
    voba_str_t * ret = (voba_str_t*)GC_MALLOC(sizeof(voba_str_t));
    assert(capacity == 0 || len + 1 < capacity);
    ret->capacity = capacity;
    ret->len = len;
    if(copy_data){
        ret->data = v__new_data(capacity,len,data);
    }else{
        ret->data = (char*)data;
    }
    return ret;
}
static inline
voba_str_t * v__from_data(const void * data, uint32_t len)
{
    const uint32_t capacity = v__len_to_capacity(len + 1);
    return v__new(capacity,len, (const char*)data, 1 /*copy_data*/);
}
static inline
void v__clear(voba_str_t * s)
{
    s->len = 0;
    if(s->capacity == 0){
        s->data = (char*)"";
    }
}
static inline
voba_str_t * v__cow(voba_str_t * s1)
{
    if(s1->capacity == 0){
        // copy-on-write
        const uint32_t capacity = v__len_to_capacity(s1->len+1);
        s1 = v__new(capacity,s1->len,s1->data, 1 /*copy data*/);
    }
    return s1;
}
static inline
voba_str_t * v__map(voba_str_t * s1, int (*f)(int))
{
    if(!v__is_valid(s1)) return NULL;
    s1 = v__cow(s1);
    for(uint32_t i = 0; i < s1->len; ++i){
        s1->data[i] = (char) f((int)s1->data[i]);
    }
    return s1;
}
static inline
voba_str_t * v__cat_data(voba_str_t * s1, const void * data, uint32_t len)
{
    voba_str_t * r = s1;
    if(r->capacity == 0) {
        // copy-on-write
        const uint32_t capacity = v__len_to_capacity(len + r->len);
        r = v__new(capacity,r->len,r->data,1 /*copy data*/);
    }else{
        const uint32_t capacity = v__len_to_capacity(len + r->len);
        if(capacity > r->capacity){
            r->capacity = capacity;
            r->data = (char*) GC_REALLOC(r->data,r->capacity);
            assert(r->data);
        }
    }
    memcpy((void*)(r->data + r->len), data, len);
    r->len += len;
    r->data[r->len] = '\0';
    return r;
}
static inline voba_str_t * voba_str_empty()
{
    return v__new(0 /*capacity*/, 0 /*len*/, "" /*data*/, 0 /*copy data*/);
}
static inline voba_str_t * voba_mkstr(char c, uint32_t len)
{
    const uint32_t capacity = v__len_to_capacity(len + 1);
    char * p = (char*) GC_MALLOC_ATOMIC(capacity);
    memset((void*)p,(int)c,len);
    p[len] = '\0';
    return v__new(capacity,len,p, 0 /*copy data*/);
}
static inline voba_str_t * voba_str_fmt_pointer(const void * p)
{
    uint32_t i = 0;
    int s = (sizeof(uintptr_t)*8) -4;
    uintptr_t mask = (uintptr_t) (((uintptr_t)0xF) << s);
    uintptr_t x = (uintptr_t) p;
    voba_str_t * r = voba_mkstr(' ', s/4 + 4);
    r->data[i++] = '0';
    r->data[i++] = 'x';
    while(((x&mask)>>s) == 0 && s>0) { mask >>= 4; s -= 4; }
    printf("%d\n",s);
    while(mask!= 0) {
        r->data[i++] = ("0123456789ABCDEF")[(x&mask)>>s];
        mask >>= 4; s -= 4;
    }
    r->len = i;
    r->data[i++] = '\0';
    return r;
}
#define VOBA_STR_FMT_INT(type,sign)                                     \
static inline voba_str_t * voba_str_fmt_##type(type x, type base)       \
{                                                                       \
    const char * digits = &("fedcba9876543210123456789abcdef"[15]);     \
    assert(base>1 && base <= 16);                                       \
    uint32_t len = 0;                                                   \
    type x2 = x;                                                        \
    uint32_t i  = 0;                                                    \
    int neg = sign == 1 && (!(x >= 0));                                 \
    while(x2/=base) len ++;                                             \
    len ++;                                                             \
    if(neg) len ++;                                                     \
    voba_str_t * r = voba_mkstr(' ',len);                               \
    i = len + 1;                                                        \
    r->data[--i] = '\0';                                                \
    do {                                                                \
        r->data[--i] = digits[x%base];                                  \
    }while ((x/=base) !=0);                                             \
    if(neg) r->data[--i] = '-';                                         \
    return r;                                                           \
}

VOBA_STR_FMT_INT(  int8_t,1)
VOBA_STR_FMT_INT( uint8_t,0)
VOBA_STR_FMT_INT( int16_t,1)
VOBA_STR_FMT_INT(uint16_t,0)
VOBA_STR_FMT_INT( int32_t,1)
VOBA_STR_FMT_INT(uint32_t,0)
VOBA_STR_FMT_INT( int64_t,1)
VOBA_STR_FMT_INT(uint64_t,0)
static inline voba_str_t * voba_str_fmt_float(float x)
{
    return voba_str_from_cstr("not implemented");
}
static inline voba_str_t * voba_str_from_cstr(const char * str)
{
    str = (str==NULL?"":str);
    const size_t len = strlen(str);
    return v__new(0 /*capacity*/,len,(const char*) str, 0 /*copy data*/);
}
static inline const char * voba_str_to_cstr(voba_str_t *s )
{
    if(s == NULL || s->len == 0 || s->data == NULL){
        return "";
    }
    // when s->capacity !=0, `s` own `data`.
    if( s->data[s->len] == '\0'){
        return s->data;
    }
    return v__new_data(s->len+1,s->len, s->data);
}
static inline char * voba_str_to_str(voba_str_t *s )
{
    if(s == NULL || s->len == 0 || s->data == NULL){
        return NULL;
    }
    // when s->capacity !=0, `s` own `data`.
    if(s->capacity != 0 && s->data[s->len] == '\0'){
        return s->data;
    }
    return v__new_data(s->len+1,s->len, s->data);
}
static inline voba_str_t * voba_str_from_data(const void * p, uint32_t len )
{
    if(len == 0 || p == NULL){
        return v__new(0 /*capacity*/, 0 /*len*/, "" /*data*/, 0 /*copy data*/);
    }
    return v__from_data(p,len);
}
static inline voba_str_t * voba_strcat(voba_str_t * s1, const voba_str_t * s2)
{
    if(!v__is_valid(s1)) return NULL;
    if (s2 == NULL || s2->data == NULL || (s2->capacity !=0 && s2->capacity < s2->len + 1)){
        return NULL;
    }
    return v__cat_data(s1,(const void*)s2->data,s2->len);
}
static inline voba_str_t * voba_strcat_cstr(voba_str_t * s1, const char * s2)
{
    if(!v__is_valid(s1)) return NULL;
    if(s2 == NULL || *s2 == '\0'){
        return s1;
    }
    return v__cat_data(s1,s2,(uint32_t)strlen(s2));
}
static inline voba_str_t * voba_strcat_char(voba_str_t * s1, const char c)
{
    if(!v__is_valid(s1)) return NULL;
    return v__cat_data(s1,&c,1);
}
static inline voba_str_t * voba_strcat_data(voba_str_t * s1, const char * data, uint32_t len)
{
    if(!v__is_valid(s1)) return NULL;
    if(data == NULL || len == 0){
        return s1;
    }
    return v__cat_data(s1,data,len);
}
static inline voba_str_t * voba_strclr(voba_str_t * s1)
{
    if(!v__is_valid(s1)) return NULL;
    v__clear(s1);
    return s1;
}
static inline voba_str_t * voba_strdup(const voba_str_t * s1)
{
    if(!v__is_valid(s1)) return NULL;
    const uint32_t capacity = s1->capacity == 0? v__len_to_capacity(s1->len+1):s1->capacity;
    return v__new(capacity, s1->len, s1->data, 1 /*copy data*/);
}
static inline voba_str_t * voba_strcpy(voba_str_t * s1, voba_str_t * s2)
{
    if(!v__is_valid(s1)) return NULL;
    if(!v__is_valid(s2)) return NULL;
    s1->capacity = s2->capacity = 0; // no one own the data
    s1->len = s2->len;
    s1->data = s2->data;
    return s1;
}
static inline voba_str_t * voba_strcpy_cstr(voba_str_t * s1, const char * s2)
{
    if(!v__is_valid(s1)) return NULL;
    if(s2 == NULL){
        s2 = "";
    }
    s1->capacity = 0;
    s1->len = (uint32_t)strlen(s2);
    s1->data = (char*)s2;
    return s1;
}
static inline voba_str_t * voba_strcpy_data(voba_str_t * s1, const void * data, uint32_t len)
{
    if(!v__is_valid(s1)) return NULL;
    if (data == NULL){
        return NULL;
    }
    s1->capacity = 0;
    s1->len = len;
    s1->data = (char*)data;
    return s1;
}
static inline voba_str_t * voba_toupper(voba_str_t * s1)
{
    return v__map(s1,toupper);
}
static inline voba_str_t * voba_tolower(voba_str_t * s1)
{
    return v__map(s1,tolower);
}
static inline int voba_strcmp(const voba_str_t* s1, const voba_str_t * s2)
{
    if(s1 == NULL || s2 == NULL ) return 0;
    if(s1 == s2) return 1;
    if(s1->len != s2->len ) return 0;
    if(s1->data == s2->data)  return 1;
    return strncmp(s1->data,s2->data, (size_t) s1->len);
}
static inline uint32_t voba_strlen(const voba_str_t * s1)
{
    if (!v__is_valid(s1)){
         return (uint32_t)(-1);
    }
    return s1->len;
}
static inline voba_str_t * voba_substr(voba_str_t * s1, uint32_t from, uint32_t len)
{
    if(!v__is_valid(s1)) return NULL;
    if (from >= s1->len || (from + len > s1->len) ){
        return NULL;
    }
    s1->capacity = 0;
    return v__new(0/*capacity*/,len, s1->data + from, 0 /*copy data*/);
}
static inline voba_str_t * voba_str_replace(voba_str_t * s1, char from, char to)
{
    if(!v__is_valid(s1)) return NULL;
    s1 = v__cow(s1);
    for(uint32_t i = 0; i < s1->len; ++i){
        s1->data[i] = s1->data[i] == from?to:s1->data[i];
    }
    return s1;
}
