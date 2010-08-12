#include <BALL/COMMON/global.h>

#include <OpenSimPlugin.h>
#include <OpenSimPluginConfiguration.h>

#include <BALL/SYSTEM/path.h>
#include <BALL/SYSTEM/systemCalls.h>

#include <BALL/KERNEL/forEach.h>

#include <BALL/VIEW/KERNEL/mainControl.h>
#include <BALL/VIEW/KERNEL/shortcutRegistry.h>

#include <BALL/VIEW/KERNEL/message.h>
#include <BALL/VIEW/KERNEL/threads.h>
#include <BALL/VIEW/PLUGIN/inputDevPluginHandler.h>

#include <BALL/DATATYPE/hashMap.h>

#include <QtGui/QMessageBox>

Q_EXPORT_PLUGIN2(pluginOpenSimPlugin, BALL::VIEW::OpenSimPlugin)

namespace BALL
{
	namespace VIEW
	{
		OpenSimPlugin::OpenSimPlugin()
			: QObject(VIEW::getMainControl()),
				ModularWidget(),
				cmdThread_(new HandleCommandFromBALLViewThread(this)),
				pluginrwLock_(),
				settings_(new OpenSimPluginConfiguration(NULL)),
				is_active_(false),
				server_(0),
				icon_(":opensim_logo.png"),
				molStructPlugin_ (new MolecularStructureContainer()),
				molModelingPlugin_(new MolecularModelingContainer()),
				molDynamicsPlugin_(new MolecularDynamicsContainer())
		{
			registerWidget(this);

			remote_host_ = (String)(settings_->remote_address->text());
			remote_port_ = settings_->remote_port->text().toInt();
			local_host_  = (String)(settings_->local_address->text());
			local_port_  = settings_->local_port->text().toInt();

			// try to implement some sane defaults for the settings
			if (local_host_ == "")
				local_host_ = "0.0.0.0";
			if (local_port_ == 0)
				local_port_ = 4711;
			if (remote_port_ == 0)
				remote_port_ = 4712;

			settings_->setLocalServer ( local_host_,  local_port_);
			settings_->setRemoteServer(remote_host_, remote_port_);

			connect(settings_, SIGNAL(accepted()), this, SLOT(settingsChanged()));
		}

		OpenSimPlugin::~OpenSimPlugin()
		{
			if (is_active_)
				deactivate();

			delete(settings_);
		}

		void OpenSimPlugin::settingsChanged()
		{
			remote_host_ = (String)(settings_->remote_address->text());
			remote_port_ = settings_->remote_port->text().toInt();
			local_host_  = (String)(settings_->local_address->text());
			local_port_  = settings_->local_port->text().toInt();

			if (!checkConfig())
				demandConfig();
		}

		bool OpenSimPlugin::checkConfig()
		{
			if (    (remote_host_ == "") || (remote_port_ == 0)
					 || (local_host_  == "") || (local_port_  == 0) )
				return false;

			return true;
		}

		void OpenSimPlugin::demandConfig()
		{
			QMessageBox msgBox;
			msgBox.setText("The OpenSim plugin has not been configured correctly.");
			msgBox.setInformativeText("Please provide correct addresses and ports.");
			msgBox.exec();

			settings_->show();
		}

		bool OpenSimPlugin::activate()
		{
			is_active_ = true;

			if (!checkConfig())
			{
				demandConfig();
			}

			server_ =  new OpenSimReceiver(local_port_, this);
			server_->start();

			// give the server a few milliseconds to start up
			sleepFor(200);

			if (!server_->isRunning())
			{
				Log.error() << "Activation of OpenSimPlugin server was not successful!" << endl;
				is_active_ = false;
			}

			cmdThread_->start();

			// give the thread a few milliseconds to start up
			sleepFor(200);

			if (!cmdThread_->isRunning())
			{
				Log.error() << "Creation of worker thread was not successful!" << endl;
				is_active_ = false;
			}

			return is_active_;
		}
		
		bool OpenSimPlugin::deactivate()
		{
			is_active_ = false;

			server_->deactivate();
			server_->wait();

			delete(server_);
			server_ = 0;

			cmdThread_->deactivate();
			cmdThread_->wait();

			return true;
		}

		void OpenSimPlugin::setReceiver(QWidget* receiver)
		{
			receiver_ = receiver;
		}

