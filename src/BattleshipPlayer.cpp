/**
* @Filename: BattleshipPlayer.cpp
* @Author:   Ben Sokol <Ben>
* @Email:    ben@bensokol.com
* @Created:  February 15th, 2019 [10:58am]
* @Modified: February 22nd, 2019 [2:20pm]
* @Version:  1.0.0
*
* Copyright (C) 2019 by Ben Sokol. All Rights Reserved.
*/

#include <algorithm>
#include <cstdlib>
#include <iostream>
#include <mutex>
#include <random>
#include <shared_mutex>
#include <string>
#include <utility>

#include "UTL_assert.h"

#include "BattleshipPlayer.hpp"

BattleshipPlayer::BattleshipPlayer(size_t aPlayerNum, size_t aSize, size_t aTotalTargets)
    : mBoard(std::unique_ptr<BattleshipBoard>(new BattleshipBoard(aSize, aTotalTargets))),
      mPlayerNum(aPlayerNum),
      mIsAlive(true),
      mTimesRevived(0),
      mAttacksRecieved(0),
      mAttacksLaunchedInitialHits(0),
      mAttacksLaunchedInitialMisses(0),
      mAttacksLaunchedSecondaryHits(0),
      mAttacksLaunchedSecondaryMisses(0) {
  UTL_assert(aTotalTargets <= (aSize * aSize));
}

BattleshipPlayer::~BattleshipPlayer() {}


bool BattleshipPlayer::isAlive() {
  std::unique_lock<std::recursive_mutex> lck(mMtx);
  mIsAlive = mBoard->isAlive();
  return mIsAlive;
}

bool BattleshipPlayer::attack(std::pair<size_t, size_t>& /*coord*/) {
  std::unique_lock<std::recursive_mutex> lck(mMtx);
  // UTL_assert(mBoard[coord.first][coord.second] == mPositionEncodedValues[OPEN][0]
  //            || mBoard[coord.first][coord.second] == mPositionEncodedValues[TARGET][0]);
  //
  // if (mBoard[coord.first][coord.second] == mPositionEncodedValues[OPEN][0]) {
  //   mBoard[coord.first][coord.second] = mPositionEncodedValues[ATTACKED][0];
  //   return false;
  // }
  // else if (mBoard[coord.first][coord.second] == mPositionEncodedValues[TARGET][0]) {
  //   mBoard[coord.first][coord.second] = mPositionEncodedValues[HIT][0];
  //   return true;
  // }

  return false;
}


std::pair<size_t, size_t> BattleshipPlayer::getTargetCoordinates() const {
  // mBoard->generateRandomCoordinates(mPositionEncodedValues[OPEN], mPositionEncodedValues[TARGET]);
  UTL_assert_always();
}

std::string BattleshipPlayer::printBoard() {
  std::lock_guard<std::recursive_mutex> lck(mMtx);
  return mBoard->printBoard(mPlayerNum);
}

std::string BattleshipPlayer::printInitialBoard() {
  std::lock_guard<std::recursive_mutex> lck(mMtx);
  return mBoard->printInitialBoard(mPlayerNum);
}

std::string BattleshipPlayer::generateReport() {
  std::unique_lock<std::recursive_mutex> lck(mMtx);
  std::string str = "";

  str += "Player " + std::to_string(mPlayerNum) + " Report:\n";

  // The number of this player's targets remaining (i.e., targets that were not hit).
  str += "  Targets Remaining: " + std::to_string(mBoard->getRemainingTargets()) + "\n";

  // The number of times this player was revived from a state of suspended animation.
  str += "  Times Revived:     " + std::to_string(mTimesRevived) + "\n";

  // The number of times this player was attacked.
  str += "  Attacks Recieved:  " + std::to_string(mAttacksRecieved) + "\n";

  // The number of attacks this player launched.
  const size_t attacksLaunched = mAttacksLaunchedInitialHits + mAttacksLaunchedInitialMisses
                                 + mAttacksLaunchedSecondaryHits + mAttacksLaunchedSecondaryMisses;
  str += "  Attacks Launched:  " + std::to_string(attacksLaunched) + "\n";

  // The number of attacks this player launched that:
  str += "  Attacks Launched Details:\n";

  // were initial hits
  str += "    Initial Hits:     " + std::to_string(mAttacksLaunchedInitialHits) + "\n";

  // were initial misses
  str += "    Initial Misses:   " + std::to_string(mAttacksLaunchedInitialMisses) + "\n";

  // were secondary hits
  str += "    Secondary Hits:   " + std::to_string(mAttacksLaunchedSecondaryHits) + "\n";

  // were secondary misses
  str += "    Secondary Misses: " + std::to_string(mAttacksLaunchedSecondaryMisses) + "\n";
  str += "\n";

  return str;
}
