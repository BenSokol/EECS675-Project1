/**
* @Filename: BattleshipPlayer.cpp
* @Author:   Ben Sokol <Ben>
* @Email:    ben@bensokol.com
* @Created:  February 15th, 2019 [10:58am]
* @Modified: February 20th, 2019 [10:53pm]
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
#include <sstream>
#include <utility>

#include "UTL_assert.h"

#include "BattleshipPlayer.hpp"

BattleshipPlayer::BattleshipPlayer(size_t aPlayerNum, size_t aSize, size_t aTotalTargets)
    : mPlayerNum(aPlayerNum),
      mIsAlive(true),
      mTimesRevived(0),
      mAttacksRecieved(0),
      mAttacksLaunchedInitialHits(0),
      mAttacksLaunchedInitialMisses(0),
      mAttacksLaunchedSecondaryHits(0),
      mAttacksLaunchedSecondaryMisses(0) {
  UTL_assert(aTotalTargets <= (aSize * aSize));

  mBoard = std::unique_ptr<BattleshipBoard>(new BattleshipBoard(aSize, aTotalTargets));
}

BattleshipPlayer::~BattleshipPlayer() {}


bool BattleshipPlayer::isAlive() {
  std::unique_lock<std::recursive_mutex> lck(mMtx);
  mIsAlive = mBoard->isAlive();
  return mIsAlive;
}

bool BattleshipPlayer::attack(std::pair<size_t, size_t> & /*coord*/) {
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

void BattleshipPlayer::printBoard(std::recursive_mutex &mtx, std::ostream &ofs) {
  std::lock(mMtx, mtx);
  std::lock_guard<std::recursive_mutex> lk1(mMtx, std::adopt_lock);
  std::lock_guard<std::recursive_mutex> lk2(mtx, std::adopt_lock);
  mBoard->printBoard(mtx, mPlayerNum, ofs);
}

void BattleshipPlayer::printInitialBoard(std::recursive_mutex &mtx, std::ostream &ofs) {
  std::lock(mMtx, mtx);
  std::lock_guard<std::recursive_mutex> lk1(mMtx, std::adopt_lock);
  std::lock_guard<std::recursive_mutex> lk2(mtx, std::adopt_lock);
  mBoard->printInitialBoard(mtx, mPlayerNum, ofs);
}

void BattleshipPlayer::generateReport(std::stringstream &ss) {
  std::unique_lock<std::recursive_mutex> lck(mMtx);
  ss << "Player " << mPlayerNum << " Report:\n";

  // The number of this player's targets remaining (i.e., targets that were not hit).
  ss << "  Targets Remaining: " << mBoard->getRemainingTargets() << "\n";

  // The number of times this player was revived from a state of suspended animation.
  ss << "  Times Revived:     " << mTimesRevived << "\n";

  // The number of times this player was attacked.
  ss << "  Attacks Recieved:  " << mAttacksRecieved << "\n";

  // The number of attacks this player launched.
  ss << "  Attacks Launched:  "
     << mAttacksLaunchedInitialHits + mAttacksLaunchedInitialMisses + mAttacksLaunchedSecondaryHits
            + mAttacksLaunchedSecondaryMisses
     << "\n";

  // The number of attacks this player launched that:
  ss << "  Attacks Launched Details:\n";

  // were initial hits
  ss << "    Initial Hits:     " << mAttacksLaunchedInitialHits << "\n";

  // were initial misses
  ss << "    Initial Misses:   " << mAttacksLaunchedInitialMisses << "\n";

  // were secondary hits
  ss << "    Secondary Hits:   " << mAttacksLaunchedSecondaryHits << "\n";

  // were secondary misses
  ss << "    Secondary Misses: " << mAttacksLaunchedSecondaryMisses << "\n";
  ss << "\n";
}
