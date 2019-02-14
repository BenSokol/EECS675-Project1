/**
* @Filename: battleship.cpp
* @Author:   Ben Sokol
* @Email:    ben@bensokol.com
* @Created:  February 13th, 2019 [11:01am]
* @Modified: February 14th, 2019 [9:42am]
* @Version:  1.0.0
*
* Copyright (C) 2019 by Ben Sokol. All Rights Reserved.
*/

#include <cstdlib>
#include <cstring>
#include <iostream>
#include <string>

#include "UTL_assert.hpp"
#include "UTL_colors.hpp"
#include "UTL_inputValidation.hpp"
#include "UTL_outputTools.hpp"

#include "battleship.hpp"


/****************************************************************
* Battleship (Constructor):
*
****************************************************************/
Battleship::Battleship(const int argc, char *argv[]) : mVersionMajor(0), mVersionMinor(1), mVersionBug(0) {
  mValidInputParameters = true;

  // Check number of arguments, print version or help as necessary
  if (argc != 4) {
    mValidInputParameters = false;
    if (argc == 2) {
      if (std::string(argv[1]).compare("--version") == 0 || std::string(argv[1]).compare("-v") == 0) {
        printVersion();
        return;
      }
      else if (std::string(argv[1]).compare("--help") == 0 || std::string(argv[1]).compare("-h") == 0) {
        printHelp();
        return;
      }
    }
    std::cout << UTL::COLOR_RED_BOLD << "ERROR" << UTL::COLOR_RESET
              << ": Invalid Command Line Arguments\n       For help, ./battleship --help\n";
    return;
  }

  // Validate all arguments are numbers
  if (UTL::isNumber(argv[1], 2)) {
    mNumThreads = std::stoull(argv[1]);
  }
  else {
    mValidInputParameters = false;
    std::cout << UTL::COLOR_RED_BOLD << "ERROR" << UTL::COLOR_RESET
              << ": Failed input validation.\n       Details: P must be greater than or equal to 2\n";
  }

  if (UTL::isNumber(argv[2], 0)) {
    mSize = std::stoull(argv[2]);
  }
  else {
    mValidInputParameters = false;
    std::cout << UTL::COLOR_RED_BOLD << "ERROR" << UTL::COLOR_RESET
              << ": Failed input validation.\n       Details: N must be greater than 0\n";
  }

  if (UTL::isNumber(argv[3], 0)) {
    mTargets = std::stoull(argv[3]);
  }
  else {
    mValidInputParameters = false;
    std::cout << UTL::COLOR_RED_BOLD << "ERROR" << UTL::COLOR_RESET
              << ": Failed input validation.\n       Details: M must be greater than 0\n";
  }

  // Validate input value
  if (mValidInputParameters && !(mTargets <= (mSize * mSize))) {
    std::cout << UTL::COLOR_RED_BOLD << "ERROR" << UTL::COLOR_RESET
              << ": Failed input validation.\n       Reason: M must be less than or equal to N²\n";
  }
}

/****************************************************************
* ~Battleship (Destructor):
*
****************************************************************/
Battleship::~Battleship() {}


/****************************************************************
* printHelp:
*
****************************************************************/
void Battleship::printHelp() const {
  using namespace UTL;

  // TItle bar (TOP)
  std::cout << COLOR_BOLD
            << "BATTLESHIP(1)                         battleship Manual                         BATTLESHIP(1)"
            << COLOR_RESET;
  std::cout << "\n";

  // Name
  std::cout << "\n" << COLOR_RED_BOLD << "NAME" << COLOR_RESET;
  std::cout << "\n\t" << COLOR_RED_BOLD << "battleship" << COLOR_RESET << " - battleship Command Line Simulation";

  // Synopsis
  std::cout << "\n";
  std::cout << "\n" << COLOR_RED_BOLD << "SYNOPSIS" << COLOR_RESET;
  std::cout << "\n\t" << COLOR_RED_BOLD << "battleship --help" << COLOR_RESET;
  std::cout << "\n\t" << COLOR_RED_BOLD << "battleship --version" << COLOR_RESET;
  std::cout << "\n\t" << COLOR_RED_BOLD << "battleship" << COLOR_RESET << " " << COLOR_GREEN_BOLD << "P N M"
            << COLOR_RESET;

  // Description
  std::cout << "\n";
  std::cout << "\n" << COLOR_RED_BOLD << "DESCRIPTION" << COLOR_RESET;
  std::cout << "\n";
  std::cout << textWrap(
      "\tThis program will run a multi-threaded simulation of the game Battleship.\n\tNOTE: The number "
      "of targets M must be less than or equal N² (M≤N²)",
      true);

  // Input Paremeters
  std::cout << "\n";
  std::cout << "\n" << COLOR_RED_BOLD << "INPUT PAREMETERS" << COLOR_RESET;
  std::cout << "\n";
  std::cout << "\n\t" << COLOR_GREEN_BOLD << "P" << COLOR_RESET << "\tNumber of threads to spawn. (P≥2)";
  std::cout << "\n\t" << COLOR_GREEN_BOLD << "N" << COLOR_RESET << "\tThe size of the board NxN (N>0)";
  std::cout << "\n\t" << COLOR_GREEN_BOLD << "M" << COLOR_RESET << "\tThe number of randomly placed targets (M>0)";

  // Notes
  std::cout << "\n";
  std::cout << "\n" << COLOR_RED_BOLD << "NOTES" << COLOR_RESET;
  std::cout << "\n\tThe number of targets M must be less than or equal N² (M≤N²)";

  // Examples
  std::cout << "\n";
  std::cout << "\n" << COLOR_RED_BOLD << "EXAMPLES" << COLOR_RESET;
  std::cout << "\n\tThe command:";
  std::cout << "\n";
  std::cout << "\n\t./battleship 4 3 4";
  std::cout << "\n";
  std::cout << textWrap(
      "\n\tWill run a simulation using 4 threads of 3x3 boards that each have 4 randomly placed targets.", true);

  // Copyright
  std::cout << "\n";
  std::cout << "\n" << COLOR_RED_BOLD << "COPYRIGHT" << COLOR_RESET;
  std::cout << "\n\tCopyright (C) 2018-2019 by Ben Sokol. All Rights Reserved.";
  std::cout << "\n";

  // TItle bar (BOTTOM)
  std::cout << COLOR_BOLD << "\nBATTLESHIP                               " << __DATE__
            << "                            BATTLESHIP(1)" << COLOR_RESET << "\n";
}


/****************************************************************
* printVersion:
*
****************************************************************/
void Battleship::printVersion() const {
  std::cout << "battleship (v" << mVersionMajor << "." << mVersionMinor << "." << mVersionBug << ")";
  std::cout << "\nCopyright (C) 2018-2019 by Ben Sokol. All Rights Reserved.";
  std::cout << "\n";
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
}
