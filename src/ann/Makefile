include ../../variables

LIB = ../../lib/libann.a
INCLUDE = -I./include
CFLAGS = $(OPTIM) $(FLAGS) $(INCLUDE)

SRC = \
	src/kd_util.cpp\
	src/kd_fix_rad_search.cpp\
	src/perf.cpp\
	src/bd_search.cpp\
	src/kd_pr_search.cpp\
	src/brute.cpp\
	src/bd_tree.cpp\
	src/kd_tree.cpp\
	src/kd_search.cpp\
	src/ANN.cpp\
	src/bd_fix_rad_search.cpp\
	src/bd_pr_search.cpp\
	src/kd_dump.cpp\
	src/kd_split.cpp\

OBJ = $(SRC:.cpp=.o)

$(LIB): $(OBJ)
	$(AR) $(LIB) $(OBJ)
	$(RANLIB) $(LIB)

.cpp.o:
	$(CXX) $(CFLAGS) -c $< -o ${<:.cpp=.o}

-include Makefile.dep
depend:
	$(CXX) -MM $(CFLAGS) $(SRC) > Makefile.dep

clean:
	rm -f src/*.o

.PHONY: clean depend
