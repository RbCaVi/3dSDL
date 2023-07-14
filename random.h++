#ifndef RANDOM_H
#define RANDOM_H

#include "shared.h++"

#include <random>

virtual class rand{
private:
  std::random_device rd;  // a seed source for the random number engine
  std::mt19937 gen; // mersenne_twister_engine seeded with rd()
  std::uniform_int_distribution<> distrib;

public:
  bool operator()() = 0;
}

class randbool{
private:
  std::random_device rd;  // a seed source for the random number engine
  std::mt19937 gen; // mersenne_twister_engine seeded with rd()
  std::uniform_int_distribution<> distrib;

public:
  randbool();
  bool operator()();
};

class randchance{
private:
  std::random_device rd;  // a seed source for the random number engine
  std::mt19937 gen; // mersenne_twister_engine seeded with rd()
  std::bernoulli_distribution<> distrib;

public:
  randchance();
  randchance(float chance);
  bool operator()();
};
#endif