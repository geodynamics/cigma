#include "common.hpp"
#include "graphics.hpp"
#include "Indexed_tetra_set.hpp"
#include "Time_series.hpp"
#include "Indexed_tetra_set_off_reader.hpp"
#include <sstream>
#include <iostream>
#include "tet_sampling.hpp"
#include "vect_stats.hpp"

using namespace gtb;
using namespace std;

#define DEFAULT_TET_PER_LEAF 100
#define DEFAULT_SAMPLES_PER_TET 0

#define BUILDING_KDTREE_PROMPT "Building kd-tree..."
#define SAMPLING_VERTICES_PROMPT "Sampling at the vertices...."
#define SAMPLING_TETS_PROMPT "Sampling inside the tets..."


void print_mesh_info(Indexed_tetra_set *its, Scalar_time_series *sts)
{
	float min_val = sts->get_min_value();
	float max_val = sts->get_max_value();
	printf("num tetrahedra: %d\n", its->get_num_tetrahedra());
	printf("scalar values: [%g, %g]\n", min_val, max_val);
}


void usage(char **argv)
{
	printf("\nUsage: %s [OPTIONS] sampled_mesh.off compared_mesh.off\n", argv[0]);
	printf("-k <max number of tets per kd-tree leaf> (default=%d)\n", DEFAULT_TET_PER_LEAF);
	printf("-t <num random samples per tet> (default=%d)\n", DEFAULT_SAMPLES_PER_TET);
}


