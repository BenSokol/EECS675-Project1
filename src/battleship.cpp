/**
* @Filename: battleship.cpp
* @Author:   Ben Sokol
* @Email:    ben@bensokol.com
* @Created:  February 13th, 2019 [11:01am]
* @Modified: February 13th, 2019 [2:01pm]
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

#include "battleship.hpp"


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

Battleship::~Battleship() {}


void Battleship::printHelp() const {
  std::cout << UTL::COLOR_BOLD
            << "BATTLESHIP(1)                         battleship Manual                         BATTLESHIP(1)"
            << UTL::COLOR_RESET;
  std::cout << "\n";

  std::cout << "\n" << UTL::COLOR_RED_BOLD << "NAME" << UTL::COLOR_RESET;
  std::cout << "\n\t" << UTL::COLOR_RED_BOLD << "battleship" << UTL::COLOR_RESET
            << " - battleship Command Line Simulation";

  // Synopsis
  std::cout << "\n";
  std::cout << "\n" << UTL::COLOR_RED_BOLD << "SYNOPSIS" << UTL::COLOR_RESET;
  std::cout << "\n\t" << UTL::COLOR_RED_BOLD << "battleship --help" << UTL::COLOR_RESET;
  std::cout << "\n\t" << UTL::COLOR_RED_BOLD << "battleship --version" << UTL::COLOR_RESET;
  std::cout << "\n\t" << UTL::COLOR_RED_BOLD << "battleship" << UTL::COLOR_RESET << " " << UTL::COLOR_GREEN_BOLD
            << "P N M" << UTL::COLOR_RESET;

  // Description
  std::cout << "\n";
  std::cout << "\n" << UTL::COLOR_RED_BOLD << "DESCRIPTION" << UTL::COLOR_RESET;
  std::cout << "\n";
  std::cout << "\tNOTE: The number of targets M must be less than or equal N² (M≤N²)";

  // Input Paremeters
  std::cout << "\n";
  std::cout << "\n" << UTL::COLOR_RED_BOLD << "INPUT PAREMETERS" << UTL::COLOR_RESET;
  std::cout << "\n\tP\tNumber of threads to spawn. (P≥2)";
  std::cout << "\n\tN\tThe size of the board NxN (N>0)";
  std::cout << "\n\tM\tThe number of randomly placed targets (M>0)";

  // Notes
  std::cout << "\n";
  std::cout << "\n" << UTL::COLOR_RED_BOLD << "NOTES" << UTL::COLOR_RESET;
  std::cout << "\n\tThe number of targets M must be less than or equal N² (M≤N²)";

  // Examples
  std::cout << "\n";
  std::cout << "\n" << UTL::COLOR_RED_BOLD << "EXAMPLES" << UTL::COLOR_RESET;

  // Copyright
  std::cout << "\n";
  std::cout << "\n" << UTL::COLOR_RED_BOLD << "COPYRIGHT" << UTL::COLOR_RESET;
  std::cout << "\n\tCopyright (C) 2018-2019 by Ben Sokol. All Rights Reserved.";
  std::cout << "\n";

  std::cout << UTL::COLOR_BOLD << "\nBATTLESHIP                               " << __DATE__
            << "                            BATTLESHIP(1)" << UTL::COLOR_RESET << "\n";
}

void Battleship::printVersion() const {
  std::cout << "battleship (v" << mVersionMajor << "." << mVersionMinor << "." << mVersionBug << ")";
  std::cout << "\nCopyright (C) 2018-2019 by Ben Sokol. All Rights Reserved.";
  std::cout << "\n";
}

void Battleship::run() {
  // Verify that input parameters are valid
  if (!mValidInputParameters) {
    return;
  }
}
