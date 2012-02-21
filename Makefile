
OBJS=keydb.o

GCC=gcc

GLIB_INCLUDE=`pkg-config --cflags glib-2.0`
GLIB_LIB=`pkg-config --libs glib-2.0`

CFLAGS= -I. $(GLIB_INCLUDE) -g
LFLAGS=$(GLIB_LIB)

test_keydb: $(OBJS) test_keydb.o
	$(GCC) $(CFLAGS) -o $@ $+ $(LFLAGS)

network: $(OBJS) network.o
	$(GCC) $(CFLAGS) -o $@ $+ $(LFLAGS)

clean:
	rm $(OBJS) network test_keydb