##############################################################################
# -*- Autoconf -*-
#
#

##############################################################################
#
# CIGMA_PYTHON_INCDIR
#
# Determine the directory containing <Python.h>
#
AC_DEFUN([CIGMA_PYTHON_INCDIR],[

    AC_REQUIRE([AM_PATH_PYTHON])

    AC_CACHE_CHECK([for $am_display_PYTHON include directory],
                   [PYTHON_INCDIR],
                   [PYTHON_INCDIR=`$PYTHON -c "from distutils import sysconfig; print sysconfig.get_python_inc()" 2>/dev/null
                                    || echo "$PYTHON_PREFIX/include/python$PYTHON_VERSION"`])

    AC_MSG_CHECKING([for $am_display_PYTHON numpy include directory])

    NUMPY_INCDIR=`$PYTHON -c "import numpy; print numpy.get_include()" 2>/dev/null`
    if [[ -n "$PYTHON_NUMPY_INCDIR" ]]; then
        AC_CHECK_FILE([$NUMPY_INCDIR/numpy/arrayobject.h], [numpyFound="OK"])
        if [[ -z "$numpyFound" ]]; then
            AC_MSG_RESULT([yes])
        else
            AC_MSG_RESULT([no])
        fi
    else
        AC_MSG_RESULT([no])
    fi

    AC_SUBST([PYTHON_INCDIR], [$PYTHON_INCDIR])
    AC_SUBST([NUMPY_INCDIR], [$NUMPY_INCDIR])

])


# vim: syntax=config
