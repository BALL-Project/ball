git clone https://github.com/BALL-Project/ball_contrib.git
git checkout tarball_revisited

pushd build
cmake .. -DPACKAGES="eigen3;sip;tbb;fftw3;openbabel" -DTHREADS=4
make


