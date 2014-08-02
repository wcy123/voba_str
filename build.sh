gcc -std=c99 \
    -ggdb -O0 \
    -I /home/chunywan/d/other-working/GC/bdwgc/include \
    -L /home/chunywan/d/other-working/GC/bdwgc/mybuild \
    -o test_voba_str\
    test_voba_str.c \
    -lgc-lib &&
    ./test_voba_str
