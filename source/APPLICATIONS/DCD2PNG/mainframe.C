// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: mainframe.C,v 1.6 2004/08/31 11:20:30 amoll Exp $
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
		for (; it != modular_widgets_.end(); ++it)
		{
			(*it)->fetchPreferences(in);
			(*it)->applyPreferences();
		}

		/*
		for (Position pos = 1; pos < 1000; pos++)
		{
			if (!in.hasEntry("BALLVIEW_PROJECT", "MolecularFile" + String(pos)))
			{
				break;
			}

			String molecular_file = in.getValue("BALLVIEW_PROJECT", "MolecularFile" + String(pos));
			display_properties_->enableCreationForNewMolecules(false);

			ifstream infile(molecular_file.c_str(), std::ios::in);
			TextPersistenceManager pm(infile);
			PersistentObject* po = pm.readObject();
			if (!RTTI::isKindOf<System>(*po))
			{
				setStatusbarText("Error while reading project file! Aborting...");
				Log.error() << "Error while reading project file! Aborting..." << std::endl;
				return;
			}
			infile.close();
			insert(*(System*) po);
		}
		*/

		try
		{
			for (Position p = 0; p < 9999999; p++)
			{
				if (!in.hasEntry("BALLVIEW_PROJECT", "Representation" + String(p))) break;

				String data_string = in.getValue("BALLVIEW_PROJECT", "Representation" + String(p));

				vector<String> string_vector;
				Size split_size;

				// Representation0=1;3 2 2 6.500000 0 0 [2]|Color|H
				// 								 ^ 																	System Number
				// 								         ^            							Model Settings
				// 								         							 ^            Composites numbers
				// 								         							     ^        Custom Color
				// 								         							     			^   Hidden Flag

				if (data_string.hasPrefix("CP:")) 
				{
					data_string = data_string.after("CP:");
					// we have a clipping plane
					split_size = data_string.split(string_vector);
					if (split_size != 4) continue;

					Representation* rep = new Representation();
					rep->setModelType(MODEL_CLIPPING_PLANE);
					rep->setProperty("AX", string_vector[0].toFloat());
					rep->setProperty("BY", string_vector[1].toFloat());
					rep->setProperty("CZ", string_vector[2].toFloat());
					rep->setProperty("D", string_vector[3].toFloat());

					insert(*rep);
					continue;
				}

				// split off information of system number
				split_size = data_string.split(string_vector, ";");
				Position system_pos = string_vector[0].toUnsignedInt();

				// split off between representation settings and composite numbers
				data_string = string_vector[1];
				vector<String> string_vector2;
				data_string.split(string_vector2, "[]");
				data_string = string_vector2[0];
				display_properties_->getSettingsFromString(data_string);

				// Composite positions
				data_string = string_vector2[1];
				data_string.split(string_vector2, ",");
				HashSet<Position> hash_set;
				for (Position p = 0; p < string_vector2.size(); p++)
				{
					hash_set.insert(string_vector2[p].toUnsignedInt());
				}

				Position pos = getCompositeManager().getNumberOfComposites() - 1;
				CompositeManager::CompositeIterator cit2 = getCompositeManager().begin();
				for (; cit2 != getCompositeManager().end() && system_pos != pos; cit2++)
				{
					pos--;
				}

				if (cit2 == getCompositeManager().end())  
				{
					setStatusbarText("Error while reading project file! Aborting...");
					Log.error() << "Error while reading project file! Aborting..." << std::endl;
					continue;
				}

				data_string = string_vector[1];
				if (data_string.has('|'))
				{
					data_string.split(string_vector2, "|");
					ColorRGBA color;
					color = string_vector2[1];
					display_properties_->setCustomColor(color);
				}

				getSelection().clear();
				Position current = 0;
				setSelection_(*cit2, hash_set, current);
				NewSelectionMessage* msg = new NewSelectionMessage();
				notify_(msg);
			
				display_properties_->apply();

				if (string_vector2.size() == 3 && string_vector2[2].has('H'))
				{
					Representation* rep = 0;
					PrimitiveManager::RepresentationsIterator pit = getPrimitiveManager().begin();
					for (; pit != getPrimitiveManager().end(); pit++)
					{
						rep = *pit;
					}

					rep->setHidden(true);
					rep->update(false);

#ifndef BALL_QT_HAS_THREADS
			 		RepresentationMessage* msg = new RepresentationMessage(*rep, RepresentationMessage::UPDATE);
 					notify_(msg);
#endif
				}
			}
		}
		catch(Exception::InvalidFormat e)
		{
			setStatusbarText("Error while reading project file! Aborting...");
			Log.error() << "Error while reading project file! Aborting..." << std::endl;
			Log.error() << e << std::endl;
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
