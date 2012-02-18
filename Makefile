
OBJS=network.o keydb.o

GCC=gcc

GLIB_INCLUDE=`pkg-config --cflags glib-2.0`
GLIB_LIB=`pkg-config --libs glib-2.0`

CFLAGS= -I. $(GLIB_INCLUDE) -g
LFLAGS=$(GLIB_LIB)


network: $(OBJS)
	$(GCC) $(CFLAGS) -o $@ $+ $(LFLAGS)

clean:
	rm $(OBJS) network