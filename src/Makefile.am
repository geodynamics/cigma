bin_PROGRAMS = cigma vtkls

noinst_LIBRARIES = libh5io.a libann.a libcigma.a

cigma_SOURCES = cigma.cpp
cigma_LDADD = libcigma.a libh5io.a libann.a -lhdf5
cigma_CPPFLAGS = -I$(srcdir)/h5io -I$(srcdir)/ann/include

vtkls_SOURCES = vtkls.cpp


##############################################################################


libh5io_a_SOURCES = \
	h5io/h5array.c \
	h5io/h5attr.c \
	h5io/h5dset.c \
	h5io/h5file.c \
	h5io/h5group.c \
	h5io/h5io.h \
	h5io/split.c \
	h5io/split.h

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
	AnnLocator.cpp \
	AnnLocator.h \
	AnyOption.cpp \
	AnyOption.h \
	Cell.cpp \
	Cell.h \
	Command.cpp \
	Command.h \
	CommandSet.cpp \
	CommandSet.h \
	CompareCmd.cpp \
	CompareCmd.h \
	convert.h \
	CubeCmd.cpp \
	CubeCmd.h \
	CubeMeshPart.cpp \
	CubeMeshPart.h \
	DofHandler.cpp \
	DofHandler.h
	EvalCmd.cpp \
	EvalCmd.h \
	ExternalField.cpp \
	ExternalField.h \
	ExtractCmd.cpp \
	ExtractCmd.h \
	FE.cpp \
	FE_Field.cpp \
	FE_Field.h \
	FE.h \
	FiatProxy.cpp \
	FiatProxy.h \
	Field.cpp \
	Field.h \
	HdfReader.cpp \
	HdfReader.h \
	HdfWriter.cpp \
	HdfWriter.h \
	HelpCmd.cpp \
	HelpCmd.h \
	Hex.cpp \
	Hex.h \
	Locator.cpp \
	Locator.h \
	MeshPart.cpp \
	MeshPart.h \
	Misc.cpp \
	Misc.h \
	Numeric.cpp \
	Numeric.h \
	Points.cpp \
	Points.h \
	Quad.cpp \
	Quad.h \
	Quadrature.cpp \
	Quadrature.h \
	Reader.cpp \
	Reader.h \
	SkelCmd.cpp \
	SkelCmd.h \
	StringUtils.cpp \
	StringUtils.h \
	Tet.cpp \
	Tet.h \
	TextReader.cpp \
	TextReader.h \
	TextWriter.cpp \
	TextWriter.h \
	Tri.cpp \
	Tri.h \
	VtkReader.cpp \
	VtkReader.h \
	VtkUgMeshPart.cpp \
	VtkUgMeshPart.h \
	VtkUgReader.cpp \
	VtkUgReader.h \
	VtkUgSimpleWriter.cpp \
	VtkUgSimpleWriter.h \
	VtkWriter.cpp \
	VtkWriter.h \
	Writer.cpp \
	Writer.h

