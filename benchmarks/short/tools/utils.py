
def skiplines(fp, n):
    for i in xrange(n):
        fp.readline()
    return

def readfromfile(fp, x, **kw):
    """
    Given a file object to an ascii file with data in a column-based
    format. Note that x is assumed to be a numpy array with rank 2.
    As many rows will be read from the file as can fit along the extent
    of the first dimension of x.

    The following keyword arguments may be specified

      skip    - skip this many lines at beginning
      factor  - conversion factor to apply to each value
      range   - slice representing range of columns to copy into array row
      dtype   - datatype factory for the column data (float or int)
      offset  - whether to apply an index offset (0 or 1)
      nodecol - column index corresponding to the element, or node, number

    """
    (n,d) = x.shape
    skip = kw.get('skip', 0)
    factor = kw.get('factor', 1.0)
    range = kw.get('range', slice(1,4))
    dtype = kw.get('dtype', float)
    offset = kw.get('offset', 1)
    nodecol = kw.get('nodecol', 0)
    # skip the specified number of lines
    for i in xrange(skip):
        fp.readline()
    # read n lines into array
    for i in xrange(n):
        line = fp.readline()
        cols = line.split()
        node = int(cols[nodecol]) - offset
        x[node] = map(dtype, cols[range])
    # finally, apply necessary corrections to x
    if dtype is int:
        x -= offset
    elif dtype is float:
        x *= factor
    return


def readfrom(filename, x, **kw):
    print "Reading file", filename
    fp = open(filename, 'r')
    readfromfile(fp, x, **kw)
    fp.close()
    return

readfrom.__doc__ = readfromfile.__doc__
