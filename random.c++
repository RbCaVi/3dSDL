#include "shared.h++"

#include <chrono>

#include "random.h++"

Rand::Rand():rd(){
  gen=new std::mt19937(static_cast<long unsigned int>(std::chrono::high_resolution_clock::now().time_since_epoch().count()));
}

randbool::randbool():distrib(0, 1){}

bool randbool::operator()(){
  return distrib(*gen);
}

randchance::randchance(float chance):distrib(chance){}

randchance::randchance():randchance(0.5){}

bool randchance::operator()(){
  return distrib(*gen);
}