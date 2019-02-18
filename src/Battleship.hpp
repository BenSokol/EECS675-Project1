/**
* @Filename: Battleship.hpp
* @Author:   Ben Sokol
* @Email:    ben@bensokol.com
* @Created:  February 13th, 2019 [10:58am]
* @Modified: February 18th, 2019 [2:58pm]
* @Version:  1.0.0
*
* Copyright (C) 2019 by Ben Sokol. All Rights Reserved.
*/

#ifndef BATTLESHIP_HPP
#define BATTLESHIP_HPP

#include <cstdlib>
#include <future>
#include <memory>
#include <shared_mutex>
#include <vector>

//#include <atomic>
//#include <condition_variable>
//#include <thread>

#include "BattleshipPlayer.hpp"

// Enable ability to use shared_mutex on any platform that supports shared_timed_mutex
#if defined(__clang__)
#if __clang_major__ < 3 && __clang_minor__ < 4
#error Requires clang version > 3.4
#elif __clang_major__ < 3 && __clang_minor__ < 7
#define shared_mutex shared_timed_mutex
#warning Detected GCC version that does NOT contain std::shared_mutex. Using std::shared_timed_mutex instead.
#endif  // #if __clang_major__ < 3 && __clang_minor__ < 7
#elif defined(__GNUC__) && __GNUC__ < 5
#error Requires clang version > 3.4
#elif defined(__GNUC__) && __GNUC__ < 6
#define shared_mutex shared_timed_mutex
#warning Detected GCC version that does NOT contain std::shared_mutex. Using std::shared_timed_mutex instead.
#endif  // #if defined(__clang__)


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
  std::vector<std::unique_ptr<BattleshipPlayer>> mPlayers;

  // Threads/Futures
  std::vector<std::future<void>> mThreads;

  // shared_mutexes/Locks
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
