#!/bin/sh


OUT="outputs"
BUILD="build_tests"
TEST="test_graphs"
COUNT=0

rm -r "$BUILD"
rm -r "$OUT"
mkdir "$BUILD"
mkdir "$OUT"

cd "$BUILD"
cmake -DCMAKE_BUILD_TYPE=Debug ..
cmake --build .

cd ../"$TEST"
for file in *
do
    echo "$file"
    ./../"$BUILD"/be "$file" "../$OUT/out$file.txt"
done;

cd "../$OUT"
for file in *
do
    echo "$file"
    diff "$file" "../""$OUT""_old/$file"
done
