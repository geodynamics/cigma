##############################################################################
# -*- Autoconf -*-
#
#

##############################################################################
#
# CIG_ARG_HDF5
#
AC_DEFUN([CIGMA_ARG_HDF5], [
    AC_ARG_VAR(HDF5_HOME, [home path to HDF5 library])
    AC_ARG_WITH([hdf5],
        [AC_HELP_STRING([--with-hdf5],
                        [enable HDF5 @<:@default=$1@:>@])],
        [with_hdf5="$withval"],
        [with_hdf5=$1])
])

##############################################################################
#
# CIG_PATH_HDF5([minimum-version], [action-if-found], [action-if-not-found])
AC_DEFUN([CIGMA_PATH_HDF5],[
    
    if [[ $with_hdf5 = "yes" ]]; then
        with_hdf5="/usr/local"
    fi

    if [[ $with_hdf5 != "no" ]]; then

        HDF5_PREFIX="$with_hdf5"

        AC_CHECK_FILE

    else
        
        /bin/true

    fi  # if [[ $with_hdf5 != "no" ]];


])

# vim: syntax=config
