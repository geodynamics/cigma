#
# Find the cigma includes and library
#
#

FIND_PATH(CIGMA_INCLUDE_DIR h5io.h
    $ENV{CIGMA_HOME}/h5io
    /usr/local/include
    /usr/include
)

FIND_LIBRARY(CIGMA_LIBRARY h5io
    $ENV{CIGMA_HOME}/lib
    /usr/local/lib
    /usr/lib
)

IF(CIGMA_INCLUDE_DIR)
    IF(CIGMA_LIBRARY)
        SET(CIGMA_LIBRARIES ${CIGMA_LIBRARY})
        SET(CIGMA_FOUND "YES")
    ENDIF(CIGMA_LIBRARY)
ENDIF(CIGMA_INCLUDE_DIR)

