// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: compositeProperties.h,v 1.4.16.1 2007/03/25 21:25:46 oliver Exp $
//

#ifndef BALL_VIEW_DIALOGS_COMPOSITEPROPERTIES_H
#define BALL_VIEW_DIALOGS_COMPOSITEPROPERTIES_H

#ifdef BALL_COMPILER_MSVC
# include <ui_compositeProperties.h>
#else
# include <BALL/VIEW/UIC/compositePropertiesData.h>
#endif


#ifndef BALL_CONCEPT_COMPOSITE
# include <BALL/CONCEPT/composite.h>
#endif

namespace BALL
{
	namespace VIEW
	{

		/** Dialog for showing and changing the properties of an Composite, e.g. an Atom or a Molecule.
				\ingroup  ViewDialogs
		*/
		class BALL_VIEW_EXPORT CompositeProperties 
			: public QDialog,
				public Ui_CompositePropertiesData
		{ 
			Q_OBJECT

		public:
			CompositeProperties(Composite* composite,  QWidget* parent = 0, const char* name = "CompositeProperties", 
													bool modal = FALSE, Qt::WFlags fl = 0 );
			~CompositeProperties();

		public slots:
			void accept();

		private:
			String getString_(float data) const
				throw();

			Composite* composite_;
		};

	} // namespace VIEW
} // namespace BALL

#endif // BALL_VIEW_DIALOGS_COMPOSITEPROPERTIES_H
