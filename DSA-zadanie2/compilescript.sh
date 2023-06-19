#!/bin/bash

cp /mnt/d/VSCode/C++/DSA-zadanie2/bdd.h /mnt/d/VSCode/C++/DSA-zadanie2/wsl-testing/
cp /mnt/d/VSCode/C++/DSA-zadanie2/test.cpp /mnt/d/VSCode/C++/DSA-zadanie2/wsl-testing/
cp /mnt/d/VSCode/C++/DSA-zadanie2/expressions-DNF.txt /mnt/d/VSCode/C++/DSA-zadanie2/wsl-testing/
cp /mnt/d/VSCode/C++/DSA-zadanie2/expression-generator.cpp /mnt/d/VSCode/C++/DSA-zadanie2/wsl-testing/

#hardcore
g++ -g -Wall -Wformat -Wextra -Wno-unused-parameter -Wno-unused-variable -Wno-unused-function -Wno-missing-field-initializers /mnt/d/VSCode/C++/DSA-zadanie2/wsl-testing/test.cpp -o /mnt/d/VSCode/C++/DSA-zadanie2/wsl-testing/test
g++ -g -Wall -Wformat -Wextra -Wno-unused-parameter -Wno-unused-variable -Wno-unused-function -Wno-missing-field-initializers /mnt/d/VSCode/C++/DSA-zadanie2/wsl-testing/test.cpp

#soft
#g++ /mnt/d/VSCode/C++/DSA-zadanie2/wsl-testing/test.cpp -o /mnt/d/VSCode/C++/DSA-zadanie2/wsl-testing/test
#g++ /mnt/d/VSCode/C++/DSA-zadanie2/wsl-testing/test.cpp -g

#hardcore
valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes -s /mnt/d/VSCode/C++/DSA-zadanie2/wsl-testing/test

#soft
#valgrind --leak-check=full /mnt/d/VSCode/C++/DSA-zadanie2/wsl-testing/test
