#ifndef RANDOM_H
#define RANDOM_H

#include "shared.h++"

#include <random>

class Rand{
protected:
  std::random_device rd;  // a seed source for the random number engine
  std::mt19937 *gen; // mersenne_twister_engine seeded with rd()

public:
  Rand();
  virtual bool operator()() = 0;
};

class randbool:public Rand{
private:
  std::uniform_int_distribution<> distrib;

public:
  randbool();
  bool operator()() override;
};

class randchance:public Rand{
private:
  std::bernoulli_distribution distrib;

public:
  randchance();
  randchance(float chance);
  bool operator()();
};

#endif