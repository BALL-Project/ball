/* config.h.  Generated from config.h.in by configure.  */
// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id:$ 
//

#ifndef MALL_CONFIG_CONFIG_H
#define MALL_CONFIG_CONFIG_H

// Here are some global configuration flags for MALL

// This flag is used by GenericPDBFile
// if it is not set, the length of each line has
// to be meet the PDB spcifications exactly.
// As virtually no existing PDB file fulfills
// this requirement, we disencourage its usage.
// Nevertheless, it can be quite useful for debugging.
/* #undef MALL_STRICT_PDB_LINE_IMPORT */

// MALL_DEBUG enables some debugging methods
// change the DEBUG entry in configure to define this flag.
// If in debug mode, inline functions won't be compiled
// as inline by defining MALL_NO_INLINE_FUNCTIONS
// This facilitates debugging, as the debugger can always
// find the corresponding source code line.
// See also COMMON/debug.h for these symbols.
/* #undef MALL_DEBUG */
/* #undef MALL_NO_INLINE_FUNCTIONS */


// This define is used to determine
// wheter MALL shall try to close a socket conection
// via shutdown or close (see socket.C).
#define MALL_USE_SOCKET_SHUTDOWN

// This define is used in string.C and enables a workaround
// on on of these poor systems that do not define vsnprintf.
/* #undef MALL_HAVE_VSNPRINTF */

// The path to the directory where MALL is installed.
#define MALL_PATH "/media/sda7/MALL"

// The string describing the binary format.
#define MALL_BINFMT "Linux-i386-g++_4.1.2"

// Used in COMMON/namespace.h to cause the inclusion of COMMON/memory.h.
/* #undef MALL_LOG_MEMORY */
#define MALL_LOG_MEMORY_FILE "memory.log"

// Define architecture
/* #undef MALL_ARCH_SPARC */
/* #undef MALL_ARCH_MIPS */
#define MALL_ARCH_I386 I386
/* #undef MALL_ARCH_ALPHA */
/* #undef MALL_ARCH_POWERPC */

// Define OS
#define MALL_OS_LINUX LINUX
/* #undef MALL_OS_SOLARIS */
/* #undef MALL_OS_SUNOS */
/* #undef MALL_OS_IRIX */
/* #undef MALL_OS_IRIX32 */
/* #undef MALL_OS_IRIX64 */
/* #undef MALL_OS_FREEBSD */
/* #undef MALL_OS_DARWIN */

// Define compiler specifics (used in VERY few places only, e.g. COMMON/rtti.C).
// Microsoft Visual Studio .NET
/* #undef MALL_COMPILER_MSVC */
// GNU g++
#define MALL_COMPILER_GXX 
// Intel C++
/* #undef MALL_COMPILER_INTEL */
// SGI MipsPro
/* #undef MALL_COMPILER_MIPSPRO */
// SUN SunPro (Forte, Workshop)
/* #undef MALL_COMPILER_SUNPRO */
// Compaq cxx
/* #undef MALL_COMPILER_COMPAQ */
// Kuck & Associate (KAI) C++
/* #undef MALL_COMPILER_KAI */

// The compiler name.
#define MALL_COMPILER GXX

// Defines for the compiler version (major.minor.minor_minor)
#define MALL_COMPILER_VERSION_MAJOR 4
#define MALL_COMPILER_VERSION_MINOR 1
#define MALL_COMPILER_VERSION_MINOR_MINOR 2

// define if gethostname is supported
#define MALL_HAVE_GETHOSTNAME 1

// define if sysinfo(SI_HOSTNAME,) has to be used instead of
// gethostname (older Solaris variants)
/* #undef MALL_HAVE_SYSINFO */

// define if gethostname is not in the unistd.h header
/* #undef MALL_DEFINE_GETHOSTNAME */

// Define if you have the inet_aton function.  
// Note: inet_aton is a marginally improved version of inet_addr
// If configure finds inet_aton, HAVE_INET_ATON is set and used
// in SYSTEM/socket.C.
/* #undef MALL_HAVE_INET_ATON */

// Define the type needed for getsockname, getpeername, and accept
// This type is probably one of socklen_t, size_t, or int
// Refer to /usr/include/sys/socket.h in case of doubt
#define MALL_SOCKLEN_TYPE socklen_t

// define some symbols for the (bit)size of some builtin types
#define MALL_INT_SIZE 4
#define MALL_CHAR_SIZE 1
#define MALL_LONG_SIZE 4
#define MALL_SIZE_T_SIZE 4
#define MALL_POINTER_SIZE 4

// define symbols for the endianness of the system
#define MALL_LITTLE_ENDIAN true
/* #undef MALL_BIG_ENDIAN */

// the signature of the function arguments used in xdrrec_create differs
// from platform to platform, so we define some symbols describing the
// correct arguments: (void*, char*, int), (char*, char*, int), or (void)
// take arguments of type (void*, char*, int)
/* #undef MALL_XDRREC_CREATE_VOID_VOID_INT */
/* #undef MALL_XDRREC_CREATE_VOID_CHAR_INT */
#define MALL_XDRREC_CREATE_CHAR_CHAR_INT 1
/* #undef MALL_XDRREC_CREATE_VOID_VOID_UINT */
/* #undef MALL_XDRREC_CREATE_VOID */

// some platforms (e.g. Debian 2.1) do not provide xdr_u_hyper, so we 
// need a workaround for this
#define MALL_HAS_XDR_U_HYPER 1

