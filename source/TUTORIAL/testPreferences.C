// $Id: testPreferences.C,v 1.1 2002/01/12 21:22:45 anker Exp $
// view tutorial example
// ---------------------
// creation of a dialog

#include "testPreferences.h"

namespace BALL
{

	namespace VIEW
	{

		TestPreferences::TestPreferences
			(QWidget* parent, const char* name)
			throw()
			: QWidget(parent, name, 0)
		{
			QPushButton* edit_button 
				= new QPushButton(this, "edit_button");
			edit_button->setGeometry(110, 30, 80, 30);
			edit_button->setText( tr( "&Edit" ) );
			connect(edit_button, SIGNAL(clicked()), SLOT(editColor()));

			color_sample_ = new QLabel(this, "color_sample__label");
			color_sample_->setGeometry(20, 30, 70, 30);
			color_sample_->setFrameStyle( 50 );
			
			resize(380,210);
		}


		void TestPreferences::fetchPreferences(INIFile& inifile)
			throw()
		{
			if (inifile.hasEntry("WINDOWS", "Test::color"))
			{
				custom_color_.set(inifile.getValue("WINDOWS", "Test::color"));
				color_sample_->setBackgroundColor(QColor(custom_color_.getRed(), 
																								 custom_color_.getGreen(), 
																								 custom_color_.getBlue()));
			}
		}

		void TestPreferences::writePreferences(INIFile& inifile)
			throw()
		{
			inifile.setValue("WINDOWS", "Test::color", custom_color_);
		}

		ColorRGBA TestPreferences::getColor() const 
			throw()
		{ 
			return custom_color_; 
		}

		void BoundingBoxPreferences::editColor()
			throw()
		{
			color_sample_->setBackgroundColor(
					QColorDialog::getColor(color_sample_->backgroundColor()));

			ColorRGBA color;
			QColor qcolor = color_sample_->backgroundColor();
			color.set((float)qcolor.red() / 255.0,
								(float)qcolor.green() / 255.0,
								(float)qcolor.blue() / 255.0);
			
			custom_color_ = color;
		}

		void TestDialog::initializePreferencesTab(Preferences &preferences)
			throw()
		{
			test_preferences_ = new TestPreferences();
			preferences.insertTab(test_preferences_, "TestPreferences");
		}

		void TestDialog::applyPreferences(Preferences &)
			throw()
		{
			if test_preferences_ != 0)
			{
				color_ = test_preferences_->getColor();
			}
		}

		void TestDialog::finalizePreferencesTab(Preferences &preferences)
			throw()
		{
			if (test_preferences_ != 0)
			{
				preferences.removeTab(test_preferences_);
				delete test_preferences_;
			}
		}

		void TestDialog::fetchPreferences(INIFile& inifile)
			throw()
		{
			int x_pos = x();
			int y_pos = y();

			if (inifile.hasEntry("WINDOWS", "Test::x"))
			{
				x_pos = inifile.getValue("WINDOWS", "Test::x").toInt();
			}

			if (inifile.hasEntry("WINDOWS", "Test::y"))
			{
				y_pos = inifile.getValue("WINDOWS", "Test::y").toInt();
			}

			move(x_pos, y_pos);

			if (test_preferences_ != 0)
			{
				test_preferences_->fetchPreferences(inifile);
				color_ = test_preferences_->getColor();
			}
		}

		void TestDialog::writePreferences(INIFile& inifile)
			throw()
		{
			inifile.setValue("WINDOWS", "Test::x", String(x()));
			inifile.setValue("WINDOWS", "Test::y", String(y()));

			if (test_preferences_ != 0)
			{
				test_preferences_->writePreferences(inifile);
			}
		}

		void TestDialog::onNotify(Message *message)
			throw()
		{
			selection_.clear();

			if (RTTI::isKindOf<SelectionMessage>(*message))
			{
				selection_ = (RTTI::castTo<SelectionMessage>(*message))
												->getSelection();
			}

			if (selection_.empty())
			{
				apply_button_->setEnabled(false);
			}
			else
			{
				apply_button_->setEnabled(true);
			}
		}




	}

}
