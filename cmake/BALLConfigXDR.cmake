SET(XDR_TEST_HEADER "#include <rpc/types.h>
            #include <rpc/xdr.h>
            extern \"C\" int dummy" )
SET(XDR_TEST_FOOTER " {return 0;}
            int main(){
                XDR xdrs;
                xdrrec_create(&xdrs, 0, 0, 0, dummy, dummy);
            }")

MESSAGE(STATUS "Checking for xdrrec_create signature")

CHECK_CXX_SOURCE_COMPILES("${XDR_TEST_HEADER}(char*, char*, int)${XDR_TEST_FOOTER}" BALL_XDRREC_CREATE_CHAR_CHAR_INT)
IF(NOT BALL_XDRREC_CREATE_CHAR_CHAR_INT)
    CHECK_CXX_SOURCE_COMPILES("${XDR_TEST_HEADER}(void*, char*, int)${XDR_TEST_FOOTER}" BALL_XDRREC_CREATE_VOID_CHAR_INT)
    IF(NOT BALL_XDRREC_CREATE_VOID_CHAR_INT)
				CHECK_CXX_SOURCE_COMPILES("${XDR_TEST_HEADER}(void*, void*, unsigned int)${XDR_TEST_FOOTER}" BALL_XDRREC_CREATE_VOID_VOID_UINT)
				IF(NOT BALL_XDRREC_CREATE_VOID_VOID_UINT)
            CHECK_CXX_SOURCE_COMPILES("${XDR_TEST_HEADER}()${XDR_TEST_FOOTER}" BALL_XDRREC_CREATE_VOID)
            IF(NOT BALL_XDRREC_CREATE_VOID)
                CHECK_CXX_SOURCE_COMPILES("${XDR_TEST_HEADER}(void*, void*, int)${XDR_TEST_FOOTER}" BALL_XDRREC_CREATE_VOID_VOID_INT)
                IF(NOT BALL_XDRREC_CREATE_VOID_VOID_INT)
                    MESSAGE(SEND_ERROR "Could not determine xdrrec_create signature")
                ENDIF()
            ENDIF()
        ENDIF()
    ENDIF()
ENDIF()
