include ../../build/variables

LIB = ../lib/libh5io.a

CFLAGS = $(OPTIM) $(FLAGS)

SRC = \
	h5array.c \
	h5attr.c \
	h5dset.c \
	h5file.c \
	h5group.c \
	split.c \

OBJ = $(SRC:.c=.o)

.SUFFIXES: .o .c

$(LIB): $(OBJ)
	$(AR) $(LIB) $(OBJ)
	$(RANLIB) $(LIB)

.c.o:
	$(CC) $(CFLAGS) -c $<

tests:
	cd t && make tests

clean:
	rm -f *.o
	cd t && make clean

-include Makefile.dep
depend:
	$(CC) -MM $(SRC) > Makefile.dep

.PHONY: clean depend