		const QPixmap* OpenSimPlugin::getIcon() const
		{
			return &icon_;
		}

		QDialog* OpenSimPlugin::getConfigDialog()
		{
			settings_->setRemoteServer(remote_host_, remote_port_);
			settings_->setLocalServer ( local_host_, local_port_ );

			return static_cast<QDialog*>(settings_);
		}
		
		bool OpenSimPlugin::hasMessage()
		{
			bool result;

			pluginrwLock_.lockForRead();
			result = !(ballviewmessage_queue_.empty());
			pluginrwLock_.unlock();

			return result;
		}

		void OpenSimPlugin::onNotify(Message* message)
		{
			// Here we need to put the messages in a queue, lock it
			// then, access the queue from a different thread as shown below
			// but this thread can not be created here, because everytime, 
			// there will be more threads create a thread outside.
			if (RTTI::isKindOf<CompositeMessage>(*message))
			{
				CompositeMessage *cm = RTTI::castTo<CompositeMessage>(*message);

				OpenSimTask task;
				bool task_type_supported = false;

				if (cm->getType() == CompositeMessage::NEW_COMPOSITE)
				{
					task.type = OpenSimTask::NEW_COMPOSITE;
					task_type_supported = true;
				}
				else if (cm->getType() == CompositeMessage::CHANGED_COMPOSITE_HIERARCHY) 
				{
					task.type = OpenSimTask::CHANGED_COMPOSITE_HIERARCHY;
					task_type_supported = true;
				}
				else if (cm->getType() == CompositeMessage::REMOVED_COMPOSITE)
				{
					// AKD: The problem with deleting entire Composites like Residues, Chains, etc is
					// that a CompositeMessage::CHANGED_COMPOSITE_HIERARCHY is thrown. 
					// So we just know whose hirarchy changend but not which atoms are deleted...
					// TODO!!
					task.type = OpenSimTask::REMOVED_COMPOSITE;
					task_type_supported = true;
				}

				if (!task_type_supported)
					return;

				AtomContainer* container = dynamic_cast<AtomContainer*>(cm->getComposite());

				if (container)
				{
					AtomIterator at_it;
					Atom::BondIterator b_it;

					BALL_FOREACH_ATOM(*container, at_it)
					{
						task.affected_atoms.push_back(&*at_it);
					}

					BALL_FOREACH_BOND(*container, at_it, b_it)
					{
						task.affected_bonds.push_back(&*b_it);
					}
				}
				else
				{
					Atom* atom = dynamic_cast<Atom*>(cm->getComposite());

					task.affected_atoms.push_back(atom);

					Atom::BondIterator b_it;
					BALL_FOREACH_ATOM_BOND(*atom, b_it)
					{
						task.affected_bonds.push_back(&*b_it);
					}
				}
				// Finally, store the message in the queue for later processing
				pluginrwLock_.lockForWrite();
				ballviewmessage_queue_.push(task);
				pluginrwLock_.unlock();
			}
			else if (RTTI::isKindOf<RepresentationMessage>(*message))
			{
				RepresentationMessage* rm = RTTI::castTo<RepresentationMessage>(*message);

				if (rm->getType() == RepresentationMessage::FINISHED_UPDATE)
				{
					OpenSimTask task;

					Representation* r = rm->getRepresentation();

					if (r && !(r->isHidden()))
					{
						Composite* composite = NULL;

						if (r->getComposites().size() > 0)
						{
							// ugly, but necessary
							composite = const_cast<Composite*>(*(r->getComposites().begin()));
						}

						if (composite && RTTI::isKindOf<AtomContainer>(*composite))
						{
							AtomContainer* container = RTTI::castTo<AtomContainer>(*composite);

							AtomIterator at_it;
							Atom::BondIterator b_it;

							BALL_FOREACH_ATOM(*container, at_it)
							{
								task.affected_atoms.push_back(&*at_it);
							}

							BALL_FOREACH_BOND(*container, at_it, b_it)
							{
								task.affected_bonds.push_back(&*b_it);
							}

							task.type = OpenSimTask::REPRESENTATION;

							pluginrwLock_.lockForWrite();
							ballviewmessage_queue_.push(task);
							pluginrwLock_.unlock();
						}
					}
				}
			}
		}

		void OpenSimPlugin::sendAcknowledgement(const String& message)
		{
			server_->sendMessageString(message);
		}

