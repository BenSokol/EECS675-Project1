/**
* @Filename: BattleshipBoard.hpp
* @Author:   Ben Sokol <Ben>
* @Email:    ben@bensokol.com
* @Created:  February 19th, 2019 [10:57am]
* @Modified: February 22nd, 2019 [2:10pm]
* @Version:  1.0.0
*
* Copyright (C) 2019 by Ben Sokol. All Rights Reserved.
*/

#ifndef BATTLESHIPBOARD_HPP
#define BATTLESHIPBOARD_HPP

#include <array>
#include <cstdint>
#include <iostream>
#include <mutex>
#include <string>
#include <vector>

class BattleshipBoard {
public:
  BattleshipBoard(size_t aSize, size_t aTotalTargets);
  ~BattleshipBoard();

  std::string printBoard(size_t playerNum = INTMAX_MAX);
  std::string printInitialBoard(size_t playerNum = INTMAX_MAX);

  enum positionType { OPEN, ATTACKED, TARGET, HIT, COUNT };
  const std::array<const char, COUNT> mPositionEncodedValues = { { '_', '.', 'O', '*' } };
  bool isAlive();

  bool generateRandomCoordinates(std::pair<size_t, size_t> &coord, const char target1 = '\0', const char target2 = '\0',
                                 const char target3 = '\0', const char target4 = '\0');

  size_t getRemainingTargets();

private:
  const size_t mSize;
  const size_t mTotalTargets;
  size_t mTargetsAvailable;
  std::vector<std::vector<char>> mInitialBoard;
  std::vector<std::vector<char>> mBoard;
  std::recursive_mutex mMtx;
};

#endif
