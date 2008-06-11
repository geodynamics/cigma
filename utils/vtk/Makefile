CXX = g++
CXXFLAGS = -Wall -Wno-deprecated
INCLUDES = -I/usr/include/vtk-5.0
LIBS = -lvtkCommon -lvtkIO -lboost_filesystem

TARGETS = \
	mesh-info \
	ucd2vtk \
	pvtu2vtk \
	pvtu-collect \
	pvtr-collect \

all: $(TARGETS)

%: %.cc
	$(CXX) $(CXXFLAGS) $(INCLUDES) $< -o $@ $(LIBS)

clean:
	rm -f $(TARGETS)

.PHONY: clean
