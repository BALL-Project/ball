# 1 "vector4.h"
# 1 "/opt/gnu/5.7/lib/gcc-lib/sparc-sun-solaris2.7/2.95.2/include/math.h" 1 3
# 1 "/usr/include/math.h" 1 3
#pragma ident "@(#)math.h 2.7 98/01/27"
typedef union _h_val {
   unsigned long _i[sizeof(double) / sizeof(unsigned long)];
 double _d;
} _h_val;
extern const _h_val __huge_val;
extern int signgam;
enum version {libm_ieee = -1, c_issue_4, ansi_1, strict_ansi};
extern const enum version _lib_version;
struct exception {
 int type;
 char *name;
 double arg1;
 double arg2;
 double retval;
};
extern double acos  (double)  ;
extern double asin  (double)  ;
extern double atan  (double)  ;
extern double atan2  (double, double)  ;
extern double cos  (double)  ;
extern double sin  (double)  ;
extern double tan  (double)  ;
extern double cosh  (double)  ;
extern double sinh  (double)  ;
extern double tanh  (double)  ;
extern double exp  (double)  ;
extern double frexp  (double, int *)  ;
extern double ldexp  (double, int)  ;
extern double log  (double)  ;
extern double log10  (double)  ;
extern double modf  (double, double *)  ;
extern double pow  (double, double)  ;
extern double sqrt  (double)  ;
extern double ceil  (double)  ;
extern double fabs  (double)  ;
extern double floor  (double)  ;
extern double fmod  (double, double)  ;
extern double erf  (double)  ;
extern double erfc  (double)  ;
extern double gamma  (double)  ;
extern double hypot  (double, double)  ;
extern int isnan  (double)  ;
extern double j0  (double)  ;
extern double j1  (double)  ;
extern double jn  (int, double)  ;
extern double lgamma  (double)  ;
extern double y0  (double)  ;
extern double y1  (double)  ;
extern double yn  (int, double)  ;
extern double acosh  (double)  ;
extern double asinh  (double)  ;
extern double atanh  (double)  ;
extern double cbrt  (double)  ;
extern double logb  (double)  ;
extern double nextafter  (double, double)  ;
extern double remainder  (double, double)  ;
extern double scalb  (double, double)  ;
extern double expm1  (double)  ;
extern int ilogb  (double)  ;
extern double log1p  (double)  ;
extern double rint  (double)  ;
extern int matherr  (struct exception *)  ;
extern double significand  (double)  ;
extern double copysign  (double, double)  ;
extern double scalbn  (double, int)  ;
extern float modff  (float, float *)  ;
# 1 "/usr/include/floatingpoint.h" 1 3
#pragma ident "@(#)floatingpoint.h 2.4 94/06/09"
# 1 "/usr/include/stdio.h" 1 3
#pragma ident "@(#)stdio.h 1.69 98/07/13 SMI" 
# 1 "/usr/include/sys/feature_tests.h" 1 3
#pragma ident "@(#)feature_tests.h 1.17 97/12/04 SMI"
# 1 "/usr/include/sys/isa_defs.h" 1 3
#pragma ident "@(#)isa_defs.h 1.15 97/11/22 SMI"
# 211 "/usr/include/sys/isa_defs.h" 3
# 309 "/usr/include/sys/isa_defs.h" 3
# 15 "/usr/include/sys/feature_tests.h" 2 3
# 22 "/usr/include/stdio.h" 2 3
# 1 "/usr/include/sys/va_list.h" 1 3
#pragma ident "@(#)va_list.h 1.11 97/11/22 SMI"
typedef void *__va_list;
# 23 "/usr/include/stdio.h" 2 3
# 1 "/usr/include/stdio_tag.h" 1 3
#pragma ident "@(#)stdio_tag.h 1.3 98/04/20 SMI"
typedef struct __FILE  __FILE;
# 24 "/usr/include/stdio.h" 2 3
# 1 "/usr/include/stdio_impl.h" 1 3
#pragma ident "@(#)stdio_impl.h 1.7 98/04/17 SMI"
typedef int ssize_t;   
# 36 "/usr/include/stdio_impl.h" 3
struct __FILE   
{
 ssize_t  _cnt;  
 unsigned char *_ptr;  
 unsigned char *_base;  
 unsigned char _flag;  
 unsigned char _file;  
 unsigned __orientation:2;  
 unsigned __filler:6;
};
# 25 "/usr/include/stdio.h" 2 3
typedef __FILE FILE;
typedef unsigned int size_t;   
typedef long long __longlong_t;
typedef long  off_t;
typedef __longlong_t off64_t;
typedef long  fpos_t;
typedef __longlong_t fpos64_t;
extern FILE __iob[20 ];
extern unsigned char  _sibuf[], _sobuf[];
# 227 "/usr/include/stdio.h" 3
extern unsigned char *_bufendtab[];
extern FILE  *_lastbuf;
# 257 "/usr/include/stdio.h" 3
extern int remove(const char *);
extern int rename(const char *, const char *);
extern FILE *tmpfile(void);
extern char *tmpnam(char *);
extern int fclose(FILE *);
extern int fflush(FILE *);
extern FILE *fopen(const char *, const char *);
extern FILE *freopen(const char *, const char *, FILE *);
extern void setbuf(FILE *, char *);
extern void setbuffer(FILE *, char *, size_t);
extern int setlinebuf(FILE *);
extern int setvbuf(FILE *, char *, int, size_t);
extern int fprintf(FILE *, const char *, ...);
extern int fscanf(FILE *, const char *, ...);
extern int printf(const char *, ...);
extern int scanf(const char *, ...);
extern int snprintf(char *, size_t, const char *, ...);
extern int sprintf(char *, const char *, ...);
extern int sscanf(const char *, const char *, ...);
extern int vfprintf(FILE *, const char *, __va_list);
extern int vprintf(const char *, __va_list);
extern int vsnprintf(char *, size_t, const char *, __va_list);
extern int vsprintf(char *, const char *, __va_list);
extern int fgetc(FILE *);
extern char *fgets(char *, int, FILE *);
extern int fputc(int, FILE *);
extern int fputs(const char *, FILE *);
extern int getc(FILE *);
extern int getchar(void);
extern char *gets(char *);
extern int putc(int, FILE *);
extern int putchar(int);
extern int puts(const char *);
extern int ungetc(int, FILE *);
extern size_t fread(void *, size_t, size_t, FILE *);
extern size_t fwrite(const void *, size_t, size_t, FILE *);
extern int fgetpos(FILE *, fpos_t *);
extern int fseek(FILE *, long, int);
extern int fsetpos(FILE *, const fpos_t *);
extern long ftell(FILE *);
extern void rewind(FILE *);
extern void clearerr(FILE *);
extern int feof(FILE *);
extern int ferror(FILE *);
extern void perror(const char *);
extern int __filbuf(FILE *);
extern int __flsbuf(int, FILE *);
extern FILE *fdopen(int, const char *);
extern char *ctermid(char *);
extern int fileno(FILE *);
# 358 "/usr/include/stdio.h" 3
extern FILE *popen(const char *, const char *);
extern char *cuserid(char *);
extern char *tempnam(const char *, const char *);
extern int getopt(int, char *const *, const char *);
extern int getsubopt(char **, char *const *, char **);
extern char *optarg;
extern int optind, opterr, optopt;
extern int getw(FILE *);
extern int putw(int, FILE *);
extern int pclose(FILE *);
extern int fseeko(FILE *, off_t, int);
extern off_t ftello(FILE *);
extern FILE *fopen64(const char *, const char *);
extern FILE *freopen64(const char *, const char *, FILE *);
extern FILE *tmpfile64(void);
extern int fgetpos64(FILE *, fpos64_t *);
extern int fsetpos64(FILE *, const fpos64_t *);
extern int fseeko64(FILE *, off64_t, int);
extern off64_t ftello64(FILE *);
# 515 "/usr/include/stdio.h" 3
# 567 "/usr/include/stdio.h" 3
# 33 "/usr/include/floatingpoint.h" 2 3
# 1 "/usr/include/sys/ieeefp.h" 1 3
#pragma ident "@(#)ieeefp.h 2.7 94/11/09"
enum fp_direction_type {  
 fp_nearest = 0,
 fp_tozero = 1,
 fp_positive = 2,
 fp_negative = 3
};
enum fp_precision_type {  
 fp_extended = 0,
 fp_single = 1,
 fp_double = 2,
 fp_precision_3 = 3
};
enum fp_exception_type {  
 fp_inexact = 0,
 fp_division = 1,
 fp_underflow = 2,
 fp_overflow = 3,
 fp_invalid = 4
};
enum fp_trap_enable_type {  
 fp_trap_inexact = 0,
 fp_trap_division = 1,
 fp_trap_underflow = 2,
 fp_trap_overflow = 3,
 fp_trap_invalid = 4
};
# 81 "/usr/include/sys/ieeefp.h" 3
# 122 "/usr/include/sys/ieeefp.h" 3
enum fp_class_type {   
 fp_zero  = 0,
 fp_subnormal = 1,
 fp_normal = 2,
 fp_infinity    = 3,
 fp_quiet = 4,
 fp_signaling = 5
};
# 35 "/usr/include/floatingpoint.h" 2 3
typedef int sigfpe_code_type;  
typedef void (*sigfpe_handler_type)();  
extern sigfpe_handler_type sigfpe  (sigfpe_code_type, sigfpe_handler_type)  ;
typedef float single;   
typedef unsigned extended[3];
typedef long double quadruple;  
typedef unsigned fp_exception_field_type;
typedef char decimal_string[512 ]; 
typedef struct {
 enum fp_class_type fpclass;
 int sign;
 int exponent;
 decimal_string ds;  
 int more;   
 int  ndigits;  
} decimal_record;
enum decimal_form {
 fixed_form,   
 floating_form   
};
typedef struct {
 enum fp_direction_type rd; 
 enum decimal_form df;  
 int ndigits;   
} decimal_mode;
enum decimal_string_form {  
 invalid_form,   
 whitespace_form,  
 fixed_int_form,   
 fixed_intdot_form,  
 fixed_dotfrac_form,  
 fixed_intdotfrac_form,  
 floating_int_form,   
 floating_intdot_form,  
 floating_dotfrac_form,  
 floating_intdotfrac_form, 
 inf_form,   
 infinity_form,   
 nan_form,   
 nanstring_form   
};
extern void single_to_decimal  (single *, decimal_mode *, decimal_record *,
    fp_exception_field_type *)  ;
