// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: coloringSettingsDialog.h,v 1.6 2003/11/19 22:44:05 amoll Exp $
//

#ifndef BALL_VIEW_DIALOGS_COLORINGSETTINGSDIALOG_H
#define BALL_VIEW_DIALOGS_COLORINGSETTINGSDIALOG_H

#include <BALL/VIEW/UIC/coloringSettingsDialogData.h>

#ifndef BALL_DATATYPE_STRING_H
 #include <BALL/DATATYPE/string.h>
#endif

#ifndef BALL_FORMAT_INIFILE_H
 #include <BALL/FORMAT/INIFile.h>
#endif

#ifndef BALL_KERNEL_PTE_H
 #include <BALL/KERNEL/PTE.h>
#endif

#include <vector>

#ifndef BALL_VIEW_DATATYPE_COLORGBA_H
 #include <BALL/VIEW/DATATYPE/colorRGBA.h>
#endif

#include <qtable.h>
#include <qcolor.h>
			
namespace BALL
{
	namespace VIEW
	{
		class ColorProcessor;

		class QColorTableItem : public QTableItem
		{
		public:
				QColorTableItem(QTable* t, EditType et, const ColorRGBA& color);
				
				void paint( QPainter *p, const QColorGroup &cg, const QRect &cr, bool selected );
				
				QWidget *createEditor() const;
				
				void setContentFromEditor( QWidget *w );

				void setColor(ColorRGBA color) { color_rgba_ = color;}
				const ColorRGBA& getColor() const { return color_rgba_;}
		protected:
				ColorRGBA color_rgba_;
		};


		class QColorTable : public QTable
		{
				Q_OBJECT

		public:
				QColorTable(QWidget* parent = 0);
				void setNamesTitle(const String& string);
				void setContent(const vector<String>& names, const vector<ColorRGBA>& colors);

		private slots:
				void recalcSum( int row, int col ){};
				QWidget* beginEdit(int row, int col, bool replace);
		private:
				void initTable();
				vector<ColorRGBA> colors_;
				vector<String>    names_;
		};

		/** Dialog for the molecular model coloring settings
				\ingroup ViewDialogs
		*/
		class ColoringSettingsDialog 
			: public ColoringSettingsDialogData
		{ 
			Q_OBJECT

			public:

			/// Constructor
			ColoringSettingsDialog( QWidget* parent = 0, const char* name = 0, WFlags fl = 0 );

			/// Destructor
			~ColoringSettingsDialog() {}

			///
			void writePreferences(INIFile& file)
				throw();

			///
			void fetchPreferences(const INIFile& file)
				throw();

			///
			void setDefaults()
				throw();

			///
			void applySettingsTo(ColorProcessor& cp) const
				throw();

			///
			vector<ColorRGBA> getElementColors() const
				throw();

			///
			vector<ColorRGBA> getResidueColors() const
				throw();

			protected slots:

			virtual void minimumOccupancyColorPressed();
			virtual void middleResidueColorPressed();
			virtual void lastResidueColorPressed();
			virtual void negativeChargeColorPressed();
			virtual void neutralChargeColorPressed();
			virtual void positiveChargeColorPressed();
			virtual void nullDistanceColorPressed();
			virtual void maxDistanceColorPressed();
			virtual void maxDistanceChanged();
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

			protected:

			void setNewColor_(QLabel* label, ColorRGBA& to)
				throw();

			QColorTable* element_table_;
			QColorTable* residue_table_;

			ColorRGBA minimum_occupancy_color_,
								middle_residue_color_,
								last_residue_color_,
								negative_charge_color_,
								neutral_charge_color_,
								positive_charge_color_,
								null_distance_color_,
								max_distance_color_,
								minimum_tf_color_,
								maximum_tf_color_,
								unassigned_tf_color_,
								maximum_occupancy_color_,
								unassigned_occupancy_color_,
								first_residue_color_,
								helix_color_,
								coil_color_,
								strand_color_,
								turn_color_;
		};

} }

#endif
