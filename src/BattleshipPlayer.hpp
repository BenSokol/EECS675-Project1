/**
* @Filename: BattleshipPlayer.hpp
* @Author:   Ben Sokol <Ben>
* @Email:    ben@bensokol.com
* @Created:  February 15th, 2019 [10:57am]
* @Modified: February 20th, 2019 [10:33pm]
* @Version:  1.0.0
*
* Copyright (C) 2019 by Ben Sokol. All Rights Reserved.
*/

#ifndef BATTLESHIPPLAYER_HPP
#define BATTLESHIPPLAYER_HPP

#include <condition_variable>
#include <cstdlib>
#include <memory>
#include <mutex>
#include <sstream>
#include <utility>
#include <vector>

#include "BattleshipBoard.hpp"

// Enable ability to use shared_mutex on any platform that supports shared_timed_mutex
// #if !defined(__cpp_lib_shared_mutex) && !defined(_LIBCPP_AVAILABILITY_SHARED_MUTEX)
// #define shared_mutex shared_timed_mutex
// #warning std::shared_mutex is not defined. Using std::shared_timed_mutex instead.
// #endif


class BattleshipPlayer {
public:
  BattleshipPlayer(size_t aPlayerNum, size_t aSize, size_t aTotalTargets);
  ~BattleshipPlayer();

  bool isAlive();
  bool attack(std::pair<size_t, size_t> &coord);
  void printBoard(std::recursive_mutex &mtx, std::ostream &ofs = std::cout);
  void printInitialBoard(std::recursive_mutex &mtx, std::ostream &ofs = std::cout);
  std::pair<size_t, size_t> getTargetCoordinates() const;

  void generateReport(std::stringstream &ss);

private:
  std::unique_ptr<BattleshipBoard> mBoard;
  const size_t mPlayerNum;
  bool mIsAlive;
  std::recursive_mutex mMtx;
  size_t mTimesRevived;
  size_t mAttacksRecieved;
  size_t mAttacksLaunchedInitialHits;
  size_t mAttacksLaunchedInitialMisses;
  size_t mAttacksLaunchedSecondaryHits;
  size_t mAttacksLaunchedSecondaryMisses;
};

#endif