		void OpenSimPlugin::handleMolecularModeling(std::vector<String> message)
		{
			if (message.empty())
			{
				Log.error() << "Damnit! This is an empty message! " << endl;
				return;
			}

			Size command_index(message[0].trim().toInt());
			server_->is_Process_Done_ = false;

			switch (command_index)
			{
				case (OpenSimReceiver::ADD_ATOM):
				{
					if (message.size() != 9)
					{
						Log.error() << "Damnit! This is not an add atom command!";
						break;
					}	

					String element(message[1]);

					Vector3 position(message[2].toFloat(),
					                 message[3].toFloat(),
													 message[4].toFloat());

					float radius(message[5].toFloat());
					
					ColorRGBA color(message[6].toInt(), 
									message[7].toInt(), 
									message[8].trim().toInt());

				
					Index atom_index_ = molStructPlugin_->addAtom( element, position, radius, color);
			
					if( atom_index_ != -1)
					{
						String acknowledgement_string(OpenSimReceiver::ACKNOWLEDGE_ADD_ATOM);

						acknowledgement_string +=  String(";") +  String(atom_index_) 
													+ String(";") + String(element) 
													+ String(";" )+ String(position.x) 
													+ String(";") + String(position.y) 
													+ String(";") + String(position.z) 
													+ String(";") + String(radius) 
													+ String(";") + String((int)color.getRed())
													+ String(";") + String((int)color.getBlue())
													+ String(";") + String((int)color.getGreen());

						Log.info() << "ADD_ATOM:acknowledgement_string Message @handleMolecularModeling from OpenSim to BALLView  : " 
											<< acknowledgement_string << endl;

						sendAcknowledgement(acknowledgement_string);
					}
					else
					{
						Log.info() << "ADD_ATOM: Failed" << endl;
						// what message should we send to OpenMol
					}
					break;
				}
				case (OpenSimReceiver::ADD_BOND):
				{
					if (message.size() != 4)
					{
						Log.error() << "Damnit! This is not an add bond command" << endl;
						break;
					}

					Index atom_one_index = message[1].toInt();
					Index atom_two_index = message[2].toInt();

					Size bond_order = message[3].trim().toInt();

					Index bond_index_  = molStructPlugin_->addBondByAtomIndex(atom_one_index, atom_two_index, bond_order);
					
					if(bond_index_ != -1)
					{
					
						String acknowledgement_string(OpenSimReceiver::ACKNOWLEDGE_ADD_BOND);

						acknowledgement_string +=	String(";") + String(bond_index_) 
												  + String(";") + String(atom_one_index)
												  + String(";") + String(atom_two_index) 
												  + String(";")+ String(bond_order);

						Log.info() << "ADD_BOND:acknowledgement_string Message @handleMolecularModeling from OpenSim to BALLView  : "
											 << acknowledgement_string << endl;


						sendAcknowledgement(acknowledgement_string);
					}
					else
					{
						Log.info() << "ADD_BOND: Failed " << endl;
						// what message should we send to OpenMol
					}
					break;
				}
				case (OpenSimReceiver::REMOVE_ATOM):
				{
						if (message.size()!= 2)
						{
							Log.error() << "Damnit! This is not an atom remval command" << endl;
							break;
						}

						Index atom_index_ = message[1].trim().toInt();

						molStructPlugin_->removeAtomByIndex(atom_index_);

						String acknowledgement_string(OpenSimReceiver::ACKNOWLEDGE_REMOVE_ATOM);
						acknowledgement_string += String(";") + String(atom_index_)  ;

						Log.info() << "REMOVE_ATOM:acknowledgement_string Message @handleMolecularModeling from OpenSim to BALLView  : "<<acknowledgement_string<<std::endl;

						sendAcknowledgement(acknowledgement_string);
					}
					break;

				case (OpenSimReceiver::REMOVE_BOND):
				{
						if (message.size()!= 4)
						{
							Log.error()<<"Damnit! This is not a bond removal command"<<std::endl;
							break;
						}

						Index bond_index_ = message[1].trim().toInt();
						Index atom_index_one_ = message[2].trim().toInt();
						Index atom_index_two_ = message[3].trim().toInt();

						molStructPlugin_->removeBondByAtomIndex(bond_index_,atom_index_one_,atom_index_two_);
					
						String acknowledgement_string(OpenSimReceiver::ACKNOWLEDGE_REMOVE_BOND);
						acknowledgement_string += String(";") + String(bond_index_) ;

						Log.info() << "REMOVE_BOND:acknowledgement_string Message @handleMolecularModeling from OpenSim to BALLView  : "<<acknowledgement_string<<std::endl;

						sendAcknowledgement(acknowledgement_string);
						break;
				}
				case (OpenSimReceiver::UPDATE_ATOM):
				{
						if (message.size() != 6)
						{
							Log.error() << "Damnit! This is not an update atom command!";
							break;
						}

						Index atom_index = message[1].toInt();

						String element(message[2]);

						Vector3 position(message[3].toFloat(),
										 message[4].toFloat(),
										 message[5].toFloat());

						molStructPlugin_->updateAtomByIndex(atom_index, element, position /*, radius, charge, velocity,force*/);

						String acknowledgement_string(OpenSimReceiver::ACKNOWLEDGE_UPDATE_ATOM);
						acknowledgement_string += String(";") + String(atom_index);
						sendAcknowledgement(acknowledgement_string);

						break;
				}
				case (OpenSimReceiver::UPDATE_BOND):
				{
						if (message.size() != 5)
						{
							Log.error() << "Damnit! This is not an update bond command!" << endl;
							break;
						}

						Index bond_index = message[1].toInt();
						Index atom_index_one_ =  message[2].toInt();
						Index atom_index_two_ =  message[3].toInt();
						Size order_ =  message[4].trim().toInt();
						molStructPlugin_->updateBondByAtomIndex(atom_index_one_,atom_index_two_,order_ );

						String acknowledgement_string(OpenSimReceiver::ACKNOWLEDGE_UPDATE_BOND);
						acknowledgement_string += String(";") + String(bond_index) ;
						sendAcknowledgement(acknowledgement_string);
						break;
				}
				case (OpenSimReceiver::SATURATE_FULL_WITH_HYDROGENS):
				{
						if (message.size() != 1)
						{
							Log.error() << "Damnit! This is not a command to run the saturation with hydrogen!" << endl;;
							break;
						}

						molModelingPlugin_->saturateFullWithHydrogens();

						String acknowledgement_string(OpenSimReceiver::ACKNOWLEDGE_SATURATE_F_HYD);
						acknowledgement_string += String(";") + String("done");
						sendAcknowledgement(acknowledgement_string);
						break;
				}
				case (OpenSimReceiver::RUN_MINIMIZATION):
				{
						if (message.size() != 1)
						{
							Log.error() << "Damnit! This is not a command to run the minization!" << endl;
							break;
						}
						molDynamicsPlugin_->minimize();

						String acknowledgement_string(OpenSimReceiver::ACKNOWLEDGE_RUN_MINI);
						acknowledgement_string += String(";") + String("done");
						sendAcknowledgement(acknowledgement_string);
						break;
				}
				case (OpenSimReceiver::CHANGE_FORCE_FIELD):
				{
						if (message.size() != 2)
						{
							Log.error() << "Damnit! This is not a command to change the force field!";
							break;
						}

						Size force_field_ = message[1].trim().toInt();
						molModelingPlugin_->ChangeForceField(force_field_);

						String acknowledgement_string(OpenSimReceiver::ACKNOWLEDGE_CHANGE_FF);
						acknowledgement_string += String(";") + String("done");
						sendAcknowledgement(acknowledgement_string);
						break;
				}
				case (OpenSimReceiver::SINGLE_POINT_CALCULATION):
				{
						if (message.size() != 1)
						{
							Log.error() << "Damnit! This is not a command to run md simulation!" << endl;;
							break;
						}
						molDynamicsPlugin_->runSinglePointCalculation();

						String acknowledgement_string(OpenSimReceiver::ACKNOWLEDGE_S_PNT_CAL);
						acknowledgement_string += String(";") + String("done");
						sendAcknowledgement(acknowledgement_string);
						break;
				}
				case (OpenSimReceiver::MD_SIMULATION):
				{
						if (message.size() != 1)
						{
							Log.error() << "Damnit! This is not a command to run md simulation!" << endl;
							break;
						}
						
						molDynamicsPlugin_->runMDSimulation();

						String acknowledgement_string(OpenSimReceiver::ACKNOWLEDGE_MD_SIMULATION);
						acknowledgement_string += String(";") + String("done");
						sendAcknowledgement(acknowledgement_string);

						break;
				}
				case (OpenSimReceiver::ADD_MOLECULE):
				{
						if (message.size() != 1)
						{
							Log.error() << "Damnit! This is not a command to add molecule!" << endl;
							break;
						}
						
						molModelingPlugin_->runCreateNewMolecule();

						String acknowledgement_string(OpenSimReceiver::ACKNOWLEDGE_ADD_MOLECULE);
						acknowledgement_string += String(";") + String("done");
						sendAcknowledgement(acknowledgement_string);

						break;
				}	
				case (OpenSimReceiver::STOP_SIMULATION):
				{
						if (message.size() != 1)
						{
							Log.error() << "Damnit! This is not a command to stop simulation!";
							break;
						}

						molDynamicsPlugin_->stopSimulation();

						String acknowledgement_string(OpenSimReceiver::ACKNOWLEDGE_STOP_SIMULATION);
						acknowledgement_string += String(";") + String("done");
						sendAcknowledgement(acknowledgement_string);

						break;
				}
				default:
					break;
			}
			server_->is_Process_Done_ = true;
		}

