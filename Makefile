CFLAGS += -DINLINE='static inline' -ggdb -O0 -fprofile-arcs -ftest-coverage -Wall -Werror
CFLAGS += -I /home/chunywan/d/other-working/GC/bdwgc/include
CFLAGS += -std=c99
LDFLAGS += -L /home/chunywan/d/other-working/GC/bdwgc/mybuild 
LDFLAGS += -lgc-lib

all: test_voba_str.out

test_voba_str: test_voba_str.c
	$(CC) $(CFLAGS) -o $@ $< $(LDFLAGS)

test_voba_str.out: test_voba_str
	sh run.sh > $@

clean:

	rm test_voba_str test_voba_str.out *.o