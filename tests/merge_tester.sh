#!/bin/bash

duration=0

make clean
make
valgrind -s --leak-check=full ./bin/main init
sleep $duration
valgrind -s --leak-check=full ./bin/main branch test
sleep $duration
valgrind -s --leak-check=full ./bin/main checkout-branch test
sleep $duration
echo test >> tests/test.txt
valgrind -s --leak-check=full ./bin/main add tests/test.txt
sleep $duration
#valgrind -s --leak-check=full ./bin/main add tests/test2.txt
sleep $duration
valgrind -s --leak-check=full ./bin/main commit test -m commit_1_test
sleep $duration
valgrind -s --leak-check=full ./bin/main checkout-branch master
sleep $duration
#valgrind -s --leak-check=full ./bin/main add tests/test_wt.txt
sleep $duration
echo master >> tests/test.txt
sleep $duration
valgrind -s --leak-check=full ./bin/main add tests/test.txt
sleep $duration
valgrind -s --leak-check=full ./bin/main commit master -m commit_1_master
sleep $duration
valgrind -s --leak-check=full ./bin/main merge test