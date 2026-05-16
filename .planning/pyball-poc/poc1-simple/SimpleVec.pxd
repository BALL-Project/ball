# autowrap-annotated PXD for SimpleVec — POC1.
# Cython 3 syntax. The "# wrap-*" comments steer autowrap.
# PITFALL #2: autowrap's std::string converter is keyed on
# "libcpp_utf8_string" (UTF-8 → Python str) or "libcpp_string" (bytes).
# Naive `from libcpp.string cimport string` fails — autowrap does not
# recognize plain `string` as a registered converter.
from libcpp.string cimport string as libcpp_utf8_string

cdef extern from "simple_vec.h" namespace "pocns":

    cdef cppclass SimpleVec:
        # wrap-doc:
        #  A 3-vector exposing constructors, arithmetic operators,
        #  comparison, and a throw-bearing divideBy method. POC for
        #  validating the autowrap+Cython toolchain end-to-end.

        SimpleVec() except +
        SimpleVec(double x, double y, double z) except +
        # PITFALL #3 + #4: drop & in copy ctor (autowrap can't generate
        # valid Cython for reference types in operator/ctor args), AND
        # never put inline comments on the same line as a method decl —
        # autowrap parses them as wrap-* annotations and crashes.
        SimpleVec(SimpleVec other) except +

        double getX() except +
        double getY() except +
        double getZ() except +
        void   setX(double v) except +
        void   setY(double v) except +
        void   setZ(double v) except +

        double length() except +
        SimpleVec normalize() except +
        # PITFALL #4 reminder: any '#' on same line as a method is treated
        # as a wrap-* annotation. Comments go ABOVE.
        # negate() returns *this in C++; autowrap can't model that.
        # Workaround: change return type to void OR ignore the return.
        SimpleVec negate() except +

        # PITFALL #3 (continued): drop & from operator-arg types too — autowrap
        # generates invalid Cython for reference parameters in operators
        # (`_T & * this` is not valid Cython). pyOpenMS convention is to
        # declare the operand as plain value type. Cython/C++ still passes
        # by reference under the hood.
        SimpleVec operator + (SimpleVec o) except +
        SimpleVec operator - (SimpleVec o) except +
        # PITFALL #1: autowrap requires operator* to return the class type.
        # SimpleVec's dot-product operator* returns double — autowrap rejects.
        # Workaround: drop operator*(SimpleVec) entirely and expose dot() instead.
        # The scalar operator*(double) is also dropped here because autowrap
        # supports only one overload per operator (known limitation).
        # double    operator * (SimpleVec o) except +
        # SimpleVec operator * (double s) except +

        bint operator == (SimpleVec o) except +
        bint operator != (SimpleVec o) except +

        # throws std::invalid_argument on 0 — `except +` catches all
        # std::exception derivatives and re-throws as RuntimeError on Python side.
        SimpleVec divideBy(double s) except +

        libcpp_utf8_string repr() except +
