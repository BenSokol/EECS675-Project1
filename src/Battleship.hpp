/**
* @Filename: Battleship.hpp
* @Author:   Ben Sokol
* @Email:    ben@bensokol.com
* @Created:  February 13th, 2019 [10:58am]
* @Modified: February 22nd, 2019 [12:55pm]
* @Version:  1.0.0
*
* Copyright (C) 2019 by Ben Sokol. All Rights Reserved.
*/

#ifndef BATTLESHIP_HPP
#define BATTLESHIP_HPP

#include <cstdlib>  // size_t
#include <fstream>  // std::ofstream
#include <future>   // std::future
#include <memory>   // std::unique_ptr
#include <mutex>    // std::recursive_mutex
#include <string>   // std::string
#include <vector>   // std::vector

// #include <atomic>
// #include <shared_mutex>
// #include <thread>

#include "BattleshipPlayer.hpp"
#include "TS_latch.hpp"
#include "TS_log.hpp"
#include "TS_print.hpp"

// Enable ability to use shared_mutex on any platform that supports shared_timed_mutex
// #if !defined(__cpp_lib_shared_mutex) && !defined(_LIBCPP_AVAILABILITY_SHARED_MUTEX)
// #define shared_mutex shared_timed_mutex
// #warning std::shared_mutex is not defined. Using std::shared_timed_mutex instead.
// #endif


class Battleship {
public:
  Battleship(const int argc, const char *argv[]);
  ~Battleship();

  void printVersion() const;
  static void printHelp();
  void run();

private:
  bool initParameters(const int &argc, const char *argv[]);
  void initPlayers(size_t playerNum);
  void battle(size_t playerNum);
  void generateReport();
#ifdef ENABLE_LOGGING
  void createLogFile();
#endif

  // logging
  std::ofstream mLogFile;

  // Version
  const size_t mVersionMajor;
  const size_t mVersionMinor;
  const size_t mVersionBug;

  // Players
  std::vector<std::unique_ptr<BattleshipPlayer>> mPlayers;

  // Threads/Futures
  std::vector<std::future<void>> mThreads;

  // shared_mutexes/Locks
  enum MTX {
    COUT,  // Used for std::cout
    LOG,   // Used for logging to file
    MTX_COUNT
  };
  std::vector<std::recursive_mutex> mtx;

  // condition_variable
  std::unique_ptr<TS::Latch> mBegin;

  // Generated report
  std::string mReport;

  // time points
  std::chrono::high_resolution_clock::time_point mInitStartTimePoint;
  std::chrono::high_resolution_clock::time_point mInitEndTimePoint;
  std::chrono::high_resolution_clock::time_point mBattleStartTimePoint;
  std::chrono::high_resolution_clock::time_point mBattleEndTimePoint;

  // Other Member variables
  size_t mNumThreads;
  size_t mSize;
  size_t mTargets;
  bool mValidInputParameters;
};

#endif