// Define to `int' if <sys/types.h> doesn't define.  
/* #undef pid_t */

// Define to `unsigned' if <sys/types.h> doesn't define.  
/* #undef size_t */

// Define if you have the ANSI C header files.  
#define STDC_HEADERS 1

// Define if numeric_limits is defined in <limits>
/* #undef MALL_HAS_NUMERIC_LIMITS */

// Define the argument type for xdr_u_hyper (64 bit)
#define MALL_XDR_UINT64_TYPE u_quad_t

// Define on 64 bit architectures to avoid the use of long long
// which is required on 32 bit machines for comatibility
/* #undef MALL_64BIT_ARCHITECTURE */

// Define a signed/unsigned numeric type of 64 bit length (used for
// platform independent persistence: stores pointers)
// this usually defaults to unsigned long on 64 bit architectures
// and unsigned long long on 32 bit machines
#define MALL_ULONG64_TYPE unsigned long long
#define MALL_LONG64_TYPE long long

// Defines an unsigned type that has the same length as size_t
#define MALL_SIZE_TYPE unsigned int

// Defines a signed type that has the same length as size_t
#define MALL_INDEX_TYPE int

// Defines an unsigned integer type of the same size as void*
#define MALL_POINTERSIZEUINT_TYPE unsigned int

// Define the template argument for friends of template classes:
// either "" or "<>"
#define MALL_NULL_TEMPLATE_ARGS 

// Basically a work-around for the broken SUNPro CC (WS 6U2),
// which handles function template incorrectly -- see amberNonBonded.C
/* #undef MALL_MUST_CAST_TEMPLATE_FUNCTION_ARGS */

// Define whether the compiler provides an ANSI-compliant implementation
// of <iostream>. This is required in COMMON/socket.h/C since we need
// the correct initialisation of the base classes. These base classes are
// either something like std::basic_ios<char> etc., or ios, istream etc.
#define MALL_HAS_ANSI_IOSTREAM 1

// Define whether we can use <sstream> or whether we have to use the
// older <strstream>
#define MALL_HAS_SSTREAM 1

// Define whether inline functions may be used as template parameters.
#define MALL_HAS_INLINE_TPL_ARGS 1

// Define the precision for the MALL Complex type. This is mainly used
// to determine the FFTW default type
#define MALL_COMPLEX_PRECISION float

// Define whether the FFTW library/header is available.
/* #undef MALL_HAS_FFTW */
/* #undef MALL_HAS_FFTW_H */

// Define which versions of fftw can be used: double, float, long double
/* #undef MALL_HAS_FFTW_DOUBLE */
/* #undef MALL_HAS_FFTW_FLOAT */
/* #undef MALL_HAS_FFTW_LONG_DOUBLE */

// The default traits for the FFTW classes
#define MALL_FFTW_DEFAULT_TRAITS FloatTraits

// Define whether to use the ARM style or ANSI style access modifications
//   ARM style:   BaseClass::foo;
//   ANSI style:  using BaseClass::foo;
#define MALL_CFG_USING_METHOD_DIRECTIVE 1

// Maximum line length for reading from files (see source/FORMAT)
#define MALL_MAX_LINE_LENGTH 65535

// on some systems (e.g. Solaris) we need to include /usr/include/ieeefp.h
// for some floating point functions
/* #undef MALL_HAS_IEEEFP_H */

// on some systems (e.g. Solaris) we need to include /usr/include/values.h
// for some floating point functions
#define MALL_HAS_VALUES_H 

// on some systems, we do not have regex.h, so we have to use regexp.h
/* #undef MALL_HAS_REGEXP_H */
#define MALL_HAS_REGEX_H 

// Define whether float.h has to be included along with limits.h
#define MALL_HAS_FLOAT_H 1

#define MALL_HAS_UNISTD_H 
/* #undef MALL_HAS_PROCESS_H */
#define MALL_HAS_TIME_H 
#define MALL_HAS_LIMITS_H 
#define MALL_HAS_FLOAT_H 1
#define MALL_HAS_DIRENT_H 
/* #undef MALL_HAS_DIRECT_H */
#define MALL_HAS_PWD_H 
#define MALL_HAS_SYS_IOCTL_H 
#define MALL_HAS_SYS_TIME_H 
#define MALL_HAS_SYS_STAT_H 
#define MALL_HAS_SYS_TIMES_H 
#define MALL_HAS_SYS_TYPES_H 
#define MALL_HAS_SYS_PARAM_H 
#define MALL_HAS_SYS_SOCKET_H 
#define MALL_HAS_SYS_SYSINFO_H 

#define MALL_HAS_NETINET_IN_H 
#define MALL_HAS_NETDB_H 
#define MALL_HAS_ARPA_INET_H 

#define MALL_HAS_KILL 
#define MALL_HAS_SYSCONF 
#define MALL_HAS_HYPOT 

// Specific to the Windows port (and actually not used here,
// included for completeness, rather).
/* #undef MALL_HAS_CLOCKS_PER_SEC */
/* #undef MALL_HAS_WINDOWS_PERFORMANCE_COUNTER */
/* #undef MALL_USE_WINSOCK */


#define HAS_BLAS
#define USE_BLAS
#define HAS_LAPACK
#define USE_LAPACK 
//#define DEBUG


#endif // MALL_CONFIG_CONFIG_H
