#!/bin/bash
cigma-compare --first=$HOME/benchmarks/bmrsnog.h5:/pylith/bmrsnog_tet4_0250m/variables/velocity/step00000             \
              --second=$HOME/benchmarks/bmrsnog.h5:/geofest/bmrsnog_tet4_0500m_A/variables/del_displacement/step00000   \
              --output=bmrsnog_pylith0250m_geofest0500m_residuals.vtk
