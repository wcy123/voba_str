#pragma once
#include <stdint.h>
#include <stdio.h>
#ifndef INLINE
# if __GNUC__ && !__GNUC_STDC_INLINE__
#  define INLINE extern inline
# else
#  define INLINE inline
# endif
#endif

#ifdef __cplusplus
extern "C" {
#endif
/** @file 
Voba String Library
=====================

\verbatim
+----------------+------------+       
|  const char * data          |       
+----------------+------------+       
| capcity(32bits)| len(32bit) |       
+-----------------------------+       
\endverbatim

in order to keep it simple, the above structure is used, see voba_str_s

*/

/** the key data structure of this library 
 */
typedef struct voba_str_s {
    /** 
	the type_tag is used for voba_value
     */
    int64_t type_tag;
    /** if voba_str_s::data is NULL, it is an invalid string.
     * there is '\0' at the end.*/
    char *   data; 
    /** the size of buffer. 
     *
     *  when voba_str_s::capacity is 0, voba_str_s::data pointed to a
     *  readonly data.
     */
    uint32_t capacity;
    /** the length of string
     *
     * it does not include the '\0' at the end of buffer.
     * 
     */
    uint32_t len;/*!< the size of the string*/
} voba_str_t;
/** \name constructors */
/**@{ */    
/** create an empty string
 *  - voba_str_s::capacity = 0, readonly data.
 *  - voba_str_s::len = 0
 *  - voba_str_s::data = ""
 * */
INLINE voba_str_t * voba_str_empty();
/** create an string from char
 * @param c the char
 * @param len length of the string
 * @return a string
 * @code{.c}
 *    voba_str_t * s = voba_str_from_char('a',4);
 * @endcode
 * \a s is a string "aaaa"
 * */     
INLINE voba_str_t * voba_str_from_char(char c, uint32_t len);
/** format a pointer
 */
INLINE voba_str_t * voba_str_fmt_pointer(const void * p);
INLINE voba_str_t * voba_str_fmt_int8_t(int8_t x, int8_t base);
INLINE voba_str_t * voba_str_fmt_uint8_t(uint8_t x, uint8_t base);
INLINE voba_str_t * voba_str_fmt_int16_t(int16_t x, int16_t base);
INLINE voba_str_t * voba_str_fmt_uint16_t(uint16_t x, uint16_t base);
INLINE voba_str_t * voba_str_fmt_int32_t(int32_t x, int32_t base);
INLINE voba_str_t * voba_str_fmt_uint32_t(uint32_t x, uint32_t base);
INLINE voba_str_t * voba_str_fmt_int64_t(int64_t x, int64_t base);
INLINE voba_str_t * voba_str_fmt_uint64_t(uint64_t x, uint64_t base);
// why there is a dummy flag? because a macro could be used to change
// str_fmt_xxx_t, other fmt_xxx function has additional argument
// `base`, so that fmt_float must have a dummy flag
INLINE voba_str_t * voba_str_fmt_float(float,int flag);
INLINE voba_str_t * voba_str_from_file(const char * filename);
INLINE voba_str_t * voba_str_from_FILE(FILE * fp);
INLINE voba_str_t * voba_str_from_cstr(const char * str);
/**@} */

/** \name methods */
/**@{ */        
INLINE const char * voba_str_to_cstr(voba_str_t *s );
INLINE char       * voba_str_to_str(voba_str_t *s );
INLINE voba_str_t * voba_str_from_data(const void * p, uint32_t len );
INLINE voba_str_t * voba_strcat(voba_str_t * s1, const voba_str_t * s2);
#define VOBA_STRCAT(...) voba_vstrcat(voba_str_empty(), ##__VA_ARGS__ , NULL)
#define VOBA_CONST_CHAR(s) voba_str_from_data(s,sizeof(s) - 1)
__attribute__((used))static voba_str_t * voba_vstrcat(voba_str_t * s1, ...);
INLINE voba_str_t * voba_strcat_cstr(voba_str_t * s1, const char * s2);
INLINE voba_str_t * voba_strcat_char(voba_str_t * s1, const char c);
INLINE voba_str_t * voba_strcat_data(voba_str_t * s1, const char * data, uint32_t len);
INLINE voba_str_t * voba_strclr(voba_str_t * s1);
INLINE voba_str_t * voba_strdup(const voba_str_t * s1);
INLINE voba_str_t * voba_strcpy(voba_str_t * s1, voba_str_t * s2);
INLINE voba_str_t * voba_strcpy_cstr(voba_str_t * s1, const char * s2);
INLINE voba_str_t * voba_strcpy_data(voba_str_t * s1, const void * data, uint32_t len);
INLINE voba_str_t * voba_toupper(voba_str_t * s1);
INLINE voba_str_t * voba_tolower(voba_str_t * s1);
INLINE int          voba_strcmp(const voba_str_t* s1, const voba_str_t * s2);
INLINE uint32_t     voba_strlen(const voba_str_t * s1);
/** @brief return a substr

No new memory allocated, the return value shares the same memory as
`s1`.

 */   
INLINE voba_str_t * voba_substr(voba_str_t * s1, uint32_t from, uint32_t to);
INLINE voba_str_t * voba_str_replace(voba_str_t * s1, char from, char to);
INLINE uint32_t     voba_str_find(voba_str_t * s1, char c, uint32_t from);
/**@} */
#include "voba_str.c"

#ifdef __cplusplus
}
#endif



