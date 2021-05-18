sudo apt -y remove cmake
sudo apt -y upgrade
sudo apt -y install build-essential libssl-dev
cd ~
wget https://github.com/Kitware/CMake/releases/download/v3.17.5/cmake-3.17.5.tar.gz
tar -zxf cmake-3.17.5.tar.gz
cd cmake-3.17.5
./bootstrap --prefix=/usr --datadir=share/cmake --docdir=doc/cmake
make -j8
sudo make install
cmake --version
which cmake
echo 'install cmake complete'
