CC = gcc -I/home/luis/opt/hdf5/1.6.5/include -L/home/luis/opt/hdf5/1.6.5/lib
CFLAGS = -Wall -g -O3
LIBS = -lm -lgfortran -lhdf5

OBJS = \
	dc3d.o \
	disloc3d.o \
	cervelli.o \
	txtarray.o \
	h5array.o \

TARGETS = \
	okadasoln_ng \
	h5points \


all: $(TARGETS) $(OBJS)

dc3d.o: disloc3d_mod2.f
	gfortran -c $< -o $@

.c.o:
	$(CC) $(CFLAGS) -c $<

okadasoln_ng: okadasoln_ng.c $(OBJS)
	$(CC) $(CFLAGS) $^ $(LIBS) -o $@

h5points: h5points.c $(OBJS)
	$(CC) $(CFLAGS) $^ $(LIBS) -o $@

clean:
	rm -f $(TARGETS) $(OBJS)

.PHONY: clean
