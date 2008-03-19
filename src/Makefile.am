
bin_PROGRAMS = cigma

noinst_LIBRARIES = libann.a libcigma.a

libann_a_CPPFLAGS = -I$(srcdir)/ann/include

libcigma_a_CPPFLAGS = -Wno-deprecated $(HDF5_INCLUDES) $(PYTHON_INCLUDES) $(VTK_INCLUDES) -I$(srcdir)/ann/include

cigma_CPPFLAGS = -Wno-deprecated $(HDF5_INCLUDES) $(PYTHON_INCLUDES) $(VTK_INCLUDES) -I$(srcdir)/ann/include
cigma_LDFLAGS = -I$(srcdir)/ann/include
cigma_LDADD = $(HDF5_LIBS) $(HDF5_LDFLAGS) -L$(srcdir) $(PYTHON_LDFLAGS) $(PYTHON_LIBS) libcigma.a libann.a

##############################################################################

cigma_SOURCES = \
	cigma.cpp


libann_a_SOURCES = \
	ann/include/ANN/ANN.h \
	ann/include/ANN/ANNperf.h \
	ann/include/ANN/ANNx.h \
	ann/src/ANN.cpp \
	ann/src/bd_fix_rad_search.cpp \
	ann/src/bd_pr_search.cpp \
	ann/src/bd_search.cpp \
	ann/src/bd_tree.cpp \
	ann/src/bd_tree.h \
	ann/src/brute.cpp \
	ann/src/kd_dump.cpp \
	ann/src/kd_fix_rad_search.cpp \
	ann/src/kd_fix_rad_search.h \
	ann/src/kd_pr_search.cpp \
	ann/src/kd_pr_search.h \
	ann/src/kd_search.cpp \
	ann/src/kd_search.h \
	ann/src/kd_split.cpp \
	ann/src/kd_split.h \
	ann/src/kd_tree.cpp \
	ann/src/kd_tree.h \
	ann/src/kd_util.cpp \
	ann/src/kd_util.h \
	ann/src/perf.cpp \
	ann/src/pr_queue.h \
	ann/src/pr_queue_k.h

libcigma_a_SOURCES = \
	\
	Numeric.h \
	Numeric.cpp \
	\
	Cell.h \
	Cell.cpp \
	Hex.h \
	Hex.cpp \
	Tet.h \
	Tet.cpp \
	Quad.h \
	Quad.cpp \
	Tri.h \
	Tri.cpp \
	\
	MeshPart.h \
	MeshPart.cpp \
	CubeMeshPart.h \
	CubeMeshPart.cpp \
	\
	HdfFile.h \
	HdfFile.cpp \
	HdfAttr.h \
	HdfAttr.cpp \
	HdfDataset.h \
	HdfDataset.cpp \
	\
	Reader.h \
	Reader.cpp \
	NullReader.h \
	NullReader.cpp \
	HdfReader.h \
	HdfReader.cpp \
	TextReader.h \
	TextReader.cpp \
	VtkReader.h \
	VtkReader.cpp \
	VtkList.h \
	VtkList.cpp \
	\
	Writer.h \
	Writer.cpp \
	NullWriter.h \
	NullWriter.cpp \
	HdfWriter.h \
	HdfWriter.cpp \
	TextWriter.h \
	TextWriter.cpp \
	VtkWriter.h \
	VtkWriter.cpp \
	\
	Command.h \
	Command.cpp \
	CommandSet.h \
	CommandSet.cpp \
	SkelCmd.h \
	SkelCmd.cpp \
	HelpCmd.h \
	HelpCmd.cpp \
	ListCmd.h \
	ListCmd.cpp \
	CubeCmd.h \
	CubeCmd.cpp \
	\
	AnyOption.h \
	AnyOption.cpp \
	StringUtils.h \
	StringUtils.cpp

