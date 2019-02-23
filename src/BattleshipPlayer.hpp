/**
* @Filename: BattleshipPlayer.hpp
* @Author:   Ben Sokol <Ben>
* @Email:    ben@bensokol.com
* @Created:  February 15th, 2019 [10:57am]
* @Modified: February 22nd, 2019 [11:40pm]
* @Version:  1.0.0
*
* Copyright (C) 2019 by Ben Sokol. All Rights Reserved.
*/

#ifndef BATTLESHIPPLAYER_HPP
#define BATTLESHIPPLAYER_HPP

#include <cstdlib>
#include <memory>
#include <mutex>
#include <string>
#include <vector>

#include "BattleshipBoard.hpp"

class BattleshipPlayer {
public:
  BattleshipPlayer(size_t aPlayerNum, size_t aSize, size_t aTotalTargets);
  ~BattleshipPlayer();

  bool isAlive();
  void launchAttack(std::shared_ptr<BattleshipPlayer> target, BattleshipBoard::coordinate_t &coord);
  std::string printBoard(BattleshipBoard::whichBoard board);
  std::string printCurrentBoard();
  std::string printInitialBoard();
  BattleshipBoard::coordinate_t getTargetCoordinates();

  std::string generateReport();

  size_t getPlayerNum() const;

  void revive();

  size_t getRemainingTargets() const;
  size_t getTimesRevived() const;
  size_t getAttacksReceived() const;
  size_t getAttacksLaunchedInitialHits() const;
  size_t getAttacksLaunchedInitialMisses() const;
  size_t getAttacksLaunchedSecondaryHits() const;
  size_t getAttacksLaunchedSecondaryMisses() const;

private:
  std::shared_ptr<BattleshipBoard> mBoard;
  const size_t mPlayerNum;
  bool mIsAlive;
  std::recursive_mutex mMtx;

  size_t mTimesRevived;
  size_t mAttacksReceived;
  size_t mAttacksLaunchedInitialHits;
  size_t mAttacksLaunchedInitialMisses;
  size_t mAttacksLaunchedSecondaryHits;
  size_t mAttacksLaunchedSecondaryMisses;
};

#endif
