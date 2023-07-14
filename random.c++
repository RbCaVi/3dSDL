#include "shared.h++"

#include "random.h++"

randbool::randbool():rd(),gen(rd()),distrib(0, 1){}

bool randbool::operator()(){
  return distrib(gen);
}

randbool::randbool():rd(),gen(rd()),distrib(0, 1){}

bool randbool::operator()(){
  return distrib(gen);
}

randbool::randbool():rd(),gen(rd()),distrib(0, 1){}

bool randbool::operator()(){
  return distrib(gen);
}