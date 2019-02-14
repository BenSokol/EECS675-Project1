/**
* @Filename: battleship.hpp
* @Author:   Ben Sokol
* @Email:    ben@bensokol.com
* @Created:  February 13th, 2019 [10:58am]
* @Modified: February 14th, 2019 [9:42am]
* @Version:  1.0.0
*
* Copyright (C) 2019 by Ben Sokol. All Rights Reserved.
*/

#ifndef BATTLESHIP_HPP
#define BATTLESHIP_HPP

//#include <atomic>
//#include <condition_variable>
//#include <future>
//#include <mutex>
//#include <thread>
//#include <vector>
#include <cstdlib>

class Battleship {
public:
  Battleship(const int argc, char *argv[]);
  ~Battleship();

  void printVersion() const;
  void printHelp() const;
  void run();

private:
  const size_t mVersionMajor;
  const size_t mVersionMinor;
  const size_t mVersionBug;

  size_t mNumThreads;
  size_t mSize;
  size_t mTargets;
  bool mValidInputParameters;
};

#endif
