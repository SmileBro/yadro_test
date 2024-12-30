## Building
Initialize googletest submodule:
- `git submodule init`
- `git submodule update`

Run those commands from `build` folder

On Windows MinGW:
- `cmake .. -G "MinGW Makefiles"`
- `cmake --build .`

Or using bash run:
- `bash build.sh`

On Linux:
- `cmake ..`
- `cmake --build .`

### Building without tests
If do not want to init googletest submodule 
you can build without tests.

On Windows MinGW:
- `cmake .. -DBUILD_TESTS=OFF -G "MinGW Makefiles"`
- `cmake --build .`

On Linux:
- `cmake .. -DBUILD_TESTS=OFF`
- `cmake --build .`

## Testing
After building in `bin` folder you can run:
- main.exe - main program 
    - run with `main.exe ..\test_file.txt`
- calc_profit_test.exe - tests of main program and calc_profit_lib
    - run with `calc_profit_test.exe`
- format_test.exe - tests of check_format_lib
    - run with `format_test.exe` 