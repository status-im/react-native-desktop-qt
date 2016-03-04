# vim:expandtab:shiftwidth=2:tabstop=2:

# Copyright (C) 2016 Canonical Ltd.

# This library is free software; you can redistribute it and/or
# modify it under the terms of the GNU Lesser General Public
# License as published by the Free Software Foundation; either
# version 2.1 of the License, or (at your option) any later version.

# This library is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
# Lesser General Public License for more details.

# You should have received a copy of the GNU Lesser General Public
# License along with this library; if not, write to the Free Software
# Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -std=c11")

find_package(Qt5Core REQUIRED)
find_package(Qt5Qml REQUIRED)
find_package(Qt5Quick REQUIRED)

add_library(
  react-native MODULE

  reactplugin.cpp
  reactvaluecoercion.cpp
  reactbridge.cpp
  reactevents.cpp
  reactsourcecode.cpp
  reactcomponentdata.cpp
  reactmoduledata.cpp
  reactmodulemethod.cpp
  reactpropertyhandler.cpp
  reactexecutor.cpp
  reactnetexecutor.cpp
  reactpipeexecutor.cpp
  reactnetworking.cpp
  reacttiming.cpp
  reactappstate.cpp
  reactasynclocalstorage.cpp
  reactitem.cpp
  reactview.cpp
  reactviewmanager.cpp
  reactrawtextmanager.cpp
  reacttextmanager.cpp
  reactimagemanager.cpp
  reactattachedproperties.cpp
  reactflexlayout.cpp
  reacttextproperties.cpp
  reactrawtextproperties.cpp
  reactuimanager.cpp
  ubuntucomponentsloader.cpp
  ubuntucomponentmodule.cpp
  ubuntuscrollviewmanager.cpp
  ubuntunavigatormanager.cpp
  ubuntupagemanager.cpp
  ubuntutextfieldmanager.cpp
  ubuntudatepickermanager.cpp
  ../../../React/Layout/Layout.c
)

set_target_properties(
  react-native
  PROPERTIES LIBRARY_OUTPUT_DIRECTORY React
 )

qt5_use_modules(react-native Core Qml Quick)

add_custom_target(
  copy-qmldir ALL
  COMMAND cp ${CMAKE_CURRENT_SOURCE_DIR}/qmldir ${CMAKE_CURRENT_BINARY_DIR}/React
 )
