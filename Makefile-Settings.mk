# @Filename: Makefile-Settings.mk
# @Author:   Ben Sokol <Ben>
# @Email:    ben@bensokol.com
# @Created:  February 14th, 2019 [5:21pm]
# @Modified: February 18th, 2019 [1:52pm]
# @Version:  1.0.0
#
# Copyright (C) 2019 by Ben Sokol. All Rights Reserved.

###############################################################################
# Makefile Settings                                                           #
###############################################################################
EXE = battleship

# KU's cycle servers version of gcc does not support all features this project uses. clang-5.0 does.
ifeq ($(shell uname),Linux)
CXX = clang++-5.0
CC = clang-5.0
endif

CCSTD = -std=c11
CXXSTD = -std=c++17

CCFLAGS = -O3 -g
CXXFLAGS = -O3 -g
WARNING_FLAGS = -Wall -Wextra -Wpedantic
WARNING_FLAGS_GCC =
WARNING_FLAGS_CLANG = -Weverything -Wno-c++98-compat -Wno-c++98-compat-pedantic -Wno-padded -Wno-date-time

MACOS_FLAGS =
LINUX_FLAGS = -pthread

COMMAND_LINE_ARGS = 4 3 4

MAKEFILE_RULE_INSTALL_DISABLE = 1
MAKEFILE_RULE_UNINSTALL_DISABLE = 1
