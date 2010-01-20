#ifndef MALL_GLOBAL_H
#define MALL_GLOBAL_H

#ifndef BALL_COMMON_GLOBAL_H
# include <BALL/COMMON/global.h>
#endif

#ifndef BALL_MATHS_COMPLEX_H
# include <BALL/MATHS/complex.h>
#endif


// rename BLAS-functions in order to match the ones within
// the lapack-cblas library
// TODO: adapt to ATLAS instead if it is used.

// BLAS1 routines 
// #define cblas_srotg f2c_srotg
// #define cblas_drotg f2c_drotg
// #define cblas_srotmg f2c_srotmg
// #define cblas_drotmg f2c_drotmg
// #define cblas_srot f2c_srot
// #define cblas_drot f2c_drot
// #define cblas_srotm f2c_srotm
// #define cblas_drotm f2c_drotm
// #define cblas_sswap f2c_sswap
// #define cblas_dswap f2c_dswap
// #define cblas_cswap f2c_cswap
// #define cblas_zswap f2c_zswap
// #define cblas_sscal f2c_sscal
// #define cblas_dscal f2c_dscal
// #define cblas_cscal f2c_cscal
// #define cblas_zscal f2c_zscal
// #define cblas_csscal f2c_csscal
// #define cblas_zdscal f2c_zdscal
// #define cblas_scopy f2c_scopy
// #define cblas_dcopy f2c_dcopy
// #define cblas_ccopy f2c_ccopy
// #define cblas_zcopy f2c_zcopy
// #define cblas_saxpy f2c_saxpy
// #define cblas_daxpy f2c_daxpy
// #define cblas_caxpy f2c_caxpy
// #define cblas_zaxpy f2c_zaxpy
// #define cblas_sdot f2c_sdot
// #define cblas_ddot f2c_ddot
// #define cblas_cdotu f2c_cdotu
// #define cblas_zdotu f2c_zdotu
// #define cblas_cdotc f2c_cdotc
// #define cblas_zdotc f2c_zdotc
// #define cblas_snrm2 f2c_snrm2
// #define cblas_dnrm2 f2c_dnrm2
// #define cblas_scnrm2 f2c_scnrm2
// #define cblas_dznrm2 f2c_dznrm2
// #define cblas_sasum f2c_sasum
// #define cblas_dasum f2c_dasum
// #define cblas_scasum f2c_scasum
// #define cblas_dzasum f2c_dzasum
// #define cblas_isamax f2c_isamax
// #define cblas_idamax f2c_idamax
// #define cblas_icamax f2c_icamax
// #define cblas_izamax f2c_izamax
//  
// // BLAS2 routines 
// #define cblas_sgemv f2c_sgemv
// #define cblas_dgemv f2c_dgemv
// #define cblas_cgemv f2c_cgemv
// #define cblas_zgemv f2c_zgemv
// #define cblas_sgbmv f2c_sgbmv
// #define cblas_dgbmv f2c_dgbmv
// #define cblas_cgbmv f2c_cgbmv
// #define cblas_zgbmv f2c_zgbmv
// #define cblas_chemv f2c_chemv
// #define cblas_zhemv f2c_zhemv
// #define cblas_chbmv f2c_chbmv
// #define cblas_zhbmv f2c_zhbmv
// #define cblas_chpmv f2c_chpmv
// #define cblas_zhpmv f2c_zhpmv
// #define cblas_ssymv f2c_ssymv
// #define cblas_dsymv f2c_dsymv
// #define cblas_ssbmv f2c_ssbmv
// #define cblas_dsbmv f2c_dsbmv
// #define cblas_sspmv f2c_sspmv
// #define cblas_dspmv f2c_dspmv
// #define cblas_strmv f2c_strmv
// #define cblas_dtrmv f2c_dtrmv
// #define cblas_ctrmv f2c_ctrmv
// #define cblas_ztrmv f2c_ztrmv
// #define cblas_stbmv f2c_stbmv
// #define cblas_dtbmv f2c_dtbmv
// #define cblas_ctbmv f2c_ctbmv
// #define cblas_ztbmv f2c_ztbmv
// #define cblas_stpmv f2c_stpmv
// #define cblas_dtpmv f2c_dtpmv
// #define cblas_ctpmv f2c_ctpmv
// #define cblas_ztpmv f2c_ztpmv
// #define cblas_strsv f2c_strsv
// #define cblas_dtrsv f2c_dtrsv
// #define cblas_ctrsv f2c_ctrsv
// #define cblas_ztrsv f2c_ztrsv
// #define cblas_stbsv f2c_stbsv
// #define cblas_dtbsv f2c_dtbsv
// #define cblas_ctbsv f2c_ctbsv
// #define cblas_ztbsv f2c_ztbsv
// #define cblas_stpsv f2c_stpsv
// #define cblas_dtpsv f2c_dtpsv
// #define cblas_ctpsv f2c_ctpsv
// #define cblas_ztpsv f2c_ztpsv
// #define cblas_sger f2c_sger
// #define cblas_dger f2c_dger
// #define cblas_cgeru f2c_cgeru
// #define cblas_zgeru f2c_zgeru
// #define cblas_cgerc f2c_cgerc
// #define cblas_zgerc f2c_zgerc
// #define cblas_cher f2c_cher
// #define cblas_zher f2c_zher
// #define cblas_chpr f2c_chpr
// #define cblas_zhpr f2c_zhpr
// #define cblas_cher2 f2c_cher2
// #define cblas_zher2 f2c_zher2
// #define cblas_chpr2 f2c_chpr2
// #define cblas_zhpr2 f2c_zhpr2
// #define cblas_ssyr f2c_ssyr
// #define cblas_dsyr f2c_dsyr
// #define cblas_sspr f2c_sspr
// #define cblas_dspr f2c_dspr
// #define cblas_ssyr2 f2c_ssyr2
// #define cblas_dsyr2 f2c_dsyr2
// #define cblas_sspr2 f2c_sspr2
// #define cblas_dspr2 f2c_dspr2
//  
// // BLAS3 routines
// #define cblas_sgemm f2c_sgemm
// #define cblas_dgemm f2c_dgemm
// #define cblas_cgemm f2c_cgemm
// #define cblas_zgemm f2c_zgemm
// #define cblas_ssymm f2c_ssymm
// #define cblas_dsymm f2c_dsymm
// #define cblas_csymm f2c_csymm
// #define cblas_zsymm f2c_zsymm
// #define cblas_chemm f2c_chemm
// #define cblas_zhemm f2c_zhemm
// #define cblas_ssyrk f2c_ssyrk
// #define cblas_dsyrk f2c_dsyrk
// #define cblas_csyrk f2c_csyrk
// #define cblas_zsyrk f2c_zsyrk
// #define cblas_cherk f2c_cherk
// #define cblas_zherk f2c_zherk
// #define cblas_ssyr2k f2c_ssyr2k
// #define cblas_dsyr2k f2c_dsyr2k
// #define cblas_csyr2k f2c_csyr2k
// #define cblas_zsyr2k f2c_zsyr2k
// #define cblas_cher2k f2c_cher2k
// #define cblas_zher2k f2c_zher2k
// #define cblas_strmm f2c_strmm
// #define cblas_dtrmm f2c_dtrmm
// #define cblas_ctrmm f2c_ctrmm
// #define cblas_ztrmm f2c_ztrmm
// #define cblas_strsm f2c_strsm
// #define cblas_dtrsm f2c_dtrsm
// #define cblas_ctrsm f2c_ctrsm
// #define cblas_ztrsm f2c_ztrsm


#include "cblas.h"
#define BLAS_PREFIX_ cblas_



#endif
