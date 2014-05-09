#!/bin/bash

bm=bmssnog
bmfile=$HOME/benchmarks/$bm.h5
prefix=ssng-p0250-g0500

#########

echo "---------------------------------------------"
var=variables/displacement/step00000
cigma compare --first=$bmfile:/pylith/${bm}_tet4_0250m/$var     \
              --second=$bmfile:/geofest/${bm}_tet4_0500m/$var   \
              --first-mesh=$bmfile:/mesh/${bm}_tet4_0250m       \
              --second-mesh=$bmfile:/mesh/${bm}_tet4_0500m      \
              --verbose --output-frequency=10000                \
              --output=$prefix.disp-000.vtk


#########

echo "---------------------------------------------"
t=step00000
var1=variables/velocity/$t
var2=variables/del_displacement/$t
cigma compare --first=$bmfile:/pylith/${bm}_tet4_0250m/$var1    \
              --second=$bmfile:/geofest/${bm}_tet4_0500m/$var2  \
              --first-mesh=$bmfile:/mesh/${bm}_tet4_0250m       \
              --second-mesh=$bmfile:/mesh/${bm}_tet4_0500m      \
              --verbose --output-frequency=10000                \
              --output=$prefix.velo-000.vtk 

