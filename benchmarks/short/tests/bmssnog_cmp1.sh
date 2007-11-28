#!/bin/bash
cigma-compare --first=$HOME/benchmarks/bmssnog.h5:/pylith/bmssnog_tet4_1000m/variables/displacement/step00010    \
              --second=$HOME/benchmarks/bmssnog.h5:/geofest/bmssnog_tet4_1000m/variables/displacement/step00010  \
              --output=bmssnog_pylith1000m_geofest1000m_residuals.vtk
