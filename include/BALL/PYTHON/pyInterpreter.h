// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: pyInterpreter.h,v 1.4 2003/02/21 16:05:57 anhi Exp $ 

#ifndef BALL_PYTHON_PYINTERPRETER_H
#define BALL_PYTHON_PYINTERPRETER_H

#ifndef BALL_COMMON_H
#	include <BALL/common.h>
#endif

#ifndef BALL_DATAYPE_STRING_H
#	include <BALL/DATATYPE/string.h>
#endif

namespace BALL 
{
	/** Embedded Python interpreter.
			There's just one global instance of the interpreter,
			so all methods are static. The use of subinterpreters
			is not yet supported.
			 \par
			<b>Definition:</b>BALL/PYTHON/pyInterpreter.h
			 \par
	*/
	class PyInterpreter
	{
		public:

		/**	Initialize the interpreter.
				Initialize the interpreter (by calling <tt>PY_Initialize</tt>) and 
				load the modules <tt>sys</tt>, <tt>site</tt>, and {\tt BALL}.
				A second call to <tt>initialize</tt> may be used to restart the intepreter.
		*/
		static void initialize();

		/**	Stop the interpreter.
				Deallocate all memory occupied by the interpreter
				(by calling <tt>PY_Finalize</tt>.
		*/
		static void finalize();

		/**	Determine the interpreter state.
				@return true if the interpreter is correctly initialized
		*/
		static bool isInitialized();

		/**	Execute a string.
				@param s the string to run (may contain multiple lines with correct indentation)
				@return the output of the interpreter (may also contain error messages)
		*/
		static String run(const String& s);

		/**	Run a Python program from a file.
				@param file_name the name of the program file
		*/
		static String runFile(const String& filename);
			
		/**	Import a module.	
				The module with name <tt>module_name</tt> is imported
				using <tt>PyImport_ImportModule</tt> and initialized.
				When called 
				@return true if the modules was found an initialized correctly
		*/
		static bool importModule(const String& module_name);
	};

} // namespace BALL

#endif // BALL_PYTHON_PYINTERPRETER_H
