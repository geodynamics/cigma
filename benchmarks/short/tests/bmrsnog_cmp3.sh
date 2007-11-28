#!/bin/bash
cigma-compare --first=$HOME/benchmarks/bmrsnog.h5:/pylith/bmrsnog_tet4_0500m/variables/displacement/step00010   \
              --second=$HOME/benchmarks/bmrsnog.h5:/geofest/bmrsnog_tet4_0500m_B/variables/displacement/step00010  \
              --output=bmrsnog_pylith0500m_geofest0500m_residuals.vtk
