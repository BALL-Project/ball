if(USE_lapack)
	list(APPEND BALL_SOURCES 
		MATHS/LINALG/eigenSolver.C
		MATHS/LINALG/linalgException.C
		MATHS/LINALG/matrix.C
		MATHS/LINALG/SVDSolver.C
		MATHS/LINALG/vector.C
	)
endif(USE_lapack)
