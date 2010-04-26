PROJECT(BENCHMARK)

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

INCLUDE_DIRECTORIES(${PROJECT_BINARY_DIR})

FOREACH(i ${EXECUTABLES})
	ADD_EXECUTABLE(${i} EXCLUDE_FROM_ALL ${i}.C)
	TARGET_LINK_LIBRARIES(${i} BALL)
ENDFOREACH()

SET(BENCH_DIR ${CMAKE_SOURCE_DIR}/source/BENCHMARKS)

ADD_CUSTOM_TARGET(BENCHMARKS
	${CMAKE_COMMAND} -E copy_directory ${BENCH_DIR}/data ${CMAKE_BINARY_DIR}/bin/data
	COMMAND ${BENCH_DIR}/runbenchmarks ${EXECUTABLES}
	DEPENDS ${EXECUTABLES}
	WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/bin
)
