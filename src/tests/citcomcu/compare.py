#!/usr/bin/env python

import os, sys

def build_args(a,b):
    kw = dict()
    kw['first'] = 'V%s.dat' % a
    kw['first-mesh-coordinates'] = 'coords%s.dat' % a
    kw['first-mesh-connectivity'] = 'connect%s.dat' % a
    if a != b:
        kw['second'] = 'V%s.dat' % b
        kw['second-mesh-coordinates'] = 'coords%s.dat' % b
        kw['second-mesh-connectivity'] = 'connect%s.dat' % b
        kw['output'] = 'res_velo_%s_%s.vtk' % (a,b)
    else:
        kw['second'] = 'zero'
        kw['output'] = 'norm_velo_%s.vtk' % a
    kw['extra'] = '-v '
    kw['extra'] += '-f %d ' % 1000
    kw['extra'] += '-q %s ' % '../../rules.h5:/hex/%d' % 3
    return kw

def run_comparison(**kw):
    fields = {'first': [], 'second': []}
    for fieldarg in ('first', 'second'):
        for suffix in ('', 'mesh', 'mesh-coordinates', 'mesh-connectivity'):
            argname = fieldarg
            if suffix != '':
                argname += '-%s' % suffix
            if argname in kw:
                argtemplate = '--%s=%%s' % argname
                fields[fieldarg].append(argtemplate % kw[argname])
    exe = 'cigma'
    first = fields['first']
    second = fields['second']
    output = "--output=%s" % kw['output']
    extra = kw.get('extra','')
    args = " ".join(first + second + [output, extra])
    command = " ".join([exe, 'compare', args])

    sep = '#' + ('-' * 78)
    print(sep)
    print("# %s " % command)

    ret = 0
    dryrun = kw.get('dryrun', False)
    if not dryrun:
        ret = os.system(command)
        print('')

    sys.stdout.flush()
    return ret

resolutions = ['08', '16', '32', '64']

oldcases = [
    ('08', '08'),
    ('16', '16'),
    ('32', '32'),
    ('08', '32'),
    ('16', '32'),
    ('32', '08'),
    ('32', '16'),
    ('08', '16'),
    ('16', '08'),
]

cases = [
    ('08', '08'),
    ('16', '16'),
    ('32', '32'),
    ('64', '64'),
    ('08', '64'),
    ('16', '64'),
    ('32', '64'),
]

for (a,b) in cases:
    kw = build_args(a,b)
    ret = run_comparison(**kw)
    if ret != 0:
        print("# Command failed with exit code %d" % ret)
        exit(ret)

