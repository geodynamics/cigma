#!/usr/bin/env python
#
#       bmssnog.h5
#       |-- mesh
#       |   |-- bmssnog_hex8_1000m
#       |   |-- bmssnog_tet4_1000m
#       |   |-- bmssnog_tet4_0500m
#       |   `-- bmssnog_tet4_0250m
#       |-- pylith
#       |   |-- bmssnog_hex8_1000m
#       |   |-- bmssnog_tet4_1000m
#       |   |-- bmssnog_tet4_0500m_old
#       |   |-- bmssnog_tet4_0500m_new_bc
#       |   |-- bmssnog_tet4_0500m_altered_bc
#       |   `-- bmssnog_tet4_0250m
#       `-- geofest
#           |-- bmssnog_tet4_1000m
#           |-- bmssnog_tet4_0500m
#           `-- bmssnog_tet4_0250m
#

import os
from os.path import join

filename = 'bmssnog.h5'
homedir  = os.environ['HOME'] 
topdir   = os.path.join(homedir, 'benchmarks', 'bmssnog')

#
# Benchmark data is available for meshes with the following resolutions
#
resolutions = ['1000m', '0500m', '0250m']


#
# The mesh_db contains information about the size of the mesh.
# The mapping is as follows
#
#   (space, resolution) -> (nno, nel, ndof)
#
mesh_db = {
    ('hex8', '1000m'): (15625, 13824, 8),
    ('tet4', '1000m'): (15625, 82944, 4),
    ('tet4', '0500m'): (117649, 663552, 4),
    ('tet4', '0250m'): (912673, 5308416, 4)
}

meshfileprefix_db = {
    ('hex8', '1000m'): join(topdir,'pylith','bmssnog_hex8_1000m','bmssnog_1000m_1.0'),
    ('tet4', '1000m'): join(topdir,'input','bmssnog_tet4_1000m','tet4_1000m'),
    ('tet4', '0500m'): join(topdir,'input','bmssnog_tet4_0500m','tet4_0500m'),
    ('tet4', '0250m'): join(topdir,'input','bmssnog_tet4_0250m','tet4_0250m')
}

meshloc_db = {
    ('hex8', '1000m'): '/mesh/bmssnog_hex8_1000m',
    ('tet4', '1000m'): '/mesh/bmssnog_tet4_1000m',
    ('tet4', '0500m'): '/mesh/bmssnog_tet4_0500m',
    ('tet4', '0250m'): '/mesh/bmssnog_tet4_0250m',
}

def mesh_coord_file(mesh_key):
    (fe_space, resolution) = mesh_key
    return meshfileprefix_db[fe_space,resolution] + '.coord'

def mesh_connect_file(mesh_key):
    (fe_space, resolution) = mesh_key
    return meshfileprefix_db[fe_space,resolution] + '.connect'

# 
# Not all data for every timestep is available on each mesh. The following
# dictionary keeps track of what fields to read.
#
pylith_steps = {
    ('hex8', '1000m'): [0, 10, 50, 100],
    ('tet4', '1000m'): [0, 10, 50, 100],
    ('tet4', '0500m'): [0, 10, 50, 100],
    ('tet4', '0250m'): [0],
}

stepformat_db = {
      0: '00000',
     10: '00010',
     50: '00050',
    100: '00100',
}


pylith_datafiles = {
    ('hex8','1000m',''           ): join(topdir,'pylith','bmssnog_hex8_1000m/bmssnog_1000m_1.0.mesh.time.%s.inp'),
    ('tet4','1000m',''           ): join(topdir,'pylith','bmssnog_tet4_1000m/bmssnog_1000m_1.0.mesh.time.%s.inp'),
    ('tet4','0500m','_old'       ): join(topdir,'pylith','bmssnog_tet4_0500m_old/bmssnog_500m_1.0.mesh.time.%s.inp'),
    ('tet4','0500m','_new_bc'    ): join(topdir,'pylith','bmssnog_tet4_0500m_new_bc/bmssnog_500m.0.mesh.time.%s.inp'),
    ('tet4','0500m','_altered_bc'): join(topdir,'pylith','bmssnog_tet4_0500m_altered_bc/bmssnog_500m.0.mesh.time.%s.inp'),
    ('tet4', '0250m', ''           ): join(topdir,'pylith','bmssnog_tet4_0250m/bmssnog_250m_1.0.mesh.time.%s.inp')
}

