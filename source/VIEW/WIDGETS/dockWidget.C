// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: dockWidget.C,v 1.15 2003/11/01 09:27:41 oliver Exp $

#include <BALL/VIEW/WIDGETS/dockWidget.h>
#include <BALL/VIEW/KERNEL/mainControl.h>
#include <BALL/VIEW/KERNEL/common.h>
#include <qmenubar.h>
#include <qlabel.h>

using std::endl;

namespace BALL
{
	namespace VIEW
	{

		DockWidget::DockWidget()
			:	QDockWindow(QDockWindow::InDock, 0),
				ModularWidget("unnamed DockWidget"),
				guest_(0)
		{
		}
			
		DockWidget::DockWidget(const DockWidget& dw)
			:	QDockWindow(QDockWindow::InDock, 0),
				ModularWidget(dw.name()),
				guest_(0)
		{
		}
			
		
		DockWidget::DockWidget(QWidget* parent, const char* name)
			: QDockWindow(QDockWindow::InDock, parent),
				ModularWidget(name),
				guest_(0)
		{
			layout_ = new QVBoxLayout();
			caption_label_ = new QLabel(this, "caption_label");
			caption_label_->resize(90, 12);
			caption_label_->setSizePolicy( QSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed, 0, 0, false));
			caption_label_->setPaletteBackgroundColor( QColor( 255, 255, 127 ) );
			QFont caption_label_font(caption_label_->font());
			caption_label_font.setFamily( "Helvetica" );
			caption_label_font.setPointSize( 11 );
			caption_label_->setFont( caption_label_font ); 
			caption_label_->setFrameShape(QLabel::NoFrame);
			caption_label_->setAlignment(QLabel::AlignCenter);
			layout_->addWidget(caption_label_);
			Log.remove(std::cerr);
			boxLayout()->addLayout(layout_);
			Log.insert(std::cerr);

			setOrientation(Qt::Vertical);

			if (name != 0) 
			{ 
				setCaption(name);
				setName(name);
				caption_label_->setText(name);
			}
			else 
			{
				setName("DockWidget");
			}

			registerWidget(this);
		}

		void DockWidget::setGuest(QWidget& guest)
		{
			QPoint p;
			guest.reparent(this, p, true);
			guest.resize(120,100);
			guest.setSizePolicy( QSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding, 0, 0, false));
			layout_->addWidget(&guest);
			guest_ = &guest;
			setMinimumSize(20, 20);
			setCloseMode(QDockWindow::Always);
			setResizeEnabled(true);
		}

		void DockWidget::initializeWidget(MainControl& main_control)
			throw()
		{
			window_menu_entry_id_ = 
				main_control.insertMenuEntry(MainControl::WINDOWS, getIdentifier(), this, SLOT(switchShowWidget()));
			getMainControl()->menuBar()->setItemChecked(window_menu_entry_id_, true);
			connect(this, SIGNAL(visibilityChanged(bool)), this, SLOT(setWindowsMenuEntry(bool)));
		}

		void DockWidget::finalizeWidget(MainControl& main_control)
			throw()
		{
			main_control.removeMenuEntry(MainControl::WINDOWS, getIdentifier(), this, SLOT(switchShowWidget()));
		}


		void DockWidget::writePreferences(INIFile& inifile)
			throw()
		{
			ModularWidget::writePreferences(inifile);

			inifile.insertValue("WINDOWS", getIdentifier() + "::docked", String(area() != 0));
			if (!area()) return;

			Dock dock;
			Index index, offset;
			bool newline;
			getMainControl()->getLocation(this, dock, index, newline, offset);
			inifile.insertValue("WINDOWS", getIdentifier() + "::dockarea", String(dock));
			inifile.insertValue("WINDOWS", getIdentifier() + "::dockindex", String(index));
			inifile.insertValue("WINDOWS", getIdentifier() + "::dockoffset", String(offset));
			inifile.insertValue("WINDOWS", getIdentifier() + "::docknewline", String(newline));
		}

		void DockWidget::fetchPreferences(INIFile & inifile)
			throw()
		{
			if (inifile.hasEntry("WINDOWS", getIdentifier() + "::docked"))
			{
				if (!inifile.getValue("WINDOWS", getIdentifier() + "::docked").toUnsignedInt())
				{
					undock();
					show();
				}
				else
				{
					if (inifile.hasEntry("WINDOWS", getIdentifier() + "::dockarea")  &&
							inifile.hasEntry("WINDOWS", getIdentifier() + "::dockindex") &&
							inifile.hasEntry("WINDOWS", getIdentifier() + "::dockoffset")    &&
							inifile.hasEntry("WINDOWS", getIdentifier() + "::docknewline"))
					{
						Dock dock = (Dock) inifile.getValue("WINDOWS", getIdentifier() + "::dockarea").toUnsignedInt();
						Index index = inifile.getValue("WINDOWS", getIdentifier() + "::dockindex").toUnsignedInt();
						Index offset = inifile.getValue("WINDOWS", getIdentifier() + "::dockoffset").toUnsignedInt();
						bool newline = inifile.getValue("WINDOWS", getIdentifier() + "::docknewline").toUnsignedInt();
						getMainControl()->moveDockWindow(this, dock, newline, index, offset);
					}
				}
			}

			ModularWidget::fetchPreferences(inifile);
			if (inifile.hasEntry("WINDOWS", getIdentifier() + "::height"))
			{
				Index width = inifile.getValue("WINDOWS", getIdentifier() + "::width").toUnsignedInt();
				Index height = inifile.getValue("WINDOWS", getIdentifier() + "::height").toUnsignedInt();

				if (guest_)
				{
					guest_->resize(width, height);
				}
			}

			if (inifile.hasEntry("WINDOWS", getIdentifier() + "::on"))
			{	
				if (!inifile.getValue("WINDOWS", getIdentifier() + "::on").toUnsignedInt())
				{
					switchShowWidget();
				}
			}
			else 
			{
				if (!default_visible_)
				{
					switchShowWidget();
				}
			}

			if (!BALL_VIEW_DOCKWINDOWS_SHOW_LABELS) caption_label_->hide();
		}

		void DockWidget::switchShowWidget()
			throw()
		{
			if (window_menu_entry_id_ == -1) return;

			if (!getMainControl()) 
			{
				Log.error() << "Problem in " << __FILE__ << __LINE__ << std::endl;
				return;
			}

			QMenuBar* menu = getMainControl()->menuBar();
			if (menu->isItemChecked(window_menu_entry_id_))
			{
				hide();
				menu->setItemChecked(window_menu_entry_id_, false);
			}
			else
			{
				show();
				menu->setItemChecked(window_menu_entry_id_, true);
			}
		}

		void DockWidget::setWindowsMenuEntry(bool state) 
		{
			QMenuBar* menu = getMainControl()->menuBar();
			if (menu->isItemChecked(window_menu_entry_id_) != isVisible())
			{
				menu->setItemChecked(window_menu_entry_id_, state);
			}
		}

		void DockWidget::applyPreferences(Preferences & /* preferences */)
			throw()
		{
			if (!BALL_VIEW_DOCKWINDOWS_SHOW_LABELS) caption_label_->hide();
			else caption_label_->show();
		}

	} // namespace VIEW 

} // namespace BALL
