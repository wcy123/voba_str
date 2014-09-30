#include <stdio.h>
#include <stdarg.h>
#include <assert.h>
#include <string.h>
#include <gc.h>
#include <ctype.h>

#include "voba_str.h"



INLINE
int v__is_valid(const voba_str_t * s1)
{
    if (s1 == NULL || s1->data == NULL ||
        (s1->capacity != 0 && s1->capacity < s1->len + 1)){
        return 0;
    }
    return 1;
}
INLINE
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
INLINE
char * v__new_data(uint32_t capacity, uint32_t len, const char * data) 
{
    // initialized with ending '\0'
    char *  p  = (char*) GC_MALLOC_ATOMIC(capacity);
    assert(p);
    if(len) memcpy(p, data, len);
    p[len] = '\0';
    return p;
}
INLINE
voba_str_t * v__new(uint32_t capacity, uint32_t len, const char * data, int copy_data)
{
    voba_str_t * ret = (voba_str_t*)GC_MALLOC(sizeof(voba_str_t));
    assert(capacity == 0 || len  < capacity);
    ret->capacity = capacity;
    ret->len = len;
    if(copy_data){
        ret->data = v__new_data(capacity,len,data);
    }else{
        ret->data = (char*)data;
    }
    return ret;
}
INLINE
void v__clear(voba_str_t * s)
{
    s->len = 0;
    if(s->capacity == 0){
        s->data = (char*)"";
    }
}
INLINE
voba_str_t * v__cow(voba_str_t * s1)
{
    if(s1->capacity == 0){
        // copy-on-write
        const uint32_t capacity = v__len_to_capacity(s1->len+1);
        s1 = v__new(capacity,s1->len,s1->data, 1 /*copy data*/);
    }
    return s1;
}
INLINE
voba_str_t * v__map(voba_str_t * s1, int (*f)(int))
{
    if(!v__is_valid(s1)) return NULL;
    s1 = v__cow(s1);
    for(uint32_t i = 0; i < s1->len; ++i){
        s1->data[i] = (char) f((int)s1->data[i]);
    }
    return s1;
}
INLINE
voba_str_t * v__cat_data(voba_str_t * s1, const void * data, uint32_t len)
{
    voba_str_t * r = s1;
    if(r->capacity == 0) {
        // copy-on-write
        const uint32_t capacity = v__len_to_capacity(len + r->len + 1);
        r = v__new(capacity,r->len,r->data,1 /*copy data*/);
    }else{
        const uint32_t capacity = v__len_to_capacity(len + r->len + 1);
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
INLINE voba_str_t * voba_str_empty()
{
    return v__new(0 /*capacity*/, 0 /*len*/, "" /*data*/, 0 /*copy data*/);
}
INLINE voba_str_t * voba_str_from_char(char c, uint32_t len)
{
    const uint32_t capacity = v__len_to_capacity(len + 1);
    char * p = (char*) GC_MALLOC_ATOMIC(capacity);
    memset((void*)p,(int)c,len);
    p[len] = '\0';
    return v__new(capacity,len,p, 0 /*copy data*/);
}
INLINE voba_str_t * voba_str_fmt_pointer(const void * p)
{
    uint32_t i = 0;
    int s = (sizeof(uintptr_t)*8) -4;
    uintptr_t mask = (uintptr_t) (((uintptr_t)0xF) << s);
    uintptr_t x = (uintptr_t) p;
    voba_str_t * r = voba_str_from_char(' ', s/4 + 4);
    r->data[i++] = '0';
    r->data[i++] = 'x';
    while(((x&mask)>>s) == 0 && s>0) { mask >>= 4; s -= 4; }
    while(mask!= 0) {
        r->data[i++] = ("0123456789ABCDEF")[(x&mask)>>s];
        mask >>= 4; s -= 4;
    }
    r->len = i;
    r->data[i++] = '\0';
    return r;
}
#define VOBA_STR_FMT_INT(type,sign)                                     \
INLINE voba_str_t * voba_str_fmt_##type(type x, type base)       \
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
    voba_str_t * r = voba_str_from_char(' ',len);                       \
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
INLINE voba_str_t * voba_str_fmt_float(float x, int flag)
{
    return voba_str_from_cstr("not implemented");
}
INLINE voba_str_t * voba_str_from_file(const char * filename)
{
    FILE * fp = fopen(filename, "r");
    if(fp == NULL){
        return NULL;
    }
    voba_str_t * ret = voba_str_from_FILE(fp);
    fclose(fp);
    return ret;
}
INLINE voba_str_t * voba_str_from_FILE(FILE * fp)
{
    voba_str_t * ret = NULL;
    int c;
    if(fp == NULL) return NULL;
    ret = voba_str_empty();
    while((c = fgetc(fp))!= EOF){
        ret = voba_strcat_char(ret,(char)c);
    }
    return ret;
}
INLINE voba_str_t * voba_str_from_cstr(const char * str)
{
    str = (str==NULL?"":str);
    const size_t len = strlen(str);
    return v__new(0 /*capacity*/,len,(const char*) str, 0 /*copy data*/);
}
INLINE const char * voba_str_to_cstr(voba_str_t * s)
{
    if(!v__is_valid(s)){
        return "";
    }
    // when s->capacity !=0, `s` own `data`.
    if( s->data[s->len] == '\0'){
        return s->data;
    }
    return v__new_data(s->len+1,s->len, s->data);
}
INLINE char * voba_str_to_str(voba_str_t *s )
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
INLINE voba_str_t * voba_str_from_data(const void * p, uint32_t len )
{
    if(len == 0 || p == NULL){
        return v__new(0 /*capacity*/, 0 /*len*/, "" /*data*/, 0 /*copy data*/);
    }
    return v__new(0,len,(const char*)p,0/*copy data*/);
}
INLINE voba_str_t * voba_strcat(voba_str_t * s1, const voba_str_t * s2)
{
    if(!v__is_valid(s1)) return NULL;
    if (s2 == NULL || s2->data == NULL || (s2->capacity !=0 && s2->capacity < s2->len + 1)){
        return NULL;
    }
    return v__cat_data(s1,(const void*)s2->data,s2->len);
}
INLINE voba_str_t * voba_strcat_cstr(voba_str_t * s1, const char * s2)
{
    if(!v__is_valid(s1)) return NULL;
    if(s2 == NULL || *s2 == '\0'){
        return s1;
    }
    return v__cat_data(s1,s2,(uint32_t)strlen(s2));
}
INLINE voba_str_t * voba_strcat_char(voba_str_t * s1, const char c)
{
    if(!v__is_valid(s1)) return NULL;
    return v__cat_data(s1,&c,1);
}
INLINE voba_str_t * voba_strcat_data(voba_str_t * s1, const char * data, uint32_t len)
{
    if(!v__is_valid(s1)) return NULL;
    if(data == NULL || len == 0){
        return s1;
    }
    return v__cat_data(s1,data,len);
}
static voba_str_t * voba_vstrcat(voba_str_t * s1, ...)
{
    int n = 0;
    va_list ap;
    va_start(ap, s1);
    voba_str_t * s ;
    for(n = 0, s = va_arg(ap,voba_str_t*);
        n < 100 && s!= NULL;
        n++, s = va_arg(ap,voba_str_t*)){
        s1 = voba_strcat(s1,s);
    }
    va_end(ap);
    return s1;
}
INLINE voba_str_t * voba_strclr(voba_str_t * s1)
{
    if(!v__is_valid(s1)) return NULL;
    v__clear(s1);
    return s1;
}
INLINE voba_str_t * voba_strdup(const voba_str_t * s1)
{
    if(!v__is_valid(s1)) return NULL;
    const uint32_t capacity = s1->capacity == 0? v__len_to_capacity(s1->len+1):s1->capacity;
    return v__new(capacity, s1->len, s1->data, 1 /*copy data*/);
}
INLINE voba_str_t * voba_strcpy(voba_str_t * s1, voba_str_t * s2)
{
    if(!v__is_valid(s1)) return NULL;
    if(!v__is_valid(s2)) return NULL;
    s1->capacity = s2->capacity = 0; // no one own the data
    s1->len = s2->len;
    s1->data = s2->data;
    return s1;
}
INLINE voba_str_t * voba_strcpy_cstr(voba_str_t * s1, const char * s2)
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
INLINE voba_str_t * voba_strcpy_data(voba_str_t * s1, const void * data, uint32_t len)
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
INLINE voba_str_t * voba_toupper(voba_str_t * s1)
{
    return v__map(s1,toupper);
}
INLINE voba_str_t * voba_tolower(voba_str_t * s1)
{
    return v__map(s1,tolower);
}
INLINE int voba_strcmp(const voba_str_t* s1, const voba_str_t * s2)
{
    int ret = 0;
    if(s1 == NULL) ret =  -1;
    else if(s2 == NULL ) ret =  1;
    else if(s1 == s2) ret =  0;
    else if(s1->len > s2->len ) ret =  1;
    else if(s1->len < s2->len ) ret =  -1;
    else if(s1->data == s2->data)  ret =  0;
    else ret = strncmp(s1->data,s2->data, (size_t) s1->len);
    //fprintf(stderr,__FILE__ ":%d:[%s] ret = %d %s %s %d %d\n", __LINE__, __FUNCTION__,ret,s1->data,s2->data,s1->len,s2->len);
    return ret;
}
INLINE uint32_t voba_strlen(const voba_str_t * s1)
{
    if (!v__is_valid(s1)){
         return 0;
    }
    return s1->len;
}
INLINE voba_str_t * voba_substr(voba_str_t * s1, uint32_t from, uint32_t len)
{
    if(!v__is_valid(s1)) return NULL;
    if (from >= s1->len || (from + len > s1->len) ){
        return NULL;
    }
    s1->capacity = 0;
    return v__new(0/*capacity*/,len, s1->data + from, 0 /*copy data*/);
}
INLINE voba_str_t * voba_str_replace(voba_str_t * s1, char from, char to)
{
    if(!v__is_valid(s1)) return NULL;
    s1 = v__cow(s1);
    for(uint32_t i = 0; i < s1->len; ++i){
        s1->data[i] = s1->data[i] == from?to:s1->data[i];
    }
    return s1;
}
INLINE uint32_t voba_str_find(voba_str_t * s1, char c, uint32_t from)
{
    if(!v__is_valid(s1)) return UINT32_MAX;
    for(uint32_t i = from; i < s1->len; ++i){
        if(s1->data[i] == c){
            return i;
        }
    }
    return UINT32_MAX;
}
