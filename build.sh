#
#    -fprofile-arcs -ftest-coverage 

gcc -std=c99 \
    -DINLINE='static inline' \
     -ggdb -O0 \
     -fprofile-arcs -ftest-coverage \
     -Wall -Werror\
    -I /home/chunywan/d/other-working/GC/bdwgc/include \
    -L /home/chunywan/d/other-working/GC/bdwgc/mybuild \
    -o test_voba_str\
    test_voba_str.c \
    -lgc-lib &&
    echo hello | ./test_voba_str &&
    echo hello | gcov ./test_voba_str 
