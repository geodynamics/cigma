include ../build/variables

FLAGS = -Wall -Wno-deprecated
FLAGS += -g

INCLUDES += $(PYTHON_INCLUDES)
INCLUDES += $(VTK_INCLUDES)
INCLUDES += $(HDF5_INCLUDES)
INCLUDES += -I./h5io
INCLUDES += -I./ann/include

FLAGS += $(INCLUDES)

LDFLAGS += $(VTK_LDFLAGS) $(VTK_LIBS)

ANNLIB = ../lib/libann.a
H5IOLIB = ../lib/libh5io.a
TARGETLIB = ../lib/libcigma.a
TESTLIB = ../lib/libcigma_d.a

OBJS = \
	Numeric.o \
	Cell.o \
	Tri.o \
	Quad.o \
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
	Reader.o \
	TextReader.o \
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
	Locator.o \
	AnnLocator.o \


default: libann libh5io testlib cigma vtkls tests

%.o: %.cpp
	$(CXX) $(FLAGS) -c $^

testlib: purge $(TESTLIB)

libh5io:
	cd h5io && make

libann:
	cd ann && make

$(TESTLIB): $(OBJS)
	rm -f $@
	$(AR) $@ $(OBJS)
	$(RANLIB) $@

cigma: cigma.o $(TESTLIB) $(H5IOLIB) $(ANNLIB)
	$(CXX) $(LDFLAGS) $^ -o $@

vtkls: vtkls.o $(TESTLIB)
	$(CXX) $(LDFLAGS) $^ -o $@

tests: $(TESTLIB)
	cd tests/ && make

clean:
	rm -f $(OBJS) cigma.o vtkls.o

purge: clean
	rm -f $(TESTLIB)

.PHONY: clean purge
