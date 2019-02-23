/**
* @Filename: Battleship.cpp
* @Author:   Ben Sokol
* @Email:    ben@bensokol.com
* @Created:  February 13th, 2019 [11:01am]
* @Modified: February 22nd, 2019 [11:47pm]
* @Version:  1.0.0
*
* Copyright (C) 2019 by Ben Sokol. All Rights Reserved.
*/

#include <algorithm>    // find_if
#include <chrono>       // std::chrono, std::chrono::duration_cast, std::chrono::high_resolution_clock
#include <cstdlib>      // size_t
#include <fstream>      // std::ofstream
#include <future>       // std::future, std::async, std::launch::async
#include <iostream>     // std::cout
#include <memory>       // std::unique_ptr
#include <mutex>        // std::recursive_mutex
#include <random>       // std::random_device
#include <string>       // std::string, std::stoull
#include <sys/errno.h>  // errno
#include <sys/stat.h>   // mkdir
#include <vector>       // std::vector

#include "BattleshipPlayer.hpp"
#include "TS_latch.hpp"
#include "TS_log.hpp"
#include "TS_logAndPrint.hpp"
#include "TS_print.hpp"
#include "UTL_assert.h"
#include "UTL_colors.h"
#include "UTL_inputValidation.hpp"

#include "Battleship.hpp"


/****************************************************************
* Battleship (Constructor):
*
****************************************************************/
Battleship::Battleship(const int argc, const char *argv[])
    : mVersionMajor(1), mVersionMinor(0), mVersionBug(0), mDone(false), mNumThreads(0), mSize(0), mTargets(0) {
  mValidInputParameters = true;
  if (!initParameters(argc, argv)) {
    return;
  }

#ifdef ENABLE_LOGGING
  createLogFile();
#endif

  mThreads = std::vector<std::future<void>>(mNumThreads);
  mMtx = std::vector<std::recursive_mutex>(MTX_COUNT);
  mPlayerMtx = std::vector<std::mutex>(mNumThreads);
  mPlayers = std::vector<std::shared_ptr<BattleshipPlayer>>(mNumThreads);
  mCvs = std::vector<std::condition_variable>(mNumThreads);
  mRevivePlayer = std::vector<bool>(mNumThreads, false);
  mBegin = std::unique_ptr<TS::Latch>(new TS::Latch(mNumThreads));
  mWinner = std::numeric_limits<size_t>::max();
  TS::logAndPrint(mLogFile, mMtx[COUT], mMtx[LOG], "Battleship Simulation Initialized...\n");
}

/****************************************************************
* ~Battleship (Destructor):
*
****************************************************************/
Battleship::~Battleship() {
#ifdef ENABLE_LOGGING
  if (mLogFile.is_open()) {
    TS::log(mLogFile, mMtx[LOG], "\nFinished Battleship Program.\n\nEnd Log.\n");
  }
#endif
  for (auto &thread : mThreads) {
    thread.wait();
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
    std::cout << UTL::COLOR_RED_BOLD << "ERROR" << UTL::COLOR_RESET << ": Invalid Command Line Arguments\n"
              << "       For help, ./battleship --help\n";
    return false;
  }

  // Validate all arguments are numbers
  if (UTL::isNumber(argv[1], 2)) {
    mNumThreads = std::stoull(argv[1]);
  }
  else {
    std::cout << UTL::COLOR_RED_BOLD << "ERROR" << UTL::COLOR_RESET
              << ": Failed input validation. P must be greater than or equal to 2\n"
              << "       For help, ./battleship --help\n";
    mValidInputParameters = false;
    return false;
  }

  if (UTL::isNumber(argv[2], 0)) {
    mSize = std::stoull(argv[2]);
  }
  else {
    std::cout << UTL::COLOR_RED_BOLD << "ERROR" << UTL::COLOR_RESET
              << ": Failed input validation. N must be greater than 0\n"
              << "       For help, ./battleship --help\n";
    mValidInputParameters = false;
    return false;
  }

  if (UTL::isNumber(argv[3], 0)) {
    mTargets = std::stoull(argv[3]);
  }
  else {
    std::cout << UTL::COLOR_RED_BOLD << "ERROR" << UTL::COLOR_RESET
              << ": Failed input validation. M must be greater than 0\n"
              << "       For help, ./battleship --help\n";
    mValidInputParameters = false;
    return false;
  }

  // Validate input value
  if (mValidInputParameters && (mTargets > (mSize * mSize))) {
    std::cout << UTL::COLOR_RED_BOLD << "ERROR" << UTL::COLOR_RESET
              << ": Failed input validation. M must be less than or equal to N²\n"
              << "       For help, ./battleship --help\n";
    mValidInputParameters = false;
    return false;
  }
  return true;
}


