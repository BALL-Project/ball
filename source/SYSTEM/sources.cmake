### list all filenames of the directory here ###
SET(SOURCES_LIST
	binaryFileAdaptor.C
	directory.C
	file.C
	fileSystem.C
	mutex.C
	networking.C
	path.C
	simpleDownloader.C
	sysinfo.C
	systemCalls.C
	timer.C
)

IF (BALL_HAS_MPI)
	SET(SOURCES_LIST ${SOURCES_LIST} MPISupport.C)
ENDIF()

ADD_BALL_SOURCES("SYSTEM" "${SOURCES_LIST}")
