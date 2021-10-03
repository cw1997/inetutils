# inetutils
[![CMake](https://github.com/cw1997/inetutils/actions/workflows/cmake.yml/badge.svg)](https://github.com/cw1997/inetutils/actions/workflows/cmake.yml)

Some network utils including 'ping' and 'traceroute'

# build
This project is building by `CMake`, so you need to install `CMake` first.

To install CMake, you can run this shell script below:

    ./install_cmake.sh

Note!! This script will remove the already exists CMake.

Then you can run this shell script below for building the source code:

    ./build.sh
    
Finally you can see two new directories that name is `bin` and `build`

# test

    cd ./bin
    sudo ./ping
    sudo ./teaceroute
    
# documentation
For more information about usage, you can see the `./doc/documentation.docx`
