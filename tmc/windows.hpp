#define NOMINMAX
#include <windows.h>

// Warnings

#pragma warning( disable : 4786 4284 34)

//
// Enable malloc debug
//
#if 0
//#include <malloc.h>
#define _CRTDBG_MAP_ALLOC
#include <crtdbg.h>
#endif // #if 0/1

// [ malloc debug ] 

// OpenGL stuff

#include <GL/glut.h>
#include <GL/glext.h>

//#define GL_BGR GL_BGR_EXT
//#define GL_BGRA GL_BGRA_EXT

// C stuff

#include <assert.h>
#include <errno.h>
#include <float.h>
#include <limits.h>
#include <math.h>
#include <stdarg.h>
#include <stdio.h>
//#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <io.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <direct.h>
#include <sys/timeb.h>

#define S_ISREG(mode) (mode & _S_IFREG)
#define S_ISDIR(mode) (mode & _S_IFDIR)

#ifndef PATH_MAX
#define PATH_MAX 512
#endif

typedef int socklen_t;

// C++ stuff

#include <algorithm>
#include <iostream>
#include <list>
#include <map>
#include <string>
#include <sstream>
#include <vector>
#include <stack>
#include <queue>

/*


template<class T>
inline const T &min(const T &x, const T &y)
{
	return (x < y) ? x : y;
}

template<class T>
inline const T &max(const T &x, const T &y)
{
	return (x > y) ? x : y;
}

*/

#define for if (0); else for

#define REAL_IS_DOUBLE 1
#define hypot _hypot
#define copysign _copysign

#define sleep Sleep

#define HAVE_STRICMP 1

#define M_PI 3.14159265358979323846
