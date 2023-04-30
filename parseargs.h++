#ifndef PARSEARGS_H
#define PARSEARGS_H

// isopt is called for every argument and returns if it is an option or not
//   its arguments are the argument being checked and data
// optargs tells how many arguments an option takes and is called once when an option is processed
//   its arguments are the option and data
// addopt is called to add an option when it gets enough arguments
//   its arguments are the option, the number of arguments the option is getting, the option's arguments, and data
// addarg is called to add an argument not related to an option
//   its arguments are the argument and data
// data is a pointer to a data object used in the three functions above
void parseargs(int argc, const char** argv,bool (&isopt)(const char*,void*),int (&optargs)(const char*,void*),void (&addopt)(const char*,int,const char**,void*),void (&addarg)(const char*,void*),void *data); // the best signature

#endif
