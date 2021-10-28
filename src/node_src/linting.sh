#!/bin/bash
# files=$(find "main/" "components/" -iregex '.*\.\(cpp\|cu\|hh\|h\|cc\|hpp\|hxx\|h++\|cuh\|cxx\|c++\|c\)$')

# Automatically install packages if they aren't already
if ! command -v cpplint &> /dev/null; then
    echo "Installing cpplint..."
    sudo pip install cpplint
fi
if ! command -v cppcheck &> /dev/null; then
    echo "Installing cppcheck..."
    sudo apt-get install cppcheck
fi

printf "\n\n"

printf "cpplint\n"
cpplint --recursive --root=. --filter=-build/include_subdir,-legal/copyright --exclude=build/ --exclude=test/build/ --exclude=components/esp32-camera .

printf "\ncppcheck\n"
cppcheck --enable=all --inline-suppr --suppress=missingInclude -i components/esp32-camera/ -i build/ -i test/build --error-exitcode=1 .