		void OpenSimPlugin::sendNewComposite(OpenSimTask task)
		{ 
			String command;

			// first add all atoms
			Size num_added_atoms = 0;
			std::list<Atom*>::iterator atom_it;
			for (atom_it = task.affected_atoms.begin(); atom_it != task.affected_atoms.end(); ++atom_it)
			{
				if (!molStructPlugin_->atom_to_index_.has(*atom_it))
				{
					Index atom_index        = molStructPlugin_->hashAtom(*atom_it);
					String element 					= (*atom_it)->getElement().getSymbol();
					float radius 						= (*atom_it)->getRadius();
					ColorRGBA& color 				= ecp_.getColorMap()[(*atom_it)->getElement().getAtomicNumber()];
					const Vector3& position = (*atom_it)->getPosition();

					String add_atom_string =    String(";") + String(atom_index) 
																		+ String(";") + String(element) 
																		+ String(";") + String(position.x) 
																		+ String(";") + String(position.y) 
																		+ String(";") + String(position.z) 
																		+ String(";") + String(radius) 
																		+ String(";") + String((int)color.getRed()) 
																		+ String(";") + String((int)color.getBlue())
																		+ String(";") + String((int)color.getGreen());

					command += add_atom_string;

					++num_added_atoms;
				}
			}

			// now iterate over all bonds
			Size num_added_bonds = 0;

			std::list<Bond*>::iterator bond_it;
			for (bond_it = task.affected_bonds.begin(); bond_it != task.affected_bonds.end(); ++bond_it)
			{
				Index atom_one_index = molStructPlugin_->getAtomIndex((*bond_it)->getFirstAtom());
				Index atom_two_index = molStructPlugin_->getAtomIndex((*bond_it)->getSecondAtom());

				Size bond_order = (*bond_it)->getOrder();

				if ( (atom_one_index != -1) && (atom_two_index != -1) )
				{
					Index bond_index = molStructPlugin_->hashBond(*bond_it);

					String add_bond_string =   String(";") + String(bond_index)
																	 + String(";") + String(atom_one_index) 
																	 + String(";") + String(atom_two_index)
																	 + String(";") + String(bond_order);

					command += add_bond_string;

					++num_added_bonds;
				}
			}

			command =   String(OpenSimReceiver::ADD_ATOM_CONTAINER) 
			          + String(";") + String(num_added_atoms) 
								+ String(";") + String(num_added_bonds) + command;

			// communicate the action
			server_->sendMessageString(command);
		}

