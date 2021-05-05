#!/bin/bash
# files=$(find "main/" "components/" -iregex '.*\.\(cpp\|cu\|hh\|h\|cc\|hpp\|hxx\|h++\|cuh\|cxx\|c++\|c\)$')

printf "cpplint\n"
cpplint --recursive --root=. --filter=-build/include_subdir,-legal/copyright main/ components/

printf "\ncppcheck\n"
cppcheck --enable=all --inline-suppr --suppress=missingInclude main/ components/ --error-exitcode=1
