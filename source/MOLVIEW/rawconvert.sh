#!/bin/sh
FILES=$*
filesed BALLVIEW BALL_VIEW ${FILES}
filesed MOLVIEW BALL_MOLVIEW ${FILES}
filesed _BALL_ BALL_ ${FILES}
filesed " <BALL_" " <BALL\/" ${FILES}
filesed "\/VIEW_MOLVIEW\/" "\/MOLVIEW\/" ${FILES}
filesed CBallVIEW "" ${FILES}
filesed CMolVIEW "" ${FILES}
filesed BallVIEW "" ${FILES}
filesed MolVIEW "" ${FILES}
filesed CBall "" ${FILES}
filesed clone_deep_bool deep ${FILES}
filesed clone_deep__bool deep ${FILES}
filesed __rReal "" ${FILES}
filesed "const Real &" "const Real " ${FILES}
filesed "(void)" "()" ${FILES}
filesed Ball::Real Real ${FILES}
filesed " __rColorRGBA" color ${FILES}
filesed "ColorRGBA\ \&" "ColorRGBA\&\ " ${FILES}
filesed "__.ColorRGBA" "" ${FILES}
filesed "ColorUnit\ \&" "ColorUnit\&\ " ${FILES}
filesed "__.ColorUnit" "" ${FILES}
filesed "__gmpac_" "_" ${FILES}
filesed "depth__ul" "depth" ${FILES}
filesed "istream &__ristream" "istream\& s" ${FILES}
filesed "ostream &__rostream" "ostream\& s" ${FILES}
filesed "\&__rErrorCode" error_code ${FILES}
filesed "__rErrorCode" error_code ${FILES}
filesed "__r.stream" s ${FILES}
filesed "BALL_.*ERROR_HANDLER.*NOT_IMPLEMENTED);" "throw ::BALL::Exception::NotImplemented(__FILE__, __LINE__);" ${FILES}