		void OpenSimPlugin::sendRemovedComposite(OpenSimTask task)
		{	
			String command;

			// first remove all atoms
			Size num_removed_atoms = 0;
			std::list<Atom*>::iterator atom_it;
			for (atom_it = task.affected_atoms.begin(); atom_it != task.affected_atoms.end(); ++atom_it)
			{
				Index atom_index = molStructPlugin_->getAtomIndex(*atom_it);

				if (atom_index != -1)
				{
					String remove_atom_string = String(";") + String(atom_index);
					command += remove_atom_string;
					++num_removed_atoms;

					molStructPlugin_->unhashAtom(atom_index);
				}
			}

			// then all bonds
			Size num_removed_bonds = 0;
			std::list<Bond*>::iterator bond_it;
			for (bond_it = task.affected_bonds.begin(); bond_it != task.affected_bonds.end(); ++bond_it)
			{
				Index bond_index = molStructPlugin_->getBondIndex(*bond_it);

				if (bond_index != -1)
				{
					String remove_bond_string = String(";") + String(bond_index);
					command += remove_bond_string;
					++num_removed_bonds;

					molStructPlugin_->unhashBond(bond_index);
				}
			}

			command =   String(OpenSimReceiver::REMOVE_ATOM_CONTAINER) 
			          + String(";") + String(num_removed_atoms) 
								+ String(";") + String(num_removed_bonds) + command;
	
			// communicate the action
			server_->sendMessageString(command);
		}

