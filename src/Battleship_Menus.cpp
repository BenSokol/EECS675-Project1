/**
* @Filename: Battleship_Menus.cpp
* @Author:   Ben Sokol <Ben>
* @Email:    ben@bensokol.com
* @Created:  February 15th, 2019 [1:19pm]
* @Modified: February 18th, 2019 [1:16pm]
* @Version:  1.0.0
*
* Copyright (C) 2019 by Ben Sokol. All Rights Reserved.
*/

#include <cassert>
#include <iostream>

#include "UTL_colors.h"
#include "UTL_textWrap.hpp"

#include "Battleship.hpp"


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
