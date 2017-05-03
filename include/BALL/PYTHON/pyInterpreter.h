// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//

#ifndef BALL_PYTHON_PYINTERPRETER_H
#define BALL_PYTHON_PYINTERPRETER_H

// This has to be included before Python.h as to macro redefinition clashes in macOS Python...
#include <BALL/SYSTEM/path.h>

#include <BALL/PYTHON/pyKernel.h>
#include <BALL/PYTHON/pyServer.h>

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

			/**
			 * Append additional search paths to sys.path upon initialization
			 * @deprecated Old String API; Might be re-added under a less misleading name in the future
			 */
			BALL_DEPRECATED static void setSysPath(const PathStrings& path_strings) { sys_path_ = path_strings; }

			/**
			 * Get the current (additional) paths added to sys.path
			 * @deprecated Old String API; Might be re-added under a less misleading name in the future
			 */
			BALL_DEPRECATED static const PathStrings& getSysPath() { return sys_path_; }

			/// @deprecated Returns PyInterpreter::isInitialized()
			BALL_DEPRECATED static bool isValid() { return isInitialized(); }
			
			/// @deprecated Error messages are directly printed to Log.error; Always returns an empty string.
			BALL_DEPRECATED static String getStartupLog() { return String(); }
			
			//@}


			/**	@name Execution */
			//@{
			/**	Execute a string.
					@param s the string to run (may contain multiple lines with correct indentation)
					@return {ok, res} with `ok` indicating whether the execution was successful and `res` representing
					        the output of the interpreter (may also contain error messages)
			*/
			static std::pair<bool, std::string> run(const std::string& s);

			/// @deprecated Use run(const std::string&, bool&) instead
			BALL_DEPRECATED static String run(const String& s, bool& result);

			/**	Run a Python program from a file.
			 		If the file does not exist, or cannot be opened, an Exception::FileNotFound is thrown
					@param file_name the name of the program file
				@deprecated Functionality replaced by Jupyter notebook plugin
			*/
			BALL_DEPRECATED static String runFile(const String& filename);

			/**
			 * Calls a single function from a given module.
			 *
			 * @param module a Python module
			 * @param func a function from the given module
			 * @param params function arguments as key-value pairs
			 * @return true if the execution succeeded
			 */
			static bool execute(const std::string& module, const std::string& func, const PyKernel::KeyValArgs& params);

			/**
			 * Returns the most recent error message emitted by the Python interpreter.
			 *
			 * @return most recent error message
			 */
			static std::string getErrorMessage();

			/// @deprecated use execute(const string&, const string&, const KeyValArgs&) instead
			BALL_DEPRECATED static bool execute(const QString& module, const QString& func, const QList<QPair<QString, QString> >& params);
			//@}


			/**@name Server */
			//@{

			/**
			 * Start a PyServer instance (if not already running) to allow remote execution of Python code, e.g., via
			 * the BALLView Jupyter kernel.
			 */
			static void startServer();

			/**
			 * Stops the PyServer (if running).
			 */
			static void stopServer();

			/**
			 * Indicates whether a PyServer instance is currently running.
			 *
			 * @return true if PyServer is running
			 */
			static bool serverIsRunning() { return bool(server_); }
			//@}

		protected:
			static PyKernel*   kernel_;
			static PathStrings sys_path_;
			static PyServer*   server_;
	};
   
} // namespace BALL

#endif // BALL_PYTHON_PYINTERPRETER_H
