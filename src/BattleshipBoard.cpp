/**
* @Filename: BattleshipBoard.cpp
* @Author:   Ben Sokol <Ben>
* @Email:    ben@bensokol.com
* @Created:  February 19th, 2019 [10:58am]
* @Modified: February 21st, 2019 [1:50pm]
* @Version:  1.0.0
*
* Copyright (C) 2019 by Ben Sokol. All Rights Reserved.
*/

#include <iostream>
#include <mutex>
#include <random>
#include <shared_mutex>
#include <utility>

#include "UTL_assert.h"

#include "BattleshipBoard.hpp"

BattleshipBoard::BattleshipBoard(size_t aSize, size_t aTotalTargets)
    : mSize(aSize),
      mTotalTargets(aTotalTargets),
      mTargetsAvailable(mTotalTargets),
      mInitialBoard(std::vector<std::vector<char>>(mSize)),
      mBoard(std::vector<std::vector<char>>(mSize)) {
  UTL_assert(mTotalTargets <= (mSize * mSize));

  for (size_t i = 0; i < mSize; ++i) {
    mBoard[i] = std::vector<char>(mSize);
    mInitialBoard[i] = std::vector<char>(mSize);
    std::fill(mBoard[i].begin(), mBoard[i].end(), '_');
    std::fill(mInitialBoard[i].begin(), mInitialBoard[i].end(), '_');
  }

  for (size_t i = 0; i < mTotalTargets; ++i) {
    std::pair<size_t, size_t> coord;
    if (!generateRandomCoordinates(coord, mPositionEncodedValues[OPEN])) {
      UTL_assert_always();
    }
    mBoard[coord.first][coord.second] = 'O';
    mInitialBoard[coord.first][coord.second] = 'O';
  }
}


BattleshipBoard::~BattleshipBoard() {}


bool BattleshipBoard::isAlive() {
  std::lock_guard<std::recursive_mutex> lk1(mMtx);
  return mTargetsAvailable > 0;
}


bool BattleshipBoard::generateRandomCoordinates(std::pair<size_t, size_t> &coord, const char target1 /* = '\0' */,
                                                const char target2 /* = '\0' */, const char target3 /* = '\0' */,
                                                const char target4 /* = '\0' */) {
  std::lock_guard<std::recursive_mutex> lk1(mMtx);
  std::random_device rd;

  if (mTargetsAvailable == 0) {
    return false;
  }

  if (target1 == '\0' && target2 == '\0' && target3 == '\0' && target4 == '\0') {
    coord = std::pair<size_t, size_t>((rd() % mSize), (rd() % mSize));
  }
  else if (target1 != '\0' && target2 == '\0' && target3 == '\0' && target4 == '\0') {
    do {
      coord = std::pair<size_t, size_t>((rd() % mSize), (rd() % mSize));
    } while (mBoard[coord.first][coord.second] != target1);
  }
  else if (target1 != '\0' && target2 != '\0' && target3 == '\0' && target4 == '\0') {
    do {
      coord = std::pair<size_t, size_t>((rd() % mSize), (rd() % mSize));
    } while (mBoard[coord.first][coord.second] != target1 && mBoard[coord.first][coord.second] != target2);
  }
  else if (target1 != '\0' && target2 != '\0' && target3 != '\0' && target4 == '\0') {
    do {
      coord = std::pair<size_t, size_t>((rd() % mSize), (rd() % mSize));
    } while (mBoard[coord.first][coord.second] != target1 && mBoard[coord.first][coord.second] != target2
             && mBoard[coord.first][coord.second] != target3);
  }
  else if (target1 != '\0' && target2 != '\0' && target3 != '\0' && target4 != '\0') {
    do {
      coord = std::pair<size_t, size_t>((rd() % mSize), (rd() % mSize));
    } while (mBoard[coord.first][coord.second] != target1 && mBoard[coord.first][coord.second] != target2
             && mBoard[coord.first][coord.second] != target3 && mBoard[coord.first][coord.second] != target4);
  }
  else {
    UTL_assert_always();
  }

  return true;
}


void BattleshipBoard::printBoard(std::recursive_mutex &mtx, size_t playerNum, std::ostream &ofs) {
  std::lock(mMtx, mtx);
  std::lock_guard<std::recursive_mutex> lk1(mMtx, std::adopt_lock);
  std::lock_guard<std::recursive_mutex> lk2(mtx, std::adopt_lock);
  if (playerNum != INTMAX_MAX) {
    ofs << "Player " << playerNum << " (Current Board):\n" << std::flush;
  }
  for (auto &vec : mBoard) {
    for (auto &i : vec) {
      ofs << i << std::flush;
    }
    ofs << "\n" << std::flush;
  }
  ofs << "\n" << std::flush;
}

void BattleshipBoard::printInitialBoard(std::recursive_mutex &mtx, size_t playerNum, std::ostream &ofs) {
  std::lock(mMtx, mtx);
  std::lock_guard<std::recursive_mutex> lk1(mMtx, std::adopt_lock);
  std::lock_guard<std::recursive_mutex> lk2(mtx, std::adopt_lock);
  if (playerNum != INTMAX_MAX) {
    ofs << "Player " << playerNum << " (Initial Board):\n" << std::flush;
  }
  for (auto &vec : mInitialBoard) {
    for (auto &i : vec) {
      ofs << i << std::flush;
    }
    ofs << "\n" << std::flush;
  }
  ofs << "\n" << std::flush;
}

size_t BattleshipBoard::getRemainingTargets() {
  std::lock_guard<std::recursive_mutex> lk1(mMtx);
  return mTargetsAvailable;
}
