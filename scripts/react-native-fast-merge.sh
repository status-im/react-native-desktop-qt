#!/usr/bin/env bash

# Copyright (c) 2017-present, Status Research and Development GmbH.
# All rights reserved.

# This source code is licensed under the BSD-style license found in the
# LICENSE file in the root directory of this source tree. An additional grant
# of patent rights can be found in the PATENTS file in the same directory.

# Example of usage:
#
# Make sure to add to git a remote repo from where merge will be done ( git remote add facebook git@github.com:facebook/react-native.git )
# Fetch latest info from remotes: git fetch --all
# From project root dir run fast merge script with passing remote repo and branch names to do merge from: ./scripts/react-native-fast-merge.sh facebook/0.56-stable

allowedConflictAutoResolve () {
  local restrictedFiles conflicted_file="$1"
  shift
  for restrictedFiles; do [[ "$restrictedFiles" == "$conflicted_file" ]] && return 0; done
  return 1
}

BRANCH_TO_MERGE_FROM="$1"
SCRIPTS_DIR=$(dirname "$0")

echo $BRANCH_TO_MERGE_FROM

if [[ ! $(git merge ${BRANCH_TO_MERGE_FROM}) ]]; then  
  echo "Can't merge from $BRANCH_TO_MERGE_FROM"
  exit 1
fi

# Read the list of files into DESKTOP_MODIFIED_FILES to ignore automatic merge for
IFS=$'\r\n' GLOBIGNORE='*' command eval 'DESKTOP_MODIFIED_FILES=($(cat $SCRIPTS_DIR/desktop-modified-files.txt))'

# Iterate over files with merge conflicts
for i in "${DESKTOP_MODIFIED_FILES[@]}"
do
  echo $i
done

# Iterate over files with merge conflicts
git diff --name-only --diff-filter=U | while read conflicted_file; do
 echo "Checking file: '$conflicted_file'"
 allowedConflictAutoResolve "$conflicted_file" "${DESKTOP_MODIFIED_FILES[@]}"
 if [[ $? == 1 ]]; then  
   echo "Going to resolve conflic automatically as theirs for file: $conflicted_file"
   git checkout --theirs "$conflicted_file"
   git add "$conflicted_file"
 fi
done

