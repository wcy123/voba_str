#include <limits.h>
#include <sys/time.h>
#include "./voba_str.h"
double get_time(void)
{
    struct timeval tv;
    gettimeofday(&tv, NULL);
    return tv.tv_sec + (tv.tv_usec / 1000000.0);
}
static void dump_string(voba_str_t *p);
inline static void dump_string(voba_str_t *p)
{
    if(p == NULL){
        printf("(null)\n");
        return;
    }
    printf("%p(%d,%d) \"%s\"\n",(void*)p->data,p->capacity,voba_strlen(p), p->data);
}
int test_1()
{
    dump_string(NULL);
    return 0;
}
int test_2()
{
    voba_str_t * s1 = voba_str_empty();
    dump_string(s1);
    return 0;
}
int test_3()
{
    voba_str_t * s1 = voba_mkstr('!',32) ;
    assert(s1->capacity == 64);
    assert(voba_strlen(s1) == 32);
    dump_string(s1);
    return 0;
}
int test_4()
{
    char buf[1024];
    voba_str_t * s1 = voba_str_empty();
    voba_str_t * s2 = voba_str_fmt_pointer((void*) s1);
    snprintf(buf,1024,"%p",(void*)s1);
    s2 = voba_tolower(s2);
    printf("the voba_str_fmt_pointer %s snprintf  %s\n",s2->data, buf);
    assert(voba_strcmp(s2,voba_str_from_cstr(buf)) == 0);
    dump_string(s2);
    return 0;
}
int test_5()
{
    char buf[1024];
    int8_t x = 0;
    for(unsigned long i = 0 ; i < 256; i++,x++){
        voba_str_t * s2 = voba_str_fmt_int8_t(x,10);
        snprintf(buf,1024,"%d",x);
        if(!(voba_strcmp(s2,voba_str_from_cstr(buf))==0)){
            dump_string(s2);
            printf("snprinf return %s\n",buf);
            assert(0);
        }
    }
    return 0;
}
int test_6()
{
    char buf[1024];
    uint8_t x = 0;
    for(unsigned long i = 0 ; i < 256; i++,x++){
        voba_str_t * s2 = voba_str_fmt_uint8_t(x,10);
        snprintf(buf,1024,"%d",x);
        if(!(voba_strcmp(s2,voba_str_from_cstr(buf))==0)){
            dump_string(s2);
            printf("snprinf return %s\n",buf);
            assert(0);
        }
    }
    return 0;
}
int test_7()
{
    char buf[1024];
    int16_t x = 0;
    for(unsigned long i = 0 ; i < 0xffff; i++,x++){
        voba_str_t * s2 = voba_str_fmt_int16_t(x,10);
        snprintf(buf,1024,"%d",x);
        if(!(voba_strcmp(s2,voba_str_from_cstr(buf))==0)){
            dump_string(s2);
            printf("snprinf return %s\n",buf);
            assert(0);
        }
    }
    return 0;
}
int test_8()
{
    char buf[1024];
    uint16_t x = 0;
    for(unsigned long i = 0 ; i < 0xffff; i++,x++){
        voba_str_t * s2 = voba_str_fmt_uint16_t(x,10);
        snprintf(buf,1024,"%d",x);
        if(!(voba_strcmp(s2,voba_str_from_cstr(buf))==0)){
            dump_string(s2);
            printf("snprinf return %s\n",buf);
            assert(0);
        }
    }
    return 0;
}
int test_9()
{
    char buf[1024];
    int32_t x = 0;
    double t1 = 0.0;
    double t2 = 0.0;
    double tic = 0.0;
    double toc = 0.0;
    for(unsigned long i = 0 ; i < 0xffff; i++,x++){
        tic = get_time();
        voba_str_t * s2 = voba_str_fmt_int32_t(x,10);
        toc = get_time();
        t1 += toc - tic;
        tic = get_time();
        snprintf(buf,1024,"%d",x);
        toc = get_time();
        t2 += toc - tic;
        if(!(voba_strcmp(s2,voba_str_from_cstr(buf))==0)){
            dump_string(s2);
            printf("snprinf return %s\n",buf);
            assert(0);
        }
    }
    printf("voba time: %lf, snprintf %lf\n",t1,t2);
    return 0;
}
int test_10()
{
    char buf[1024];
    uint32_t x = 0;
    for(unsigned long i = 0 ; i < 0xffff; i++,x++){
        voba_str_t * s2 = voba_str_fmt_uint32_t(x,10);
        snprintf(buf,1024,"%d",x);
        if(!(voba_strcmp(s2,voba_str_from_cstr(buf))==0)){
            dump_string(s2);
            printf("snprinf return %s\n",buf);
            assert(0);
        }
    }
    return 0;
}
int test_11()
{
    voba_str_t * p1 = voba_str_from_cstr("hello");
    dump_string(p1);
    p1 = voba_str_from_cstr(NULL);
    assert(p1->capacity == 0);
    assert(voba_strlen(p1) == 0);
    assert(p1->data != 0);
    assert(*p1->data == '\0');
    dump_string(p1);
    p1 = voba_str_from_cstr("");
    assert(p1->capacity == 0);
    assert(voba_strlen(p1) == 0);
    assert(p1->data != 0);
    assert(*p1->data == '\0');
    dump_string(p1);
    return 0;
}
int test_12()
{
    voba_str_t* p1 = voba_str_from_cstr("");
    assert(p1->capacity == 0);
    assert(voba_strlen(p1) == 0);
    assert(p1->data != 0);
    assert(*p1->data == '\0');
    voba_str_t* p2 = voba_strcat(p1, voba_str_from_cstr("GOOD"));
    // p1 is not touched
    assert(p1->capacity == 0);
    assert(voba_strlen(p1) == 0);
    assert(p1->data != 0);
    assert(*p1->data == '\0');
    assert(p2->capacity == 8);
    assert(voba_strlen(p2) == 4);
    assert(p2->data != 0);
    p1 = voba_strcat_cstr(p2, " GOOD");
    assert(p1 == p2);
    assert(p2->capacity == 16);
    assert(voba_strlen(p2) == 9);
    dump_string(p2);
    p1 = voba_strcat_cstr(p2,NULL);
    assert(p2->capacity == 16);
    assert(voba_strlen(p2) == 9);
    assert(p1 == p2);
    p1 = voba_strcat_cstr(p2,"");
    assert(p2->capacity == 16);
    assert(voba_strlen(p2) == 9);
    assert(p1 == p2);
    p1 = voba_strcat_data(p2,"!",1);
    assert(p2->capacity == 16);
    assert(voba_strlen(p2) == 10);
    assert(p1 == p2);
    while(voba_strlen(p2) < 16){
        p2 = voba_strcat_char(p2,' ');
    }
    assert(p2->capacity == 32);
    assert(voba_strlen(p2) == 16);
    dump_string(p2);
    p1 = voba_strcat(p2,NULL);
    assert(p1 == NULL);
    assert(p2->data !=NULL);
    p1 = voba_strcat(NULL,p2);
    assert(p1 == NULL);
    p1 = voba_strcat_cstr(p2,NULL);
    assert(p1 == p2);
    assert(p2->data !=NULL);
    p1 = voba_strcat_cstr(NULL,NULL);
    assert(p1 == NULL);
    p1 = voba_strcat_char(NULL,' ');
    assert(p1 == NULL);
    p1 = voba_strcat_data(NULL,NULL,100);
    assert(p1 == NULL);
    p1 = voba_strcat_data(p2,NULL,100);
    assert(p1 == p2);
    return 0;
}
int test_13()
{
    voba_str_t * p3 = voba_str_from_cstr("GOOD");
    assert(p3->capacity == 0);
    assert(voba_strlen(p3) == 4);
    voba_str_t * p4 = voba_strdup(p3);
    assert(p3->capacity == 0);
    assert(voba_strlen(p3) == 4);
    assert(p4 != p3);
    assert(p4->capacity == 8);
    assert(voba_strlen(p4) == 4);
    assert(p3->data != p4->data);
    voba_str_t * p5 = voba_strdup(p4);
    assert(p4->capacity == 8);
    assert(voba_strlen(p4) == 4);
    assert(p5->capacity == 8);
    assert(voba_strlen(p5) == 4);
    assert(p5->data != p4->data);
    return 1;
}
int test_14()
{
    voba_str_t * p1 = voba_strclr(NULL);
    assert(p1 == NULL);
    p1  = voba_strdup(NULL);
    assert(p1 == NULL);
    voba_str_t * p4 = voba_str_from_cstr("GOOD");
    voba_strclr(p4);
    assert(p4->capacity == 0);
    assert(voba_strlen(p4) == 0);
    assert(*p4->data == '\0');
    p4 = voba_strdup(p4);
    voba_strclr(p4);
    assert(p4->capacity == 2);
    assert(voba_strlen(p4) == 0);
    assert(p4->data!=NULL);
    assert(*p4->data == '\0');
    return 1;
}
#define RUN_REST(n) do {                                \
printf("runnint test " # n  " begin\n"); test_##n();    \
printf("runnint test " # n " done\n")            ;      \
}while(0)
int test_15()
{
    voba_str_t * p1 = voba_strcpy_cstr(NULL,"GOOD");
    assert(p1 == NULL);

    voba_str_t * p4 = voba_str_empty();
    assert(p4->capacity == 0);
    assert(voba_strlen(p4) == 0);
    assert(p4->data != NULL);
    assert(*p4->data == '\0');    

    p4 = voba_strcpy_cstr(p4,"GOOD");
    assert(p4->capacity == 0);
    assert(voba_strlen(p4) == 4);
    assert(p4->data != NULL);
    assert(*p4->data == 'G');    

    p4 = voba_strcpy_cstr(p4,NULL);
    assert(p4->capacity == 0);
    assert(voba_strlen(p4) == 0);
    assert(p4->data != NULL);
    assert(*p4->data == '\0');    

    p4 = voba_strcpy_data(p4,"GOOD",4);
    assert(p4->capacity == 0);
    assert(voba_strlen(p4) == 4);
    assert(p4->data != NULL);
    assert(*p4->data == 'G');    

    p1 = voba_strcpy_data(NULL,NULL,10);
    assert(p1 == NULL);

    p1 = voba_strcpy_data(p4,NULL,10);
    assert(p1 == NULL);

    
    p4 = voba_mkstr('\0',4);
    char * data = p4->data;
    assert(p4->capacity == 8);
    assert(voba_strlen(p4) == 4);
    assert(p4->data == data);
    assert(*p4->data == '\0');
    p4 = voba_strcpy_cstr(p4,"VERY GOOD");
    assert(p4->capacity == 0);
    assert(voba_strlen(p4) == 9);
    assert(p4->data != data);
    assert(*p4->data == 'V');    
    p4 = voba_strcpy_data(p4,"BAD",3);
    assert(p4->capacity == 0);
    assert(voba_strlen(p4) == 3);
    assert(p4->data != data);
    assert(*p4->data == 'B');
    return 1;
}
int test_16()
{
    voba_str_t* p1 = voba_str_from_cstr("good");

    voba_str_t* p4 = p1;
    assert(p4->capacity == 0);
    assert(voba_strlen(p4) == 4);
    assert(p4->data != NULL);
    assert(*p4->data == 'g');    

    p4 = voba_toupper(p4);
    assert(p4 != p1);
    assert(p4->capacity == 8);
    assert(voba_strlen(p4) == 4);
    assert(p4->data != NULL);
    assert(*p4->data == 'G');    


    p1 = p4;
    p4 = voba_tolower(p4);
    assert(p4 == p1);
    assert(p4->capacity == 8);
    assert(voba_strlen(p4) == 4);
    assert(p4->data != NULL);
    assert(*p4->data == 'g');    


    p1 = voba_toupper(NULL);
    assert(p1 == NULL);
    p1 = voba_tolower(NULL);
    assert(p1 == NULL);

    p1 = voba_str_from_cstr("GOOD");
    p4 = voba_tolower(p1);
    assert(p4 != p1);
    assert(p4->capacity == 8);
    assert(voba_strlen(p4) == 4);
    assert(p4->data != NULL);
    assert(*p4->data == 'g');    
    
    return 1;
}
int test_17()
{
    voba_str_t* p1 = voba_str_from_cstr("good");
    voba_str_t* p4 = p1;
    voba_str_t * p5 = voba_substr(p4,1,3);
    assert(voba_strcmp(p5,voba_str_from_cstr("ood"))==0);

        
    p1 = voba_substr(NULL,1,12);
    assert(p1 == NULL);

    p1 = voba_substr(p4,1,12);
    assert(p1 == NULL);
    return 1;
}
int main(int argc, char *argv[])
{
    RUN_REST(1);
    RUN_REST(2);
    RUN_REST(3);
    RUN_REST(4);
    RUN_REST(5);
    RUN_REST(6);
    RUN_REST(7);
    RUN_REST(8);
    RUN_REST(9);
    RUN_REST(10);
    RUN_REST(11);
    RUN_REST(12);
    RUN_REST(13);
    RUN_REST(14);
    RUN_REST(15);
    RUN_REST(16);        
    RUN_REST(17);
    return 0;
}