geofest_datafiles = {
    '1000m': join(topdir,'geofest','bmssnog_tet4_1000m','bm4_uni1000.out'),
    '0500m': join(topdir,'geofest','bmssnog_tet4_0500m','bm4_uni0500.out'),
    '0250m': join(topdir,'geofest','bmssnog_tet4_0250m','bm4_uni0250.out'),
}

geofest_steps = [0, 10, 50, 100]


if __name__ == '__main__':

    import numpy
    import tables

    from utils import skiplines, readfrom, readfromfile

    h5 = tables.openFile(filename, 'w')


    mesh_reader_kwarg_db = {
        'tet4': {
            'coord'  : dict(skip=1, factor=1000.0, dtype=float, range=slice(1,4)),
            'connect': dict(skip=0, offset=1, dtype=int, range=slice(4,8)) 
        },
        'hex8': {
            'coord'  : dict(skip=6, factor=1000.0, dtype=float, range=slice(1,4)),
            'connect': dict(skip=3, offset=1, dtype=int, range=slice(4,12))
        }
    }

    mesh_group = h5.createGroup('/', 'mesh')

    for key in mesh_db:
        
        (space, res) = key
        (nno, nel, ndof) = mesh_db[space,res]

        coordinates = numpy.zeros((nno,3), numpy.float32)
        connectivity = numpy.zeros((nel, ndof), numpy.int32)

        coorfile = mesh_coord_file(key)
        connfile = mesh_connect_file(key)

        kw = mesh_reader_kwarg_db[space]
        readfrom(coorfile, coordinates, **kw['coord'])
        readfrom(connfile, connectivity, **kw['connect'])

        group = h5.createGroup(mesh_group, 'bmssnog_%s_%s' % key)
        h5.createArray(group, 'coordinates', coordinates)
        h5.createArray(group, 'connectivity', connectivity)
    

    pylith_group = h5.createGroup('/', 'pylith')
    for key in pylith_datafiles:
        
        space, res, postfix = key
        nno, nel, ndof = mesh_db[space,res]
        steps = pylith_steps[space,res]

        dispvelo = numpy.zeros((nno, 6), numpy.float32)

        model_group = h5.createGroup(pylith_group, 'bmssnog_%s_%s%s' % key)
        vars_group  = h5.createGroup(model_group, 'variables')
        disp_group  = h5.createGroup(vars_group, 'displacement')
        velo_group  = h5.createGroup(vars_group, 'velocity')

        for step in steps:
            
            stepfmt = stepformat_db[step]
            datafile = pylith_datafiles[key] % stepfmt
            readfrom(datafile, dispvelo, skip=7, factor=1.0, dtype=float, range=slice(1,7))
            
            d = h5.createArray(disp_group, 'step%s' % stepfmt, dispvelo[:,0:3])
            d._v_attrs.MeshLocation = meshloc_db[space,res]
            d._v_attrs.FunctionSpace = space
            
            v = h5.createArray(velo_group, 'step%s' % stepfmt, dispvelo[:,3:6])
            v._v_attrs.MeshLocation = meshloc_db[space,res]
            v._v_attrs.FunctionSpace = space


    def geofest_readstep(fp, x):
        skiplines(fp, 6)
        readfromfile(fp, x, nodecol=1, offset=1, range=slice(5,11))
        skiplines(fp, 7)


    geofest_group = h5.createGroup('/', 'geofest')
    for key in geofest_datafiles:
        
        space, res = 'tet4', key
        nno, nel, ndof = mesh_db[space,res]

        # prepare numpy array
        dispdel = numpy.zeros((nno,6), numpy.float32)
        
        # prepare hdf5 group for data
        model_group = h5.createGroup(geofest_group, 'bmssnog_%s_%s' % (space,res))
        vars_group  = h5.createGroup(model_group, 'variables')
        disp_group  = h5.createGroup(vars_group, 'displacement')
        del_group   = h5.createGroup(vars_group, 'del_displacement')
        
        # read data file with all steps
        datafile = geofest_datafiles[key]
        print "Reading file", datafile
        fp = open(datafile, 'r')
        skiplines(fp, 3)

        for step in geofest_steps:
            
            geofest_readstep(fp, dispdel)

            stepfmt = stepformat_db[step]
            
            d = h5.createArray(disp_group, 'step%s' % stepfmt, dispdel[:,0:3])
            d._v_attrs.MeshLocation = meshloc_db[space, res]
            d._v_attrs.FunctionSpace = space

            dd = h5.createArray(del_group, 'step%s' % stepfmt, dispdel[:,3:6])
            dd._v_attrs.MeshLocation = meshloc_db[space, res]
            dd._v_attrs.FunctionSpace = space

    h5.close()
