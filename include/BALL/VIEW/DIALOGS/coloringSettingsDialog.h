// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: coloringSettingsDialog.h,v 1.21 2005/02/24 15:52:26 amoll Exp $
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

			/// Called when defaults is pressed in Preferences, calls setDefaults
			virtual void setDefaultValues(bool all = false)
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
			QWidget* getEntryFor(ColoringMethod method)
				throw();

			///
			vector<ColorRGBA> getColors(ColoringMethod method) const
				throw();

			///
			void writeColorTable(const QColorTable& table, INIFile& inifile);

			///
			void readColorTable(QColorTable& table, const INIFile& inifile);

			protected slots:

			void middleResidueColorPressed() 	{ chooseColor(middle_residue_label); }
			void lastResidueColorPressed() 		{ chooseColor(last_residue_label); }
			void firstResidueColorPressed() 	{ chooseColor(first_residue_label); }

			void negativeChargeColorPressed() { chooseColor(negative_charge_label); }
			void neutralChargeColorPressed() 	{ chooseColor(neutral_charge_label); }
			void positiveChargeColorPressed() { chooseColor(positive_charge_label);}
			
			void nullDistanceColorPressed() 	{ chooseColor(null_distance_label); }
			void maxDistanceColorPressed() 		{ chooseColor(max_distance_label); }
			
			void minimumTFColorPressed() 			{ chooseColor(minimum_tf_label); }
			void maximumTFColorPressed() 			{ chooseColor(maximum_tf_label); }
			void unassignedTFColorPressed() 	{ chooseColor(unassigned_tf_label); }
			
			void minimumOccupancyColorPressed() 		{ chooseColor(minimum_o_label); }
			void maximumOccupancyColorPressed() 		{ chooseColor(maximum_o_label); }
			void unassignedOccupancyColorPressed() 	{ chooseColor(unassigned_o_label); }
			
			void helixColorPressed() 		{ chooseColor(helix_color_label); }
			void turnColorPressed() 		{ chooseColor(turn_color_label); }
			void strandColorPressed() 	{ chooseColor(strand_color_label);}
			void coilColorPressed() 		{ chooseColor(coil_color_label); }
			
			void forceMaxColorPressed() { chooseColor(force_max_color_label); }
			void forceMinColorPressed() { chooseColor(force_min_color_label); }
			
			// residue types ===============================
			void acidicColorPressed() 			{ chooseColor(acidic_color_label); }
			void aromaticColorPressed() 		{ chooseColor(aromatic_color_label); }
			void basicColorPressed() 				{ chooseColor(basic_color_label); }
			void otherColorPressed() 				{ chooseColor(other_color_label); }
			void polarColorPressed() 				{ chooseColor(polar_color_label); }
			void hydrophobicColorPressed() 	{ chooseColor(hydrophobic_color_label); }
			// ---------------------------------------------

			virtual void maxDistanceChanged();
			virtual void maxTFChanged();
			virtual void forceMaxValueChanged();
			virtual void forceMinValueChanged();

			protected:

			QColorTable* element_table_;
			QColorTable* residue_table_;
			QColorTable* chain_table_;
		};

} }

#endif
