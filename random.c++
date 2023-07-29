#include "shared.h++"

#include "random.h++"

Rand::Rand():rd(),gen(rd()){}

randbool::randbool():distrib(0, 1){}

bool randbool::operator()(){
  return distrib(gen);
}

randchance::randchance(float chance):distrib(chance){}

randchance::randchance():randchance(0.5){}

bool randchance::operator()(){
  return distrib(gen);
}