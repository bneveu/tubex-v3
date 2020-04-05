# ==================================================================
#  tubex-lib - build script
# ==================================================================

#!/bin/bash

# Possible commands:
#  ./build.sh
#  ./build.sh all
#  ./build.sh tests
#  ./build.sh examples
#  ./build.sh clean
#  ./build.sh all clean
#  ./build.sh tests clean
#  ./build.sh examples clean

TUBEX_DIR="$(pwd)"


if [ -z "${WITH_TUBE_TREE}" ]
then
  # in case of undefined WITH_TUBE_TREE variable
  WITH_TUBE_TREE=OFF
fi

# Cleaning before build

  if [ $# -ne 0 ] && ([ "$1" = "clean" ] || [ "$2" = "clean" ])
  then
    find . -name make | xargs rm -fr
  fi

# Building Tubex library

  mkdir make -p
  cd make

  # Possibly with tests

  if [ $# -ne 0 ] && ([ "$1" = "tests" ] || [ "$1" = "all" ])
  then
    cmake -DBUILD_TESTS=ON -DWITH_TUBE_TREE="${WITH_TUBE_TREE}" ..
  else
    cmake -DBUILD_TESTS=OFF -DWITH_TUBE_TREE="${WITH_TUBE_TREE}" ..
  fi

  make
  cd ..

# Building examples independently

  if [ $# -ne 0 ] && ([ "$1" = "examples" ] || [ "$1" = "all" ])
  then
    cd make
    sudo make install
    cd ..
    cd examples/basics
    find . -name "ex\_*" | xargs -L 1 bash -c 'cd "$0" && ./build.sh && cd ..'
    cd ../..
    cd examples/robotics
    find . -name "ex\_*" | xargs -L 1 bash -c 'cd "$0" && ./build.sh && cd ..'
    cd ../..
  fi