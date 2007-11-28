#include "vect_stats.hpp"

float vect_max(std::vector<float> &v) {
	float max = 0;
	for (unsigned i=0; i < v.size(); i++) {
		max = std::max(max, v[i]);
	}
	return max;
}


float vect_min(std::vector<float> &v) {
	float min = FLT_MAX;
	for (unsigned i=0; i < v.size(); i++) {
		min = std::min(min, v[i]);
	}
	return min;
}


float vect_mean(std::vector<float> &v) {
	float mean = 0;
	for (unsigned i=0; i < v.size(); i++) {
		mean += v[i];
	}
	mean /= v.size();
	return mean;
}


float vect_mean_squared(std::vector<float> &v) {
        float mean = 0;
        for (unsigned i=0; i < v.size(); i++) {
                mean += v[i]*v[i];
        }
        mean /= v.size();
        return mean;
}


float vect_std_dev(std::vector<float> &v, float mean) {
	float dev = 0;
	unsigned N = v.size();
	for (unsigned i=0; i < N; i++) {
		float diff = v[i] - mean;
		dev += diff*diff;
	}
	dev /= (N - 1);
	return sqrt(dev);
}


float vect_avg_dev(std::vector<float> &v, float mean) {
	float dev = 0;
	unsigned N = v.size();
	for (unsigned i=0; i < N; i++) {
		dev += fabs(v[i] - mean);
	}
	dev /= N;
	return dev;
}
