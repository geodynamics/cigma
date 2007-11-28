#!/bin/bash
cigma-compare --first=$HOME/benchmarks/bmssnog.h5:/pylith/bmssnog_tet4_0250m/variables/velocity/step00000             \
              --second=$HOME/benchmarks/bmssnog.h5:/geofest/bmssnog_tet4_0500m/variables/del_displacement/step00000   \
              --output=bmssnog_pylith0250m_geofest0500m_residuals.vtk
