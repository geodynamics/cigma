include ../../variables
FLAGS = -Wall -Wno-deprecated
FLAGS += -g
FLAGS += -I..
FLAGS += -I../ann/include
FLAGS += $(VTK_INCLUDES)
FLAGS += $(PYTHON_INCLUDES)
FLAGS += $(HDF5_INCLUDES)


LDFLAGS += -L..
LDFLAGS += $(VTK_LDFLAGS) $(VTK_LIBS)
LDFLAGS += $(PYTHON_LDFLAGS) $(PYTHON_LIBS)
LDFLAGS += $(HDF5_LDFLAGS) $(HDF5_LIBS)

ANNLIB = ../libann.a
CIGMALIB = ../libcigma.a

TESTOBJS = \
	TestTri.o \
	TestQuad.o \
	TestTet.o \
	TestHex.o \
	TestPoints.o \
	TestZeroField.o \
	TestTimer.o \
	TestStringUtils.o \
	TestCellVolume.o \


TESTS = $(TESTOBJS:.o=.out)

default: $(TESTOBJS) $(TESTS)

Test%.o: Test%.cpp
	$(CXX) $(FLAGS) -c $^

Test%.out: Test%.o
	$(CXX) -g $(LDFLAGS) $^ $(CIGMALIB) $(ANNLIB) -o $@

clean:
	rm -f $(TESTOBJS)
	rm -f $(TESTS)

purge:
	rm -f $(TESTS)

.PHONY: clean

