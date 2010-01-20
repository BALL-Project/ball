#ifndef LAPACK_H
#define LAPACK_H
// eigenvalues / eigenvectors
extern void sgeev_(const char *, const char *, const int *, float *, const int *, 
									 float *, float *, float *, const int *, float *, const int *, 
									 float *, int *, int *);
extern void dgeev_(const char *, const char *, const int *, double *, const int *, 
									 double *, double *, double *, const int *, double *, const int *, 
									 double *, int *, int *);
extern void cgeev_(const char *, const char *, const int *, std::complex<float> *, const int *, 
									 std::complex<float>*, std::complex<float> *, const int *, std::complex<float> *, 
									 const int *, std::complex<float> *, int *, float*, int *);
extern void zgeev_(const char *, const char *, const int *, std::complex<double> *, const int *, 
									 std::complex<double>*, std::complex<double> *, const int *, std::complex<double> *, 
									 const int *, std::complex<double> *, int *, double*, int *);
// symmetric eigenvalues / eigenvectors
extern void sspevd_(const char *, const char *, const int *, float *, float *, float *,
										const int *, float *, const int *, int *, const int *, int *); 
extern void dspevd_(const char *, const char *, const int *, double *, double *, double *,
										const int *, double *, const int *, int *, const int *, int *); 
// SVD
extern void sgesdd_(const char *, const int *, const int *, float *, const int *,
										float *, float *, const int *, float *, const int *, float *,
										const int *, int *, int *);
extern void dgesdd_(const char *, const int *, const int *, double *, const int *,
										double *, double *, const int *, double *, const int *, double *,
										const int *, int *, int *);
extern void cgesdd_(const char *, const int *, const int *, std::complex<float> *, const int *,
										float *, std::complex<float> *, const int *, std::complex<float> *, const int *, 
										std::complex<float> *, const int *, float*, int *, int *);
extern void zgesdd_(const char *, const int *, const int *, std::complex<double> *, const int *,
										double *, std::complex<double> *, const int *, std::complex<double> *, const int *, 
										std::complex<double> *, const int *, double*, int *, int *);
#endif
