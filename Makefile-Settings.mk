# @Filename: Makefile-Settings.mk
# @Author:   Ben Sokol <Ben>
# @Email:    ben@bensokol.com
# @Created:  February 14th, 2019 [5:21pm]
# @Modified: February 15th, 2019 [2:18pm]
# @Version:  1.0.0
#
# Copyright (C) 2019 by Ben Sokol. All Rights Reserved.

###############################################################################
# Makefile Settings                                                           #
###############################################################################
EXE = battleship

CXXSTD = -std=c++17

CXXFLAGS = -O3 -g
WARNING_FLAGS = -Wall -Wextra -Wpedantic

MACOS_FLAGS = -Weverything -Wno-c++98-compat -Wno-c++98-compat-pedantic -Wno-padded -Wno-date-time
LINUX_FLAGS = -pthread

COMMAND_LINE_ARGS = 4 3 4
