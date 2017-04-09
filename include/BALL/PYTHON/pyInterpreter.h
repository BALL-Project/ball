// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//

#ifndef BALL_PYTHON_PYINTERPRETER_H
#define BALL_PYTHON_PYINTERPRETER_H

#include <BALL/PYTHON/pyKernel.h>
#include <BALL/SYSTEM/path.h>

#include <vector>

#include <QtCore/QList>
#include <QtCore/QPair>
#include <QtCore/QString>

namespace BALL 
{
	/** Embedded Python interpreter.
			There's just one global instance of the interpreter,
			so all methods are static. The use of subinterpreters
			is not yet supported.
			\ingroup PythonExtensions
	*/
	class BALL_EXPORT PyInterpreter
	{
		private:
			// We don't want anybody to instantiate this!
			PyInterpreter() {}
			~PyInterpreter() {}
		
		public:
			/**	@name Type definitions */
			//@{
			/// Used to encode the individual paths appended to sys.path for dynamic loading of modules.
			using PathStrings = std::vector<String>;
			//@}

			/**	@name Initialization */
			//@{

			/**	Initialize the interpreter.
					Initialize the interpreter (by calling <tt>Py_Initialize</tt>) and 
					load the modules <tt>sys</tt>, <tt>site</tt>, and <tt>BALL</tt>.
					A second call to <tt>initialize</tt> may be used to restart the intepreter.
					Upon start, the paths defined by \link setSysPath \endlink are added to sys.path.
					If your interpreter cannot load specific modules, add the location of your
					modules here.
			*/
			static void initialize();

			/**	Stop the interpreter.
					Deallocate all memory occupied by the interpreter
					(by calling <tt>Py_Finalize</tt>).
			*/
			static void finalize();

			/**	Determine the interpreter state.
					@return true if the interpreter is correctly initialized
			*/
			static bool isInitialized() { return kernel_ && kernel_->isStarted(); }

			///	Append additional search paths to sys.path upon initialization
			static void setSysPath(const PathStrings& path_strings) { sys_path_ = path_strings; }

			/// Get the current paths added to sys.path
			static const PathStrings& getSysPath() { return sys_path_; }

			///
			static bool isValid() { return isInitialized(); }
			
			///
			static String getStartupLog() { return start_log_; }
			
			//@}


			/**	@name Execution */
			//@{
			/**	Execute a string.
					@param s the string to run (may contain multiple lines with correct indentation)
					@param result bool reference which contains the result after call of function
					@return the output of the interpreter (may also contain error messages)
			*/
			static String run(const String& s, bool& result);

			/**	Run a Python program from a file.
			 		If the file does not exist, or cannot be opened, an Exception::FileNotFound is thrown
					@param file_name the name of the program file
			*/
			static String runFile(const String& filename);

			static bool execute(const QString& module, const QString& func, const QList<QPair<QString, QString> >& params);

		protected:
			static PyKernel* kernel_;
			static PathStrings sys_path_;
			static String start_log_;
	};
   
} // namespace BALL

#endif // BALL_PYTHON_PYINTERPRETER_H
