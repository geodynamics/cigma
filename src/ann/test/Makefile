include ../../../variables
INCLUDE = -I../include
CFLAGS = $(OPTIM) $(FLAGS) $(INCLUDE)

LDFLAGS = -L../../../lib
ANNLIBS = -lann -lm

TARGETS = \
	ann_test\
	ann_sample\
	ann2fig\

SRC = \
	ann2fig.cpp\
	ann_sample.cpp\
	ann_test.cpp\
	rand.cpp\

OBJ = $(SRC:.cpp=.o)

.SUFFIXES: .o .cpp


all: $(TARGETS)

ann_test: ann_test.o rand.o
	$(CXX) $^ -o $@ $(LDFLAGS) $(ANNLIBS)

ann_sample: ann_sample.o
	$(CXX) $^ -o $@ $(LDFLAGS) $(ANNLIBS)

ann2fig: ann2fig.o
	$(CXX) $^ -o $@  $(LDFLAGS) $(ANNLIBS)

.cpp.o:
	$(CXX) $(CFLAGS) -c $<


-include Makefile.dep
depend:
	$(CXX) -MM $(CFLAGS) $(SRC) > Makefile.dep

clean:
	rm -f *.o 
	rm -f $(TARGETS)

.PHONY: clean depend

