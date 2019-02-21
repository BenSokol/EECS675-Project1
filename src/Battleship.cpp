/**
* @Filename: Battleship.cpp
* @Author:   Ben Sokol
* @Email:    ben@bensokol.com
* @Created:  February 13th, 2019 [11:01am]
* @Modified: February 21st, 2019 [3:12am]
* @Version:  1.0.0
*
* Copyright (C) 2019 by Ben Sokol. All Rights Reserved.
*/

#include <chrono>       // std::chrono, std::chrono::duration_cast, std::chrono::high_resolution_clock
#include <cstdlib>      // size_t
#include <fstream>      // std::ofstream
#include <future>       // std::future, std::async, std::launch::async
#include <iostream>     // std::cout
#include <memory>       // std::unique_ptr
#include <mutex>        // std::recursive_mutex
#include <random>       // std::random_device
#include <sstream>      // std::stringstream
#include <string>       // std::string, std::stoull
#include <sys/errno.h>  // errno
#include <sys/stat.h>   // mkdir
#include <vector>       // std::vector

#include "BattleshipPlayer.hpp"
#include "TS_latch.hpp"
#include "TS_log.hpp"
#include "TS_print.hpp"
#include "UTL_assert.h"
#include "UTL_colors.h"
#include "UTL_inputValidation.hpp"

#include "Battleship.hpp"


/****************************************************************
* Battleship (Constructor):
*
****************************************************************/
Battleship::Battleship(const int argc, const char *argv[]) : mVersionMajor(0), mVersionMinor(3), mVersionBug(0) {
  mValidInputParameters = true;
  if (!initParameters(argc, argv)) {
    return;
  }

#ifdef ENABLE_LOGGING
  createLogFile();
#endif

  mThreads = std::vector<std::future<void>>(mNumThreads);
  mPlayers = std::vector<std::unique_ptr<BattleshipPlayer>>(mNumThreads);
  mtx = std::vector<std::recursive_mutex>(MTX_COUNT);
  mBegin = std::unique_ptr<TS::Latch>(new TS::Latch(mNumThreads));

  logAndPrint("Battleship Simulation Initialized...\n");
}

/****************************************************************
* ~Battleship (Destructor):
*
****************************************************************/
Battleship::~Battleship() {
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
* initParameters:
*
****************************************************************/
#ifdef ENABLE_LOGGING
void Battleship::createLogFile() {
  mLogFileSuccess = true;
  // Creatue logs directory if it doesnt exist
  if (mkdir("logs", 0777) != -1) {
#ifndef NDEBUG
    std::cout << "Created logs Directory.\n";
#endif
  }
  else if (errno != EEXIST) {
    mLogFileSuccess = false;
    std::cout << UTL::COLOR_PURPLE_BOLD << "Warning" << UTL::COLOR_RESET
              << ": Unable to create logs directory. Error code: " << errno
              << "\n         To see what that means, see /usr/include/sys/errno.h\n";
  }

  if (mLogFileSuccess) {
    auto clk =
        std::chrono::duration_cast<std::chrono::seconds>(std::chrono::high_resolution_clock::now().time_since_epoch())
            .count();
    mLogFile.open("logs/debug-" + std::to_string(clk) + ".log", std::ofstream::out);
    if (!mLogFile.is_open()) {
      mLogFileSuccess = false;
      std::cout << UTL::COLOR_PURPLE_BOLD << "Warning" << UTL::COLOR_RESET << ": Unable to create log file.\n";
    }
    else {
#ifndef NDEBUG
      std::cout << "Created log file: logs/debug-" + std::to_string(clk) + ".log\n";
#endif
    }
  }
}
#endif


/****************************************************************
* initPlayers:
*
****************************************************************/
void Battleship::initPlayers(size_t playerNum) {
  // Create BattleshipPlayer
  mPlayers[playerNum] = std::unique_ptr<BattleshipPlayer>(new BattleshipPlayer(playerNum, mSize, mTargets));

  // Report done
  logAndPrint("Player ", playerNum, " has been initialized.\n");
}


/****************************************************************
* battle:
*
****************************************************************/
void Battleship::battle(size_t playerNum) {
  mBegin->count_down_and_wait();
  logAndPrint("Starting player ", playerNum, ".\n");
  // while (mPlayers[playerNum]->isAlive()) {
  //   std::random_device rd;
  //   size_t target = rd() % mNumThreads;
  // }
  // mPlayers[playerNum]->printBoard(mtx[DEBUG], *mLogFile);
  // // Report done to standard out
  // TS::print(mtx[COUT], "Player ", playerNum, " has been initialized.\n");
}


/****************************************************************
* generateReport:
*
****************************************************************/
void Battleship::generateReport() {
  std::stringstream ss;

  // Store boards in ss if conditions below are true
  if (mNumThreads == 2 && mSize <= 40) {
    ss << "Boards:\n";
    for (auto &player : mPlayers) {
      player->printInitialBoard(mtx[REPORT], ss);
      player->printBoard(mtx[REPORT], ss);
    }
  }

  // Generate report data and store in ss
  for (auto &player : mPlayers) {
    player->generateReport(ss);
  }

  // Report time statistics
  ss << "Time Statistics:\n";
  ss << "  Initial Phase took "
     << std::chrono::duration_cast<std::chrono::duration<double>>(mInitEndTimePoint - mInitStartTimePoint).count()
     << " seconds.\n";
  ss << "  Battle Phase took "
     << std::chrono::duration_cast<std::chrono::duration<double>>(mBattleEndTimePoint - mBattleStartTimePoint).count()
     << " seconds.\n";

  // Store report in mReport
  mReport = ss.str();
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
  logAndPrintAlways("Initializing Players...\n");
  mInitStartTimePoint = std::chrono::high_resolution_clock::now();
  for (size_t i = 0; i < mNumThreads; ++i) {
    logAndPrint("Launching thread ", i, " to initialize player.\n");
    mThreads[i] = std::async(std::launch::async, &Battleship::initPlayers, this, i);
  }

  // Wait init to finish
  for (auto &thread : mThreads) {
    thread.wait();
  }
  mInitEndTimePoint = std::chrono::high_resolution_clock::now();
  logAndPrintAlways("Finished Initializing Players.\n");

  // launch battle
  logAndPrintAlways("Starting Battle...\n");
  mBattleStartTimePoint = std::chrono::high_resolution_clock::now();
  for (size_t i = 0; i < mNumThreads; ++i) {
    logAndPrint("Launching thread ", i, " to go.\n");
    mThreads[i] = std::async(std::launch::async, &Battleship::battle, this, i);
  }

  // Wait for battle to end
  for (auto &thread : mThreads) {
    thread.wait();
  }
  mBattleEndTimePoint = std::chrono::high_resolution_clock::now();
  logAndPrintAlways("Completed Battle.\n\n");

  // Generate report
  logAndPrintAlways("Generating Report: ");
  generateReport();
  logAndPrintAlways("Done\n\n");

#ifdef ENABLE_LOGGING
  // Always log boards to file
  if (mLogFileSuccess && !(mNumThreads == 2 && mSize <= 40)) {
    for (auto &player : mPlayers) {
      player->printInitialBoard(mtx[LOG], mLogFile);
      player->printBoard(mtx[LOG], mLogFile);
    }
  }
#endif

  // Output report
  logAndPrintAlways(mReport);

#ifdef ENABLE_LOGGING
  if (mLogFileSuccess) {
    TS::log(mLogFile, mtx[LOG], "Finished Battleship Program.\nEnd Log.\n");
  }
#endif
}
