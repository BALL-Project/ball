/* TRANSLATOR BALL::VIEW::SequenceControl

		Necessary for lupdate.
*/

// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
#include <BALL/VIEW/WIDGETS/sequenceControl.h>
#include <BALL/VIEW/KERNEL/mainControl.h>
#include <BALL/VIEW/KERNEL/message.h>
#include <BALL/VIEW/DIALOGS/compositeProperties.h>
#include <BALL/KERNEL/system.h>

#include <QtGui/QPushButton>
#include <QtGui/QLineEdit>
#include <QtGui/QMessageBox>
#include <QtGui/QToolTip>
#include <QtGui/QGridLayout>
#include <QtGui/QCompleter>

#include <set>

namespace BALL
{
	namespace VIEW
	{

		SequenceControl::SequenceControl(QWidget* parent, const char* name)
				: DockWidget(parent, name),
				  PreferencesEntry()
		{
		#ifdef BALL_VIEW_DEBUG
			Log.error() << "new SequenceControl " << this << std::endl;
		#endif

			//TODO
			QTabWidget* tab = new QTabWidget(this);
			ui_.setupUi(tab);
			setGuest(*tab);

			setINIFileSectionName("SEQUENCECONTROL");
			setObjectName(name);
			// for preferences	
			registerWidgets_();
			// for actions, icons...
			ModularWidget::registerWidget(this);

			// signals and slots connections


		/*	QGridLayout* glay = getGuestLayout();
			QGridLayout* lay = new QGridLayout();
			glay->addLayout(lay, 2, 0);

			listview->setObjectName("SequenceControlList");
			listview->headerItem()->setText(0, tr("Name") + " ["+ tr("highlight") + "]");
			listview->headerItem()->setText(1, tr("checked"));
			listview->headerItem()->setText(2, tr("Sequence"));
			listview->resizeColumnToContents(0);
			listview->resizeColumnToContents(2);
			lay->addWidget(listview,0, 0, 1, -1);

			resize(200,400);
*/
			registerWidget(this);

			//buildContextMenu_();
		}

		SequenceControl::~SequenceControl()
		{
		#ifdef BALL_VIEW_DEBUG
			Log.error() << "Destroying SequenceControl " << this << std::endl;
		#endif
		}

		void SequenceControl::initializeWidget(MainControl& main_control)
		{
			// add specific menu entries
			// TODO

			DockWidget::initializeWidget(main_control);

			registerForHelpSystem(this, "sequenceControl.html");

		}

		void SequenceControl::fetchPreferences(INIFile& inifile)
		{ //TODO
			DockWidget::fetchPreferences(inifile);
			//TODO
			if (inifile.hasEntry("SEQUENCECONTROL", "RegularExpressions"))
			{
				vector<String> fields;
				Size size = inifile.getValue("SEQUENCECONTROL", "RegularExpressions").split(fields, "|");
				for (Position p = 0; p < size; p++)
				{
				//	selector_edit_->addItem(fields[p].c_str());
				}
			}
		}

		void SequenceControl::writePreferences(INIFile& inifile)
		{
			//TODO
			inifile.appendSection("SEQUENCECONTROL");
			//inifile.insertValue("SEQUENCECONTROL", "ShowSS", show_ss_);

			DockWidget::writePreferences(inifile);
		}


	} // namespace VIEW
} // namespace BALL