extern void double_to_decimal  (double *, decimal_mode *, decimal_record *,
    fp_exception_field_type *)  ;
extern void extended_to_decimal  (extended *, decimal_mode *,
    decimal_record *, fp_exception_field_type *)  ;
extern void quadruple_to_decimal  (quadruple *, decimal_mode *,
    decimal_record *, fp_exception_field_type *)  ;
extern void decimal_to_single  (single *, decimal_mode *, decimal_record *,
    fp_exception_field_type *)  ;
extern void decimal_to_double  (double *, decimal_mode *, decimal_record *,
    fp_exception_field_type *)  ;
extern void decimal_to_extended  (extended *, decimal_mode *,
    decimal_record *, fp_exception_field_type *)  ;
extern void decimal_to_quadruple  (quadruple *, decimal_mode *,
    decimal_record *, fp_exception_field_type *)  ;
extern void string_to_decimal  (char **, int, int, decimal_record *,
    enum decimal_string_form *, char **)  ;
extern void func_to_decimal  (char **, int, int, decimal_record *,
    enum decimal_string_form *, char **,
    int (*)(void), int *, int (*)(int))  ;
extern void file_to_decimal  (char **, int, int, decimal_record *,
    enum decimal_string_form *, char **,
    FILE *, int *)  ;
