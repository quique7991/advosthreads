#### GTThread Library Makefile

CFLAGS  = -Wall -g
LFLAGS  =
CC      = gcc
RM      = /bin/rm -rf
AR      = ar rc
RANLIB  = ranlib

CODE_SRC = philosophers.o

LIBRARY = gtthread.a

LIB_SRC = gtthread.c gtthread_mutex.c linked_list.c mutex.c scheduler.c timer.c

LIB_OBJ = $(patsubst %.c,%.o,$(LIB_SRC))

# pattern rule for object files
%.o: %.c
	$(CC) -c $(CFLAGS) $< -o $@

philosophers: $(LIBRARY)
	$(CC) $(CFLAGS) -pedantic -o philosophers.o philosophers.c gtthread.a

all: $(LIBRARY)

$(LIBRARY): $(LIB_OBJ)
	$(AR) $(LIBRARY) $(LIB_OBJ)
	$(RANLIB) $(LIBRARY)

clean:
	$(RM) $(LIBRARY) $(LIB_OBJ) $(CODE_SRC)

.PHONY: depend
depend:
	$(CFLAGS) -- $(LIB_SRC)  2>/dev/null
