
# Copyright (C) 2016, Canonical Ltd.
# All rights reserved.

# This source code is licensed under the BSD-style license found in the
# LICENSE file in the root directory of this source tree. An additional grant
# of patent rights can be found in the PATENTS file in the same directory.

include_directories(runtime/src)
set (APP_NAME "ReactNativeApp")
set (JS_APP_NAME "ReactNativeApp")
add_subdirectory(application)
add_subdirectory(runtime)
add_subdirectory(tests)
