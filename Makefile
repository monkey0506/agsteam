# Makefile for AGSteam
#
# define some paths...
PATH_SRC=.
PATH_AGS2CLIENT=$(PATH_SRC)/ags2client
# path to Steamworks sdk folder may be passed in from command line
PATH_STEAMWORKS?=$(PATH_SRC)/../steamworks/142/sdk
PATH_STEAMWORKS_INC=$(PATH_STEAMWORKS)/public
PATH_STEAMWORKS_LIB=$(PATH_STEAMWORKS)/redistributable_bin
PATH_BUILD=$(PATH_SRC)/Solutions/build
SRCS=ags2client/IAGS2Client.cpp ags2client/IClientAchievements.cpp ags2client/IClientLeaderboards.cpp \
	ags2client/IClientStats.cpp ags2client/main.cpp AGS2Client.cpp AGSteamPlugin.cpp \
	SteamAchievements.cpp SteamLeaderboards.cpp SteamStats.cpp
# .o object files for ags2client end up in a separate directory, but this is fine
OBJS=$(patsubst %.cpp,%.o,$(SRCS))
CXXFLAGS=-g -Wall -std=c++11 -O2 -fPIC -I$(PATH_STEAMWORKS_INC)

# define some functions to help build other needed paths

# get path to the ignored "build" folder's platform subdirectory (e.g., ./Solutions/build/osx)
# requires 1 arg, platform to build (e.g., osx)
define platform_build_path
$(PATH_BUILD)/$1
endef

# get path to platform-specific object folder (e.g., ./Solutions/build/osx/obj)
# first arg (required) specifies platform (e.g., osx)
# second arg (optional) may be used for variant builds (e.g., ./Solutions/build/osx/obj/disjoint)
define platform_obj_path
$(call platform_build_path,$1)/obj$(if $2,/$2,)
endef

# get object file path names (e.g., ./Solutions/build/osx/obj/ags2client/IAGS2Client.o) for all object files
# first arg (required) specifies platform (e.g., osx)
# second arg (optional) specifies variant (e.g., disjoint)
define obj_file_paths
$(patsubst %,$(call platform_obj_path,$1,$2)/%,$(OBJS))
endef

# template to build a specific platform
# requires 1 arg, platform to build (e.g., osx)
# requires additional variables to be predefined if expected as non-empty:
# 	* [PLATFORM]_CXX_FLAGS: additional C++ flags for making object files (e.g., -DMAC_VERSION)
# 	* [PLATFORM]_STEAMWORKS_DIR: platform-specific subdirectory of Steamwork's "redistributable_bin"
# 	* [PLATFORM]_LIB_FLAGS: linker flags for "unified" builds (e.g., "-dynamiclib" and
# 		"-o $PATH_[PLATFORM]_BUILD)/libagsteam.dylib" should probably be specified here for OS X)
# 	* [PLATFORM]_LIB_DISJOINT_FLAGS: linker flags for "disjoint" builds
define build_platform

# make uppercase version of platform argument for constructing variables
$(eval VPLAT=$(shell echo $1 | tr '[a-z]' '[A-Z]'))

# construct platform-specific and variant-specific paths
$(eval PATH_$(VPLAT)_BUILD=$(call platform_build_path,$1))
$(eval PATH_$(VPLAT)_OBJ=$(call platform_obj_path,$1))
$(eval $(VPLAT)_OBJ_FILE_PATHS=$(call obj_file_paths,$1))
$(eval PATH_$(VPLAT)_OBJ_DISJOINT=$(call platform_obj_path,$1,disjoint))
$(eval $(VPLAT)_OBJ_DISJOINT_FILE_PATHS=$(call obj_file_paths,$1,disjoint))

# rule for making "unified" build
$1-unified: $($(VPLAT)_OBJ_FILE_PATHS)
	g++ -L$(PATH_STEAMWORKS_LIB)/$($(VPLAT)_STEAMWORKS_DIR) -lsteam_api $($(VPLAT)_LIB_FLAGS) $($(VPLAT)_OBJ_FILE_PATHS)

