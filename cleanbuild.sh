#! /bin/zsh

rm -rf bin build
cmake -DCMAKE_BUILD_TYPE=Release -B build -S .
cmake --build build --config Release -j4
echo "Run with \"$(find . -name cstart)\""

