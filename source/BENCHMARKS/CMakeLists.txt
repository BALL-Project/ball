PROJECT("BALL_benchmarks")

SET(EXECUTABLES
	CompositeCreation_bench
	CompositeClone_bench
	DataStructures_bench
	KernelCreation_bench
	KernelClone_bench
	KernelIteration_bench
	FragmentDB_bench
	AmberFF_bench
	CharmmFF_bench
	PDB_bench
	PoissonBoltzmann_bench
	ContourSurface_bench
	MolmecSupport_bench
)

SET(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin/BENCHMARKS)
INCLUDE_DIRECTORIES(${PROJECT_BINARY_DIR})

FOREACH(i ${EXECUTABLES})
	ADD_EXECUTABLE(${i} EXCLUDE_FROM_ALL ${i}.C)
	TARGET_LINK_LIBRARIES(${i} BALL)
ENDFOREACH()

SET(BENCH_DIR ${CMAKE_SOURCE_DIR}/source/BENCHMARKS)

ADD_CUSTOM_TARGET(BENCHMARKS
	COMMAND ${BENCH_DIR}/runbenchmarks ${EXECUTABLES}
	DEPENDS ${EXECUTABLES}
	WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/bin/BENCHMARKS
)
