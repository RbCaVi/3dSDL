#include "shared.h++"

#include "random.h++"

Rand::Rand():rd(),gen(rd()){}

randbool::randbool():distrib(0, 1){}

bool randbool::operator()(){
  return distrib(gen);
}

randchance::randchance():Rand(),distrib(0.5){}

randchance::randchance(float chance):Rand(),distrib(chance){}

bool randchance::operator()(){
  return distrib(gen);
}