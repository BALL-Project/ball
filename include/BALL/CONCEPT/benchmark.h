// $Id: benchmark.h,v 1.4 2001/07/14 12:56:44 amoll Exp $

#ifndef BALL_COMMON_H
# include <BALL/common.h>
#endif

#ifndef BALL_SYSTEM_TIMER_H
# include <BALL/SYSTEM/timer.h>
#endif

#include <string>

/**	@name	Benchmarking of classes.
		\URL[source/BENCHMARKS/Skeleton_bench.C]{../../source/BALL/BENCHMARKS/Skeleton_bench.C}
		\\
		{\bf Definitions:} \URL{BALL/CONCEPT/benchmark.h}
*/
//@{

/**	Start a new benchmark section.
		The argument weight determines the weighting factor of the section.
*/
#define START_SECTION(name, weight) \
	BENCHMARK::section_time = BENCHMARK::timer.getCPUTime();\
	BENCHMARK::section_name = #name;\
	BENCHMARK::section_weight = weight;


/**	End of a benchmark section.
*/
#define END_SECTION \
	BENCHMARK::timer.stop();\
	BENCHMARK::section_time = BENCHMARK::timer.getCPUTime() - BENCHMARK::section_time;\
	if (BENCHMARK::verbose > 0)\
	{\
		std::cout << BENCHMARK::section_name << ": " \
		  << BENCHMARK::section_time << " s"\
			<< " (weight = " << BENCHMARK::section_weight << ")" << std::endl;\
	}\
	BENCHMARK::total_time += BENCHMARK::section_time * BENCHMARK::section_weight;\


/**	Start the timer.
		This macro is used to determine the running time of a set of commands.
		It may be used in benchmarks and requires a prior invocation of the
		\Ref{START_BENCHMARK} macro.
		All commands that are between the START_TIMER and the \Ref{STOP_TIMER}
		command contribute to the overall running time of the benchmark.
*/
#define START_TIMER \
	BENCHMARK::timer.start();\


/**	Stop the timer.
		This macro is used to determine the running time of a set of commands.
		It may be used in benchmarks and requires a prior invocation of the
		\Ref{START_BENCHMARK} and \Ref{START_TIMER} macros.
		All commands that are between the START_TIMER and the \Ref{STOP_TIMER}
		command contribute to the overall running time of the benchmark.
*/
#define STOP_TIMER \
	BENCHMARK::timer.stop();

/**	Program body for the benchmark.
		The parameter {\tt weight} determines the overall weight of
		this test in the accumulated benchmark (BALLStones).
*/
#define START_BENCHMARK(class_name, overall_weight, version)\
/* define a special namespace for all internal variables */\
/* to avoid potential collisions                         */\
namespace BENCHMARK {\
	int						verbose = 0;\
	bool					all_tests = true;\
	int						exception = 0;\
	string				exception_name = "";\
	const char*		version_string = version;\
	string				section_name = "";\
	float					section_weight = 1.0;\
	float					weight = overall_weight;\
	float					total_time;\
	float					section_time;\
	BALL::Timer		timer;\
}\
\
\
int main(int argc, char **argv)\
{\
\
	if (argc == 2) {\
		if (!strcmp(argv[1], "-v"))\
			BENCHMARK::verbose = 1;\
	};\
\
	if ((argc > 2) || ((argc == 2) && (BENCHMARK::verbose == 0))) {\
		std::cerr << "Execute a benchmark for the " #class_name " class." << std::endl;\
		std::cerr << "Overall weight of the test: " << BENCHMARK::weight << std::endl;\
\
		std::cerr << "On successful operation, the total CPU time (in seconds)," << std::endl;\
		std::cerr << "is printed." << std::endl;\
		std::cerr << "If called with an argument of -v, " << argv[0] << " detailed" << std::endl;\
		std::cerr << "information about individual benchmarks is printed." << std::endl;\
		return 1;\
	}\
\
	if (BENCHMARK::verbose > 0)\
		std::cout << "Version: " << BENCHMARK::version_string << std::endl;\
\
	try {\

/**	End of the test program
*/
#define END_BENCHMARK \
	/* global try block */\
	}\
	/* catch FileNotFound exceptions to print out the file name */\
	catch (BALL::Exception::FileNotFound e)\
	{\
		BENCHMARK::all_tests = false;\
  	if (BENCHMARK::verbose > 1)\
		{\
			if (BENCHMARK::exception == 1) /* dummy to avoid compiler warnings */\
				BENCHMARK::exception++;\
    	std::cout << std::endl << "    (caught exception of type ";\
			std::cout << e.getName();\
			if ((e.getLine() > 0) && (!(e.getFile() == "")))\
				std::cout << " outside a benchmark block, which was thrown in line " << e.getLine() << " of file " << e.getFile();\
			std::cout << " while looking for file " << e.getFilename();\
			std::cout << " - unexpected!) " << std::endl;\
		}\
  }\
	/* catch BALL exceptions to retrieve additional information */\
	catch (BALL::Exception::GeneralException e)\
	{\
		BENCHMARK::all_tests = false;\
  	if (BENCHMARK::verbose > 1)\
		{\
			if (BENCHMARK::exception == 1) /* dummy to avoid compiler warnings */\
				BENCHMARK::exception++;\
    	std::cout << std::endl << "    (caught exception of type ";\
			std::cout << e.getName();\
			if ((e.getLine() > 0) && (!(e.getFile() == "")))\
				std::cout << " outside a benchmark block, which was thrown in line " << e.getLine() << " of file " << e.getFile();\
			std::cout << " - unexpected!) " << std::endl;\
		}\
  }\
	/* catch all non-BALL exceptions */\
	catch (...)\
	{\
		BENCHMARK::all_tests = false;\
  	if (BENCHMARK::verbose > 1)\
		{\
    	std::cout << std::endl << "    (caught unidentified and unexpected exception outside a benchmark block!) " << std::endl;\
		}\
	}\
\
	/* check for exit code */\
	if (!BENCHMARK::all_tests)\
	{\
		std::cout << "(" << BENCHMARK::weight * BENCHMARK::total_time << ")" << std::endl;\
		return 1;\
	} else {\
		std::cout << BENCHMARK::weight * BENCHMARK::total_time << std::endl;\
		return 0;\
	}\
}\

//@}