/****************************************************************
* createLogFile:
*
****************************************************************/
#ifdef ENABLE_LOGGING
void Battleship::createLogFile() {
  // Creatue logs directory if it doesnt exist
  if (mkdir("logs", 0777) != -1) {
#ifndef NDEBUG
    std::cout << "Created logs Directory.\n";
#endif
  }
  else if (errno != EEXIST) {
    std::cout << UTL::COLOR_PURPLE_BOLD << "Warning" << UTL::COLOR_RESET
              << ": Unable to create logs directory. Error code: " << errno
              << "\n         To see what that means, see /usr/include/sys/errno.h\n";
    return;
  }


  auto clk =
      std::chrono::duration_cast<std::chrono::seconds>(std::chrono::high_resolution_clock::now().time_since_epoch())
          .count();
  mLogFile.open("logs/debug-" + std::to_string(clk) + ".log", std::ofstream::out);
  if (!mLogFile.is_open()) {
    std::cout << UTL::COLOR_PURPLE_BOLD << "Warning" << UTL::COLOR_RESET << ": Unable to create log file.\n";
  }
  else {
#ifndef NDEBUG
    std::cout << "Created log file: logs/debug-" + std::to_string(clk) + ".log\n";
#endif
  }
}
#endif


/****************************************************************
* initPlayers:
*
****************************************************************/
void Battleship::initPlayers(size_t playerNum) {
  // Create BattleshipPlayer
  UTL_assert(mSize > 0 && mTargets <= mSize * mSize);
  mPlayers[playerNum] = std::shared_ptr<BattleshipPlayer>(new BattleshipPlayer(playerNum, mSize, mTargets));

  // Report done
  TS::logAndPrint(mLogFile, mMtx[COUT], mMtx[LOG], "Player ", playerNum, " has been initialized.\n");
}


