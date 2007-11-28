#ifndef GTB_GL_HEADERS_INCLUDED
#define GTB_GL_HEADERS_INCLUDED

#ifdef __APPLE_CC__
#  include <OpenGL/OpenGL.h>
//#  include <OpenGL/CGLMacro.h>
#  include <OpenGL/gl.h>
// #  include <OpenGL/glext.h>
// #  include <GLUT/glut.h>
#elif defined __sgi
#  include <GL/gl.h>
// #  include <GL/glx.h>
// #  include <GL/glut.h>
#else
#  if 01
#    include <GL/gl.h>
#  endif
// #  if 01
// #    include <GL/glx.h>
// #  endif
// #  if 01
// #    include <GL/glext.h>
// #  endif
// #  if 01
// #    include <GL/glut.h>
// #  endif
#endif

#endif // GTB_GL_HEADERS_INCLUDED

// Local Variables:
// mode:C++
// End:
