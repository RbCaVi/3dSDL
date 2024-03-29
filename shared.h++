#ifndef SHARED_H
#define SHARED_H

#define IGNORE(x) (void)(x)

#ifdef DEBUG
#define DEBUGR(cond,x) if(_##cond==1){x;}
#define _DEBUGR(cond,x) if(cond==1){x;}
#else
#define DEBUGR(cond,x) void(0)
#define _DEBUGR(cond,x) void(0)
#endif
#define DEBUGP(cond,...) _DEBUGR(_##cond,printf( #cond ": " __VA_ARGS__))
#endif
