### list all filenames of the directory here ###
SET(SOURCES_LIST
	binaryFileAdaptor.C
	directory.C
	file.C
	fileSystem.C
	mutex.C
	path.C
	simpleDownloader.C
	sysinfo.C
	systemCalls.C
	timer.C
)

IF (BALL_HAS_ASIO)
	SET(SOURCES_LIST ${SOURCES_LIST} networking.C)
ENDIF()

IF (BALL_HAS_MPI)
	SET(SOURCES_LIST ${SOURCES_LIST} MPISupport.C)
ENDIF()

SET(MOC_SOURCES_LIST "simpleDownloader.C")

ADD_BALL_SOURCES("SYSTEM" "${SOURCES_LIST}")

ADD_BALL_CORE_MOCFILES("SYSTEM" "include/BALL/SYSTEM" "${MOC_SOURCES_LIST}")
