// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: coloringSettingsDialog.h,v 1.2 2003/11/19 02:09:07 amoll Exp $
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
			
namespace BALL
{
	namespace VIEW
	{

		class QColorTableItem : public QTableItem
		{
		public:
				QColorTableItem(QTable* t, EditType et, const ColorRGBA& color);
				void paint( QPainter *p, const QColorGroup &cg, const QRect &cr, bool selected );
		protected:
				ColorRGBA color_;
		};


		class QColorTable : public QTable
		{
				Q_OBJECT

		public:
				QColorTable(QWidget* parent = 0);
				void setNamesName(const String& string);
				void setContent(const vector<String>& names, const vector<ColorRGBA>& colors);

		private slots:
				void recalcSum( int row, int col ){};

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

			QColorTable* element_table_;

		};

} }

#endif
