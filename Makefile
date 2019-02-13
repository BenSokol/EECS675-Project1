# @Filename: Makefile
# @Author:   Ben Sokol
# @Email:    ben@bensokol.com
# @Created:  October 25th, 2018 [7:15pm]
# @Modified: February 13th, 2019 [1:40pm]
# @Version:  2.3.0
#
# Copyright (C) 2018 by Ben Sokol. All Rights Reserved.

ifneq ($(words $(MAKECMDGOALS)),1) # if no argument was given to make...
.DEFAULT_GOAL = build
%:
	@$(MAKE) $@ --no-print-directory -rf Makefile
else
ifndef ECHOCOUNT
TOTAL_STEPS := $(shell $(MAKE) $(MAKECMDGOALS) --no-print-directory -nrf $(firstword $(MAKEFILE_LIST)) ECHOCOUNT="COUNTTHIS" | grep -c "COUNTTHIS")
N := x
CURRENT_STEP = $(words $N)$(eval N := x $N)
endif

# Makefile Settings
EXE = battleship

SRCDIR = src
OBJDIR = obj
DEPDIR = dep

CSTD   = -std=c99
CXXSTD = -std=c++14

CFLAGS   = -O3 -g
CXXFLAGS = -O3 -g
WARNING_FLAGS = -Wall -Wextra -Wpedantic
DEBUGGER = gdb

MACOS_FLAGS =
LINUX_FLAGS = -pthread

IN_FILE =
OUT_FILE =
ERR_FILE =

COMMAND_LINE_ARGS = -1 3 4

PRE_BUILD_TARGETS =

ifeq ($(CXX),)
CXX = g++
endif

ifeq ($(CC),)
CC = gcc
endif

-include $(shell find $(DEPDIR) -type f -name '*.d' >/dev/null 2>/dev/null)

# Disable compiler specific warnings (unable to fix because they are caused by flex/bison generated files)
ifeq ($(shell uname),Darwin)
WARNING_FLAGS += $(MACOS_FLAGS)
DEBUGGER = lldb # macos doesnt have gdb, so must use lldb
else ifeq ($(shell uname),Linux)
WARNING_FLAGS += $(LINUX_FLAGS)
else
$(error $(shell echo "\033[31m")ERROR$(shell echo "\033[39m"): Unknown Operating System "$(shell uname)" [Line ~62])
endif

# Set include paths, allows inclusion of filenames without paths
INC = $(strip $(addprefix -I, $(shell find $(SRCDIR) -type d -print)))

CFLAGS   := $(strip $(CSTD) $(CFLAGS) $(WARNING_FLAGS) $(INC))
CXXFLAGS := $(strip $(CXXSTD) $(CXXFLAGS) $(WARNING_FLAGS) $(INC))

PREBUILD += $(strip $(addprefix $(OBJDIR)/,$(PRE_BUILD_TARGETS)))

CSOURCES = $(shell find $(SRCDIR) -type f -name '*.c')
COBJECTS = $(patsubst $(SRCDIR)/%.c,$(OBJDIR)/%.o,$(CSOURCES))

CPPSOURCES += $(shell find $(SRCDIR) -type f -name '*.cpp')
CPPOBJECTS += $(patsubst $(SRCDIR)/%.cpp,$(OBJDIR)/%.o,$(CPPSOURCES))

SOURCES = $(strip $(CSOURCES) $(CPPSOURCES))
OBJECTS = $(strip $(COBJECTS) $(CPPOBJECTS) $(PREBUILD))

.PHONY: build
build: $(EXE)

# Link $(EXE)
$(EXE): .pre_build $(strip $(OBJECTS))
	@printf "[%02d/%02d] Linking $(EXE)\n" `expr $(CURRENT_STEP)` $(TOTAL_STEPS) #$(ECHOCOUNT)
	@$(CXX) $(CXXFLAGS) -o $(EXE) $(OBJECTS)

# Build C with headers
$(OBJDIR)/%.o: $(SRCDIR)/%.c $(SRCDIR)/%.h .pre_build
	@printf "[%02d/%02d] Compiling C object $@\n" `expr $(CURRENT_STEP)` $(TOTAL_STEPS) #$(ECHOCOUNT)
	@$(CC) $(CFLAGS) -c -MT $@ -MD -MP -MF $(DEPDIR)/$(patsubst %.o,%,$(@F)).Td $< -o $@
	@mv -f $(DEPDIR)/$(patsubst %.o,%,$(@F)).Td $(DEPDIR)/$(patsubst %.o,%,$(@F)).d

