// $Id: mainControl.C,v 1.6 2000/11/05 14:31:38 hekl Exp $

// this is required for QMenuItem
#define INCLUDE_MENUITEM_DEF


#include <BALL/VIEW/GUI/KERNEL/mainControl.h>
#include <BALL/VIEW/GUI/WIDGETS/modularWidget.h>
#include <BALL/VIEW/GUI/WIDGETS/scene.h>

#include <qmenudata.h>
#include <qwidget.h>
#include <qmenubar.h>
#include <qpopupmenu.h>

using std::istream;
using std::ostream;



namespace BALL
{

	namespace VIEW
	{

	  MainControl::OptionsFileError::OptionsFileError(const char* file, int line, const string& data)
			: Exception::GeneralException(file, line, string("OptionsFileError"), data)
		{
    }
  
		MainControl::MainControl
			(QWidget* parent, const char* name, String /* option_filename */)
			:	QMainWindow(parent, name),
				composite_map_(),
				descriptor_map_(),
				descriptors_(),
				options_()
		{
			//			if (!options_.readOptionFile(option_filename))
			//			{
			//					throw ::BALL::VIEW::MainControl::OptionsFileError(__FILE__, __LINE__, "file error.");			
			//			}
		}
		
		MainControl::MainControl(const MainControl& main_control)
			:	QMainWindow(main_control.parentWidget(), main_control.name()),
				ConnectionObject(main_control),
				Embeddable(main_control),
				composite_map_(main_control.composite_map_),
				descriptor_map_(main_control.descriptor_map_),
				descriptors_(main_control.descriptors_),
				options_(main_control.options_),
				modular_widgets_(main_control.modular_widgets_)
		{	
		}
		
		MainControl::~MainControl()
		{
			#ifdef BALL_VIEW_DEBUG
				cout << "Destructing object " << (void *)this 
					<< " of class " << RTTI::getName<MainControl>() << endl;
			#endif 

			destroy();
		}

		QPopupMenu* MainControl::initPopupMenu(int ID)
		{
			QPopupMenu* menu = 0;
			QMenuItem* item = menuBar()->findItem(ID);
			if ((item == 0) || (item->popup() == 0))
			{
				menu = new QPopupMenu(this);
				CHECK_PTR(menu);

				connect(menu,
								SIGNAL(aboutToShow()),
								this,
								SLOT(checkMenus()));

				Log.info() << "new menu entry: " << ID << endl;
				int max_id = menuBar()->count();
				switch (ID)
				{
					case FILE:
						menuBar()->insertItem("&File", menu, FILE, 0);
						break;
   				case FILE_IMPORT:
						initPopupMenu(MainControl::FILE)
							->insertItem("&Import File", menu, FILE_IMPORT);
						break;
					case EDIT:
						menuBar()->insertItem("&Edit", menu, EDIT, (1 <= max_id) ? 1 : -1);
						break;
					case BUILD:
						menuBar()->insertItem("&Build", menu, BUILD, (2 <= max_id) ? 2 : -1);
						break;
					case DISPLAY:
						menuBar()->insertItem("&Display", menu, DISPLAY, (3 <= max_id) ? 3 : -1);
						break;
					case TOOLS:
						menuBar()->insertItem("&Tools", menu, TOOLS, (4 <= max_id) ? 4 : -1);
						break;
					case USER:
						menuBar()->insertItem("&User", menu, USER, (5 <= max_id) ? 5 : -1);
						break;

					case HELP:
						menuBar()->insertSeparator();
						menuBar()->insertItem("&Help", menu, HELP, -1);
						break;
					default:
						delete menu;
						menu = 0;
				}
			}
			else 
			{
				// return the existing popup menu
				menu = item->popup();
			}

			return menu;
		}

		void MainControl::clear()
		{
			//			list<CompositeDescriptor *>::iterator it = descriptors_.begin();

			//			for (; it != descriptors_.end(); ++it)
			{
				// BAUSTELLE:
				// Segmentation Fault nach Beenden des Programms, obwohl ueberhaupt
				// keine Descriptoren angelegt wurden

				//				delete *it;
			}

			//			descriptors_.destroy();
			//			composite_map_.destroy();
			//			descriptor_map_.destroy();
		}
			
		void MainControl::destroy()
		{
			clear();
		}

