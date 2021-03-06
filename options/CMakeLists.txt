################################################################################
#                  Copyright 2010 Joel Falcou - Thomas Heller
#
#          Distributed under the Boost Software License, Version 1.0.
#                 See accompanying file LICENSE.txt or copy at
#                     http://www.boost.org/LICENSE_1_0.txt
################################################################################
cmake_minimum_required(VERSION 2.8)

project(boost_options)

################################################################################
# Set compile Flags
################################################################################
IF(MSVC)
ENDIF(MSVC)

IF(CMAKE_COMPILER_IS_GNUCXX) # set appropriate compiler flags
 SET(CMAKE_CXX_FLAGS "-Wall -Wextra")
ENDIF(CMAKE_COMPILER_IS_GNUCXX)

################################################################################
# Global Options
################################################################################
INCLUDE_DIRECTORIES( ${PROJECT_SOURCE_DIR} )
INCLUDE_DIRECTORIES( ${PROJECT_BINARY_DIR} )

SET(Boost_ADDITIONAL_VERSIONS "1.44" "1.44.0")
INCLUDE(FindBoost)
FIND_PACKAGE( Boost 1.44.0 )

IF(Boost_FOUND)
 LINK_DIRECTORIES(${Boost_LIBRARY_DIRS})
 INCLUDE_DIRECTORIES(${Boost_INCLUDE_DIRS})
ELSE()
	MESSAGE(ERROR "Boost >=1.44 not found")
ENDIF()

################################################################################
# Add Unit Tests
################################################################################
ENABLE_TESTING()
ADD_SUBDIRECTORY(test)

