/**
* @Filename: battleship.hpp
* @Author:   Ben Sokol
* @Email:    ben@bensokol.com
* @Created:  February 13th, 2019 [10:58am]
* @Modified: February 15th, 2019 [2:35pm]
* @Version:  1.0.0
*
* Copyright (C) 2019 by Ben Sokol. All Rights Reserved.
*/

#ifndef BATTLESHIP_HPP
#define BATTLESHIP_HPP

#include <cstdlib>
#include <future>
#include <mutex>
#include <shared_mutex>
#include <vector>

//#include <atomic>
//#include <condition_variable>
//#include <thread>

#include "BattleshipPlayer.hpp"

class Battleship {
public:
  Battleship(const int argc, const char *argv[]);
  ~Battleship();

  void printVersion() const;
  void printHelp() const;
  void run();

private:
  bool initParameters(const int &argc, const char *argv[]);
  void initPlayers(size_t playerNum);

  // Version
  const size_t mVersionMajor;
  const size_t mVersionMinor;
  const size_t mVersionBug;

  // Players
  std::vector<BattleshipPlayer *> mPlayers;

  // Threads/Futures
  std::vector<std::future<void>> mThreads;

  // Mutexes/Locks
  enum MTX { COUT, DEBUG, MTX_COUNT };
  std::vector<std::shared_mutex> mtx;
  std::vector<std::unique_lock<std::shared_mutex>> lck;

  // Other Member variables
  size_t mNumThreads;
  size_t mSize;
  size_t mTargets;
  bool mValidInputParameters;
};

#endif
