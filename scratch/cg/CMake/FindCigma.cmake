#
# Find the cigma includes and library
#
#

FIND_PATH(CIGMA_INCLUDE_DIR CompareCmd.h
    $ENV{HOME}/cigma/src
    $ENV{CIGMA_HOME}/src
    /usr/local/include
    /usr/include
)

FIND_LIBRARY(CIGMA_LIBRARY cigma
    $ENV{HOME}/cigma/src
    $ENV{CIGMA_HOME}/src
    /usr/local/lib
    /usr/lib
)

IF(CIGMA_INCLUDE_DIR)
    IF(CIGMA_LIBRARY)
        SET(CIGMA_LIBRARIES ${CIGMA_LIBRARY})
        SET(CIGMA_FOUND "YES")
    ENDIF(CIGMA_LIBRARY)
ENDIF(CIGMA_INCLUDE_DIR)

