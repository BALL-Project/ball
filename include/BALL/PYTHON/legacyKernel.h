#ifndef BALL_PYTHON_LEGACYKERNEL_H
#define BALL_PYTHON_LEGACYKERNEL_H

#include <BALL/PYTHON/pyKernel.h>

namespace BALL
{
	class LegacyKernel : public PyKernel
	{
		public:
			LegacyKernel();
			~LegacyKernel();

			bool isStarted() const override;
			std::string getErrorMessage() const override;

			std::pair<bool, std::string> run(std::string str) override;
			BALL_DEPRECATED bool runFile(std::string filename) override;
			bool execute(const std::string& module, const std::string& func, const KeyValArgs& args) override;

		private:
			/**
			 * Imports the Python module with the given name.
			 *
			 * @param name Python module
			 * @return new reference to the imported module, or nullptr in case of an error
			 */
			PyObject* loadModule(const std::string& name);

			PyObject* cio_;
			PyObject* context_;
	};
}

#endif // BALL_PYTHON_LEGACYKERNEL_H
