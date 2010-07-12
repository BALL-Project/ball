#include <BALL/COMMON/global.h>

#include <OpenSimPlugin.h>
#include <OpenSimPluginConfiguration.h>

#include <BALL/VIEW/KERNEL/mainControl.h>
#include <BALL/SYSTEM/path.h>
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
				cmdThread_(new OpenSimCommandExecutionThread(this)),
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

			cmdThread_->start();

			return true;
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
			// then, access the queue from different thread as show below
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
					task.type = OpenSimTask::REMOVED_COMPOSITE;
					task_type_supported = true;
				}

				if (!task_type_supported)
					return;

				AtomContainer* container = dynamic_cast<AtomContainer*>(cm->getComposite());

				if (container)
				{
					HashMap<Handle, const Atom*> tmp_handle_to_atom;
					HashMap<Handle, OpenSimTask::BondStruct> tmp_handle_to_bond;

					for (AtomIterator at_it = container->beginAtom(); +at_it; ++at_it)
					{
						//get the handle of original atom, before copying
						Handle atom_one_handle = at_it->getHandle();
						const Atom* copy_atom_one = new Atom(*at_it, true);

						if (!tmp_handle_to_atom.has(atom_one_handle) && copy_atom_one)
						{
							// store the handle of original atom and map with copied atom
							tmp_handle_to_atom[atom_one_handle]= copy_atom_one;
						}

						for (Atom::BondIterator b_it = at_it->beginBond(); +b_it; ++b_it)
						{
							const Atom* atom_two = b_it->getPartner(*at_it);

							//get the handle of original atom
							Handle atom_two_handle = atom_two->getHandle();

							const Atom* copy_atom_two = new Atom(*atom_two,true);

							if (!tmp_handle_to_atom.has(atom_two_handle) && copy_atom_two != NULL)
							{
								// store the handle of original atom and map with copied atom
								tmp_handle_to_atom[atom_two_handle]= copy_atom_two;
							}

							const Bond* bond = at_it->getBond(*atom_two);

							Handle bond_handle = -1;

							if (bond)
							{
								// get the handle of original bond
								bond_handle = bond->getHandle();
								const Bond* copy_bond = new Bond(*bond,true);

								OpenSimTask::BondStruct bondStruct;

								// copied bond
								bondStruct.bond = copy_bond;

								bondStruct.atom_one_handle = atom_one_handle;
								bondStruct.atom_two_handle = atom_two_handle;

								if (!tmp_handle_to_bond.has(bond_handle) && (bond_handle != INVALID_HANDLE))
								{
									// store the handle of original bond and map with copied bond
									tmp_handle_to_bond[bond_handle] = bondStruct;
								}
							}
						}
					}

					// TODO: validation if hashmap is empty?
					task.handle_to_atom_ = tmp_handle_to_atom;
					task.handle_to_bond_ = tmp_handle_to_bond;

					// Finally, store the message in the queue for later processing
					pluginrwLock_.lockForWrite();
					ballviewmessage_queue_.push(task);
					pluginrwLock_.unlock();
				}
			}
			else if (RTTI::isKindOf<RepresentationMessage>(*message))
			{
				RepresentationMessage* rm = RTTI::castTo<RepresentationMessage>(*message);

				if (rm->getType() == RepresentationMessage::FINISHED_UPDATE)
				{
					Representation* r = rm->getRepresentation();

					if (r && !(r->isHidden()))
					{
						Composite const * composite = NULL;

						if (r->getComposites().size() > 0)
						{
							composite = *(r->getComposites().begin());
						}

						if (composite)
						{
							if (RTTI::isKindOf<const AtomContainer>(*composite))
							{
								AtomContainer* container = RTTI::castTo<AtomContainer>(*composite);

								HashMap<Handle, const Atom*> tmp_handle_to_atom;
								HashMap<Handle, OpenSimTask::BondStruct> tmp_handle_to_bond;

								for (AtomIterator at_it = container->beginAtom(); +at_it; ++at_it)
								{
									//Copy the atom's content into a new variable.
									const Atom* atom_one = &*at_it;

									if (atom_one)
									{
										//get the handle of original atom
										Handle atom_one_handle = atom_one->getHandle();

										const Atom* copy_atom_one = new Atom(*atom_one,true);

										if (!tmp_handle_to_atom.has(atom_one_handle) && (copy_atom_one))
										{
											// store the handle of original atom and map with copied atom
											tmp_handle_to_atom[atom_one_handle]= copy_atom_one;
										}

										for (Atom::BondIterator b_it = at_it->beginBond(); +b_it; ++b_it)
										{
											const Atom* atom_two = b_it->getPartner(*atom_one);

											if (atom_two)
											{
												//get the handle of original atom
												Handle atom_two_handle = atom_two->getHandle();

												const Atom* copy_atom_two = new Atom(*atom_two,true);

												if (!tmp_handle_to_atom.has(atom_two_handle) && (copy_atom_two))
												{
													// store the handle of original atom and map with copied atom
													tmp_handle_to_atom[atom_two_handle]= copy_atom_two;
												}

												const Bond* bond = atom_one->getBond(*atom_two);
												Handle bond_handle = INVALID_HANDLE;

												if (bond)
												{
													// get the handle of original bond	
													bond_handle = bond->getHandle();
													const Bond* copy_bond = new Bond(*bond,true);

													if (!tmp_handle_to_bond.has(bond_handle) && bond_handle != INVALID_HANDLE)
													{
														OpenSimTask::BondStruct bondStruct;

														// copied bond
														bondStruct.bond = copy_bond;

														bondStruct.atom_one_handle = atom_one_handle;
														bondStruct.atom_two_handle = atom_two_handle;

														if (!tmp_handle_to_bond.has(bond_handle) && bond_handle != INVALID_HANDLE)
														{
															// store the handle of original bond and map with copied bond
															tmp_handle_to_bond[bond_handle] = bondStruct;
														}
													}
												}
											}
										}
									}
								}

								OpenSimTask task;

								task.type = OpenSimTask::REPRESENTATION;

								// TODO validation if hashmap isempty?
								task.handle_to_atom_ = tmp_handle_to_atom;
								task.handle_to_bond_ = tmp_handle_to_bond;

								pluginrwLock_.lockForWrite();
								ballviewmessage_queue_.push(task);
								pluginrwLock_.unlock();
							}
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
			if(message.empty())
			{
				Log.error()<<"Damnit! This is an empty message! " <<std::endl;
				return;
			}

			Size command_index(message[0].trim().toInt());
					
			server_->is_Process_Done_ = false;

			switch (command_index)
			{
				case(OpenSimReceiver::ADD_ATOM):
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

						Log.info() << "ADD_ATOM:acknowledgement_string Message @handleMolecularModeling from OpenSim to BALLView  : "<<acknowledgement_string<<std::endl;


						sendAcknowledgement(acknowledgement_string);
					}
					else
					{
						Log.info() << "ADD_ATOM: Failed"<<std::endl;
						// what message should we send to OpenMol
					}
				break;
			}
				case (OpenSimReceiver::ADD_BOND):
				{

						if(message.size() != 4)
						{
							Log.error()<<"Damnit! This is not an add bond command"<<std::endl;
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

							Log.info() << "ADD_BOND:acknowledgement_string Message @handleMolecularModeling from OpenSim to BALLView  : "<<acknowledgement_string<<std::endl;


							sendAcknowledgement(acknowledgement_string);
						}
						else
						{
							Log.info() << "ADD_BOND: Failed "<<std::endl;
							// what message should we send to OpenMol
						}
					break;
				}
				case (OpenSimReceiver::REMOVE_ATOM):
				{
						if(message.size()!= 2)
						{
							Log.error()<<"Damnit! This is not an atom remval command"<<std::endl;
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

						acknowledgement_string += String(";") + String(atom_index) ;

						sendAcknowledgement(acknowledgement_string);

						break;
				}
				case (OpenSimReceiver::UPDATE_BOND):
				{
						if (message.size() != 5)
						{
							Log.error() << "Damnit! This is not an update bond command!";
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
							Log.error() << "Damnit! This is not a command to run the saturation with hydrogen!";
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
							Log.error() << "Damnit! This is not a command to run the minization!";
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
							Log.error() << "Damnit! This is not a command to run md simulation!";
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
							Log.error() << "Damnit! This is not a command to run md simulation!";
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
							Log.error() << "Damnit! This is not a command to add molecule!";
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

		void OpenSimPlugin::handleNewComposite(OpenSimTask task)
		{ 
			String command(OpenSimReceiver::ADD_ATOM_CONTAINER);
			command += String(";") + String(task.handle_to_atom_.size()) + ";" + String(task.handle_to_bond_.size());

			HashMap<Handle, const Atom *>::ConstIterator atom_it;	
			for (atom_it= task.handle_to_atom_.begin(); atom_it!= task.handle_to_atom_.end() ;++atom_it)
			{
				Handle atom_identifier 	= atom_it->first;
				const Atom* atom 				= atom_it->second;

				// ToDo if "atom_identifier != INVALID_HANDLE" is required
				if (atom && (atom_identifier != INVALID_HANDLE))
				{
					//Log.info() << "new atom_identifier at handlenew : " << atom_identifier << std::endl; 

					if (!molStructPlugin_->atom_to_index_.has(atom_identifier))
					{
						String element 					= atom->getElement().getSymbol();
						float radius 						= atom->getRadius();
						ColorRGBA& color 				= ecp_.getColorMap()[atom->getElement().getAtomicNumber()];
						const Vector3& position = atom->getPosition();

						String add_atom_string = String(";") + String(molStructPlugin_->next_atom_index_) 
							+ String(";") + String(element) 
							+ String(";")+ String(position.x) 
							+ String(";") + String(position.y) 
							+ String(";") + String(position.z) 
							+ String(";") + String(radius) 
							+ String(";") + String((int)color.getRed()) 
							+ String(";") + String((int)color.getBlue())
							+ String(";") + String((int)color.getGreen());

						command += add_atom_string;

//						Log.info() << "Add_atom_string Message @handleNewComposite from BALLView to OpenSim : " << add_atom_string << std::endl;

						// NOTE:
						// When one atom was added in BALLView or OpenSim, the hashmaps 
						// atom_to_index_ and index_to_atom_ should be updated.
						// The newly added atom will have index from next_atom_index_.
						// This index is used within plugin and opensim; do not confuse 
						// this index with index created in BALLview itself. 
						if (molStructPlugin_)
						{
							molStructPlugin_->readWriteLock_.lockForWrite();
							molStructPlugin_->atom_to_index_[atom_identifier] = molStructPlugin_->next_atom_index_;
							molStructPlugin_->index_to_atom_[molStructPlugin_->next_atom_index_] = atom_identifier;
							molStructPlugin_->handle_to_atom_[atom_identifier] =  new Atom(*atom,true);
							molStructPlugin_->next_atom_index_++;

							while (molStructPlugin_->index_to_atom_.has(molStructPlugin_->next_atom_index_)) 
							{
								++molStructPlugin_->next_atom_index_;
							}
							
							molStructPlugin_->readWriteLock_.unlock();
						}
						else
						{
							Log.info() << "Could not initialize instance of MolecularStructureContainer" << std::endl;
							break;
						}
					}
					else
					{
						Log.info() << " molStructPlugin_->atom_to_index_ does have  atom_identifier " <<  atom_identifier << std::endl;
					}
				}
			}

			// now iterate over all bonds
			HashMap<Handle, OpenSimTask::BondStruct>::ConstIterator bond_it;
			for (bond_it = task.handle_to_bond_.begin(); bond_it!= task.handle_to_bond_.end(); ++bond_it)
			{
				Handle new_bond_identifier = bond_it->first;
				const Bond* bond = bond_it->second.bond;

				if (bond && (new_bond_identifier != INVALID_HANDLE))
				{
					Size bond_order = bond->getOrder();

					if (!molStructPlugin_->bond_to_index_.has(new_bond_identifier))
					{
						Handle atom_one_handle = INVALID_HANDLE; 
						Handle atom_two_handle = INVALID_HANDLE; 
						
						Index atom_one_index = -1;
						Index atom_two_index = -1;

						if (task.handle_to_bond_.has(new_bond_identifier))
						{
							OpenSimTask::BondStruct bondStruct = task.handle_to_bond_[new_bond_identifier];
							atom_one_handle = bondStruct.atom_one_handle;
							atom_two_handle = bondStruct.atom_two_handle;
						}

						if (molStructPlugin_->atom_to_index_.has(atom_one_handle))
						{
							atom_one_index = molStructPlugin_->atom_to_index_[atom_one_handle];
						}

						if (molStructPlugin_->atom_to_index_.has(atom_two_handle))
						{
							atom_two_index = molStructPlugin_->atom_to_index_[atom_two_handle];
						}

						if ((atom_one_index != -1) && (atom_two_index != -1))
						{
							//Log.info() << " new bond_identifier at newcomp : " << new_bond_identifier << std::endl;

							String add_bond_string = String(";") + String(molStructPlugin_->next_bond_index_)
								+ String(";")+ String(atom_one_index) 
								+ String(";") + String(atom_two_index)
								+ String(";") + String(bond_order);

							command += add_bond_string;

							if (molStructPlugin_)
							{
								molStructPlugin_->readWriteLock_.lockForWrite();

								molStructPlugin_->bond_to_index_[new_bond_identifier] = molStructPlugin_->next_bond_index_;
								molStructPlugin_->index_to_bond_[molStructPlugin_->next_bond_index_] = new_bond_identifier;
								molStructPlugin_->handle_to_bond_[new_bond_identifier] =  new Bond(*bond,true);

								molStructPlugin_->next_bond_index_++;

								while (molStructPlugin_->index_to_bond_.has(molStructPlugin_->next_bond_index_))
								{
									++molStructPlugin_->next_bond_index_;
								}
								molStructPlugin_->readWriteLock_.unlock();
							}
							else
							{
								Log.info() << "Could not initialize instance of MolecularStructureContainer" << std::endl;
								break;
							}
						}
					}
				}
			}

			// communicate the action
			server_->sendMessageString(command);
		}

		void OpenSimPlugin::handleRemovedComposite(OpenSimTask task)
		{	
			String command(OpenSimReceiver::REMOVE_ATOM_CONTAINER);
			command += String(";") + String(task.handle_to_atom_.size()) + ";" + String(task.handle_to_bond_.size());

			HashMap<Handle, const Atom *>::ConstIterator atom_it;	
			for (atom_it = task.handle_to_atom_.begin(); atom_it != task.handle_to_atom_.end() ;++atom_it)
			{
				Handle atom_identifier = atom_it->first;
				const Atom* atom = atom_it->second;

				if (atom && (atom_identifier != INVALID_HANDLE))
				{
					Index atom_index= -1;

					if (molStructPlugin_->atom_to_index_.has(atom_identifier))
					{
						atom_index = molStructPlugin_->atom_to_index_[atom_identifier];
					}

					if (molStructPlugin_->atom_to_index_.has(atom_identifier) && (atom_index != -1))
					{
						String remove_atom_string = String(";") + String(atom_index);
						command += remove_atom_string;
						
						//Log.info() << "Remove_atom_string Messagge from BALLView to OpenSim @ handleRemovedComposite : " << remove_atom_string << std::endl;

						if (molStructPlugin_)
						{
							molStructPlugin_->readWriteLock_.lockForWrite();
							molStructPlugin_->atom_to_index_[atom_identifier] = -1;
							molStructPlugin_->readWriteLock_.unlock();
						}
					}
				}
			}

			HashMap<Handle, OpenSimTask::BondStruct>::ConstIterator bond_it;
			for (bond_it = task.handle_to_bond_.begin(); bond_it != task.handle_to_bond_.end(); ++bond_it)
			{
				Handle bond_identifier = bond_it->first;
				const Bond* bond = bond_it->second.bond;

				if (bond)
				{
					Index bondIndex = -1;

					if (molStructPlugin_->bond_to_index_.has(bond_identifier))
					{
						bondIndex = molStructPlugin_->bond_to_index_[bond_identifier];
					}

					if (molStructPlugin_->bond_to_index_.has(bond_identifier) && (bondIndex != -1))
					{
						String remove_bond_string = String(";") + String(bondIndex);
						command += remove_bond_string;

						//Log.info() << "Remove_bond_string Messagge from BALLView to OpenSim : " << remove_bond_string << std::endl;
						
						if (molStructPlugin_)
						{
							molStructPlugin_->readWriteLock_.lockForWrite();
							molStructPlugin_->bond_to_index_[bond_identifier] = -1;
							molStructPlugin_->readWriteLock_.unlock();
						}
					}
				}
			}
	
			// communicate the action
			server_->sendMessageString(command);

			molStructPlugin_->index_to_atom_.clear();
			molStructPlugin_->atom_to_index_.clear();
			molStructPlugin_->index_to_bond_.clear();
			molStructPlugin_->bond_to_index_.clear();

			molStructPlugin_->handle_to_atom_.clear();
			molStructPlugin_->handle_to_bond_.clear();
		}

		
		void OpenSimPlugin::handleChangedComposite(OpenSimTask task)
		{
			String command(OpenSimReceiver::UPDATE_ATOM_CONTAINER);
			command += String(";") + String(task.handle_to_atom_.size()) + ";" + String(task.handle_to_bond_.size());

			HashMap<Handle, const Atom *>::ConstIterator atom_it;	
			for (atom_it= task.handle_to_atom_.begin(); atom_it != task.handle_to_atom_.end() ;++atom_it)
			{
				Handle atom_identifier 	= atom_it->first;
				const Atom* atom 				= atom_it->second;
	
				if (atom && (atom_identifier != INVALID_HANDLE))
				{
					Index atom_index = -1;
					
					String element 					= atom->getElement().getSymbol();
					const Vector3& position = atom->getPosition();
					ColorRGBA& color 				= ecp_.getColorMap()[atom->getElement().getAtomicNumber()];
					float radius 						= atom->getRadius();

					// new atom
					if (!molStructPlugin_->atom_to_index_.has(atom_identifier))
					{
						atom_index = molStructPlugin_->next_atom_index_;
						
						String add_atom_string =  String(";") + String(atom_index) 
										+ String(";") + String(element) 
										+ String(";" )+ String(position.x) 
										+ String(";") + String(position.y) 
										+ String(";") + String(position.z) 
										+ String(";") + String(radius) 
										+ String(";") + String((int)color.getRed())
										+ String(";") + String((int)color.getBlue())
										+ String(";") + String((int)color.getGreen());
		
						command += add_atom_string;

						Log.info() << "Add_atom_string Message @ handleChangedComposite from BALLView to OpenSim : " << add_atom_string << std::endl;
						if (molStructPlugin_)
						{
							molStructPlugin_->readWriteLock_.lockForWrite();
							molStructPlugin_->atom_to_index_[atom_identifier]  = atom_index;
							molStructPlugin_->index_to_atom_[atom_index] = atom_identifier;
							
							//TODO why not the following?
							//molStructPlugin_->handle_to_atom_[atom_identifier] =  new Atom(*atom,true);

							molStructPlugin_->next_atom_index_ ++;

							while (molStructPlugin_->index_to_atom_.has(molStructPlugin_->next_atom_index_))
							{
								++molStructPlugin_->next_atom_index_;
							}

							molStructPlugin_->readWriteLock_.unlock();
						}
						else
						{
							Log.info() << "Could not initialize instance of MolecularStructureContainer" << std::endl;
							break;
						}
					}
					else //if (molStructPlugin_->atom_to_index_.has(atom_identifier))
					{ 
						// we already know this atom
						atom_index =  molStructPlugin_->atom_to_index_[atom_identifier]; 
						if (atom_index != -1)
						{
							// Check if the current atom has really changed by 
							// comparing to the corresponding atom in OpenSim
							bool atom_has_changed = false;
							molStructPlugin_->readWriteLock_.lockForWrite();

							if (molStructPlugin_->handle_to_atom_.has(atom_identifier))
							{
								//TODO: AKD: what is store? -->rename 
								Atom * stored_atom = molStructPlugin_->handle_to_atom_[atom_identifier];
								if (stored_atom)
								{
									String 		 stored_atom_element 	= stored_atom->getElement().getSymbol();
									Vector3& 	 stored_atom_position = stored_atom->getPosition();
									float 		 stored_atom_radius 	= stored_atom->getRadius();
									//ColorRGBA& stored_atom_color 	  = ecp_.getColorMap()[stored_atom->getElement().getAtomicNumber()];
									
									if (element != stored_atom_element)
									{
										Element new_element(atom->getElement());
										stored_atom->setElement(new_element);
										atom_has_changed = true;
									}
		
									if ( 	(position.x != stored_atom_position.x) 
											||(position.y != stored_atom_position.y) 
											||(position.z != stored_atom_position.z))
									{
										stored_atom->setPosition(position);
										atom_has_changed = true;
									}
		
									if (radius != stored_atom_radius)
									{
										stored_atom->setRadius(radius);
										atom_has_changed = true;
									}

									// TODO: We need to check for color changes. since there is no method "setColor()".
									/*if(color != stored_atom_color)
									{
										stored_atom->setColor(color);
										atom_has_changed = true;
									}*/
									/*if((int)color.getBlue() != (int)stored_color.getBlue())
									{
										atom_has_changed = true;
									}
									if((int)color.getGreen() != (int)stored_color.getGreen())
									{
										atom_has_changed = true;
									}
									if((int)color.getRed() != (int)stored_color.getRed())
									{
										atom_has_changed = true;
									}*/

									molStructPlugin_->readWriteLock_.unlock();

									if (atom_has_changed)
									{
										String update_atom_string = String(";") + String(atom_index) 
												+ String(";") + String(element) 
												+ String(";") + String(position.x) 
												+ String(";") + String(position.y) 
												+ String(";") + String(position.z) 
												+ String(";") + String(radius) 
												+ String(";") + String((int)color.getRed())
												+ String(";") + String((int)color.getBlue())
												+ String(";") + String((int)color.getGreen());
										
										command += update_atom_string;

										Log.info() << "Update_atom_string Messagge from BALLView to OpenSim : "<<update_atom_string<<std::endl;
									}
								}
							}
						}				
					} // end of known atom
				}
			} // end of all atoms


			// now lets iterate over all bonds
			HashMap<Handle, OpenSimTask::BondStruct>::ConstIterator bond_it;
			for (bond_it = task.handle_to_bond_.begin(); bond_it != task.handle_to_bond_.end(); ++bond_it)
			{
				Handle bond_identifier = bond_it->first;
				const Bond* bond = bond_it->second.bond;

				if (bond && (bond_identifier != INVALID_HANDLE))
				{
					short bond_order  = bond->getOrder();
					Index bond_index = -1;

					// both atoms connected by that current bond
					Handle atom_one_handle = INVALID_HANDLE;
					Handle atom_two_handle = INVALID_HANDLE;

					Index atom_one_index = -1;
					Index atom_two_index = -1;

					if (task.handle_to_bond_.has(bond_identifier))
					{
						OpenSimTask::BondStruct bondStruct = task.handle_to_bond_[bond_identifier];
						atom_one_handle = bondStruct.atom_one_handle;
						atom_two_handle = bondStruct.atom_two_handle;
					}

					if (molStructPlugin_->atom_to_index_.has(atom_one_handle))
					{
						atom_one_index = molStructPlugin_->atom_to_index_[atom_one_handle];
					}

					if (molStructPlugin_->atom_to_index_.has(atom_two_handle))
					{
						atom_two_index = molStructPlugin_->atom_to_index_[atom_two_handle];
					}

					// the bond is a sofar unknown 
					if (!molStructPlugin_->bond_to_index_.has(bond_identifier))
					{
						if ((atom_one_index != -1) && (atom_two_index != -1))
						{
							String add_bond_string = String(";") + String(molStructPlugin_->next_bond_index_)
								+ String(";")+ String(atom_one_index) 
								+ String(";") + String(atom_two_index) 
								+ String(";") + String(bond_order);

							command += add_bond_string;

							Log.info() << "Add_bond_string Messagge @ handleNewComposite from BALLView to OpenSim : " << add_bond_string << std::endl;

							if (molStructPlugin_)
							{
								molStructPlugin_->readWriteLock_.lockForWrite();
								molStructPlugin_->bond_to_index_[bond_identifier] = molStructPlugin_->next_bond_index_;
								molStructPlugin_->index_to_bond_[molStructPlugin_->next_bond_index_] = bond_identifier;
								molStructPlugin_->next_bond_index_++;
								while (molStructPlugin_->index_to_bond_.has(molStructPlugin_->next_bond_index_)) ++molStructPlugin_->next_bond_index_;
								molStructPlugin_->readWriteLock_.unlock();
							}
							else
							{
								Log.info() << "Could not initialize instance of MolecularStructureContainer" << std::endl;
								break;
							}
						}
					}
					else //if (molStructPlugin_->bond_to_index_.has(bond_identifier))
					{  
						// we know this bond already
						bond_index = molStructPlugin_->bond_to_index_[bond_identifier];

						if (bond_index != -1)
						{
							if ((atom_one_index != -1) && (atom_two_index != -1))
							{
								// Check if the current bond is really changed 
								// by comparing to the existing bond in the store
								//TODO AKD: what is store?

								bool bond_has_changed = false;

								molStructPlugin_->readWriteLock_.lockForWrite();

								if (molStructPlugin_->handle_to_bond_.has(bond_identifier))
								{
									Bond * bond_in_store =  molStructPlugin_->handle_to_bond_[bond_identifier];
									if (bond_in_store)
									{
										if (bond_order != bond_in_store->getOrder())
										{
											bond_in_store->setOrder(bond_order);
											bond_has_changed = true;
										}

										// TODO: AKD: shouldn we check the bond length also?
										if (bond_has_changed)
										{
											String update_bond_string = String(";")+ String(bond_index) 
												+ String(";") + String(atom_one_index) 
												+ String(";") + String(atom_two_index) 
												+ String(";") + String(bond_order);

											command += update_bond_string;

											Log.info() << "update_bond_string Messagge from BALLView to OpenSim : "<<update_bond_string<<std::endl;
										}
									}
								}
								molStructPlugin_->readWriteLock_.unlock();
							}
						}
					}
				}
			}
			
			// communicate the action
			server_->sendMessageString(command);

			// TODO: Should we need to handle the removed bonds that attached to removed atoms?
			// AKD: I dont think so! BALLView directly deletes bonds to deleted atoms!
		}

		void OpenSimPlugin::handleRepresentation(OpenSimTask task)
		{
			String new_positions(NULL);
			Size number_of_changed_atoms = 0;
			HashMap<Handle, const Atom *>::ConstIterator atom_it;	

			for (atom_it = task.handle_to_atom_.begin(); atom_it != task.handle_to_atom_.end(); ++atom_it)
			{
				const Atom* atom =  atom_it->second;
				
				// Don't use atom->getHandle() since we need to know stored handle
				Handle atom_identifier = atom_it->first;
			
				if (atom && (atom_identifier != INVALID_HANDLE))
				{
					Index atom_index = -1;	

					String 				 element 	= atom->getElement().getSymbol();
					const Vector3& position = atom->getPosition();
					float  				 radius 	= atom->getRadius();
					ColorRGBA& 		 color 		= ecp_.getColorMap()[atom->getElement().getAtomicNumber()];

					if (!molStructPlugin_->atom_to_index_.has(atom_identifier))
					{
						//Log.info() << " new atom_identifier at handlerep : " << atom_identifier << std::endl; 

						atom_index =  molStructPlugin_->next_atom_index_;
						String add_atom_string(OpenSimReceiver::ADD_ATOM);

    				add_atom_string +=  String(";") + String(atom_index) 
								+ String(";") + String(element) 
								+ String(";" )+ String(position.x) 
								+ String(";") + String(position.y) 
								+ String(";") + String(position.z) 
								+ String(";") + String(radius) 
								+ String(";") + String((int)color.getRed())
								+ String(";") + String((int)color.getBlue())
								+ String(";") + String((int)color.getGreen());
		
						server_->sendMessageString(add_atom_string);

						//Log.info() << "Add_atom_string @ handleRepresentation "  << add_atom_string << std::endl;
					
						if (molStructPlugin_)
						{
							molStructPlugin_->readWriteLock_.lockForWrite();
							molStructPlugin_->index_to_atom_[atom_index] = atom_identifier;
							molStructPlugin_->atom_to_index_[atom_identifier]  = atom_index;
							molStructPlugin_->handle_to_atom_[atom_identifier] =  new Atom(*atom,true);
							molStructPlugin_->next_atom_index_++;

							while (molStructPlugin_->index_to_atom_.has(molStructPlugin_->next_atom_index_))
							{
								++molStructPlugin_->next_atom_index_;
							}
					
							molStructPlugin_->readWriteLock_.unlock();
						}
					}
					else //if (molStructPlugin_->atom_to_index_.has(atom_identifier))
					{
						atom_index = molStructPlugin_->atom_to_index_[atom_identifier]; 
								
						if (atom_index != -1)
						{
							// TODO: Would this slow down the process?

							// Check if atom is really changed by comparing to the existing atom in the store
							bool atom_has_changed 			= false;
							bool atom_position_changed 	= false;

							molStructPlugin_->readWriteLock_.lockForWrite();

							if (molStructPlugin_->handle_to_atom_.has(atom_identifier))
							{
								Atom* stored_atom =  molStructPlugin_->handle_to_atom_[atom_identifier];

								if (stored_atom)
								{
									String 			stored_element 	= stored_atom->getElement().getSymbol();
									Vector3& 		stored_position = stored_atom->getPosition();
									float 			stored_radius 	= stored_atom->getRadius();
									//ColorRGBA& 	stored_color 	= ecp_.getColorMap()[stored_atom->getElement().getAtomicNumber()];

									if (element != stored_element)
									{
										Element newelement(atom->getElement());
										stored_atom->setElement(newelement);
										atom_has_changed = true;
									}

									if (   (position.x != stored_position.x) 
											|| (position.y != stored_position.y) 
											|| (position.z != stored_position.z))
									{
										stored_atom->setPosition(position);
										atom_has_changed = true;
									}
									if (radius != stored_radius)
									{
										stored_atom->setRadius(radius);
										atom_has_changed = true;
									}
									/*if((int)color.getBlue() != (int)stored_color.getBlue())
									{
										atom_has_changed = true;
									}
									if((int)color.getGreen() != (int)stored_color.getGreen())
									{
										atom_has_changed = true;
									}
									if((int)color.getRed() != (int)stored_color.getRed())
									{
										atom_has_changed = true;
									}*/

									if (atom_has_changed)
									{
										String update_atom_string(OpenSimReceiver::UPDATE_ATOM);

										update_atom_string =   update_atom_string + String(";") 
											 									 + String(atom_index) + String(";") 
											                   + String(element) + String(";") 
																				 + String(position.x) + String(";") 
																				 + String(position.y) + String(";") 
																				 + String(position.z) + String(";") 
																				 + String(radius) + String(";") 
																				 + String((int)color.getRed())+ String(";") 
																				 + String((int)color.getBlue())+ String(";") 
																				 + String((int)color.getGreen());

										server_->sendMessageString(update_atom_string);

										Log.info() << "Update_atom_string Messagge @ handleRepresentation from BALLView to OpenSim : " << update_atom_string << std::endl;	
									}

									if (atom_position_changed)
									{
										// Update the position later as a batch.
										number_of_changed_atoms++;
										const Vector3& pos = atom->getPosition();
										
										new_positions += String(";") + String(atom_index) 
												+ String(";")+ String(pos.x)
												+ String(";") + String(pos.y)
												+ String(";") + String(pos.z);

										//Log.info() << "Update representation for atom " << atom_index << "  " << pos.x << "  " << pos.y << "  "<< pos.z << std::endl;
									}
								}
							}
							molStructPlugin_->readWriteLock_.unlock();
						}
					}
				}
			}

			if ((number_of_changed_atoms > 0) && (!new_positions.isEmpty()))
			{
				String update_string(OpenSimReceiver::UPDATE_POSITIONS);

				update_string =	  update_string + String(";") 
								+ String(number_of_changed_atoms) 
								+ new_positions;

				server_->sendMessageString(update_string);
		
				Log.info() << "Update representation Message for atoms @ handleRepresentation ,  from BALLView to OpenSim : "<<update_string<<std::endl;
			}
	
			// Note : This code should be here. Can not be moved up, since the logic
			// Check for removed atoms here

			HashMap<Handle, Index>::ConstIterator it;
			for (it = molStructPlugin_->atom_to_index_.begin(); it != molStructPlugin_->atom_to_index_.end(); ++it)
			{
				Handle atomTobeChecked_identifier = it->first;
				bool  atom_found = false ;
				Index atom_index = -1;

				if (   task.handle_to_atom_.has(atomTobeChecked_identifier) 
						&& (atomTobeChecked_identifier != INVALID_HANDLE))
				{
					if (task.handle_to_atom_[atomTobeChecked_identifier])
					{
						atom_found = true;
					}
				}
				
				if (!atom_found) 
				{
					
					if (molStructPlugin_->atom_to_index_.has(atomTobeChecked_identifier))
					{
						atom_index = molStructPlugin_->atom_to_index_[atomTobeChecked_identifier]; 
					}

					//Log.info() << "atomTobeChecked_identifier : "<<atomTobeChecked_identifier<<std::endl;
					//Log.info() << "atom_index : "<<atom_index<<std::endl;	
					//Log.info() << "atom_index to be removed : "<< atom_index<<std::endl;	

					if (atom_index != -1) 
					{
						// This means the perticulr atom with 'index' is removed from the structure
						String remove_atom_string(OpenSimReceiver::REMOVE_ATOM);
						
						// use "atm_index"  only
						remove_atom_string = remove_atom_string +  String(";") + String(atom_index) ;
						
						server_->sendMessageString(remove_atom_string);

						Log.info() << "Remove_atom_string Message @ handleRepresentation from BALLView to OpenSim : "<<remove_atom_string<<std::endl;	

						// NOTE: //TODO
						// I can not remove atoms or atom_index from the hashmaps 
						// I get exception
						// with out removing then.. it works fine, but conceptuallty..they shouuld be removed  
						// whent they are removed  from the structure
						// I think , when atoms are removed in BALLView , since their reference are stored in hashmap 
						// hashmap looses atoms' references
						if (molStructPlugin_)
						{
							molStructPlugin_->readWriteLock_.lockForWrite();
							molStructPlugin_->atom_to_index_[atomTobeChecked_identifier] = -1;
							molStructPlugin_->handle_to_atom_[atomTobeChecked_identifier] = NULL;
							molStructPlugin_->readWriteLock_.unlock();
						}
					}
				}
			}

			HashMap<Handle, OpenSimTask::BondStruct>::ConstIterator bond_it;
				
			for (bond_it = task.handle_to_bond_.begin(); bond_it != task.handle_to_bond_.end(); ++bond_it)
			{
				const Bond* bond = bond_it->second.bond;

				// Dont use bond->getHandle()
				Handle bond_identifier = bond_it->first ;
				
				if (bond)
				{
					if (!molStructPlugin_->bond_to_index_.has(bond_identifier))
					{
						// Create a bond code here since it was not found in the bond_to_index_
						Size bond_order =  bond->getOrder();

						Handle atom_one_handle = -1;
						Handle atom_two_handle = -1;

						Index atom_one_index = -1;
						Index atom_two_index = -1;

						if (task.handle_to_bond_.has(bond_identifier))
						{
							OpenSimTask::BondStruct bondStruct = task.handle_to_bond_[bond_identifier];
							atom_one_handle = bondStruct.atom_one_handle;
							atom_two_handle = bondStruct.atom_two_handle;
						}

						if (molStructPlugin_->atom_to_index_.has(atom_one_handle))
						{
							atom_one_index = molStructPlugin_->atom_to_index_[atom_one_handle];
						}
					
						if (molStructPlugin_->atom_to_index_.has(atom_two_handle))
						{
							atom_two_index = molStructPlugin_->atom_to_index_[atom_two_handle];
						}

						if (atom_one_index != -1 && atom_two_index != -1 )
						{
							//Log.info() << " new_bond_identifier at handlerep : " << bond_identifier << std::endl;

							String add_bond_string(OpenSimReceiver::ADD_BOND);

							add_bond_string = add_bond_string + String(";")+ String(molStructPlugin_->next_bond_index_)+ String(";") + String(atom_one_index) + String(";") + String(atom_two_index) + String(";") + String(bond_order) ;
								
							server_->sendMessageString(add_bond_string);
									
							Log.info() << "Add_bond_string Message @ handleRepresentation from BALLView to OpenSim : "<<add_bond_string<<std::endl;
						
							// Note //TODO
							// I can not add bond  from the hashmaps 
							// I get exception as the atoms are already removed
							if (molStructPlugin_)
							{
								molStructPlugin_->readWriteLock_.lockForWrite();

								molStructPlugin_->bond_to_index_[bond_identifier] = molStructPlugin_->next_bond_index_;
								molStructPlugin_->index_to_bond_[molStructPlugin_->next_bond_index_] = bond_identifier;

								molStructPlugin_->handle_to_bond_[bond_identifier] =  new Bond(*bond,true);

								molStructPlugin_->next_bond_index_++;

								while (molStructPlugin_->index_to_bond_.has(molStructPlugin_->next_bond_index_)) 
								{
									++molStructPlugin_->next_bond_index_;
								}
				
								molStructPlugin_->readWriteLock_.unlock();
							}
						}
					}
					else if(molStructPlugin_->bond_to_index_.has(bond_identifier))
					{
						// NOw I know "bond" is in molStructPlugin_->bond_to_index_
						// update bond information should be here

						Index bondIndex = molStructPlugin_->bond_to_index_[bond_identifier]; 

						if (bondIndex != -1)
						{
							Bond::Order bond_order =  bond->getOrder();
							Handle atom_one_handle = INVALID_HANDLE;
							Handle atom_two_handle = INVALID_HANDLE;
							Index atom_one_index = -1;
							Index atom_two_index = -1;

							if (task.handle_to_bond_.has(bond_identifier))
							{
								OpenSimTask::BondStruct bondStruct = task.handle_to_bond_[bond_identifier];
								atom_one_handle = bondStruct.atom_one_handle;
								atom_two_handle = bondStruct.atom_two_handle;
							}

							if (molStructPlugin_->atom_to_index_.has(atom_one_handle))
							{
									atom_one_index = molStructPlugin_->atom_to_index_[atom_one_handle];
							}
						
							if (molStructPlugin_->atom_to_index_.has(atom_two_handle))
							{
									atom_two_index = molStructPlugin_->atom_to_index_[atom_two_handle];
							}

							if ((atom_one_index != -1) && (atom_two_index != -1))
							{
								// Check if bond is really changed by comparing to the existing atom in the store
								bool bond_has_changed = false;

								molStructPlugin_->readWriteLock_.lockForWrite();

								if (molStructPlugin_->handle_to_bond_.has(bond_identifier))
								{
									Bond * bond_in_store =  molStructPlugin_->handle_to_bond_[bond_identifier];
									if (bond_in_store)
									{
										if (bond_order != bond_in_store->getOrder())
										{
											bond_in_store->setOrder(bond_order);
											bond_has_changed = true;
										}
										molStructPlugin_->readWriteLock_.unlock();

										if (bond_has_changed)
										{
											String update_bond_string(OpenSimReceiver::UPDATE_BOND);

											// Note : Bond index in the commnad string
											update_bond_string =  update_bond_string 
																 + String(";") + String(bondIndex) 
																 + String(";") + String(atom_one_index) 
																 + String(";") + String(atom_two_index) 
																 + String(";") + String(bond_order) ;
																					
											server_->sendMessageString(update_bond_string);

											Log.info() << "Update_bond_string Message @ handleRepresentation from BALLView to OpenSim : "<<update_bond_string<<std::endl;
										}
									}
								}
							}
						}
					}
				}
			}

			// The code for removing bonds  should be here
			
			HashMap<Handle, Index>::ConstIterator bit;
				
			for (bit= molStructPlugin_->bond_to_index_.begin(); bit!= molStructPlugin_->bond_to_index_.end() ;++bit)
			{
				Handle bondTobeChecked_identifier = bit->first;
				bool isBondfound = false ;

				if (task.handle_to_bond_.has(bondTobeChecked_identifier))
				{
					if (task.handle_to_bond_[bondTobeChecked_identifier].bond != NULL )
					{
						isBondfound = true;
						//break;
					}
				}

				if (!isBondfound) 
				{
					Index bond_index =  -1;

					if (molStructPlugin_->bond_to_index_.has(bondTobeChecked_identifier))
					{
						bond_index = molStructPlugin_->bond_to_index_[bondTobeChecked_identifier];
					}

					if (bond_index != -1)
					{
						// This means the perticulr bond with 'index' is removed from the structure
						String remove_bond_string(OpenSimReceiver::REMOVE_BOND);

						//atom index has to be from bond : this is not correct to use in the following way
						remove_bond_string = remove_bond_string + String(";")+ String(bond_index);
						
						server_->sendMessageString(remove_bond_string);

						Log.info() << "Remove_bond_string Message @ handleRepresentation from BALLView to OpenSim : "<<remove_bond_string<<std::endl;
			
						if(molStructPlugin_)
						{
							molStructPlugin_->readWriteLock_.lockForWrite();
							molStructPlugin_->bond_to_index_[bondTobeChecked_identifier] = -1;
							molStructPlugin_->handle_to_bond_[bondTobeChecked_identifier] = NULL;
							molStructPlugin_->readWriteLock_.unlock();
						}
					}
				}
			}
		}// End of function
			
	}
}
