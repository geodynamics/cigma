include ../../build/variables
FLAGS = -Wall -Wno-deprecated
FLAGS += -g
FLAGS += -I..
FLAGS += $(VTK_INCLUDES)
FLAGS += $(PYTHON_INCLUDES)
FLAGS += $(HDF5_INCLUDES)
LDFLAGS += -L..
LDFLAGS += $(VTK_LDFLAGS) $(VTK_LIBS)
LDFLAGS += $(PYTHON_LDFLAGS) $(PYTHON_LIBS)
LDFLAGS += $(HDF5_LDFLAGS) $(HDF5_LIBS)

TESTLIB = ../../lib/libcigma_d.a

TESTOBJS = \
	TestAnyOption.o \
	TestStringUtils.o \
	TestNumeric.o \
	TestTet.o \
	TestHex.o \
	TestPoints.o \
	TestQuadrature.o \
	TestMeshPart.o \
	TestDofHandler.o \
	TestFiatProxy.o \


TESTS = $(TESTOBJS:.o=.out)

default: clean $(TESTOBJS) $(TESTS)

Test%.o: Test%.cpp
	$(CXX) $(FLAGS) -c $^

Test%.out: Test%.o
	$(CXX) $(LDFLAGS) $^ $(TESTLIB) -o $@

clean:
	rm -f $(TESTOBJS)
	rm -f $(TESTS)

purge:
	rm -f $(TESTS)

.PHONY: clean

