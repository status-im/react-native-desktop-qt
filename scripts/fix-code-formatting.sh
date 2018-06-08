#!/bin/bash

SOURCE_FILES=`find ./ReactQt/ -name \*.cpp -type f -or -name \*.h -type f`
for SOURCE_FILE in $SOURCE_FILES
do
  export FORMATTING_ISSUE_COUNT=`clang-format -output-replacements-xml $SOURCE_FILE | grep offset | wc -l`
  if [ "$FORMATTING_ISSUE_COUNT" -gt "0" ]; then
    clang-format -i $SOURCE_FILE
    echo "Formatting issues fixed for: $SOURCE_FILE"
  fi
done
