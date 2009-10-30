### list all filenames of the directory here ###
SET(SOURCES_LIST
	directory.C
	file.C
	fileSystem.C
	mutex.C
	path.C
	sysinfo.C
	systemCalls.C
	TCPTransfer.C
	timer.C
)

IF (BALL_HAS_ASIO)
	SET(SOURCES_LIST ${SOURCES_LIST} networking.C)
ENDIF()

IF (BALL_HAS_MPI)
	SET(SOURCES_LIST ${SOURCES_LIST} MPISupport.C)
ENDIF()

ADD_BALL_SOURCES("SYSTEM" "${SOURCES_LIST}")
