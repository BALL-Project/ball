// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: coloringSettingsDialog.h,v 1.18 2004/09/29 20:38:11 amoll Exp $
//

#ifndef BALL_VIEW_DIALOGS_COLORINGSETTINGSDIALOG_H
#define BALL_VIEW_DIALOGS_COLORINGSETTINGSDIALOG_H

#include <BALL/VIEW/UIC/coloringSettingsDialogData.h>

#ifndef BALL_VIEW_KERNEL_PREFERENCESENTRY
# include <BALL/VIEW/KERNEL/preferencesEntry.h>
#endif

#ifndef BALL_VIEW_KERNEL_COMMON_H
# include <BALL/VIEW/KERNEL/common.h>
#endif

#ifndef BALL_VIEW_DATATYPE_COLORRGBA_H
# include <BALL/VIEW/DATATYPE/colorRGBA.h>
#endif 

#include <qtable.h>
#include <vector>
			
namespace BALL
{
	class PTE;
	class INIFile;

	namespace VIEW
	{
		class ColorProcessor;

		class BALL_EXPORT QColorTableItem : public QTableItem
		{
			public:
				QColorTableItem(QTable* t, EditType et, const ColorRGBA& color);
				
				void paint( QPainter *p, const QColorGroup &cg, const QRect &cr, bool selected );
				
				void setColor(ColorRGBA color) { color_rgba_ = color;}
				
				const ColorRGBA& getColor() const { return color_rgba_;}

			protected:
				ColorRGBA color_rgba_;
		};


		class QColorTable : public QTable
		{
				Q_OBJECT

			public:
				QColorTable(QWidget* parent = 0)
					throw();
				
				void setNamesTitle(const String& string)
					throw();
				
				void setContent(const vector<String>& names, const vector<ColorRGBA>& colors)
					throw();

				void setColors(const vector<ColorRGBA>& colors)
					throw();

				const vector<ColorRGBA>& getColors() const
					throw() { return colors_;}

				const vector<String>& getNames() const
					throw() { return names_;}

			private slots:
				
				QWidget* beginEdit(int row, int col, bool replace);
				
			private:
				vector<ColorRGBA> colors_;
				vector<String>    names_;
				bool 							setting_content_;
		};


		/** Dialog for the molecular model coloring settings.
		 		It is inserted to the Preferences.
				\ingroup ViewDialogs
		*/
		class ColoringSettingsDialog 
			: public ColoringSettingsDialogData,
				public PreferencesEntry
		{ 
			Q_OBJECT

			public:

			/// Constructor
			ColoringSettingsDialog( QWidget* parent = 0, const char* name = 0, WFlags fl = 0 );

			/// Destructor
			~ColoringSettingsDialog() {}

			///
			void writePreferenceEntries(INIFile& inifile);

			///
			void readPreferenceEntries(const INIFile& inifile);

			/// 
			virtual void setDefaults(bool all = true)
				throw();
			
			/// Called when defaults is pressed in Preferences, calls setDefaults
			virtual void setDefaultValues()
				throw();

			///
			void applySettingsTo(ColorProcessor& cp) const
				throw();

			///
			ColorProcessor* createColorProcessor(ColoringMethod method) const
				throw(Exception::InvalidOption);

			///
			void getSettings(const ColorProcessor& cp)
				throw();

			///
			void showPage(ColoringMethod method)
				throw();

			///
			vector<ColorRGBA> getElementColors() const
				throw();

			///
			vector<ColorRGBA> getResidueColors() const
				throw();

			protected slots:

			void showPage_(int nr);
			virtual void minimumOccupancyColorPressed();
			virtual void middleResidueColorPressed();
			virtual void lastResidueColorPressed();
			virtual void negativeChargeColorPressed();
			virtual void neutralChargeColorPressed();
			virtual void positiveChargeColorPressed();
			virtual void nullDistanceColorPressed();
			virtual void maxDistanceColorPressed();
			virtual void minimumTFColorPressed();
			virtual void maximumTFColorPressed();
			virtual void unassignedTFColorPressed();
			virtual void maximumOccupancyColorPressed();
			virtual void unassignedOccupancyColorPressed();
			virtual void firstResidueColorPressed();
			virtual void helixColorPressed();
			virtual void turnColorPressed();
			virtual void strandColorPressed();
			virtual void coilColorPressed();
			virtual void forceMinColorPressed();
			virtual void forceMaxColorPressed();

			virtual void maxDistanceChanged();
			virtual void maxTFChanged();
			virtual void forceMaxValueChanged();
			virtual void forceMinValueChanged();

			protected:

			QColorTable* element_table_;
			QColorTable* residue_table_;
		};

} }

#endif