# Build C without headers
$(OBJDIR)/%.o: $(SRCDIR)/%.c .pre_build
	@printf "[%02d/%02d] Compiling C object $@\n" `expr $(CURRENT_STEP)` $(TOTAL_STEPS) #$(ECHOCOUNT)
	@$(CC) $(CFLAGS) -c -MT $@ -MD -MP -MF $(DEPDIR)/$(patsubst %.o,%,$(@F)).Td $< -o $@
	@mv -f $(DEPDIR)/$(patsubst %.o,%,$(@F)).Td $(DEPDIR)/$(patsubst %.o,%,$(@F)).d

# Build C++ with headers
$(OBJDIR)/%.o: $(SRCDIR)/%.cpp $(SRCDIR)/%.hpp .pre_build
	@printf "[%02d/%02d] Compiling CXX object $@\n" `expr $(CURRENT_STEP)` $(TOTAL_STEPS) #$(ECHOCOUNT)
	@$(CXX) $(CXXFLAGS) -c -MT $@ -MD -MP -MF $(DEPDIR)/$(patsubst %.o,%,$(@F)).Td $< -o $@
	@mv -f $(DEPDIR)/$(patsubst %.o,%,$(@F)).Td $(DEPDIR)/$(patsubst %.o,%,$(@F)).d

# Build C++ without headers
$(OBJDIR)/%.o: $(SRCDIR)/%.cpp .pre_build
	@printf "[%02d/%02d] Compiling CXX object $@\n" `expr $(CURRENT_STEP)` $(TOTAL_STEPS) #$(ECHOCOUNT)
	@$(CXX) $(CXXFLAGS) -c -MT $@ -MD -MP -MF $(DEPDIR)/$(patsubst %.o,%,$(@F)).Td $< -o $@
	@mv -f $(DEPDIR)/$(patsubst %.o,%,$(@F)).Td $(DEPDIR)/$(patsubst %.o,%,$(@F)).d


# Clean output
.PHONY: clean
clean:
	@find . -name '.DS_Store' -type f -delete
	@if [ -f .pre_build ]; then rm -f .pre_build && printf "rm -f .pre_build\n"; fi
	@if [ -d $(OBJDIR) ]; then rm -rf $(OBJDIR) && printf "rm -rf $(OBJDIR)\n"; fi
	@if [ -d $(DEPDIR) ]; then rm -rf $(DEPDIR) && printf "rm -rf $(DEPDIR)\n"; fi
	@if [ -f $(EXE) ]; then rm -f $(EXE) && printf "rm -f $(EXE)\n"; fi

.PHONY: memtest
memtest: $(EXE)
	valgrind --leak-check=full --show-leak-kinds=all ./$(EXE) $(COMMAND_LINE_ARGS) || true

# Runs "clean" then makes a tar file of the whole directory in the folder ../tar/.
.PHONY: tar
tar: clean
	@printf "Creating \"../$(notdir $(CURDIR)).tgz\":\n" && \
	COPYFILE_DISABLE=1 &&                                      \
	cd $(dir $(CURDIR)) &&                                     \
	tar -czf $(notdir $(CURDIR)).tgz $(notdir $(CURDIR)) &&   \
	mv -f $(CURDIR).tgz $(notdir $(CURDIR)).tgz

# Make and run program. Doesnt report errors to make or terminal
.PHONY: run
run: $(EXE)
	@./$(EXE) $(COMMAND_LINE_ARGS) || printf "Non-zero return code detected.\n"; true

.PHONY: debug
debug: $(EXE)
	@$(DEBUGGER) ./$(EXE) $(COMMAND_LINE_ARGS)

# Debug variables
.PHONY: var
var:
	@printf "CXX: $(CXX)\n"
	@printf "CXXFLAGS: $(CXXFLAGS)\n"
	@printf "WARNING_FLAGS: $(WARNING_FLAGS)\n"
	@printf "SOURCES: \n$(strip $(foreach d, $(SOURCES), \t$d\n))\n"
	@printf "OBJECTS: \n$(strip $(foreach d, $(OBJECTS), \t$d\n))\n"
	@printf "INC: $(INC)\n"
	@printf "Debugger: $(DEBUGGER)\n"


# Run through pre-build steps
.pre_build: Makefile $(DEPDIR)/.pre_build $(OBJDIR)/.pre_build $(PREBUILD)
	@touch .pre_build

%/.pre_build:
	@mkdir -p $(shell dirname $@) && rsync -aq -f"+ */" -f"- *" $(SRCDIR)/ $(shell dirname $@)/
	@printf "[%02d/%02d] Created $(shell dirname $@) folder structure\n" `expr $(CURRENT_STEP)` $(TOTAL_STEPS) #$(ECHOCOUNT)
	@touch $@

endif
