#!/bin/bash

# C++ compilation properties
STD=c++11

# Colors
COLOR_RED="\033[31m"
COLOR_GREEN="\033[32m"
COLOR_YELLOW="\033[33m"
COLOR_CYAN="\033[36m"
COLOR_GRAY="\033[38;2;100;100;100m"
NO_COLOR="\033[0m"

EXIT_CODE=0

# Tests starting time
tests_start_time=`date +%s%N`
echo -e "Started testing on $(date +%Y-%m-%dT%H:%M:%S%z)\n"

if [ -z "$CXX" ];
then
    CXX=g++
fi

success=0
failed_tests=()

for cppfile in $(find * -type f -name 'test_*.cpp');
do
    echo -e "${COLOR_CYAN}›${NO_COLOR} Running... [${COLOR_CYAN}$cppfile${NO_COLOR}]"
    start_time=`date +%s%N`

    $CXX -std=$STD -I . -I ../ -fsanitize=undefined -fno-sanitize-recover $cppfile -o $cppfile.out
    COMPILATION=$?
    if [ $COMPILATION -eq 0 ];
    then
        ./$cppfile.out
        test_success=$?
        end_time=`date +%s%N`
        if [ $test_success -eq 0 ];
        then
            success=$((success + 1))
            echo -en "  ${COLOR_GREEN}› PASSED${NO_COLOR}"
        else
            echo -en "  ${COLOR_RED}› FAILED${NO_COLOR}"
            failed_tests+=($cppfile)
        fi
        echo -e " ${COLOR_GRAY}($(((end_time - $start_time)/1000000))ms)${NO_COLOR}"

        rm $cppfile.out
    else
        echo -e "  ${COLOR_YELLOW}"Compilation error on $cppfile!"${NO_COLOR}"
        failed_tests+=($cppfile)
    fi
done

tests_end_time=`date +%s%N`
time_taken=$(echo -e "$tests_start_time $tests_end_time" | awk '{printf "%.2f\n", ($2-$1)/1000000000.0}')

if [ ${#failed_tests[@]} -eq 0 ];
then
    echo -e "\n${COLOR_GREEN}$success PASSED${NO_COLOR} in ${time_taken}s"
else
    echo -e "${COLOR_RED}\n${#failed_tests[@]} FAILED, ${COLOR_GREEN}$success PASSED${NO_COLOR} in ${time_taken}s\n"
    echo -e "Failed tests:"
    for cppfile in "${failed_tests[@]}"
    do
        echo -e "  › $cppfile"
    done
    EXIT_CODE=1
fi

echo -e "\nFinished testing on $(date +%Y-%m-%dT%H:%M:%S%z)"
exit $EXIT_CODE
