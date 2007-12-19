include ../../build/variables
FLAGS = -Wall -Wno-deprecated
FLAGS += -g
FLAGS += -I..
FLAGS += $(VTK_INCLUDES)
FLAGS += $(PYTHON_INCLUDES)
LDFLAGS += -L..
LDFLAGS += $(VTK_LDFLAGS) $(VTK_LIBS)
LDFLAGS += $(PYTHON_LIBS)

TESTLIB = ../../lib/libcigma_d.a

TESTOBJS = \
	TestVtkUgReader.o \


TESTS = $(TESTOBJS:.o=)

default: clean $(TESTOBJS) $(TESTS)

Test%.o: Test%.cpp
	$(CXX) $(FLAGS) -c $^

Test%: Test%.o
	$(CXX) $(LDFLAGS) $^ $(TESTLIB) -o $@

clean:
	rm -f $(TESTOBJS)
	rm -f $(TESTS)

purge:
	rm -f $(TESTS)

.PHONY: clean
