// $Id: benchmark.h,v 1.1 2001/04/10 16:23:19 oliver Exp $

#include <BALL/common.h>
#include <BALL/SYSTEM/timer.h>

#include <string>

/**	@name	Benchmarking of classes.
		\URL[source/BENCHMARKS/Skeleton_bench.C]{../../source/BALL/BENCHMARKS/Skeleton_bench.C}
		\\
		{\bf Definitions:} \URL{BALL/CONCEPT/benchmark.h}
*/
//@{

/**	Start a new benchmark section.
*/
#define START_SECTION(name) \
	BENCHMARK::section_time = BENCHMARK::timer.getCPUTime();\
	BENCHMARK::section_name = #name;\


/**	End of a benchmark section.
*/
#define END_SECTION \
	BENCHMARK::timer.stop();\
	BENCHMARK::section_time = BENCHMARK::timer.getCPUTime() - BENCHMARK::section_time;\
	if (BENCHMARK::verbose > 0)\
	{\
		std::cout << BENCHMARK::section_name << ": " << BENCHMARK::section_time << " s" << std::endl;\
	}\
	BENCHMARK::total_time += BENCHMARK::section_time;\


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

#define START_BENCHMARK(class_name, version)\
/* define a special namespace for all internal variables */\
/* to avoid potential collisions                         */\
namespace BENCHMARK {\
	int						verbose = 0;\
	bool					all_tests = true;\
	int						exception = 0;\
	string				exception_name = "";\
	char*					version_string = version;\
	string				section_name = "";\
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
		std::cout << "(" << BENCHMARK::total_time << ")" << std::endl;\
		return 1;\
	} else {\
		std::cout << BENCHMARK::total_time << std::endl;\
		return 0;\
	}\
}\

//@}

