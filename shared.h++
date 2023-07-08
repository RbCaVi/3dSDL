#ifndef SHARED_H
#define SHARED_H

//#define GLEW_STATIC 

#define IGNORE(x) (void)(x)

#ifdef DEBUG
#define DEBUGR(x) x
#define DEBUGP(s,...) printf(s,__VA_ARGS__)
#else
#define DEBUGR(x) void(0);
#define DEBUGP(s,...) void(0)
#endif
#endif
