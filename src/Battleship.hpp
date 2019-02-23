/**
* @Filename: Battleship.hpp
* @Author:   Ben Sokol
* @Email:    ben@bensokol.com
* @Created:  February 13th, 2019 [10:58am]
* @Modified: February 22nd, 2019 [11:29pm]
* @Version:  1.0.0
*
* Copyright (C) 2019 by Ben Sokol. All Rights Reserved.
*/

#ifndef BATTLESHIP_HPP
#define BATTLESHIP_HPP

#include <atomic>   // std::atomic
#include <cstdlib>  // size_t
#include <fstream>  // std::ofstream
#include <future>   // std::future
#include <memory>   // std::unique_ptr
#include <mutex>    // std::recursive_mutex
#include <string>   // std::string
#include <vector>   // std::vector

#include "BattleshipPlayer.hpp"
#include "TS_latch.hpp"
#include "TS_log.hpp"
#include "TS_print.hpp"


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
  std::vector<std::shared_ptr<BattleshipPlayer>> mPlayers;
  std::vector<bool> mRevivePlayer;

  // Threads/Futures
  std::vector<std::future<void>> mThreads;

  // Atomic Variables
  std::atomic<bool> mDone;

  // Mutexes
  enum MTX {
    COUT,  // Used for std::cout
    LOG,   // Used for logging to file
    DATA_STRUCTURE_ACCESS,
    MTX_COUNT
  };
  std::vector<std::recursive_mutex> mMtx;
  std::vector<std::mutex> mPlayerMtx;

  // condition_variable
  std::unique_ptr<TS::Latch> mBegin;
  std::vector<std::condition_variable> mCvs;

  // Generated report
  std::string mReport;

  // Time points
  std::chrono::high_resolution_clock::time_point mInitStartTimePoint;
  std::chrono::high_resolution_clock::time_point mInitEndTimePoint;
  std::chrono::high_resolution_clock::time_point mBattleStartTimePoint;
  std::chrono::high_resolution_clock::time_point mBattleEndTimePoint;

  // Initialization variables, these are only modified during construction
  size_t mNumThreads;
  size_t mSize;
  size_t mTargets;
  bool mValidInputParameters;
  size_t mWinner;
};

#endif
