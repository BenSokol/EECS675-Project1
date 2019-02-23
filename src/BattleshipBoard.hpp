/**
* @Filename: BattleshipBoard.hpp
* @Author:   Ben Sokol <Ben>
* @Email:    ben@bensokol.com
* @Created:  February 19th, 2019 [10:57am]
* @Modified: February 22nd, 2019 [9:31pm]
* @Version:  1.0.0
*
* Copyright (C) 2019 by Ben Sokol. All Rights Reserved.
*/

#ifndef BATTLESHIPBOARD_HPP
#define BATTLESHIPBOARD_HPP

#include <limits>
#include <mutex>
#include <string>
#include <vector>


class BattleshipBoard {
public:
  class coordinate_t {
    friend class BattleshipBoard;

  public:
    coordinate_t(size_t aCol, size_t aRow) {
      col = aCol;
      row = aRow;
    }
    coordinate_t() {
      row = std::numeric_limits<size_t>::max();
      col = std::numeric_limits<size_t>::max();
    }
    size_t getCol() const {
      return col;
    }
    size_t getRow() const {
      return row;
    }
    void setCol(size_t aCol) {
      col = aCol;
    }
    void setRow(size_t aRow) {
      row = aRow;
    }

    size_t invalid() const {
      return std::numeric_limits<size_t>::max();
    }

  private:
    size_t col;
    size_t row;
  };

  enum whichBoard { INITIAL, CURRENT };
  enum ATTACK_RESULT {
    ATTACK_RESULT_INITIAL_HIT,
    ATTACK_RESULT_INITIAL_MISS,
    ATTACK_RESULT_SECONDARY_HIT,
    ATTACK_RESULT_SECONDARY_MISS,
    COUNT
  };

  BattleshipBoard(size_t aSize, size_t aTotalTargets);
  ~BattleshipBoard();

  std::string printBoard(whichBoard board = CURRENT, size_t playerNum = std::numeric_limits<size_t>::max());
  std::string printCurrentBoard(size_t playerNum = std::numeric_limits<size_t>::max());
  std::string printInitialBoard(size_t playerNum = std::numeric_limits<size_t>::max());

  bool isAlive();

  BattleshipBoard::coordinate_t getAvailableTarget();

  size_t getRemainingTargets();

  ATTACK_RESULT attackLocation(coordinate_t &coordinate);

  void revive(size_t numberOfTargetsToAdd = 2);

private:
  const size_t mSize;
  size_t mTotalTargets;
  size_t mTargetsAvailable;
  size_t mNotAttackedSpotsRemaining;
  std::vector<std::vector<char>> mInitialBoard;
  std::vector<std::vector<char>> mBoard;
};

#endif
