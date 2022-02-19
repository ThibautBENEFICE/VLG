rm -r build

BUILD="build"

mkdir "$BUILD"
cd "$BUILD"
cmake -DCMAKE_BUILD_TYPE=Release ..
cmake --build .
