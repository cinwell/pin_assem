##############################################################
#
# Customize build
#
##############################################################

# Debug
DEBUG=1

# Print debug output
DEBUG_OUTPUT = 1

# To use custom compiler
CXXHOME = /usr/local/gcc-4.8.2

#macros for log
LOG_FLAGS = -DLOG_ASSEM
OPT_FLAGS = -O0 -g

CXXFLAGS += $(OPT_FLAGS)
CXXFLAGS += $(LOG_FLAGS)

# Actual compiler to use
CXX = $(CXXHOME)/bin/g++ -std=c++0x $(CXXFLAGS)

#For static libraries
CUSTOM_LINK += -static-libstdc++ -static-libgcc
#CUSTOM_LINK += -static -static-libstdc++ -static-libgcc

# Where PIN is installed
PIN_ROOT ?= /afs/cs.pitt.edu/usr0/xianeizhang/private/bench/SATIAGO_SIMU/pin

#TOOL_LIBS += $(CUSTOM_LINK)
#TOOL_CXXFLAGS  += $(CUSTOM_FLAGS) -I$(PINPLAY_INCLUDE_HOME)
#TOOL_LPATHS += -L$(PINPLAY_LIB_HOME)


##############################################################
#
# set up and include *.config files
#
##############################################################

CONFIG_ROOT := $(PIN_ROOT)/source/tools/Config
include $(CONFIG_ROOT)/makefile.config
#include $(PIN_ROOT)/source/tools/SimpleExamples/makefile.rules
include makefile.rules
include $(TOOLS_ROOT)/Config/makefile.default.rules
