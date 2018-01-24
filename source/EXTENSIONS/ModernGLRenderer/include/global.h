#ifndef GLOBAL_H
#define GLOBAL_H

#if 0
#define CHECK_GL_ERRORS  \
  { \
  GLenum err = glGetError(); \
  if (err) \
    printf( "%s: Error %x at line %d: %s\n", __FILE__, err, __LINE__, gluErrorString(err)); \
  }
#else
#define CHECK_GL_ERRORS {}
#endif

namespace Global
{
	float getScaleFactor();

	void setScaleFactor(float scale_factor);

	bool getTransparencySupport();

	void setTransparencySupport(bool supported);
}

#endif //GLOBAL_H