		void OpenSimPlugin::sendChangedComposite(OpenSimTask task)
		{
			String command;

			HashSet<Index> changed_atoms;

			// iterate over all atoms
			Size num_changed_atoms = 0;
			for (std::list<Atom*>::iterator atom_it = task.affected_atoms.begin(); atom_it != task.affected_atoms.end(); ++atom_it)
			{
				Index atom_index = molStructPlugin_->getAtomIndex(*atom_it);

				String element 					= (*atom_it)->getElement().getSymbol();
				const Vector3& position = (*atom_it)->getPosition();
				ColorRGBA& color 				= ecp_.getColorMap()[(*atom_it)->getElement().getAtomicNumber()];
				float radius 						= (*atom_it)->getRadius();

				if (atom_index == -1)
				{
					// this is a new atom
					atom_index = molStructPlugin_->hashAtom(*atom_it);

					String add_atom_string =   String(";") + String(atom_index) 
										               + String(";") + String(element) 
																	 + String(";") + String(position.x) 
																	 + String(";") + String(position.y) 
																	 + String(";") + String(position.z) 
																	 + String(";") + String(radius) 
																	 + String(";") + String((int)color.getRed())
																	 + String(";") + String((int)color.getBlue())
																	 + String(";") + String((int)color.getGreen());
		
					changed_atoms.insert(atom_index);

					command += add_atom_string;

					++num_changed_atoms;
				}
				else
				{
					// has the atom been changed since the last update?
					if (molStructPlugin_->needsUpdate(atom_index))
					{
							String update_atom_string =   String(";") + String(atom_index) 
																					+ String(";") + String(element) 
																					+ String(";") + String(position.x) 
																					+ String(";") + String(position.y) 
																					+ String(";") + String(position.z) 
																					+ String(";") + String(radius) 
																					+ String(";") + String((int)color.getRed())
																					+ String(";") + String((int)color.getBlue())
																					+ String(";") + String((int)color.getGreen());
							
							changed_atoms.insert(atom_index);

							command += update_atom_string;

							++num_changed_atoms;
					}
				}
			}

			// now iterate over all bonds
			Size num_changed_bonds = 0;
			for (std::list<Bond*>::iterator bond_it = task.affected_bonds.begin(); bond_it != task.affected_bonds.end(); ++bond_it)
			{
				Index bond_index = molStructPlugin_->getBondIndex(*bond_it);

				Index atom_one_index = molStructPlugin_->getAtomIndex((*bond_it)->getFirstAtom());
				Index atom_two_index = molStructPlugin_->getAtomIndex((*bond_it)->getSecondAtom());

				Size bond_order = (*bond_it)->getOrder();

				if (bond_index == -1)
				{
					// this is a new bond
					if ( (atom_one_index != -1) && (atom_two_index != -1) )
					{
						bond_index = molStructPlugin_->hashBond(*bond_it);

						String add_bond_string =   String(";") + String(bond_index)
																		 + String(";") + String(atom_one_index) 
																		 + String(";") + String(atom_two_index)
																		 + String(";") + String(bond_order);

						command += add_bond_string;

						++num_changed_bonds;
					}
				}
				else
				{
					// this bond has been added already

					// simple update rule: if one of the atoms has been modified, modify this bond
					if (changed_atoms.has(atom_one_index) || changed_atoms.has(atom_two_index))
					{
						String update_bond_string =   String(";") + String(bond_index) 
																				+ String(";") + String(atom_one_index) 
																				+ String(";") + String(atom_two_index) 
																				+ String(";") + String(bond_order);

						command += update_bond_string;

						++num_changed_bonds;
					}
				}
			}

			command =   String(OpenSimReceiver::UPDATE_ATOM_CONTAINER) 
			          + String(";") + String(num_changed_atoms)
							  + String(";") + String(num_changed_bonds) + command;
	
			// communicate the action
			server_->sendMessageString(command);
		}

