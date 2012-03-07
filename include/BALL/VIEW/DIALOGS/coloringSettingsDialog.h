// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//

#ifndef BALL_VIEW_DIALOGS_COLORINGSETTINGSDIALOG_H
#define BALL_VIEW_DIALOGS_COLORINGSETTINGSDIALOG_H

#ifndef BALL_COMMON_GLOBAL_H
# include <BALL/COMMON/global.h>
#endif

#ifndef BALL_VIEW_KERNEL_PREFERENCESENTRY
# include <BALL/VIEW/KERNEL/preferencesEntry.h>
#endif

#ifndef BALL_VIEW_KERNEL_COMMON_H
# include <BALL/VIEW/KERNEL/common.h>
#endif

#ifndef BALL_VIEW_DATATYPE_COLORRGBA_H
# include <BALL/VIEW/DATATYPE/colorRGBA.h>
#endif

#include <BALL/VIEW/UIC/ui_coloringSettingsDialog.h>

#include <vector>

namespace BALL
{
	class PTE;

	namespace VIEW
	{
		class ColorProcessor;


		/** Dialog for the molecular model coloring settings.
		 		It is inserted to the Preferences.
				\ingroup ViewDialogs
		*/
		class BALL_VIEW_EXPORT ColoringSettingsDialog 
			: public QWidget,
				public Ui_ColoringSettingsDialogData,
				public PreferencesEntry
		{ 
			Q_OBJECT

			public:

			/// Constructor
			ColoringSettingsDialog( QWidget* parent = 0, const char* name = "ColoringSettings", Qt::WFlags fl = 0 );

			/// Destructor
			virtual ~ColoringSettingsDialog() {}

			///
			virtual void applySettingsTo(ColorProcessor& cp) const;

			///
			virtual ColorProcessor* createColorProcessor(ColoringMethod method) const
				throw(Exception::InvalidOption);

			///
			virtual void getSettings(const ColorProcessor& cp);

			///
			virtual QWidget* getEntryFor(ColoringMethod method);

			///
			virtual vector<ColorRGBA> getColors(ColoringMethod method) const;

			///
			virtual void writePreferenceEntries(INIFile& inifile);

			///
			virtual void readPreferenceEntries(INIFile& inifile);

			protected slots:

			virtual void maxDistanceChanged();
			virtual void maxTFChanged();
			virtual void forceMaxValueChanged();
			virtual void forceMinValueChanged();

			protected:

			virtual void setDefaultValues_();
		};

} }

#endif