		void MainControl::show()
		{
			cerr << "MainControl::show()  list.size() = " << modular_widgets_.size() << endl;

			// initialize all modular widgets 
			List<ModularWidget*>::Iterator it = modular_widgets_.begin(); 
			for (; it != modular_widgets_.end(); ++it)
			{
				registerConnectionObject(**it);
				(*it)->initializeWidget(*this);
			}
			for (it = modular_widgets_.begin(); it != modular_widgets_.end(); ++it)
			{
				(*it)->checkMenu(*this);
			}
			
			// own menu entries
			insertPopupMenuSeparator(MainControl::FILE);
			insertMenuEntry(MainControl::FILE, "E&xit", qApp, SLOT(quit()), CTRL+Key_X);	

			QMainWindow::show();
		}

		void MainControl::checkMenus()
		{
			// checks all modular widgets 
			List<ModularWidget*>::Iterator it = modular_widgets_.begin(); 
			for (it = modular_widgets_.begin(); it != modular_widgets_.end(); ++it)
			{
				(*it)->checkMenu(*this);
			}
		}

		bool MainControl::remove(const Composite& composite, bool sent_message)
		{
			ListIteratorHashMap::Iterator map_iterator 
				= composite_map_.find((void*)&composite);

			if (map_iterator != composite_map_.end())
			{
				if (sent_message)
				{
					// send a message that the composite descriptor
					// will be deleted
					RemovedCompositeMessage *message = new RemovedCompositeMessage;
					message->setComposite(composite);
					message->setDeletable(true);
					notify_(message);
				}
 
				// delete all information concerning the composite
				CompositeDescriptor* descriptor = *(map_iterator->second);

				// remove Iterator from DescriptorList
				descriptors_.erase(map_iterator->second);

				// remove Composite from Composite2CompositeDescriptorIterator HashMap
				composite_map_.erase((void*)&composite);
				
				// remove CompositeDescriptor from CompositeDescriptor2CompositeDescriptorIterator HashMap
				descriptor_map_.erase((void *)descriptor);

				// delete CompositeDescriptor
				delete descriptor;

				return true;
			}

			return false;
		}

		bool MainControl::remove
			(const CompositeDescriptor& composite_descriptor)
		{
			ListIteratorHashMap::Iterator map_iterator 
				= descriptor_map_.find((void*)&composite_descriptor);

			if (map_iterator != descriptor_map_.end())
			{
				composite_map_.erase((void*)composite_descriptor.getComposite());

				descriptor_map_.erase((void*)&composite_descriptor);

				delete (CompositeDescriptor*)map_iterator->first;

				descriptors_.erase(map_iterator->second);

				return true;
			}

			return false;
		}

		bool MainControl::setName
			(const Composite& composite, const String& s)
		{
			ListIteratorHashMap::Iterator map_iterator = 
				composite_map_.find((void*)&composite);

			if (map_iterator != composite_map_.end())
			{
				(*map_iterator->second)->setName(s);

				return true;
			}

			return false;
		}

		String* MainControl::getName(const Composite& composite)
		{
			ListIteratorHashMap::Iterator map_iterator = 
				composite_map_.find((void*)&composite);

			if (map_iterator != composite_map_.end())
			{
				return &((*map_iterator->second)->getName());
			}

			return 0;  
		}

		CompositeDescriptor* MainControl::getDescriptor
			(const String& name)
		{
			list<CompositeDescriptor *>::iterator it = descriptors_.begin();

			for (; it != descriptors_.end(); ++it)
			{
				if ((*it)->getName() == name)
				{
					return (*it);
				}
			}
			
			return 0;
		}

		bool MainControl::setCenter
			(const Composite& composite, const Vector3& v)
		{
			ListIteratorHashMap::Iterator map_iterator =
				composite_map_.find((void*)&composite);

			if (map_iterator != composite_map_.end())
			{
				(*map_iterator->second)->setCenter(v);

				return true;
			}

			return false;
		}

		Vector3* MainControl::getCenter
			(const Composite& composite)
		{
			ListIteratorHashMap::Iterator map_iterator =
				composite_map_.find((void*)&composite);

			if (map_iterator != composite_map_.end())
			{
				return &(*map_iterator->second)->getCenter();
			}

			return 0;
		}

		CompositeDescriptor* MainControl::getDescriptor
			(const Vector3& center)
		{
			list<CompositeDescriptor *>::iterator it = descriptors_.begin();

			for (; it != descriptors_.end();++it)
			{
				if ((*it)->getCenter() == center)
				{
					return (*it);
				}
			}
			
			return 0;
		}

