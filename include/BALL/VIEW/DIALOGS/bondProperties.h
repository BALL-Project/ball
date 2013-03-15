// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
#ifndef BALL_VIEW_DIALOGS_BONDPROPERTIES_H
#define BALL_VIEW_DIALOGS_BONDPROPERTIES_H

#ifndef BALL_COMMON_GLOBAL_H
# include <BALL/COMMON/global.h>
#endif

#ifndef BALL_KERNEL_ATOM_H
# include <BALL/KERNEL/atom.h>
#endif

#include <BALL/VIEW/UIC/ui_bondProperties.h>

namespace BALL
{
	namespace VIEW
	{

		/** Dialog for showing and changing the properties of the bonds of an atom
				\ingroup  ViewDialogs
		*/
		class BALL_VIEW_EXPORT BondProperties 
			: public QDialog,
			  public Ui_BondPropertiesData
		{ 
			Q_OBJECT

		public:
			BondProperties(Atom* atom,  QWidget* parent = 0, const char* name = "BondProperties", 
										 bool modal = false, Qt::WindowFlags fl = 0 );
			~BondProperties();

		public slots:
			void bondSelected();
			void focusAtom();
			void focusPartner();

		private:
			Atom* atom_;
			QWidget* parent_;
		};

	} // namespace VIEW

} // namespace BALL

#endif // BALL_VIEW_DIALOGS_BONDPROPERTIES_H