		// TODO: if only the positions of the atoms have changed, it would be better
		//       and faster to just send position updates to open sim
		void OpenSimPlugin::sendRepresentation(OpenSimTask task)
		{
			String command;

			HashSet<Index> changed_atoms;

			// iterate over all atoms
			Size num_changed_atoms = 0;
			for (std::list<Atom*>::iterator atom_it = task.affected_atoms.begin(); atom_it != task.affected_atoms.end(); ++atom_it)
			{
				Index atom_index = molStructPlugin_->getAtomIndex(*atom_it);

				String element 					= (*atom_it)->getElement().getSymbol();
				const Vector3& position = (*atom_it)->getPosition();
				ColorRGBA& color 				= ecp_.getColorMap()[(*atom_it)->getElement().getAtomicNumber()];
				float radius 						= (*atom_it)->getRadius();

				if (atom_index == -1)
				{
					// this is a new atom
					atom_index = molStructPlugin_->hashAtom(*atom_it);

					String add_atom_string =   String(";") + String(atom_index) 
										               + String(";") + String(element) 
																	 + String(";") + String(position.x) 
																	 + String(";") + String(position.y) 
																	 + String(";") + String(position.z) 
																	 + String(";") + String(radius) 
																	 + String(";") + String((int)color.getRed())
																	 + String(";") + String((int)color.getBlue())
																	 + String(";") + String((int)color.getGreen());
		
					changed_atoms.insert(atom_index);

					command += add_atom_string;

					++num_changed_atoms;
				}
				else
				{
					// has the atom been changed since the last update?
					if (molStructPlugin_->needsUpdate(atom_index))
					{
							String update_atom_string =   String(";") + String(atom_index) 
																					+ String(";") + String(element) 
																					+ String(";") + String(position.x) 
																					+ String(";") + String(position.y) 
																					+ String(";") + String(position.z) 
																					+ String(";") + String(radius) 
																					+ String(";") + String((int)color.getRed())
																					+ String(";") + String((int)color.getBlue())
																					+ String(";") + String((int)color.getGreen());
							
							changed_atoms.insert(atom_index);

							command += update_atom_string;

							++num_changed_atoms;
					}
				}
			}

			// now iterate over all bonds
			Size num_changed_bonds = 0;
			for (std::list<Bond*>::iterator bond_it = task.affected_bonds.begin(); bond_it != task.affected_bonds.end(); ++bond_it)
			{
				Index bond_index = molStructPlugin_->getBondIndex(*bond_it);

				Index atom_one_index = molStructPlugin_->getAtomIndex((*bond_it)->getFirstAtom());
				Index atom_two_index = molStructPlugin_->getAtomIndex((*bond_it)->getSecondAtom());

				Size bond_order = (*bond_it)->getOrder();

				if (bond_index == -1)
				{
					// this is a new bond

					if ( (atom_one_index != -1) && (atom_two_index != -1) )
					{
						bond_index = molStructPlugin_->hashBond(*bond_it);

						String add_bond_string =   String(";") + String(bond_index)
																		 + String(";") + String(atom_one_index) 
																		 + String(";") + String(atom_two_index)
																		 + String(";") + String(bond_order);

						command += add_bond_string;

						++num_changed_bonds;
					}
				}
				else
				{
					// this bond has been added already

					// simple update rule: if one of the atoms has been modified, modify this bond
					if (changed_atoms.has(atom_one_index) || changed_atoms.has(atom_two_index))
					{
						String update_bond_string =   String(";") + String(bond_index) 
																				+ String(";") + String(atom_one_index) 
																				+ String(";") + String(atom_two_index) 
																				+ String(";") + String(bond_order);

						command += update_bond_string;

						++num_changed_bonds;
					}
				}
			}

			command =   String(OpenSimReceiver::UPDATE_ATOM_CONTAINER) 
			          + String(";") + String(num_changed_atoms)
							  + String(";") + String(num_changed_bonds) + command;
	
			// communicate the action
			server_->sendMessageString(command);
		}
	}
}