/****************************************************************
* battle:
*
****************************************************************/
void Battleship::battle(size_t playerNum) {
  mBegin->count_down_and_wait();
  TS::logAndPrint(mLogFile, mMtx[COUT], mMtx[LOG], "Starting player ", playerNum, ".\n");

  while (!mDone) {

    std::unique_lock<std::mutex> playerLck(mPlayerMtx[playerNum]);
    std::unique_lock<std::recursive_mutex> battleLck(mMtx[DATA_STRUCTURE_ACCESS]);
    if (!mPlayers[playerNum]->isAlive()) {
      // Check if done...
      if (mDone.load(std::memory_order_relaxed)) {
        TS::logAndPrint(mLogFile, mMtx[COUT], mMtx[LOG], "Player ", playerNum, " is exiting.\n");
        battleLck.unlock();
        return;
      }
      battleLck.unlock();

      // Wait for revive or done...
      TS::logAndPrint(mLogFile, mMtx[COUT], mMtx[LOG], "Player ", playerNum, " is waiting.\n");
      mCvs[playerNum].wait(playerLck, [this]() { return mDone.load(std::memory_order_relaxed); });
      TS::logAndPrint(mLogFile, mMtx[COUT], mMtx[LOG], "Player ", playerNum, " has been notified.\n");
      continue;
    }

    // Find target, and revive a target 10% of time
    bool foundTarget = false;
    size_t target;
    {
      std::random_device rd;
      target = rd() % mNumThreads;
      std::lock_guard<std::recursive_mutex> lck(mMtx[DATA_STRUCTURE_ACCESS]);
      // Get a random opponent that is still alive.
      for (size_t i = 0; i < mNumThreads; ++i) {
        if (target != playerNum && mPlayers[target]->isAlive()) {
          foundTarget = true;
          break;
        }
        else if (target == mNumThreads - 1) {
          target = 0;
        }
        else {
          target++;
        }
      }

      if (foundTarget) {
        size_t targetsAlive = 0;

        // Find number of players that are alive
        for (auto player : mPlayers) {
          if (player->isAlive()) {
            targetsAlive++;
          }
        }

        // 10% of the time attempt to revive a target if over half the targets are dead, but at least 2 are alive
        if ((targetsAlive) > 2 && (targetsAlive < (mNumThreads / 2))) {
          if ((rd() % 10) == 0) {
            size_t reviveTarget = rd() % mNumThreads;
            // Find random !isAlive thread and revive.
            for (size_t i = 0; i < mNumThreads; ++i) {
              if (reviveTarget != playerNum && !mPlayers[reviveTarget]->isAlive()) {
                TS::logAndPrint(mLogFile, mMtx[COUT], mMtx[LOG], "Player ", reviveTarget, " is being revived.\n");
                mPlayers[playerNum]->revive();
                TS::logAndPrint(mLogFile, mMtx[COUT], mMtx[LOG], "Player ", playerNum, " has been revived.\n");
                mCvs[reviveTarget].notify_all();
                break;
              }
              else if (reviveTarget == mNumThreads - 1) {
                reviveTarget = 0;
              }
              else {
                reviveTarget++;
              }
            }
          }
        }
      }
    }

    // If unable to find target, you must be last one alive.
    if (!foundTarget) {
      std::unique_lock<std::recursive_mutex> lck(mMtx[DATA_STRUCTURE_ACCESS]);
      // Verify no targets are alive
      size_t targetsAlive = 0;
      for (auto player : mPlayers) {
        if (player->isAlive()) {
          targetsAlive++;
        }
      }

      if (targetsAlive > 1) {
        TS::logAndPrint(mLogFile, mMtx[COUT], mMtx[LOG], "ERROR: At exit but multiple players are still alive...\n");
        continue;
      }

      mDone.store(true, std::memory_order_relaxed);
      lck.unlock();
      for (size_t i = 0; i < mNumThreads; i++) {
        mCvs[i].notify_all();
      }
      TS::logAndPrintAlways(mLogFile, mMtx[COUT], mMtx[LOG], "Player ", playerNum, " just won the battle.\n");
      mWinner = playerNum;
      return;
    }

    // get coordinate to attack
    BattleshipBoard::coordinate_t coordToAttack = mPlayers[target]->getTargetCoordinates();

    if (coordToAttack.getRow() != coordToAttack.invalid() && coordToAttack.getCol() != coordToAttack.invalid()) {
      TS::logAndPrint(mLogFile, mMtx[COUT], mMtx[LOG], "Player ", playerNum, " is attacking player ",
                      mPlayers[target]->getPlayerNum(), " at location (", coordToAttack.getRow(), ",",
                      coordToAttack.getCol(), ").\n");
      mPlayers[playerNum]->launchAttack(mPlayers[target], coordToAttack);
    }
  }
}


