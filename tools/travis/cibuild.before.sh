sudo apt-get update
sudo apt-get -y upgrade
sudo apt-get install -y git cmake gcc build-essential
sudo apt-get -y install flex bison libpython2.7-dev qt5-default libqt5webkit5-dev

git clone https://github.com/BALL-Project/ball_contrib.git

pushd ball_contrib
git checkout tarball_revisited

mkdir build
pushd build
cmake .. -DPACKAGES="eigen3;sip;tbb;fftw;openbabel" -DTHREADS=4
make