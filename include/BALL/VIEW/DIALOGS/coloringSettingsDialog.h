// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: coloringSettingsDialog.h,v 1.22.16.1 2007/03/25 21:25:45 oliver Exp $
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

#include <QtGui/QTableWidget>
#include <vector>
			
namespace BALL
{
	class PTE;

	namespace VIEW
	{
		class ColorProcessor;

		/** Class for storing colors in a GUI table
				\ingroup ViewDialogs
		*/
		class BALL_VIEW_EXPORT QColorTable
			:	public QTableWidget,
				public PreferencesEntry::ExtendedPreferencesObject
		{
				Q_OBJECT

			public:
				QColorTable(QWidget* parent = 0, const char* name = 0);
				
				void setNamesTitle(const String& string);

				String getNamesTitle() const;
				
				void setContent(const vector<String>& names, const vector<ColorRGBA>& colors);

				void setColors(const vector<ColorRGBA>& colors);

				const vector<ColorRGBA>& getColors() const
					{ return colors_;}

				const vector<String>& getNames() const
					{ return names_;}

				///
				virtual bool getValue(String&) const;

				///
				virtual bool setValue(const String& value);

			private slots:
				
				void beginEdit(int row, int col);

				virtual void mousePressEvent(QMouseEvent* event);
				
			private:
				vector<ColorRGBA> colors_;
				vector<String>    names_;
				bool 							setting_content_;
		};


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

			virtual void setDefaultValues_();

			QColorTable* element_table_;
			QColorTable* residue_table_;
			QColorTable* chain_table_;
			QColorTable* molecule_table_;
		};

} }

#endif
