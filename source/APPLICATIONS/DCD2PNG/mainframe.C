// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: mainframe.C,v 1.2 2004/07/20 11:50:11 amoll Exp $
//

#include "mainframe.h"
#include <BALL/VIEW/RENDERING/POVRenderer.h>

#include <qapplication.h>

#include <BALL/VIEW/KERNEL/message.h>
#include <BALL/CONCEPT/textPersistenceManager.h>
#include <sstream>

#include <BALL/VIEW/DIALOGS/mainControlPreferences.h>
#include <BALL/VIEW/DIALOGS/preferences.h>

namespace BALL
{
	using namespace std;
	using namespace BALL::VIEW;

	Mainframe::Mainframe(QWidget* parent, const char* name)
		:	MainControl(parent, name, ".BALLView"),
			control_(0),
			dataset_control_(0),
			geometric_control_(0),
			display_properties_(0),
			molecular_structure_(0)
	{
	#ifdef BALL_VIEW_DEBUG
	Log.error() << "new Mainframe " << this << std::endl;
	#endif
		Log.remove(std::cout);
  	Log.remove(std::cerr);

		control_ = new MolecularControl(this, "Structures");
		CHECK_PTR(control_);

 		geometric_control_ = new GeometricControl(this, "Representations");
 		CHECK_PTR(geometric_control_);

		scene_ = new Scene(this, "3D View");
		CHECK_PTR(scene_);
		scene_->setMinimumSize(10, 10);
		setCentralWidget(scene_);

		display_properties_ = new DisplayProperties(this, "DisplayProperties");
		CHECK_PTR(display_properties_);

		file_dialog_ = new MolecularFileDialog(this, "MolecularFileDialog");
		CHECK_PTR(file_dialog_);

 		molecular_structure_ = new MolecularStructure(this, "MolecularStructure");
 		CHECK_PTR(molecular_structure_);


			// initialize own preferences tab
			initializePreferencesTab(*preferences_dialog_);

			// initialize all modular widgets 
			List<ModularWidget*>::Iterator it = modular_widgets_.begin(); 
			for (; it != modular_widgets_.end(); ++it)
			{
				(*it)->initializeWidget(*this);
				(*it)->initializePreferencesTab(*preferences_dialog_);
			}

			// check own preferences 
			preferences_dialog_->fetchPreferences(preferences_);

			// fetch own preferences tab
			fetchPreferences(preferences_);

			// apply on own preferences tab
			applyPreferences();

			// check menu entries, fetch and apply preferences
			for (it = modular_widgets_.begin(); it != modular_widgets_.end(); ++it)
			{
				(*it)->checkMenu(*this);
				(*it)->fetchPreferences(preferences_);
				(*it)->applyPreferences();
			}

	}

	Mainframe::~Mainframe()
		throw()
	{
	}

	void Mainframe::loadBALLViewProjectFile(const String& filename)
		throw()
	{
		INIFile in(filename);
		in.read();

		// check menu entries, fetch and apply preferences
		List<ModularWidget*>::Iterator it = modular_widgets_.begin(); 
		for (it = modular_widgets_.begin(); it != modular_widgets_.end(); ++it)
		{
			(*it)->fetchPreferences(in);
			(*it)->applyPreferences();
		}

		System* new_system = 0;
		new_system = getSelectedSystem();
		/*
		String molecular_file;
		if (in.hasEntry("BALLVIEW_PROJECT", "MolecularFile"))
		{
			molecular_file = in.getValue("BALLVIEW_PROJECT", "MolecularFile");
			display_properties_->enableCreationForNewMolecules(false);

			ifstream infile(molecular_file.c_str(), std::ios::in);
			TextPersistenceManager pm(infile);
			PersistentObject* po = pm.readObject();
			if (!RTTI::isKindOf<System>(*po))
			{
			}
			new_system = (System*) po;
			infile.close();
			insert(*new_system);
		}
		*/
		try
		{
			for (Position p = 0; p < 9999999; p++)
			{
				if (!in.hasEntry("BALLVIEW_PROJECT", "Representation" + String(p)))
				{
					break;
				}

				String data_string = in.getValue("BALLVIEW_PROJECT", "Representation" + String(p));


				vector<String> string_vector;
				Size split_size = data_string.split(string_vector, "[]");
				if (split_size == 1) 
				{
					Size split_size = data_string.split(string_vector);
					if (split_size != 4)
					{
						continue;
					}

					Representation* rep = new Representation();
					rep->setModelType(MODEL_CLIPPING_PLANE);
					rep->setProperty("AX", string_vector[0].toFloat());
					rep->setProperty("BY", string_vector[1].toFloat());
					rep->setProperty("CZ", string_vector[2].toFloat());
					rep->setProperty("D", string_vector[3].toFloat());

					insert(*rep);
					continue;
				}
				display_properties_->getSettingsFromString(data_string);

				if (new_system == 0) continue;
				
				data_string = string_vector[1];
				data_string.split(string_vector, ",");
				HashSet<Position> hash_set;
				for (Position p = 0; p < string_vector.size(); p++)
				{
					hash_set.insert(string_vector[p].toUnsignedInt());
				}
	
				getSelection().clear();
				Position current = 0;
				setSelection_(new_system, hash_set, current);
				NewSelectionMessage* msg = new NewSelectionMessage();
				notify_(msg);
			
				display_properties_->applyButtonClicked();
			}
		}
		catch(...)
		{
			setStatusbarText("Error while reading project file! Aborting...");
			Log.error() << "Error while reading project file! Aborting..." << std::endl;
			return;
		}
	
		getSelection().clear();
		NewSelectionMessage* msg = new NewSelectionMessage();
		notify_(msg);
 	
		fetchPreferences(in);

		if (in.hasEntry("BALLVIEW_PROJECT", "Camera"))
		{
			Stage stage;
			Camera c;
			if (!c.readFromString(in.getValue("BALLVIEW_PROJECT", "Camera")))
			{
				setStatusbarText("Could not read Camera position from project");
				Log.error() << "Could not read Camera position from project" << std::endl;
				return;
			}
			stage.setCamera(c);
			SceneMessage* msg = new SceneMessage(SceneMessage::UPDATE_CAMERA);
			msg->setStage(stage);
			notify_(msg);
		}

		display_properties_->enableCreationForNewMolecules(true);

		Scene::getInstance(0)->fetchPreferences(in);
		Scene::getInstance(0)->applyPreferences();
	}

	void Mainframe::setSelection_(Composite* c, HashSet<Position>& hash_set, Position& current)
		throw()
	{
		if (hash_set.has(current))
		{
			getSelection().insert(c);
			hash_set.erase(current);
		}

		current++;

		for (Position p = 0; p < c->getDegree() && hash_set.size() > 0; p++)
		{
			setSelection_(c->getChild(p), hash_set, current);
		}
	}

}
