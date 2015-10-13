git clone https://github.com/BALL-Project/ball.git

pushd ball
git checkout qt5_150827

mkdir build
pushd build
cmake ..
make