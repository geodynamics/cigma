include ../../build/variables
FLAGS = -Wall -Wno-deprecated
FLAGS += -g
FLAGS += -I..
FLAGS += -I../h5io
FLAGS += -I../ann/include
FLAGS += $(VTK_INCLUDES)
FLAGS += $(PYTHON_INCLUDES)
FLAGS += $(HDF5_INCLUDES)


LDFLAGS += -L..
LDFLAGS += $(VTK_LDFLAGS) $(VTK_LIBS)
LDFLAGS += $(PYTHON_LDFLAGS) $(PYTHON_LIBS)
LDFLAGS += $(HDF5_LDFLAGS) $(HDF5_LIBS)

ANNLIB = ../../lib/libann.a
H5IOLIB = ../../lib/libh5io.a
TESTLIB = ../../lib/libcigma_d.a

TESTOBJS = \
	TestAnyOption.o \
	TestStringUtils.o \
	TestNumeric.o \
	TestTri.o \
	TestQuad.o \
	TestTet.o \
	TestHex.o \
	TestPoints.o \
	TestQuadrature.o \
	TestMeshPart.o \
	TestDofHandler.o \
	TestFiatProxy.o \
	TestAnn.o \


TESTS = $(TESTOBJS:.o=.out)

default: clean $(TESTOBJS) $(TESTS)

Test%.o: Test%.cpp
	$(CXX) $(FLAGS) -c $^

Test%.out: Test%.o
	$(CXX) $(LDFLAGS) $^ $(TESTLIB) $(H5IOLIB) $(ANNLIB) -o $@

clean:
	rm -f $(TESTOBJS)
	rm -f $(TESTS)

purge:
	rm -f $(TESTS)

.PHONY: clean

