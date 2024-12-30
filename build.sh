git submodule init
git submodule update
cd build
cmake .. -G "MinGW Makefiles"
cmake --build .