int main(int argc, char **argv)
{
	printf("TMC (TetMesh Comparator) Version 1.0 (June 15, 2005)\n");
	printf("Copyright (C) 2005, Louis Bavoil <bavoil@sci.utah.edu>\n");
	printf("This program is distributed under the GNU GPL license version 2.\n");

	if (argc < 3) {
		usage(argv);
		exit(EXIT_FAILURE);
	}

	float max_tet_per_leaf = DEFAULT_TET_PER_LEAF;
	unsigned num_samples_per_tet = DEFAULT_SAMPLES_PER_TET;
	const char *samp_file_name = NULL;
	const char *comp_file_name = NULL;

	argc--;
	argv++;
	while (argc > 0) {
		if ((strcmp("-k", argv[0]) == 0) && (argc > 1)) {
			max_tet_per_leaf = atof(argv[1]);
			argc -= 2;
			argv += 2;
		} else if ((strcmp("-t", argv[0]) == 0) && (argc > 1)) {
			num_samples_per_tet = atoi(argv[1]);
			argc -= 2;
			argv += 2;
		} else if (samp_file_name == NULL) {
			samp_file_name = argv[0];
			argc--;
			argv++;
		} else if (comp_file_name == NULL) {
			comp_file_name = argv[0];
			argc--;
			argv++;
		} else {
			usage(argv);
			exit(EXIT_FAILURE);
		}
	}

	if (!samp_file_name || !comp_file_name) {
		usage(argv);
		exit(EXIT_FAILURE);
	}

	///////////////////////////////////////////////////////////////////////////////

	Indexed_tetra_set_off_reader reader;
	FILE *samp_off_fp, *comp_off_fp;
	Indexed_tetra_set *samp_its, *comp_its;
	Scalar_time_series *samp_sts, *comp_sts;

	printf("\nSampled mesh: %s\n", samp_file_name);
	GTB_FOPEN(samp_off_fp, samp_file_name, "r");
	reader.read(samp_off_fp, &samp_its, &samp_sts);
	print_mesh_info(samp_its, samp_sts);

	printf("\nCompared mesh: %s\n", comp_file_name);
	GTB_FOPEN(comp_off_fp, comp_file_name, "r");
	reader.read(comp_off_fp, &comp_its, &comp_sts);
	print_mesh_info(comp_its, comp_sts);

	const float range = samp_sts->get_max_value() - samp_sts->get_min_value();
	const std::vector<Point_3f> &vertices = samp_its->get_vertices();
	const std::vector<Tetrahedron> &tets = samp_its->get_tetrahedra();

	printf("\n%s\n", BUILDING_KDTREE_PROMPT);
	KDTet<unsigned> comp_tree(max_tet_per_leaf, comp_its, comp_sts);
	comp_tree.MakeTree();

	///////////////////////////////////////////////////////////////////////////////

	printf("%s\r", SAMPLING_VERTICES_PROMPT);

	std::vector<float> errors;
	unsigned num_eval_misses = 0;

	for (unsigned vi=0; vi < vertices.size(); vi++) {

		Point_3f s_f = vertices[vi];
		Point_3d s_d(s_f[0], s_f[1], s_f[2]);

		KDTet<unsigned>::TreeNode *parent;
		KDTet<unsigned>::LeafNode *node = comp_tree.Find(s_d, parent);

		float comp_val;
		if (!comp_tree.FindAndEvalTet(node, s_f, &comp_val)) {
			num_eval_misses++;
			continue;
		}

		float samp_val = samp_sts->get_value(vi);
		errors.push_back(fabs(comp_val - samp_val));

		if (vi % 1000 == 0) {
			printf("\r%s %.0f%%",
			       SAMPLING_VERTICES_PROMPT,
			       (float)vi/vertices.size()*100);
			fflush(stdout);
		}
	}

	printf("\r%s        \n", SAMPLING_VERTICES_PROMPT);

	///////////////////////////////////////////////////////////////////////////////

	if (num_samples_per_tet> 0 ) {
		printf("%s\r", SAMPLING_TETS_PROMPT);

		for (unsigned ti=1; ti < tets.size(); ti++) {
			const Tetrahedron &t = tets[ti];

			Vector_3f v0(vertices[t.get_vertex(0)]);
			Vector_3f v1(vertices[t.get_vertex(1)]);
			Vector_3f v2(vertices[t.get_vertex(2)]);
			Vector_3f v3(vertices[t.get_vertex(3)]);

			for (unsigned i=0; i < num_samples_per_tet; i++) {
				Vector_3f pick = Pick(v0, v1, v2, v3);
				Point_3f s_f(pick[0], pick[1], pick[2]);
				Point_3d s_d(s_f[0], s_f[1], s_f[2]);
				
				KDTet<unsigned>::TreeNode *parent;
				KDTet<unsigned>::LeafNode *node = comp_tree.Find(s_d, parent);
			
				float comp_val;
				if (!comp_tree.FindAndEvalTet(node, s_f, &comp_val)) {
					num_eval_misses++;
					continue;
				}

				float samp_val;
				if (!samp_its->interpol_tetra(samp_sts, t, s_f, &samp_val)) {
					num_eval_misses++;
					continue;
				}

				errors.push_back(fabs(comp_val - samp_val));
			}

			if (ti % 1000 == 0) {
				printf("\r%s %.0f%%",
				       SAMPLING_TETS_PROMPT,
				       (float)ti/tets.size()*100);
				fflush(stdout);
			}
		}

		printf("\r%s     \n", SAMPLING_TETS_PROMPT);
	}

	///////////////////////////////////////////////////////////////////////////////

	printf("\nSampling:\n");
	printf("num samples per tet: \t%d\n", num_samples_per_tet);
	printf("total num of samples:\t%d\n", (int)errors.size());
	printf("ignored samples:     \t%d (%f%%)\n", num_eval_misses,
	       (float)num_eval_misses*100/vertices.size());

	printf("\nField Error:\n");
	float max_err = vect_max(errors);
	float mean_err = vect_mean(errors);
	float std_dev = vect_std_dev(errors, mean_err);
	float root_ms_err = sqrt(vect_mean_squared(errors));
	printf("max:             \t%e (%f%%)\n", max_err, max_err*100/range);
	printf("mean:            \t%e (%f%%)\n", mean_err, mean_err*100/range);
	printf("std dev:         \t%e (%f%%)\n", std_dev, std_dev*100/range);
	printf("root mean squared:\t%e (%f%%)\n", root_ms_err, root_ms_err*100/range);

	return 0;
}
