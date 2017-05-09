#ifndef BALL_PYTHON_PYCAPIKERNEL_H
#define BALL_PYTHON_PYCAPIKERNEL_H

#include <BALL/PYTHON/pyKernel.h>

#include <vector>

namespace BALL
{
	class PyCAPIKernel : public PyKernel
	{
		public:
			PyCAPIKernel();
			~PyCAPIKernel();

			bool isStarted() const override;
			std::string getErrorMessage() const override { return last_err_; };

			std::pair<bool, std::string> run(std::string str) override;
			BALL_DEPRECATED bool runFile(std::string filename) override;
			bool execute(const std::string& module, const std::string& func, const KeyValArgs& args) override;

		private:
			/**
			 * Imports the Python module with the given name.
			 *
			 * @param name Python module
			 * @return borrowed reference to the imported module, or nullptr in case of an error
			 */
			PyObject* loadModule(const std::string& name);

			/**
			 * Returns whether the Python error flag is currently set and, if so, sets the internal error message
			 * buffer accordingly (accessible via `getErrorMessage`).
			 *
			 * @return true if Python error flag is currently set
			 */
			bool errorOccurred();

			PyObject* main_module_;
			PyObject* context_;
			std::string last_err_;
			std::vector<PyObject*> modules_;
	};
}

#endif // BALL_PYTHON_PYCAPIKERNEL_H
