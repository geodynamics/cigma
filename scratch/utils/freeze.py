#!/usr/bin/env python
"""freeze.py

This script will store the contents of a given file inside a
C/C++ string literal.
"""

import sys

def quote(s):
    r = repr(s)
    if len(r) > 2:
        a,b = r[0],r[-1]
        r = r[1:-1]
        r = r.replace(r'\\','\\')
        if a == b == "'":
            r = r.replace(r"\'","'")
            r = r.replace('"',r'\"')
        elif a == b == '"':
            r = r.replace(r'\"','"')
        else:
            raise Exception("Bad repr()?")
        return r
    return ''

def escape(s):
    return '"' + quote(s) + '"'

def freeze(filename, varname="foobar"):
    fp = open(filename, "" + 'r')

    prefix = "    "
    print "const char %s[] = \\" % varname
    for line in fp:
        line2 = prefix + escape(line)
        #print line2 + " <-- " + repr(line)
        print line2
    print prefix + '"";'
    fp.close()


def main(argv=None):
    if argv == None:
        argv = sys.argv[1:]
    if len(argv) == 0:
        return 1
    elif len(argv) == 1:
        freeze(filename=argv[0])
        return 0
    elif len(argv) >= 2:
        freeze(filename=argv[0],
               varname=argv[1])
        return 0
    else:
        return 2

if __name__ == "__main__":
    sys.exit(main())
