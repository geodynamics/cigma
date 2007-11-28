#!/bin/bash
cigma-compare --first=$HOME/benchmarks/bmrsnog.h5:/pylith/bmrsnog_tet4_1000m/variables/displacement/step00010    \
              --second=$HOME/benchmarks/bmrsnog.h5:/geofest/bmrsnog_tet4_1000m_A/variables/displacement/step00010  \
              --output=bmrsnog_pylith1000m_geofest1000m_residuals.vtk
