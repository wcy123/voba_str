CFLAGS += -DINLINE='static inline'  -fprofile-arcs -ftest-coverage -Wall -Werror
ifneq ($(CONFIG),release)
	CFLAGS += -ggdb -O0
else
	CFLAGS += -O3 -DNDEBUG -Wno-unused-variable -Wno-unused-but-set-variable
endif
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
