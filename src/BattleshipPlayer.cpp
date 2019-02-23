/**
* @Filename: BattleshipPlayer.cpp
* @Author:   Ben Sokol <Ben>
* @Email:    ben@bensokol.com
* @Created:  February 15th, 2019 [10:58am]
* @Modified: February 22nd, 2019 [11:43pm]
* @Version:  1.0.0
*
* Copyright (C) 2019 by Ben Sokol. All Rights Reserved.
*/

#include <algorithm>
#include <cstdlib>
#include <iostream>
#include <mutex>
#include <random>
#include <string>
#include <utility>

#include "UTL_assert.h"

#include "BattleshipPlayer.hpp"

BattleshipPlayer::BattleshipPlayer(size_t aPlayerNum, size_t aSize, size_t aTotalTargets)
    : mBoard(std::shared_ptr<BattleshipBoard>(new BattleshipBoard(aSize, aTotalTargets))),
      mPlayerNum(aPlayerNum),
      mIsAlive(true),
      mTimesRevived(0),
      mAttacksReceived(0),
      mAttacksLaunchedInitialHits(0),
      mAttacksLaunchedInitialMisses(0),
      mAttacksLaunchedSecondaryHits(0),
      mAttacksLaunchedSecondaryMisses(0) {
  UTL_assert(aTotalTargets <= (aSize * aSize));
}


BattleshipPlayer::~BattleshipPlayer() {}


bool BattleshipPlayer::isAlive() {
  std::unique_lock<std::recursive_mutex> lck(mMtx);
  return mIsAlive;
}


void BattleshipPlayer::launchAttack(std::shared_ptr<BattleshipPlayer> target, BattleshipBoard::coordinate_t &coord) {
  std::lock(mMtx, target->mMtx);
  std::lock_guard<std::recursive_mutex> lck(mMtx, std::adopt_lock);
  std::lock_guard<std::recursive_mutex> lckTarget(target->mMtx, std::adopt_lock);
  UTL_assert(coord.getRow() != coord.invalid() && coord.getCol() != coord.invalid());

  BattleshipBoard::ATTACK_RESULT result = target->mBoard->attackLocation(coord);

  // Update this attack launched status
  if (result == BattleshipBoard::ATTACK_RESULT_INITIAL_HIT) {
    mAttacksLaunchedInitialHits++;
  }
  else if (result == BattleshipBoard::ATTACK_RESULT_INITIAL_MISS) {
    mAttacksLaunchedInitialMisses++;
  }
  else if (result == BattleshipBoard::ATTACK_RESULT_SECONDARY_HIT) {
    mAttacksLaunchedSecondaryHits++;
  }
  else if (result == BattleshipBoard::ATTACK_RESULT_SECONDARY_MISS) {
    mAttacksLaunchedSecondaryMisses++;
  }
  else {
    UTL_assert_always();
  }

  // Update target mAttacksReceived
  target->mAttacksReceived++;

  // Update target isAlive
  target->mIsAlive = target->mBoard->isAlive();
}


BattleshipBoard::coordinate_t BattleshipPlayer::getTargetCoordinates() {
  std::lock_guard<std::recursive_mutex> lck(mMtx);
  if (!mIsAlive) {
    return BattleshipBoard::coordinate_t();
  }
  BattleshipBoard::coordinate_t coordinate = mBoard->getAvailableTarget();

  return coordinate;
}


std::string BattleshipPlayer::printBoard(BattleshipBoard::whichBoard board) {
  std::lock_guard<std::recursive_mutex> lck(mMtx);
  return mBoard->printBoard(board, mPlayerNum);
}

std::string BattleshipPlayer::printCurrentBoard() {
  std::lock_guard<std::recursive_mutex> lck(mMtx);
  return mBoard->printCurrentBoard(mPlayerNum);
}

std::string BattleshipPlayer::printInitialBoard() {
  std::lock_guard<std::recursive_mutex> lck(mMtx);
  return mBoard->printInitialBoard(mPlayerNum);
}


std::string BattleshipPlayer::generateReport() {
  std::lock_guard<std::recursive_mutex> lck(mMtx);

  const size_t attacksLaunched = mAttacksLaunchedInitialHits + mAttacksLaunchedInitialMisses
                                 + mAttacksLaunchedSecondaryHits + mAttacksLaunchedSecondaryMisses;

  std::string str = "";

  str += "Player " + std::to_string(mPlayerNum) + " Report:\n";
  str += "  Targets Remaining: " + std::to_string(mBoard->getRemainingTargets()) + "\n";
  str += "  Times Revived: " + std::to_string(mTimesRevived) + "\n";
  str += "  Attacks Received: " + std::to_string(mAttacksReceived) + "\n";
  str += "  Attacks Launched: " + std::to_string(attacksLaunched) + "\n";
  str += "    Details:\n";
  str += "      Initial Hits:     " + std::to_string(mAttacksLaunchedInitialHits) + "\n";
  str += "      Initial Misses:   " + std::to_string(mAttacksLaunchedInitialMisses) + "\n";
  str += "      Secondary Hits:   " + std::to_string(mAttacksLaunchedSecondaryHits) + "\n";
  str += "      Secondary Misses: " + std::to_string(mAttacksLaunchedSecondaryMisses) + "\n";
  str += "\n";

  return str;
}


void BattleshipPlayer::revive() {
  std::lock_guard<std::recursive_mutex> lck(mMtx);
  mBoard->revive(2);
  mTimesRevived++;
  mIsAlive = true;
}


size_t BattleshipPlayer::getPlayerNum() const {
  return mPlayerNum;
}


size_t BattleshipPlayer::getRemainingTargets() const {
  return mBoard->getRemainingTargets();
}
size_t BattleshipPlayer::getTimesRevived() const {
  return mTimesRevived;
}
size_t BattleshipPlayer::getAttacksReceived() const {
  return mAttacksReceived;
}
size_t BattleshipPlayer::getAttacksLaunchedInitialHits() const {
  return mAttacksLaunchedInitialHits;
}
size_t BattleshipPlayer::getAttacksLaunchedInitialMisses() const {
  return mAttacksLaunchedInitialMisses;
}
size_t BattleshipPlayer::getAttacksLaunchedSecondaryHits() const {
  return mAttacksLaunchedSecondaryHits;
}
size_t BattleshipPlayer::getAttacksLaunchedSecondaryMisses() const {
  return mAttacksLaunchedSecondaryMisses;
}