# rule for making "unified" object files
$(PATH_$(VPLAT)_OBJ)/%.o: $(PATH_SRC)/%.cpp
	mkdir -p $(PATH_$(VPLAT)_OBJ)/ags2client
	g++ $(CXXFLAGS) -DAGS2CLIENT_UNIFIED_CLIENT_NAME $($(VPLAT)_CXX_FLAGS) -c $$< -o $$@

# rule for making "disjoint" build
$1-disjoint: $($(VPLAT)_OBJ_DISJOINT_FILE_PATHS)
	g++ -L$(PATH_STEAMWORKS_LIB)/$($(VPLAT)_STEAMWORKS_DIR) -lsteam_api $($(VPLAT)_LIB_DISJOINT_FLAGS) $($(VPLAT)_OBJ_DISJOINT_FILE_PATHS)

# rule for making "disjoint" object files
$(PATH_$(VPLAT)_OBJ_DISJOINT)/%.o: $(PATH_SRC)/%.cpp
	mkdir -p $(PATH_$(VPLAT)_OBJ_DISJOINT)/ags2client
	g++ $(CXXFLAGS) $($(VPLAT)_CXX_FLAGS) -c $$< -o $$@

# undefine VPLAT, it shouldn't be reused (other platform paths defined above may be reused)
undefine VPLAT

endef


# default rule: do nothing
none:

# clean all platforms
clean: clean-osx clean-linux


# OS X
OSX_CXX_FLAGS=-DMAC_VERSION
OSX_STEAMWORKS_DIR=osx32
OSX_LIB_FLAGS=-dynamiclib -o $(PATH_OSX_BUILD)/libagsteam.dylib
OSX_LIB_DISJOINT_FLAGS=-dynamiclib -o $(PATH_OSX_BUILD)/libagsteam-disjoint.dylib

osx: osx-unified osx-disjoint
$(eval $(call build_platform,osx))

clean-osx:
	rm -rf $(PATH_OSX_OBJ) $(PATH_OSX_BUILD)/libagsteam.dylib $(PATH_OSX_BUILD)/libagsteam-disjoint.dylib


# Linux (64-bit)
LINUX64_CXX_FLAGS=-DLINUX_VERSION
LINUX64_STEAMWORKS_DIR=linux64
LINUX64_LIB_FLAGS=-shared -o $(PATH_LINUX64_BUILD)/libagsteam.so
LINUX64_LIB_DISJOINT_FLAGS=-shared -o $(PATH_LINUX64_BUILD)/libagsteam-disjoint.so

linux64: linux64-unified linux64-disjoint
$(eval $(call build_platform,linux64))

clean-linux64:
	rm -rf $(PATH_LINUX64_OBJ) $(PATH_LINUX64_BUILD)/libagsteam.so $(PATH_LINUX64_BUILD)/libagsteam-disjoint.so

# Linux (32-bit)
LINUX32_CXX_FLAGS=$(LINUX64_CXX_FLAGS) -m32
LINUX32_STEAMWORKS_DIR=linux32
LINUX32_LIB_FLAGS=-shared -o $(PATH_LINUX32_BUILD)/libagsteam.so -m32
LINUX32_LIB_DISJOINT_FLAGS=-shared -o $(PATH_LINUX32_BUILD)/libagsteam-disjoint.so -m32

linux32: linux32-unified linux32-disjoint
$(eval $(call build_platform,linux32))

clean-linux32:
	rm -rf $(PATH_LINUX32_OBJ) $(PATH_LINUX32_BUILD)/libagsteam.so $(PATH_LINUX32_BUILD)/libagsteam-disjoint.so

# Linux (all)
linux: linux64 linux32

clean-linux: clean-linux64 clean-linux32
