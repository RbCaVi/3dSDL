#include "shared.h"

#include <stack>

#include "parseargs.h++"

struct optdata{
  optdata(const char *option,int args):option(option),neededargs(args),argc(0),args(NULL){}
  const char *option;
  int neededargs;
  int argc;
  const char **args;
}

void parseargs(int argc, const char** argv,bool (&isopt)(const char*,void*),int (&optargs)(const char*,void*),void (&addopt)(const char*,int,const char**,void*),void (&addarg)(const char*,void*),void *data){
  // initialize the option stack
  std::stack<optdata> options;
  
  for(i=1;i<argc;i++){
    const char* arg=argv[i];
    if(isopt(arg,data)){ // the argument is an option
      options.push(pair(arg,optargs(arg,data));
    }else{
      if(options.empty()){
        addarg(arg,data);
      }else{
        // add the arg to the top option on the stack
        optdata d=options.top;
        d.argc++;
        d.args=(const char**)realloc(d.args,d.argc*sizeof(const char*));
        d.args[d.argc-1]=arg;
        if(d.argc==d.neededargs){
          // add the option
          addopt(d.option,d.neededargs,d.args,data);
          // remove the option on top
          options.pop();
        }
      }
    }
  }
  if(!options.empty()){
    // some of the options didn't get enough args
    
  }
}
