/**********************************************************************

	--- Qt Architect generated file ---

	File: DlgAmberMinimization.h
	Last generated: Fri Jan 14 22:20:31 2000

 *********************************************************************/

#ifndef DlgAmberMinimization_included
#define DlgAmberMinimization_included

#include "DlgAmberMinimizationData.h"

#include <BALL/FORMAT/INIFile.h>
#include <BALL/COMMON/path.h>

using namespace BALL;

class DlgAmberMinimization : public DlgAmberMinimizationData
{
    Q_OBJECT

		public:

    DlgAmberMinimization(QWidget* parent = NULL, const char* name = NULL );

    virtual ~DlgAmberMinimization();

		void setPreferences(INIFile& inifile) const;
		void getPreferences(const INIFile& inifile);

		Size getMaxIterations() const;
		void setMaxIterations(Size n);
	
		Size getRefresh() const;
		void setRefresh(Size n);
	
		double getMaxGradient() const;
		void setMaxGradient(double max_gradient);
	
		double getEnergyDifference() const;
		void setEnergyDifference(double energy_difference);
	
		const String& getFilename() const;
		void setFilename(const String& filename);

		bool getUseDistanceDependentDC() const;
		void setUseDistanceDependentDC(bool use_dddc);

		bool getUseConjugateGradient() const;
		void setUseConjugateGradient(bool use_CG);
	
		protected slots:
    virtual void browseParameterFiles();
 
};
#endif // DlgAmberMinimization_included
