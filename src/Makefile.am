
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
	Locator.h \
	Locator.cpp \
	AnnLocator.h \
	AnnLocator.cpp \
	\
	Points.h \
	Points.cpp \
	QuadraturePoints.h \
	QuadraturePoints.cpp \
	QuadratureRule.h \
	QuadratureRule.cpp \
	\
	MeshPart.h \
	MeshPart.cpp \
	CubeMeshPart.h \
	CubeMeshPart.cpp \
	\
	DofHandler.h \
	DofHandler.cpp \
	\
	Field.h \
	Field.cpp \
	FieldSet.h \
	FieldSet.cpp \
	UserField.h \
	UserField.cpp \
	NullField.h \
	NullField.cpp \
	FE_Field.h \
	FE_Field.cpp \
	PointField.h \
	PointField.cpp \
	ExtField.h \
	ExtField.cpp \
	ZeroField.h \
	ZeroField.cpp \
	UnitScalarField.h \
	UnitScalarField.cpp \
	TestBenchmarkFields.h \
	TestBenchmarkFields.cpp \
	GaleBenchmarkFields.h \
	GaleBenchmarkFields.cpp \
	OkadaBenchmarkFields.h \
	OkadaBenchmarkFields.cpp \
	okada/dc3d.h \
	okada/dc3d.cpp \
	okada/disloc3d.h \
	okada/disloc3d.cpp \
	okada/cervelli.h \
	okada/cervelli.cpp \
	\
	Residuals.h \
	Residuals.cpp \
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
	VtkReader2.h \
	VtkReader2.cpp \
	VtkList.h \
	VtkList.cpp \
	\
	MeshPartReader.h \
	MeshPartReader.cpp \
	QuadratureReader.h \
	QuadratureReader.cpp \
	PointsReader.h \
	PointsReader.cpp \
	FieldReader.h \
	FieldReader.cpp \
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
	HelpCmd.h \
	HelpCmd.cpp \
	ListCmd.h \
	ListCmd.cpp \
	ExtractCmd.h \
	ExtractCmd.cpp \
	EvalCmd.h \
	EvalCmd.cpp \
	CompareCmd.h \
	CompareCmd.cpp \
	\
	SkelCmd.h \
	SkelCmd.cpp \
	CubeCmd.h \
	CubeCmd.cpp \
	SearchCmd.h \
	SearchCmd.cpp \
	InfoCmd.h \
	InfoCmd.cpp \
	\
	AnyOption.h \
	AnyOption.cpp \
	convert.h \
	StringUtils.h \
	StringUtils.cpp \
	PathUtils.h \
	PathUtils.cpp \
	Timer.h \
	Timer.cpp \
	\
	Misc.h \
	Misc.cpp

EXTRA_DIST = \
	rules.py \
	cigmalib/__init__.py \
	cigmalib/api.py \
	cigmalib/Cell.py \
	cigmalib/FiatLagrange.py \
	cigmalib/FiatQuadrature.py \
	cigmalib/FiatShapes.py \
	cigmalib/FiatSimplex.py

