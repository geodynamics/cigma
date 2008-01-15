include ../build/variables

FLAGS = -Wall -Wno-deprecated
FLAGS += -g

INCLUDES += $(PYTHON_INCLUDES)
INCLUDES += $(VTK_INCLUDES)
INCLUDES += $(HDF5_INCLUDES)

FLAGS += $(INCLUDES)

LDFLAGS += $(VTK_LDFLAGS) $(VTK_LIBS)

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
	CubeMeshPart.o \
	DofHandler.o \
	FE.o \
	Field.o \
	FE_Field.o \
	ExternalField.o \
	TextReader.o \
	HdfReader.o \
	VtkUgReader.o \
	Writer.o \
	TextWriter.o \
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
	CubeCmd.o \
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
	$(CXX) $(LDFLAGS) $^ -o $@

tests: $(TESTLIB)
	cd tests/ && make

clean:
	rm -f $(OBJS) cigma.o

purge: clean
	rm -f $(TESTLIB)

.PHONY: clean purge
