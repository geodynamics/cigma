include ../build/variables

FLAGS = -Wall -Wno-deprecated
FLAGS += -g

INCLUDES += $(PYTHON_INCLUDES)
INCLUDES += $(VTK_INCLUDES)

FLAGS += $(INCLUDES)

TARGETLIB = ../lib/libcigma.a

OBJS = \
	Numeric.o \
	Cell.o \
	Tet.o \
	Hex.o \
	Field.o \
	DofHandler.o \
	FiatProxy.o \
	TextReader.o \
	TextWriter.o \
	VtkUgReader.o \
	VtkUgSimpleWriter.o \
	Command.o \


default: $(OBJS)

%.o: %.cpp
	$(CXX) $(FLAGS) -c $^



TESTLIB = ./tests/libcigma_d.a

testlib: $(TESTLIB)

$(TESTLIB): $(OBJS)
	rm -f $@
	$(AR) $@ $(OBJS)
	$(RANLIB) $@


clean:
	rm -f $(OBJS)

purge: clean
	rm -f $(TARGETLIB)

.PHONY: clean purge
