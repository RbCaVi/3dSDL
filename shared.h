#ifndef SHARED_H
#define SHARED_H

//#define GLEW_STATIC 

#ifdef DEBUG
#define DEBUGR(x) x
#define DEBUGP(...) printf(__VA_ARGS__)
#else
#define DEBUGR(x) (void)(x)
#define DEBUGP(s,...) (void)(s)
#endif
#endif
