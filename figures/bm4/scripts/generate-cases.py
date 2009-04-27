#!/usr/bin/env python

benchmarks = ['rsng', 'ssng']
codes = ['pylith', 'geofest']

resolutions = ['1000m', '0500m', '0250m']
elements = ['tet4', 'hex8']

cases = []
for code in codes:
    for elt in elements:
        for res in resolutions:
            cases.append("%s_%s_%s" % (code,elt,res))

comparisons = []
for a in cases:
    for b in cases:
        comparisons.append('%s-%s' % (a,b))

benchmark_comparisons = []
for bm in benchmarks:
    for cmp in comparisons:
        benchmark_comparisons.append('%s-%s' % (bm,cmp))

for bm in benchmark_comparisons:
    print bm

