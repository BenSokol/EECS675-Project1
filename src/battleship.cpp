/**
* @Filename: battleship.cpp
* @Author:   Ben Sokol
* @Email:    ben@bensokol.com
* @Created:  February 13th, 2019 [11:01am]
* @Modified: February 15th, 2019 [2:58pm]
* @Version:  1.0.0
*
* Copyright (C) 2019 by Ben Sokol. All Rights Reserved.
*/

#include <cstdlib>
#include <future>
#include <iostream>
#include <mutex>
#include <shared_mutex>
#include <string>
#include <thread>

#include "TS_print.hpp"
#include "UTL_assert.h"
#include "UTL_colors.h"
#include "UTL_inputValidation.hpp"
//#include "UTL_textWrap.hpp"

#include "battleship.hpp"


/****************************************************************
* Battleship (Constructor):
*
****************************************************************/
Battleship::Battleship(const int argc, const char *argv[]) : mVersionMajor(0), mVersionMinor(1), mVersionBug(0) {
  mValidInputParameters = true;
  if (!initParameters(argc, argv)) {
    return;
  }

  mThreads = std::vector<std::future<void>>(mNumThreads);
  mPlayers = std::vector<BattleshipPlayer *>(mNumThreads);

  mtx = std::vector<std::shared_mutex>(MTX_COUNT);
  lck = std::vector<std::unique_lock<std::shared_mutex>>(MTX_COUNT);

  for (size_t i = 0; i < MTX_COUNT; ++i) {
    lck[i] = std::unique_lock<std::shared_mutex>(mtx[i], std::defer_lock);
  }
}

/****************************************************************
* ~Battleship (Destructor):
*
****************************************************************/
Battleship::~Battleship() {
  for (auto &thread : mThreads) {
    thread.wait();
  }

  for (auto &player : mPlayers) {
    delete player;
  }
}


/****************************************************************
* initParameters:
*
****************************************************************/
bool Battleship::initParameters(const int &argc, const char *argv[]) {
  // Check number of arguments, print version or help as necessary
  if (argc != 4) {
    mValidInputParameters = false;
    if (argc == 2) {
      if (std::string(argv[1]).compare("--version") == 0 || std::string(argv[1]).compare("-v") == 0) {
        printVersion();
        return false;
      }
      else if (std::string(argv[1]).compare("--help") == 0 || std::string(argv[1]).compare("-h") == 0) {
        printHelp();
        return false;
      }
    }
    std::cout << UTL::COLOR_RED_BOLD << "ERROR" << UTL::COLOR_RESET
              << ": Invalid Command Line Arguments\n       For help, ./battleship --help\n";
    return false;
  }

  // Validate all arguments are numbers
  if (UTL::isNumber(argv[1], 2)) {
    mNumThreads = std::stoull(argv[1]);
  }
  else {

    std::cout << UTL::COLOR_RED_BOLD << "ERROR" << UTL::COLOR_RESET
              << ": Failed input validation.\n       Details: P must be greater than or equal to 2\n";
    mValidInputParameters = false;
    return false;
  }

  if (UTL::isNumber(argv[2], 0)) {
    mSize = std::stoull(argv[2]);
  }
  else {
    std::cout << UTL::COLOR_RED_BOLD << "ERROR" << UTL::COLOR_RESET
              << ": Failed input validation.\n       Details: N must be greater than 0\n";
    mValidInputParameters = false;
    return false;
  }

  if (UTL::isNumber(argv[3], 0)) {
    mTargets = std::stoull(argv[3]);
  }
  else {
    std::cout << UTL::COLOR_RED_BOLD << "ERROR" << UTL::COLOR_RESET
              << ": Failed input validation.\n       Details: M must be greater than 0\n";
    mValidInputParameters = false;
    return false;
  }

  // Validate input value
  if (mValidInputParameters && !(mTargets <= (mSize * mSize))) {
    std::cout << UTL::COLOR_RED_BOLD << "ERROR" << UTL::COLOR_RESET
              << ": Failed input validation.\n       Reason: M must be less than or equal to N²\n";
    mValidInputParameters = false;
    return false;
  }
  return true;
}


/****************************************************************
* initPlayers:
*
****************************************************************/
void Battleship::initPlayers(size_t playerNum) {
  mPlayers[playerNum] = new BattleshipPlayer();

  TS::print(mtx[COUT], "Player ", playerNum, " has been initialized.\n");
}


/****************************************************************
* run:
*
****************************************************************/
void Battleship::run() {
  // Verify that input parameters are valid
  if (!mValidInputParameters) {
    return;
  }

  TS::print(mtx[COUT], "Initializing Players:\n");
  for (size_t i = 0; i < mNumThreads; ++i) {
    TS::print(mtx[COUT], "Launching thread to initialize player ", i, ".\n");
    mThreads[i] = std::async(std::launch::async, &Battleship::initPlayers, this, i);
  }
}