extern char *seconvert  (single *, int, int *, int *, char *)  ;
extern char *sfconvert  (single *, int, int *, int *, char *)  ;
extern char *sgconvert  (single *, int, int, char *)  ;
extern char *econvert  (double, int, int *, int *, char *)  ;
extern char *fconvert  (double, int, int *, int *, char *)  ;
extern char *gconvert  (double, int, int, char *)  ;
extern char *qeconvert  (quadruple *, int, int *, int *, char *)  ;
extern char *qfconvert  (quadruple *, int, int *, int *, char *)  ;
extern char *qgconvert  (quadruple *, int, int, char *)  ;
extern char *ecvt  (double, int, int *, int *)  ;
extern char *fcvt  (double, int, int *, int *)  ;
extern char *gcvt  (double, int, char *)  ;
extern double atof  (const char *)  ;
extern double strtod  (const char *, char **)  ;
# 213 "/usr/include/math.h" 2 3
# 5 "/opt/gnu/5.7/lib/gcc-lib/sparc-sun-solaris2.7/2.95.2/include/math.h" 2 3
# 14 "vector4.h" 2
namespace BALL 
{
 template <class T>
 class TVector4
 {
  public:
  BALL_CREATE(TVector4<T>)
  TVector4(void);
  TVector4(const T* ptr);
  TVector4(const TVector_<T>& vector);
  TVector4(const T& x, const T& y, const T& z, const T& h = (T)1);
  TVector4(const TVector4& vector);
  virtual ~TVector4(void);
  operator TVector_<T>(void) const;
  void set(const T* ptr);
  void set(const TVector_<T>& vector);
  void set(const T& x, const T& y, const T& z, const T& h = (T)1);
  void set(const TVector4& vector);
  TVector4& operator = (const T* ptr);
  TVector4& operator = (const TVector_<T>& vector);
  TVector4 &operator = (const TVector4& vector);
  void get(T* ptr) const;
  void get(TVector_<T>& vector) const;
  void get(T& x, T& y, T& z, T& h) const;
  void get(TVector4& vector, bool deep = true) const;
  void swap(TVector4& vector);
  T getLength(void) const;
  T getSquareLength(void) const;
  void normalize(void);
  static const TVector4& getZero(void);
  void setUnit(void);
  static const TVector4& getUnit(void);
  void fill(const T& value = (T)1);
  T& operator [] (Index index);
  const T& operator [] (Index index) const;
  TVector4 operator + (void) const;
  TVector4 operator - (void) const;
  template <typename C>
  friend TVector4<C> operator + (const TVector4<C>& a, const TVector4<C>& b);
  TVector4& operator += (const TVector4& vector);
  template <typename C>
  friend TVector4<C> operator - (const TVector4<C>& a, const TVector4<C>& b);
  TVector4& operator -= (const TVector4& vector);
  TVector4 operator * (const T& scalar);
  template <typename C>
  friend TVector4<C> operator * (const T& scalar,const TVector4<C>& vector);
  TVector4& operator *= (const T& scalar);
  TVector4 operator / (const T& scalar);
  TVector4 &operator /= (const T& scalar);
  T operator * (const TVector4& vector) const;
  T getDistance(const TVector4& vector) const;
  T getSquareDistance(const TVector4& vector) const;
  bool operator == (const TVector4& vector) const;
  bool operator != (const TVector4& vector) const;
  bool isOrthogonalTo(TVector4& vector) const;
  bool isValid(void) const;
  void dump(std::ostream& s = std::cout, unsigned long depth = 0) const;
  template <typename C>
  friend std::istream& operator >> (std::istream& s, TVector4<C>& vector);
  template <typename C>
  friend std::ostream& operator << (std::ostream& s, const TVector4<C>& vector);
  T x;
  T y;
  T z;
  T h;
 };
 template <class T>
 TVector4<T>::TVector4(void)
  : x(0),
   y(0),
   z(0),
   h(0)
 {
 }
 template <class T>
 TVector4<T>::TVector4(const T* ptr)
 {
  if (ptr == 0)
   throw Exception::NullPointer("vector4.h", 273);
  x = *ptr++;
  y = *ptr++;
  z = *ptr++;
  h = *ptr;
 }
 template <class T>
 TVector4<T>::TVector4(const TVector_<T>& v)
  : x(v.x),
   y(v.y),
   z(v.z),
   h(v.h)
 {
 }
 template <class T>
 TVector4<T>::TVector4(const T& x,const T& y, const T& z, const T& h)
  : x(x),
   y(y),
   z(z),
   h(h)
 {
 }
 template <class T>
 TVector4<T>::TVector4(const TVector4<T>& v, bool  )
  : x(v.x),
   y(v.y),
   z(v.z),
   h(v.h)
 {
 }
 template <class T>
 TVector4<T>::~TVector4(void)
 {
 }
 template <class T>
 BALL_INLINE
 TVector4<T>::operator TVector_<T>(void) const
 {
  return TVector_<T>(x, y, z, h);
 }
 template <class T>
 BALL_INLINE 
 void TVector4<T>::set(const T* ptr)
 {
  if (ptr == 0)
   throw Exception::NullPointer("vector4.h", 325);
  x = *ptr++;
  y = *ptr++;
  z = *ptr++;
  h = *ptr;
 }
 template <class T>
 BALL_INLINE 
 void TVector4<T>::set(const TVector_<T>& v)
 {
  x = v.x;
  y = v.y;
  z = v.z;
  h = v.h;
 }
 template <class T>
 BALL_INLINE 
 void TVector4<T>::set(const T& x, const T& y, const T& z, const T& h)
 {
  x = x;
  y = y;
  z = z;
  h = h;
 }
 template <class T>
 BALL_INLINE 
 void TVector4<T>::set(const TVector4<T>& v, bool deep)
 {
  x = v.x;
  y = v.y;
  z = v.z;
  h = v.h;
 }
 template <class T>
 BALL_INLINE 
 TVector4<T>& TVector4<T>::operator = (const T* ptr)
 {
  if (ptr == 0)
   throw Exception::NullPointer("vector4.h", 368);
  x = *ptr++;
  y = *ptr++;
  z = *ptr++;
  h = *ptr;
  return *this;
 }
 template <class T>
 BALL_INLINE 
 TVector4<T>& TVector4<T>::operator = (const TVector_<T>& v)
 {
  x = v.x;
  y = v.y;
  z = v.z;
  h = v.h;
  return *this;
 }
 template <class T>
 BALL_INLINE 
 TVector4<T>& TVector4<T>::operator = (const TVector4<T>& v)
 {
  x = v.x;
  y = v.y;
  z = v.z;
  h = v.h;
  return *this;
 }
 template <class T>
 BALL_INLINE 
 void TVector4<T>::get(T* ptr) const
 {
  if (ptr == 0)
   throw Exception::NullPointer("vector4.h", __LINE___);
  *ptr++ = x;
  *ptr++ = y;
  *ptr++ = z;
  *ptr   = h;
 }
 template <class T>
 BALL_INLINE 
 void TVector4<T>::get(TVector_<T>& v) const
 {
  v.x = x;
  v.y = y;
  v.z = z;
  v.h = h;
 }
 template <class T>
 BALL_INLINE 
 void TVector4<T>::get(T& rx, T& ry, T& rz, T& rh) const
 {
  rx = x;
  ry = y;
  rz = z;
  rh = h;
 }
 template <class T>
 BALL_INLINE 
 void TVector4<T>::get(TVector4<T>& v, bool deep) const
 {
  v.x = x;
  v.y = y;
  v.z = z;
  v.h = h;
 }
 template <class T>
 void TVector4<T>::swap(TVector4<T>& v)
 {
  T temp = x;
  x = v.x;
  v.x = temp;
  temp = y;
  y = v.y;
  v.y = temp;
  temp = z;
  z = v.z;
  v.z = temp;
  temp = h;
  h = v.h;
  v.h = temp;
 }
 template <class T>
 BALL_INLINE 
 T TVector4<T>::getLength(void) const
 {
  return (T)sqrt(x * x + y * y + z * z + h * h);
 }
 template <class T>
 BALL_INLINE 
 T TVector4<T>::getSquareLength(void) const
 {
  return (T)(x * x + y * y + z * z + h * h);
 }
 template <class T>
 void TVector4<T>::normalize(void)
 {
  T len = (T)sqrt(x * x + y * y + z * z + h * h);
  if (Maths::isZero(len))
   throw Exception::DivisionByZero("vector4.h", 485);  
  x /= len;
  y /= len;
  z /= len;
  h /= len;
 }
 template <class T>
 BALL_INLINE 
 const TVector4<T>& TVector4<T>::getZero(void)
 {
  static TVector4<T> null4(0, 0, 0, 0);
  return null;
 }
 template <class T>
 BALL_INLINE 
 const TVector4<T>& TVector4<T>::getUnit(void)
 {
  static TVector4<T> unit_vector(1, 1, 1, 1);
  return unit_vector;
 }
 template <class T>
 BALL_INLINE 
 void TVector4<T>::fill(const T& value)
 {
  x = y = z = h = value;
 }
 template <class T>
 BALL_INLINE 
 T& TVector4<T>::operator [] (Index i)
 {
  if (i < 0)
   throw Exception::IndexUnderflow("vector4.h", 523);
  if (i > 2)
   throw Exception::IndexOverflow("vector4.h", 526);
  switch (i) {
   case 0: return x;
   case 1: return y;
   case 2: return z;
   case 3:
   default:
    return h;
  }
 }
 template <class T>
 BALL_INLINE 
 const T& TVector4<T>::operator [] (Index i) const
 {
  if (i < 0)
   throw Exception::IndexUnderflow("vector4.h", 543);
  if (i > 2)
   throw Exception::IndexOverflow("vector4.h", 546);
  switch (i) {
   case 0: return x;
   case 1: return y;
   case 2: return z;
   case 3:
   default:
    return h;
  }
 }
 template <class T>
 BALL_INLINE 
 TVector4<T> TVector4<T>::operator + (void) const
 {
  return *this;
 }
 template <class T>
 BALL_INLINE 
 TVector4<T> TVector4<T>::operator - (void) const
 {
  return TVector4<T>(-x, -y, -z, -h);
 }
 template <class T>
 BALL_INLINE 
 TVector4<T> operator + (const TVector4<T>& a, const TVector4<T>& b)
 {
  return TVector4<T>(a.x + b.x, a.y + b.y, a.z + b.z, a.h + b.h);
 }
 template <class T>
 BALL_INLINE 
 TVector4<T>& TVector4<T>::operator += (const TVector4<T>& v)
 {
  x += v.x;
  y += v.y;
  z += v.z;
  h += v.h;
  return *this;
 }
 template <class T>
 BALL_INLINE 
 TVector4<T> operator - (const TVector4<T>& a, const TVector4<T>& b)
 {
  return TVector4<T>(a.x - b.x, a.y - b.y, a.z - b.z, a.h - b.h);
 }
 template <class T>
 BALL_INLINE 
 TVector4<T>& TVector4<T>::operator -= (const TVector4<T> &v)
 {
  x -= v.x;
  y -= v.y;
  z -= v.z;
  h -= v.h;
  return *this;
 }
 template <class T>
 BALL_INLINE 
 TVector4<T> TVector4<T>::operator * (const T& scalar)
 {
  return TVector4<T>(x * scalar, y * scalar, z * scalar, h * scalar);
 }
 template <class T>
 BALL_INLINE 
 TVector4<T> operator * (const T& scalar, const TVector4<T>& v)
 {
  return TVector4<T>(scalar * v.x, scalar * v.y, scalar * v.z, scalar * v.h);
 }
 template <class T>
 BALL_INLINE 
 TVector4<T>& TVector4<T>::operator *= (const T &scalar)
 {
  x *= scalar;
  y *= scalar;
  z *= scalar;
  h *= scalar;
  return *this;
 }
 template <class T>
 TVector4<T>TVector4<T>::operator / (const T &scalar)
 {
  if (Maths::isZero(scalar))
   throw Exception::DivisionByZero("vector4.h", 641);
  return TVector4<T>(x / scalar, y / scalar, z / scalar, h / scalar); 
 }
 template <class T>
 TVector4<T>& TVector4<T>::operator /= (const T& scalar)
 {
  if (Maths::isZero(scalar))
   throw Exception::DivisionByZero("vector4.h", 650);
  x /= scalar;
  y /= scalar;
  z /= scalar;
  h /= scalar;
  return *this;
 }
 template <class T>
 BALL_INLINE 
 T TVector4<T>::operator * (const TVector4<T>& v) const
 {
  return (x * v.x + y * v.y + z * v.z + h * v.h);
 }
 template <class T>
 BALL_INLINE 
 T TVector4<T>::getDistance(const TVector4<T> &v) const
 {
  T da = x - v.x;
  T db = y - v.y;
  T dc = z - v.z;
  T dd = h - v.h;
  return (T)sqrt(da * da + db * db + dc * dc + dd * dd); 
 }
 template <class T>
 BALL_INLINE 
 T TVector4<T>::getSquareDistance(const TVector4<T> &v) const
 {
  T da = x - v.x;
  T db = y - v.y;
  T dc = z - v.z;
  T dd = h - v.h;
  return (da * da + db * db + dc * dc + dd * dd); 
 }
 template <class T>
 BALL_INLINE 
 bool TVector4<T>::operator == (const TVector4<T>& v) const
 {
  return (bool)(Maths::isEqual(x, v.x) && Maths::isEqual(y, v.y) 
         && Maths::isEqual(z, v.z) && Maths::isEqual(h, v.h));
 }
 template <class T>
 BALL_INLINE 
 bool TVector4<T>::operator != (const TVector4<T>& v) const
 {
  return (bool)(Maths::isNotEqual(x, v.x) || Maths::isNotEqual(y, v.y)
         || Maths::isNotEqual(z, v.z) || Maths::isNotEqual(h, v.h));
 }
 template <class T>
 BALL_INLINE 
 bool TVector4<T>::isOrthogonalTo(TVector4<T>& v) const
 {
  return Maths::isZero(*this * v);
 }
 template <class T>
 BALL_INLINE 
 bool TVector4<T>::isValid(void) const
 {
  return true;
 }
 template <class T>
 void TVector4<T>::dump(std::ostream& s, unsigned long depth) const
 {
  BALL_DUMP_STREAM_PREFIX(s);
  BALL_DUMP_HEADER(s, this, this);
  BALL_DUMP_DEPTH(s, depth);
  s << "x= " << x 
   << ", y = " << y
   << ", z = " << z
   << ", h = " << h << endl;
  BALL_DUMP_STREAM_SUFFIX(s);
 }
 template <class T>
 std::istream& operator >> (std::istream& s, TVector4<T>& v)
 {
  s >> v.x >> v.y >> v.z >> v.h;
  return s;
 }
 template <class T>
 std::ostream& operator << (std::ostream& s, const TVector4<T>& v)
 {
  s << '(' <<v.x << ' ' << v.y << ' ' << v.z << ' ' << v.h << ')';
  return s;
 }
 typedef TVector4<Real> Vector4;
}  