		bool MainControl::update(const Composite& composite)
		{
			ListIteratorHashMap::Iterator map_iterator =
				composite_map_.find((void*)&composite);

			if (map_iterator != composite_map_.end())
			{
				(*map_iterator->second)->update();

				return true;
			}

			return false;
		}

		void MainControl::updateAll()
		{
			list<CompositeDescriptor *>::iterator it = descriptors_.begin();

			for (; it != descriptors_.end();++it)
			{
				(*it)->update();
			}

			
			// BAUSTELLE!!
			// update scene
			SceneMessage *scene_message = new SceneMessage;
			scene_message->updateOnly();
			scene_message->setDeletable(true);
			notify_(scene_message); 
		}

	  Options *MainControl::getOptions()
    {
			return &options_;
    }

		void MainControl::insert
		  (Composite* composite, const String& name,
			 const Vector3& center)
		{
        // create a new composite descriptor and enable the self deletion mechanism
        // for the composite
        List<CompositeDescriptor*>::Iterator list_iterator = descriptors_.end();

        CompositeDescriptor* composite_descriptor =
          new CompositeDescriptor();

        composite_descriptor->setComposite(composite, true);
        composite_descriptor->setName(name);
				composite_descriptor->setCenter(center);

        descriptors_.push_back(composite_descriptor);
        list_iterator = descriptors_.end();
        list_iterator--;

        composite_map_.
          insert(ListIteratorHashMap::ValueType((void *)composite, list_iterator));

        descriptor_map_.
          insert(ListIteratorHashMap::ValueType((void *)composite_descriptor, list_iterator));
 		}	

		void MainControl::onNotify(Message *message)
    {
			if (RTTI::isKindOf<NewCompositeMessage>(*message))
			{
				NewCompositeMessage* new_message 
				  = RTTI::castTo<NewCompositeMessage>(*message);

				insert(new_message->getComposite(),
							 new_message->getCompositeName());
 			}
			else if (RTTI::isKindOf<RemovedCompositeMessage>(*message))
			{
				RemovedCompositeMessage *composite_message 
					= RTTI::castTo<RemovedCompositeMessage>(*message);

				remove(*composite_message->getComposite(), false);
			}
			else if (RTTI::isKindOf<ChangedCompositeMessage>(*message))
			{
				ChangedCompositeMessage *composite_message
					= RTTI::castTo<ChangedCompositeMessage>(*message);

				update(composite_message->getComposite()->getRoot());
			}
    }

		bool MainControl::isInserted(const Composite& composite) const
		{
			return (composite_map_.find((void*)&composite)
							!= composite_map_.end());
		}

		bool MainControl::isInserted
			(const CompositeDescriptor& composite_descriptor) const
		{
			return (descriptor_map_.find((void*)&composite_descriptor)
							!= descriptor_map_.end());
		}

		bool MainControl::isValid() const
		{
			List<CompositeDescriptor*>::ConstIterator it = descriptors_.begin();

			for ( ; it != descriptors_.end();++it)
			{
				if ((*it)->isValid() == false)
				{
					return false;
				}
			}

			return true;
		}

		void MainControl::dump
			(ostream& s, Size depth) const
		{
			BALL_DUMP_STREAM_PREFIX(s);

			BALL_DUMP_DEPTH(s, depth);
			BALL_DUMP_HEADER(s, this, this);

			List<CompositeDescriptor*>::ConstIterator it = descriptors_.begin();

			for ( ; it != descriptors_.end(); ++it)
			{
				(*it)->dump(s, depth + 1);
			}

			BALL_DUMP_STREAM_SUFFIX(s);     
		}

		void MainControl::read(istream &/* s */)
		{
			throw ::BALL::Exception::NotImplemented(__FILE__, __LINE__);
		}

		void MainControl::write(ostream &/*s*/) const
		{
			throw ::BALL::Exception::NotImplemented(__FILE__, __LINE__);
		}

