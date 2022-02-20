#!/bin/sh


OUT="outputs"
BUILD="build_tests"
TEST="test_graphs"
COUNT=0
EXECUTABLE="be"

rm -r "$BUILD"
rm -r "$OUT"
mkdir "$BUILD"
mkdir "$OUT"

cd "$BUILD"
cmake -DCMAKE_BUILD_TYPE=Debug ..
cmake --build .

mv "$EXECUTABLE" ..
cd ../"$TEST"
for file in *
do
    echo "$file"
    ./"$EXECUTABLE" "$file" "../$OUT/out_$file"
done;