/****************************************************************
* generateReport:
*
****************************************************************/
void Battleship::generateReport() {
  std::lock(mMtx[COUT], mMtx[LOG]);
  std::lock_guard<std::recursive_mutex> lckCout(mMtx[COUT], std::adopt_lock);
  std::lock_guard<std::recursive_mutex> lckLog(mMtx[LOG], std::adopt_lock);

  mReport += "Battle Details:\n";
  mReport += "  P = " + std::to_string(mNumThreads) + "\n";
  mReport += "  M = " + std::to_string(mTargets) + "\n";
  mReport += "  N = " + std::to_string(mSize) + "\n";

  // Store boards in ss if conditions below are true
  if (mNumThreads == 2 && mSize <= 40) {
    mReport += "Boards:\n";
    for (auto &player : mPlayers) {
      mReport += player->printInitialBoard();
      mReport += player->printCurrentBoard();
    }
  }

  mReport += "\nPlayer Details:\n";

  size_t remainingTargets = 0;
  size_t timesRevived = 0;
  size_t attacksReceived = 0;
  size_t attacksLaunchedInitialHits = 0;
  size_t attacksLaunchedInitialMisses = 0;
  size_t attacksLaunchedSecondaryHits = 0;
  size_t attacksLaunchedSecondaryMisses = 0;

  // Generate report data and store in ss
  for (auto &player : mPlayers) {
    mReport += player->generateReport();
    remainingTargets += player->getRemainingTargets();
    timesRevived += player->getTimesRevived();
    attacksReceived += player->getAttacksReceived();
    attacksLaunchedInitialHits += player->getAttacksLaunchedInitialHits();
    attacksLaunchedInitialMisses += player->getAttacksLaunchedInitialMisses();
    attacksLaunchedSecondaryHits += player->getAttacksLaunchedSecondaryHits();
    attacksLaunchedSecondaryMisses += player->getAttacksLaunchedSecondaryMisses();
  }
  const size_t attacksLaunched = attacksLaunchedInitialHits + attacksLaunchedInitialMisses
                                 + attacksLaunchedSecondaryHits + attacksLaunchedSecondaryMisses;

  mReport += "Overall Results:\n";
  mReport += "  The winner was player " + std::to_string(mWinner) + "\n";
  mReport += "  Targets Remaining: " + std::to_string(remainingTargets) + "\n";
  mReport += "  Times Revived: " + std::to_string(timesRevived) + "\n";
  mReport += "  Attacks Received: " + std::to_string(attacksReceived) + "\n";
  mReport += "  Attacks Launched: " + std::to_string(attacksLaunched) + "\n";
  mReport += "    Details:\n";
  mReport += "      Initial Hits:     " + std::to_string(attacksLaunchedInitialHits) + "\n";
  mReport += "      Initial Misses:   " + std::to_string(attacksLaunchedInitialMisses) + "\n";
  mReport += "      Secondary Hits:   " + std::to_string(attacksLaunchedSecondaryHits) + "\n";
  mReport += "      Secondary Misses: " + std::to_string(attacksLaunchedSecondaryMisses) + "\n";
  mReport += "\n";

  // Report time statistics
  mReport += "Time Statistics:\n";
  mReport += "  Initial Phase took ";
  mReport += std::to_string(
      std::chrono::duration_cast<std::chrono::duration<double>>(mInitEndTimePoint - mInitStartTimePoint).count());
  mReport += " seconds.\n";
  mReport += "  Battle Phase took ";
  mReport += std::to_string(
      std::chrono::duration_cast<std::chrono::duration<double>>(mBattleEndTimePoint - mBattleStartTimePoint).count());
  mReport += " seconds.\n";
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

  // Init players
  TS::logAndPrintAlways(mLogFile, mMtx[COUT], mMtx[LOG], "Initializing Players...\n");
  mInitStartTimePoint = std::chrono::high_resolution_clock::now();
  for (size_t i = 0; i < mNumThreads; ++i) {
    //TS::logAndPrint(mLogFile, mMtx[COUT], mMtx[LOG], "Launching thread ", i, " to initialize player.\n");
    mThreads[i] = std::async(std::launch::async, &Battleship::initPlayers, this, i);
  }

  // Wait init to finish
  for (auto &thread : mThreads) {
    thread.wait();
  }
  mInitEndTimePoint = std::chrono::high_resolution_clock::now();
  TS::logAndPrintAlways(mLogFile, mMtx[COUT], mMtx[LOG], "Finished Initializing Players.\n");

  // launch battle
  TS::logAndPrintAlways(mLogFile, mMtx[COUT], mMtx[LOG], "Starting Battle...\n");
  mBattleStartTimePoint = std::chrono::high_resolution_clock::now();
  for (size_t i = 0; i < mNumThreads; ++i) {
    //TS::logAndPrint(mLogFile, mMtx[COUT], mMtx[LOG], "Launching thread ", i, " to go.\n");
    mThreads[i] = std::async(std::launch::async, &Battleship::battle, this, i);
  }

  // Wait for battle to end
  // #ifdef NDEBUG
  //   TS::print(mMtx[COUT], "Please wait");
  //   for (auto &thread : mThreads) {
  //     while (thread.wait_for(std::chrono::milliseconds(100)) == std::future_status::timeout) {
  //       TS::print(mMtx[COUT], ".");
  //     }
  //   }
  // #else
  for (auto &thread : mThreads) {
    thread.wait();
  }
  // #endif

  mBattleEndTimePoint = std::chrono::high_resolution_clock::now();
  TS::logAndPrintAlways(mLogFile, mMtx[COUT], mMtx[LOG], "Completed Battle.\n\n");

  // Generate report
  TS::logAndPrintAlways(mLogFile, mMtx[COUT], mMtx[LOG], "Generating Report: ");
  generateReport();
  TS::logAndPrintAlways(mLogFile, mMtx[COUT], mMtx[LOG], "Done\n\n");

#ifdef ENABLE_LOGGING
  // Always log boards to file
  if (mLogFile.is_open() && !(mNumThreads == 2 && mSize <= 40)) {
    TS::log(mLogFile, mMtx[LOG], "Boards:\n");
    for (auto &player : mPlayers) {
      TS::log(mLogFile, mMtx[LOG], player->printInitialBoard());
      TS::log(mLogFile, mMtx[LOG], player->printCurrentBoard());
    }
  }
#endif

  // Output report
  TS::logAndPrintAlways(mLogFile, mMtx[COUT], mMtx[LOG], mReport);
}
