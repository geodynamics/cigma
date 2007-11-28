CIGMA = .
HDF5_HOME = /usr
#HDF5_HOME = $(HOME)/opt/hdf5/1.6.5-serial

###############################################################################

FC  = gfortran
CC  = gcc
LD  = gcc
CXX = g++

###############################################################################

WARNINGS = -Wall
OPTIMIZE =
DEBUG    = -g -UNDEBUG
CFLAGS   = $(WARNINGS) $(OPTIMIZE) $(DEBUG)
FFLAGS   = $(WARNINGS) $(OPTIMIZE) $(DEBUG)
CXXFLAGS = $(WARNINGS) $(OPTIMIZE) $(DEBUG)

###############################################################################

LIBS        = -lhdf5 -lpthread -ldl -lm
CIGMA_LIBS  = -lcigma $(LIBS)
OKADA_LIBS  = -lgfortran

STATIC_LIBS = $(CIGMA)/lib/libtmc.a $(CIGMA)/lib/libcigma.a

###############################################################################

INCLUDES  =
INCLUDES += -I$(HDF5_HOME)/include
INCLUDES += -I$(CIGMA)/libcigma
INCLUDES += -I$(CIGMA)/tmc
CFLAGS   += $(INCLUDES)
CXXFLAGS += $(INCLUDES)

###############################################################################

LIBRARIES  =
LIBRARIES += -L$(HDF5_HOME)/lib
LIBRARIES += -L$(CIGMA)/lib
CFLAGS    += $(LIBRARIES)
CXXFLAGS  += $(LIBRARIES)

###############################################################################

LDFLAGS  =
LDFLAGS += -Wl,--rpath -Wl,$(HDF5_HOME)/lib
LDFLAGS += -Wl,--rpath -Wl,$(CIGMA)/lib
LDFLAGS += -Wl,--rpath -Wl,../lib
LDFLAGS += -Wl,--rpath -Wl,.

###############################################################################

COMMANDS = \
	$(CIGMA)/bin/tmc \
	$(CIGMA)/bin/cigma-compare 

TARGETS = \
	$(CIGMA)/lib/libcigma.so \
	$(CIGMA)/lib/libcigma.a \
	$(CIGMA)/lib/libtmc.a

CIGMA_OBJS = \
	$(CIGMA)/libcigma/split.o \
	$(CIGMA)/libcigma/h5_io.o \
	$(CIGMA)/libcigma/h5_attr.o \
	$(CIGMA)/libcigma/dataset.o \
	$(CIGMA)/libcigma/array.o \
	$(CIGMA)/libcigma/mesh.o

TMC_OBJS = \
	$(CIGMA)/tmc/vect_stats.o \
	$(CIGMA)/tmc/Line_3f.o \
	$(CIGMA)/tmc/Vector_3f.o \
	$(CIGMA)/tmc/epsilon.o \
	$(CIGMA)/tmc/Point_3f.o \
	$(CIGMA)/tmc/Plane_RST_3d.o \
	$(CIGMA)/tmc/Plane_3f.o \
	$(CIGMA)/tmc/version.o \
	$(CIGMA)/tmc/Subject.o \
	$(CIGMA)/tmc/Indexed_tetra_set.o \
	$(CIGMA)/tmc/math_util.o \
	$(CIGMA)/tmc/Vector_3d.o \
	$(CIGMA)/tmc/Matrix_4d.o \
	$(CIGMA)/tmc/Point_3d.o \
	$(CIGMA)/tmc/Tetrahedron.o \
	$(CIGMA)/tmc/io_util.o \
	$(CIGMA)/tmc/errors.o \
	$(CIGMA)/tmc/Time_series.o \
	$(CIGMA)/tmc/program_name.o \
	$(CIGMA)/tmc/Exterior_face.o \
	$(CIGMA)/tmc/Shape.o \
	$(CIGMA)/tmc/Vector_3b.o \
	$(CIGMA)/tmc/Indexed_tetra_set_off_reader.o \
	$(CIGMA)/tmc/Observer.o \
	$(CIGMA)/tmc/Bounding_box_3d.o \
	$(CIGMA)/tmc/Point_4f.o \
	$(CIGMA)/tmc/tet_sampling.o \
	$(CIGMA)/tmc/Visitor.o

TMC_OBJS += \
	$(CIGMA)/tmc/anyoption.o \
	$(CIGMA)/tmc/StringUtils.o

TMC_OBJS += \
	$(CIGMA)/tmc/HDF5_Reader.o \
	$(CIGMA)/tmc/Writers.o \
	$(CIGMA)/tmc/QuadratureRule.o

TESTS = 

ALLOBJS = $(CIGMA_OBJS) $(TMC_OBJS)

###############################################################################

all: $(TARGETS) $(COMMANDS) $(TESTS)

$(CIGMA)/lib/libtmc.a: $(TMC_OBJS)
	ar rcs $@ $^

$(CIGMA)/lib/libcigma.a: $(CIGMA_OBJS)
	ar rcs $@ $^

$(CIGMA)/lib/libcigma.so: $(CIGMA_OBJS)
	$(LD) -shared -fPIC $(LDFLAGS) $^ -o $@

$(CIGMA)/bin/tmc: $(CIGMA)/tmc/tmc.cpp $(CIGMA)/lib/libtmc.a
	$(CXX) $(CXXFLAGS) $(LIBS) $^ -o $@

$(CIGMA)/bin/cigma-%: $(CIGMA)/src/cigma-%.cpp $(STATIC_LIBS)
	$(CXX) $(CXXFLAGS) $(LDFLAGS) $(LIBS) $^ -o $@

.c.o:
	$(CC) $(CFLAGS) -c $< -o $@

.cpp.o:
	$(CXX) $(CXXFLAGS) -c $< -o $@

###############################################################################

clean:
	rm -f $(ALLOBJS)

clean-all: clean
	rm -f $(TARGETS)

.PHONY: clean clean-all
