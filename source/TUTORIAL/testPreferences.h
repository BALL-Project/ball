// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: testPreferences.h,v 1.2 2002/02/27 12:25:08 sturm Exp $
// view tutorial example
// ---------------------
// creation of a dialog

#include

namespace BALL
{

	namespace VIEW
	{

		class TestPreferences 
			: public QWidget
		{
			Q_OBJECT
				
			public:

				TestPreferences(QWidget *parent = NULL, const char *name = NULL )
					throw();
				virtual ~TestPreferences()
					throw();
				
				void fetchPreferences(INIFile& inifile)
					throw();
				void writePreferences(INIFile& inifile)
					throw();

				ColorRGBA getColor() const 
					throw();

			public slots:

				virtual void editColor()
					throw();

			private:

				QLabel *color_sample_;
				ColorRGBA custom_color_;
		};

	}

}
