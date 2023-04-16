#ifndef SHARED_H
#define SHARED_H

//#define GLEW_STATIC 

#ifdef DEBUG
#define DEBUG(x) x
#define DEBUGP(...) printf(__VA_ARGS__)
#else
#define DEBUG(x) (void)(x)
#define DEBUGP(s,...) (void)(s)
#endif
#endif
