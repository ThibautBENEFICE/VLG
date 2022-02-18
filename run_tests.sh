#!/bin/sh


OUT="outputs"
BUILD="build"
TEST="test_graphs"
COUNT=0

rm -r "$BUILD"
rm -r "$OUT"
mkdir "$BUILD"
mkdir "$OUT"

cd "$BUILD"
cmake ..
cmake --build .

cd ../"$TEST"
for file in *
do
    echo "$file"
    ./../"$BUILD"/be "$file" "../$OUT/out$file.txt"
    echo ""
done;
