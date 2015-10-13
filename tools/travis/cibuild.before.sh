git clone https://github.com/BALL-Project/ball_contrib.git

pushd ball_contrib
git checkout tarball_revisited

mkdir build
pushd build
cmake .. -DPACKAGES="eigen3;sip;tbb;fftw;openbabel" -DTHREADS=4
make