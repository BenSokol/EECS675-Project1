/**
* @Filename: main.cpp
* @Author:   Ben Sokol
* @Email:    ben@bensokol.com
* @Created:  February 11th, 2019 [11:15am]
* @Modified: February 13th, 2019 [1:16pm]
* @Version:  1.0.0
*
* Copyright (C) 2019 by Ben Sokol. All Rights Reserved.
*/
#include "battleship.hpp"
#include <iostream>

int main(int argc, char *argv[]) {
  Battleship *b = new Battleship(argc, argv);
  b->run();
  delete b;
  return 0;
}
