#include "./voba_str.h"
static void dump_string(voba_str_t *p);
inline static void dump_string(voba_str_t *p)
{
    if(p == NULL){
        printf("(null)\n");
        return;
    }
    printf("%p(%d,%d) \"%s\"\n",(void*)p->data,p->capacity,p->len, p->data);
}

int main(int argc, char *argv[])
{
    voba_str_t * p1 = voba_str_from_cstr("hello");
    dump_string(p1);
    voba_str_t * p2 = voba_str_from_cstr(NULL);
    dump_string(p2);
    voba_str_t * p3 = voba_str_from_data("hello",5);
    dump_string(p3);
    p3 = voba_str_cat(p3, voba_str_from_cstr(" world"));
    dump_string(p3);
    p3 = voba_str_cat_cstr(p3,NULL);
    p3 = voba_str_cat_cstr(p3,"");
    p3 = voba_str_cat_data(p3,"!",1);
    dump_string(p3);
    voba_str_t * p4 = voba_str_copy(p3);
    dump_string(p4);
    while(p4->len < 16){
        p4 = voba_str_cat_char(p4,' ');
        dump_string(p4);        
    }
    p4 = voba_str_clear(p4);
    dump_string(p4);
    p4 = voba_str_assign(p4,p1);
    dump_string(p4);
    p4 = voba_str_assign_cstr(p4,"hello world!");
    dump_string(p4);
    p4 = voba_str_assign_data(p4,"hello world!",4);
    dump_string(p4);
    p4 = voba_str_toupper(p4);
    dump_string(p4);
    p4 = voba_str_tolower(p4);
    dump_string(p4);
    printf("cmp = %s\n", voba_str_eq(p4,voba_str_from_cstr("hell"))?"true":"false");
    printf("cmp = %s\n", voba_str_eq(p4,voba_str_from_cstr("1hell"))?"true":"false");
    voba_str_t * p5 = voba_str_substr(p4,1,3);
    dump_string(p5);
    return 0;
}

