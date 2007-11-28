#ifndef GTB_INDEXED_TETRA_SET_OFF_READER_INCLUDED
#include "Indexed_tetra_set_off_reader.hpp"
#endif

#ifndef GTB_INDEXED_TETRA_SET_INCLUDED
#include "Indexed_tetra_set.hpp"
#endif

#ifndef GTB_TIME_SERIES_INCLUDED
#include "Time_series.hpp"
#endif

GTB_BEGIN_NAMESPACE


Indexed_tetra_set_off_reader::Indexed_tetra_set_off_reader()
{
}


void Indexed_tetra_set_off_reader::read(FILE *in_off_fp,
					Indexed_tetra_set **out_its,
					Scalar_time_series **out_sts)
{
	GTB_REQUIRE(in_off_fp);

        // start fresh (same reader can be used multiple times)
	m_vertices.clear();
	m_tetrahedra.clear();

	unsigned num_vertices;
	unsigned num_tetrahedra;
	if (fscanf(in_off_fp, "%u %u", &num_vertices, &num_tetrahedra) != 2) {
		GTB_ERROR("failed to read the header");
	}

	// write scalars header
// 	unsigned num_times = 1;
// 	unsigned num_scalars_per_time = num_vertices;
// 	Scalar_time_series::File_header header(num_times,
// 					       num_scalars_per_time,
// 					       FLT_MAX,
// 					       FLT_MIN);
// 	header.write(out_scalars_fp);
	std::vector<float> scalars(num_vertices);

	// for each vertex
	float min_scalar = FLT_MAX;
	float max_scalar = FLT_MIN;
	m_vertices.resize(num_vertices);
	for (unsigned i = 0; i < num_vertices; i++) {
		// read vertex and scalar
		Point_3f p;
		float s;
		if (fscanf(in_off_fp, "%f %f %f %f",
			   &p[0], &p[1], &p[2], &s) != 4) {
			GTB_ERROR("failed to read the vertices");
		}
		m_vertices[i] = p;

		// update min and max
		if (s < min_scalar) {
			min_scalar = s;
		}
		if (s > max_scalar) {
			max_scalar = s;
		}

		// write scalar
// 		if (fwrite(&s, sizeof(s), 1, out_scalars_fp) != 1) {
// 			GTB_ERROR("failed to write scalar");
// 		}
		scalars[i] = s;
	}

	// update min and max
// 	header.set_min_value(min_scalar);
// 	header.set_max_value(max_scalar);
// 	fseek(out_scalars_fp, 0, SEEK_SET);
// 	header.write(out_scalars_fp);
	Scalar_time_series *sts = new Scalar_time_series(scalars);
	sts->set_min_value(min_scalar);
	sts->set_max_value(max_scalar);

	// read tetrahedra 
	m_tetrahedra.reserve(num_tetrahedra + 1);
	Tetrahedron dummy;
	m_tetrahedra.push_back(dummy);
	for (unsigned i = 0; i < num_tetrahedra; i++) {
		unsigned a, b, c, d;
		if (fscanf(in_off_fp, "%u %u %u %u", &a, &b, &c, &d) != 4) {
			GTB_ERROR("failed to read the tetrahedra");
		}
		Tetrahedron t(a, b, c, d);
		m_tetrahedra.push_back(t);	
	}

        // create indexed tetra set
	*out_its = new Indexed_tetra_set(m_vertices, m_tetrahedra);
	*out_sts = sts;
}


GTB_END_NAMESPACE
