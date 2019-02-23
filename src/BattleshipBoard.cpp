/**
* @Filename: BattleshipBoard.cpp
* @Author:   Ben Sokol <Ben>
* @Email:    ben@bensokol.com
* @Created:  February 19th, 2019 [10:58am]
* @Modified: February 22nd, 2019 [11:18pm]
* @Version:  1.0.0
*
* Copyright (C) 2019 by Ben Sokol. All Rights Reserved.
*/

#include <mutex>
#include <random>

#include "UTL_assert.h"

#include "BattleshipBoard.hpp"

BattleshipBoard::BattleshipBoard(size_t aSize, size_t aTotalTargets)
    : mSize(aSize),
      mTotalTargets(aTotalTargets),
      mTargetsAvailable(aTotalTargets),
      mNotAttackedSpotsRemaining(aSize * aSize),
      mInitialBoard(std::vector<std::vector<char>>(mSize, std::vector<char>(mSize, '_'))),
      mBoard(std::vector<std::vector<char>>(mSize, std::vector<char>(mSize, '_'))) {
  UTL_assert(mTotalTargets <= (mSize * mSize));

  std::random_device rd;
  for (size_t i = 0; i < mTotalTargets; ++i) {
    coordinate_t coordinate((rd() % mSize), (rd() % mSize));

    // Find and add targets to board
    for (size_t j = 0;; ++j) {
      UTL_assert(j < (mSize * mSize));
      if (j >= (mSize * mSize)) {
        // Should never reach here.
        throw("ERROR: Couldnt find free spot to add target.");
      }
      else if (mBoard[coordinate.row][coordinate.col] == '_') {
        mBoard[coordinate.row][coordinate.col] = 'O';
        mInitialBoard[coordinate.row][coordinate.col] = 'O';
        break;
      }
      if (coordinate.row < (mSize - 1)) {
        coordinate.row++;
      }
      else if (coordinate.col < (mSize - 1)) {
        coordinate.row = 0;
        coordinate.col++;
      }
      else {
        coordinate.row = 0;
        coordinate.col = 0;
      }
    }
  }
}


BattleshipBoard::~BattleshipBoard() {}


bool BattleshipBoard::isAlive() {
  return mTargetsAvailable > 0;
}


BattleshipBoard::coordinate_t BattleshipBoard::getAvailableTarget() {
  std::random_device rd;

  if (mNotAttackedSpotsRemaining == 0) {
    return coordinate_t();
  }

  coordinate_t coordinate((rd() % mSize), (rd() % mSize));
  // for (size_t j = 0; j < mSize; ++j) {
  //   for (size_t k = 0; k < mSize; ++k) {
  //     if (mBoard[coordinate.row][coordinate.col] == '_' || mBoard[coordinate.row][coordinate.col] == 'O') {
  //       break;
  //     }
  //     coordinate.row = (coordinate.row + k) % mSize;
  //   }
  //   if (mBoard[coordinate.row][coordinate.col] == '_' || mBoard[coordinate.row][coordinate.col] == 'O') {
  //     break;
  //   }
  //   coordinate.col = (coordinate.col + j) % mSize;
  // }
  for (size_t i = 0;; ++i) {
    UTL_assert(i < (mSize * mSize));
    if (i >= (mSize * mSize)) {
      throw("ERROR: Unable to find target, but mNotAttackedSpotsRemaining > 0.");
    }
    else if (mBoard[coordinate.row][coordinate.col] == '_' || mBoard[coordinate.row][coordinate.col] == 'O') {
      break;
    }
    else if (coordinate.row < (mSize - 1)) {
      coordinate.row++;
    }
    else if (coordinate.col < (mSize - 1)) {
      coordinate.row = 0;
      coordinate.col++;
    }
    else {
      coordinate.row = 0;
      coordinate.col = 0;
    }
  }
  return coordinate;
}


std::string BattleshipBoard::printCurrentBoard(size_t playerNum) {
  return printBoard(CURRENT, playerNum);
}

std::string BattleshipBoard::printInitialBoard(size_t playerNum) {
  return printBoard(INITIAL, playerNum);
}

std::string BattleshipBoard::printBoard(whichBoard board, size_t playerNum) {
  std::string str = "";
  if (playerNum != std::numeric_limits<size_t>::max()) {
    str += "Player ";
    str += std::to_string(playerNum);
    if (board == INITIAL) {
      str += " (Initial Board):\n";
    }
    else if (board == CURRENT) {
      str += " (Current Board):\n";
    }
    else {
      UTL_assert_always();
    }
  }
  std::vector<std::vector<char>> *boardToPrint = nullptr;
  if (board == INITIAL) {
    boardToPrint = &mInitialBoard;
  }
  else if (board == CURRENT) {
    boardToPrint = &mBoard;
  }
  else {
    UTL_assert_always();
  }
  if (boardToPrint) {
    for (std::vector<char> &vec : *boardToPrint) {
      std::string tempStr = "";
      tempStr.resize(vec.size());
      std::copy(vec.begin(), vec.end(), tempStr.begin());
      str += tempStr;
      str += "\n";
    }
  }
  str += "\n";
  return str;
}

size_t BattleshipBoard::getRemainingTargets() {
  return mTargetsAvailable;
}

BattleshipBoard::ATTACK_RESULT BattleshipBoard::attackLocation(coordinate_t &coordinate) {
  if (mBoard[coordinate.row][coordinate.col] == '_') {
    mBoard[coordinate.row][coordinate.col] = '.';
    mNotAttackedSpotsRemaining--;
    return ATTACK_RESULT_INITIAL_MISS;
  }
  else if (mBoard[coordinate.row][coordinate.col] == 'O') {
    mBoard[coordinate.row][coordinate.col] = '*';
    mTargetsAvailable--;
    mNotAttackedSpotsRemaining--;
    return ATTACK_RESULT_INITIAL_HIT;
  }
  else if (mBoard[coordinate.row][coordinate.col] == '.') {
    return ATTACK_RESULT_SECONDARY_MISS;
  }
  else if (mBoard[coordinate.row][coordinate.col] == '*') {
    return ATTACK_RESULT_SECONDARY_HIT;
  }
  else {
    return COUNT;
  }
}


void BattleshipBoard::revive(size_t numberOfTargetsToAdd) {
  std::random_device rd;

  for (size_t i = 0; i < numberOfTargetsToAdd; ++i) {
    coordinate_t coordinate((rd() % mSize), (rd() % mSize));
    bool foundTarget = false;

    // Find and add targets to board
    for (size_t j = 0; j < mSize && !foundTarget; ++j) {
      for (size_t k = 0; k < mSize && !foundTarget; ++k) {
        if (mBoard[coordinate.row][coordinate.col] != 'O') {
          foundTarget = true;
          break;
        }
        coordinate.col = (coordinate.col + k) % mSize;
      }
      if (mBoard[coordinate.row][coordinate.col] != 'O') {
        break;
      }
      coordinate.row = (coordinate.row + j) % mSize;
    }
    mBoard[coordinate.row][coordinate.col] = 'O';
    mTotalTargets++;
    mTargetsAvailable++;
    mNotAttackedSpotsRemaining++;
  }
}
