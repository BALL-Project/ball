// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: testDialog.C,v 1.1.2.1 2003/01/07 13:23:16 anker Exp $
// view tutorial example
// ---------------------
// creation of a dialog

namespace BALL
{

	namespace VIEW
	{

		TestDialog::TestDialog(QWidget* parent, const char* name)
			: QDialog(parent, name, FALSE, 2768)
		{
			// set the caption of the dialog
			setCaption("TestDialog");
			// ... and register it. Don't forget that!
			ModularWidget::registerWidget(this);

			// create a button
			apply_button_ = new QPushButton(this, "apply_button");
			apply_button_->setGeometry(10, 25, 110, 30);
			apply_button_->setText( tr( "&Apply" ) );
			connect(apply_button_, SIGNAL(clicked()), 
							SLOT(applyButtonClicked()));
			
			// set a default colour
			color_.set("FF0000FF");
		}


		void TestDialog::initializeWidget(MainControl& main_control)
		{
			(main_control.initPopupMenu(MainControl::DISPLAY))->setCheckable(true);

			id_ = main_control.insertMenuEntry(MainControl::DISPLAY,
					"&Test Dialog", this, SLOT(openDialog()), CTRL+Key_T);   
		}


		void TestDialog::checkMenu(MainControl& main_control)
			throw()
		{
			(main_control.menuBar())->setItemChecked(id_, isVisible());
		}


		void TestDialog::finalizeWidget(MainControl& main_control)
			throw()
		{
			main_control.removeMenuEntry
				(MainControl::DISPLAY, "&Test Dialog", this,
				 SLOT(openDialog()), CTRL+Key_T);   
		}

	}

}

