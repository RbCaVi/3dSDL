AC_INIT([3dsdl], [0.0.0], [robert@robertvail.info])
AM_INIT_AUTOMAKE([-Wall -Werror foreign])
AC_PROG_CXX
AC_LANG_PUSH(C++)
AC_CHECK_LIB([SDL2],[SDL_Init])
AC_CHECK_LIB([GL],[glEnable])
AC_CHECK_LIB([GLEW],[__glewCreateShader])
AC_CHECK_LIB([opencv_core],[cvAlloc])
AC_CHECK_LIB([opencv_highgui],[cvCreateImage])
AC_LANG_POP(C++)
AC_CONFIG_HEADERS([config.h])
AC_CONFIG_FILES([
 Makefile
])
AC_OUTPUT