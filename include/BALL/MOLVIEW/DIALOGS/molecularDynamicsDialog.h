// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//

#ifndef BALL_MOLVIEW_DIALOGS_MOLECULARDYNAMICSDIALOG_H
#define BALL_MOLVIEW_DIALOGS_MOLECULARDYNAMICSDIALOG_H

#include "molecularDynamicsDialogData.h"

#include <BALL/FORMAT/INIFile.h>
#include <BALL/SYSTEM/path.h>

namespace BALL
{
	namespace MOLVIEW
	{

///
class MolecularDynamicsDialog 
	: public MolecularDynamicsDialogData
{
    Q_OBJECT

		public:

		///
    MolecularDynamicsDialog(QWidget* parent = NULL, const char* name = NULL );

		///
    virtual ~MolecularDynamicsDialog();

		///
		void writePreferences(INIFile& inifile) const;

		///
		void readPreferences(const INIFile& inifile);

		///
		const String& getFilename() const;
		
		///
		void setFilename(const String& filename);

		///
		bool getUseDistanceDependentDC() const;
		
		///
		void setUseDistanceDependentDC(bool use_dddc);

		///
		float getSimulationTime() const;

		///
		Size getNumberOfSteps() const;

		///
		void setNumberOfSteps(Size steps);
	
		///
		void setTimeStep(float time);

		///
		float getTimeStep() const;

		///
		bool useMicroCanonical() const;
		
		///
		void setMicroCanonical(bool state);

		///
		void setTemperature(float temperature);

		///
		float getTemperature() const;

		///
		bool saveImages() const;

		/// 
		String getDCDFile() const;

		///
		Size getStepsBetweenRefreshs() const;

		protected slots:
    virtual void browseParameterFiles();

		virtual void enableDCDFileSelected();

		void timeChanged();
};

} } // namespaces
#endif
