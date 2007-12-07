include ../build/variables
CXXFLAGS = -Wall -g
INCLUDES = -I/usr/include/python2.5 -I/usr/lib/python2.5/site-packages/numpy/core/include/numpy
LDFLAGS =
LIBS = -lpython2.5

FLAGS += $(INCLUDES)
CIGMA_LIBS += $(LIBS)

OBJS = \
	Numeric.o \
	DataIO.o \
	TextIO.o \
	VtkIO.o \
	Cell.o \
	Tet.o \
	Hex.o \
	Field.o \
	MeshPart.o \
	DofHandler.o \
	FiatProxy.o \


default: $(OBJS)

%.o: %.cpp
	$(CXX) $(FLAGS) -c $^

clean:
	rm -f $(OBJS)

.PHONY: clean
