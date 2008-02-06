#!/bin/bash

bm=bmssnog
bmfile=$HOME/benchmarks/$bm.h5
prefix=ssng-p0500-g0500

#########

echo "---------------------------------------------"
var=variables/displacement/step00000
cigma compare --first=$bmfile:/pylith/${bm}_tet4_0500m_new_bc/$var    \
              --second=$bmfile:/geofest/${bm}_tet4_0500m/$var         \
              --first-mesh=$bmfile:/mesh/${bm}_tet4_0500m             \
              --second-mesh=$bmfile:/mesh/${bm}_tet4_0500m            \
              --verbose --output-frequency=10000                      \
              --output=$prefix.disp-000.vtk 

echo "---------------------------------------------" 
var=variables/displacement/step00010
cigma compare --first=$bmfile:/pylith/${bm}_tet4_0500m_new_bc/$var    \
              --second=$bmfile:/geofest/${bm}_tet4_0500m/$var         \
              --first-mesh=$bmfile:/mesh/${bm}_tet4_0500m             \
              --second-mesh=$bmfile:/mesh/${bm}_tet4_0500m            \
              --verbose --output-frequency=10000                      \
              --output=$prefix.disp-010.vtk 

echo "---------------------------------------------" 
var=variables/displacement/step00050
cigma compare --first=$bmfile:/pylith/${bm}_tet4_0500m_new_bc/$var    \
              --second=$bmfile:/geofest/${bm}_tet4_0500m/$var         \
              --first-mesh=$bmfile:/mesh/${bm}_tet4_0500m             \
              --second-mesh=$bmfile:/mesh/${bm}_tet4_0500m            \
              --verbose --output-frequency=10000                      \
              --output=$prefix.disp-050.vtk 

echo "---------------------------------------------" 
var=variables/displacement/step00100
cigma compare --first=$bmfile:/pylith/${bm}_tet4_0500m_new_bc/$var    \
              --second=$bmfile:/geofest/${bm}_tet4_0500m/$var         \
              --first-mesh=$bmfile:/mesh/${bm}_tet4_0500m             \
              --second-mesh=$bmfile:/mesh/${bm}_tet4_0500m            \
              --verbose --output-frequency=10000                      \
              --output=$prefix.disp-100.vtk 

#########

echo "---------------------------------------------" 
t=step00000
var1=variables/velocity/$t
var2=variables/del_displacement/$t
cigma compare --first=$bmfile:/pylith/${bm}_tet4_0500m_new_bc/$var1   \
              --second=$bmfile:/geofest/${bm}_tet4_0500m/$var2        \
              --first-mesh=$bmfile:/mesh/${bm}_tet4_0500m             \
              --second-mesh=$bmfile:/mesh/${bm}_tet4_0500m            \
              --verbose --output-frequency=10000                      \
              --output=$prefix.velo-000.vtk 

echo "---------------------------------------------" 
t=step00010
var1=variables/velocity/$t
var2=variables/del_displacement/$t
cigma compare --first=$bmfile:/pylith/${bm}_tet4_0500m_new_bc/$var1   \
              --second=$bmfile:/geofest/${bm}_tet4_0500m/$var2        \
              --first-mesh=$bmfile:/mesh/${bm}_tet4_0500m             \
              --second-mesh=$bmfile:/mesh/${bm}_tet4_0500m            \
              --verbose --output-frequency=10000                      \
              --output=$prefix.velo-010.vtk 

echo "---------------------------------------------" 
t=step00050
var1=variables/velocity/$t
var2=variables/del_displacement/$t
cigma compare --first=$bmfile:/pylith/${bm}_tet4_0500m_new_bc/$var1   \
              --second=$bmfile:/geofest/${bm}_tet4_0500m/$var2        \
              --first-mesh=$bmfile:/mesh/${bm}_tet4_0500m             \
              --second-mesh=$bmfile:/mesh/${bm}_tet4_0500m            \
              --verbose --output-frequency=10000                      \
              --output=$prefix.velo-050.vtk

echo "---------------------------------------------" 
t=step00100
var1=variables/velocity/$t
var2=variables/del_displacement/$t
cigma compare --first=$bmfile:/pylith/${bm}_tet4_0500m_new_bc/$var1   \
              --second=$bmfile:/geofest/${bm}_tet4_0500m/$var2        \
              --first-mesh=$bmfile:/mesh/${bm}_tet4_0500m             \
              --second-mesh=$bmfile:/mesh/${bm}_tet4_0500m            \
              --verbose --output-frequency=10000                      \
              --output=$prefix.velo-100.vtk 

