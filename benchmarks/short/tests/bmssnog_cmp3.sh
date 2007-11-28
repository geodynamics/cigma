#!/bin/bash
cigma-compare --first=$HOME/benchmarks/bmssnog.h5:/pylith/bmssnog_tet4_0500m_new_bc/variables/displacement/step00010   \
              --second=$HOME/benchmarks/bmssnog.h5:/geofest/bmssnog_tet4_0500m/variables/displacement/step00010  \
              --output=bmssnog_pylith0500m_geofest0500m_residuals.vtk
