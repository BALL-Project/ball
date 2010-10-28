### list all filenames of the directory here ###
SET(GROUP QSAR)

FILE(GLOB HEADERS_LIST "include/BALL/${GROUP}/*.h" "include/BALL/${GROUP}/*.iC")	

IF(NOT BALL_HAS_LAPACK)
	LIST(REMOVE_ITEM HEADERS_LIST
		"include/BALL/${GROUP}/allModel.h"
		"include/BALL/${GROUP}/featureSelection.h"
		"include/BALL/${GROUP}/gpModel.h"
		"include/BALL/${GROUP}/knnModel.h"
		"include/BALL/${GROUP}/kpcrModel.h"
		"include/BALL/${GROUP}/kplsModel.h"
		"include/BALL/${GROUP}/ldaModel.h"
		"include/BALL/${GROUP}/mlrModel.h"
		"include/BALL/${GROUP}/oplsModel.h"
		"include/BALL/${GROUP}/pcrModel.h"
		"include/BALL/${GROUP}/plsModel.h"
		"include/BALL/${GROUP}/rrModel.h"
	)
ENDIF()

ADD_BALL_HEADERS("${GROUP}" "${HEADERS_LIST}")
