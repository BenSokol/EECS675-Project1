/**
* @Filename: main.cpp
* @Author:   Ben Sokol
* @Email:    ben@bensokol.com
* @Created:  February 11th, 2019 [11:15am]
* @Modified: February 18th, 2019 [2:30pm]
* @Version:  1.0.0
*
* Copyright (C) 2019 by Ben Sokol. All Rights Reserved.
*/

#include "Battleship.hpp"
#include <memory>

int main(int argc, const char *argv[]) {
  std::unique_ptr<Battleship> ptr(new Battleship(argc, argv));
  ptr->run();
  return 0;
}
