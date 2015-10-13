git clone https://github.com/BALL-Project/ball.git

pushd ball
git checkout qt5_150827

cmake --version
which cmake

mkdir build
pushd build
cmake ..
make