include ../build/variables

FLAGS = -Wall -Wno-deprecated
FLAGS += -g

INCLUDES += $(PYTHON_INCLUDES)
INCLUDES += $(VTK_INCLUDES)
INCLUDES += $(HDF5_INCLUDES)

FLAGS += $(INCLUDES)

TARGETLIB = ../lib/libcigma.a

OBJS = \
	Numeric.o \
	Cell.o \
	Tet.o \
	Hex.o \
	Points.o \
	Quadrature.o \
	FiatProxy.o \
	MeshPart.o \
	VtkUgMeshPart.o \
	DofHandler.o \
	FEValues.o \
	Field.o \
	FEField.o \
	ExternalField.o \
	TextReader.o \
	TextWriter.o \
	VtkUgReader.o \
	VtkUgSimpleWriter.o \
	AnyOption.o \
	StringUtils.o \
	Command.o \
	CommandSet.o \
	SkelCmd.o \
	HelpCmd.o \
	ExtractCmd.o \
	EvalCmd.o \
	CompareCmd.o \
	Misc.o \


default: testlib cigma tests

%.o: %.cpp
	$(CXX) $(FLAGS) -c $^

TESTLIB = ../lib/libcigma_d.a

testlib: purge $(TESTLIB)

$(TESTLIB): $(OBJS)
	rm -f $@
	$(AR) $@ $(OBJS)
	$(RANLIB) $@


cigma: cigma.o $(TESTLIB)
	$(CXX) $(FLAGS) $^ -o $@

tests: $(TESTLIB)
	cd tests/ && make

clean:
	rm -f $(OBJS)

purge: clean
	rm -f $(TESTLIB)

.PHONY: clean purge