		CompositeDescriptor*  MainControl::insert_
			(const Composite& composite, const String& s, const Vector3& v)
		{
			if (composite_map_.has((void *)&composite) == true)
			{
				return 0;
			}

			List<CompositeDescriptor*>::Iterator list_iterator = descriptors_.end();

			CompositeDescriptor* composite_descriptor =
				new CompositeDescriptor();

			composite_descriptor->setComposite(&composite);
			composite_descriptor->setName(s);
			composite_descriptor->setCenter(v);

			descriptors_.push_back(composite_descriptor);
			list_iterator = descriptors_.end();
			list_iterator--;

			composite_map_.insert(ListIteratorHashMap::ValueType((void *)&composite, list_iterator));

			descriptor_map_.insert(ListIteratorHashMap::ValueType((void *)composite_descriptor, list_iterator));

			return composite_descriptor;
		}

		CompositeDescriptor* MainControl::insert_
			(CompositeDescriptor& composite_descriptor, bool deep)
		{
			if (descriptor_map_.has((void*)&composite_descriptor) == true && deep == false)
			{
				return 0;
			}

			List<CompositeDescriptor*>::Iterator list_iterator;

			CompositeDescriptor* new_composite_descriptor 
				= (CompositeDescriptor*)composite_descriptor.create(deep);

			descriptors_.push_back(new_composite_descriptor);
			list_iterator = descriptors_.end();
			list_iterator--;

			composite_map_.insert(ListIteratorHashMap::ValueType((void*)new_composite_descriptor->getComposite(), list_iterator));

			descriptor_map_.insert(ListIteratorHashMap::ValueType((void*)new_composite_descriptor, list_iterator));

			return new_composite_descriptor;
		}

		// VIEW automatic module registration
		MainControl* MainControl::getMainControl(const QWidget* widget)
		{
			QWidget* parent = widget->parentWidget();
			while ((parent != 0) && (parent->parentWidget() != 0))
			{
				parent = parent->parentWidget();
			}

			MainControl* mc = 0;
			if (parent != 0)
			{
				// check whether the top-level widget
				// is a MainControl
				mc = dynamic_cast<MainControl*>(parent);
			}
			else 
			{
				// try whether the widget itself is the main control
				mc = dynamic_cast<MainControl*>(const_cast<QWidget*>(widget));
			}

			// BAUSTELLE
			cout << "Top level widget : mc = " << mc << endl;
			// 
			return mc;
		}
		
		int MainControl::current_id_ = 15000;

		int MainControl::getNextID()
		{
			return current_id_++;
		}

    int MainControl::insertMenuEntry
			(int ID, const String& name, const QObject* receiver, const char* slot, int accel, int entry_ID)
		{
			QMenuBar* menu_bar = menuBar();
			if (menu_bar != 0)
			{
				// enable the corresponding popup menu
				menu_bar->setItemEnabled(ID, true);
				//
				QPopupMenu* popup = initPopupMenu(ID);
				if (popup == 0)
				{
					Log.error() << "MainControl::insertMenuEntry: cannot find popup menu for ID " << ID << endl;
				}
				else
				{
					// insert the menu entry
					if (entry_ID == -1)
					{
						entry_ID = getNextID();
					}

					popup->insertItem(name.c_str(), receiver, slot, accel, entry_ID);
					return entry_ID;
				}
			}

			return -1;
		}
		
		void MainControl::insertPopupMenuSeparator
		  (int ID)
		{
			QMenuBar* menu_bar = menuBar();
			if (menu_bar != 0)
			{
				// enable the corresponding popup menu
				menu_bar->setItemEnabled(ID, true);
				//
				QPopupMenu* popup = initPopupMenu(ID);
				if (popup == 0)
				{
					Log.error() << "MainControl::insertMenuEntry: cannot find popup menu for ID " << ID << endl;
				}
				else
				{
					popup->insertSeparator();
				}
			}
		}

		void MainControl::removeMenuEntry
			(int /* ID */, const String& /* name */, 
			 const QObject* /* receiver */, const char* /* slot */, 
			 int /* accel */, int /* entry_ID */)
		{
			// BAUSTELLE
		}

		void MainControl::addModularWidget(ModularWidget* widget)
		{
			cerr << "MainControl::addModularWidget(" << widget << ")" << endl;
			modular_widgets_.push_back(widget);
		}

		void MainControl::removeModularWidget(ModularWidget* widget)
		{
			cerr << "MainControl::addModularWidget(" << widget << ")" << endl;
			modular_widgets_.remove(widget);
		}
 
  
#		ifdef BALL_NO_INLINE_FUNCTIONS
#			include <BALL/VIEW/GUI/KERNEL/mainControl.iC>
#		endif

	} // namespace VIEW

} // namespace BALL
