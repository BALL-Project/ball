
#include <BALL/VIEW/DIALOGS/evolutionDockingDialog.h>
#include "../../STRUCTURE/DOCKING/evolutionaryDocking.h"
#include <BALL/VIEW/KERNEL/common.h>
#include <BALL/VIEW/KERNEL/mainControl.h>
#include <BALL/VIEW/WIDGETS/molecularStructure.h>
#include <BALL/SYSTEM/path.h>
#include <BALL/STRUCTURE/geometricProperties.h>

#include <QtGui/qlabel.h>
#include <QtGui/qlineedit.h>
#include <QtGui/qpushbutton.h>
#include <QtGui/QFileDialog>
#include <QtGui/qmessagebox.h>

//#define BALL_VIEW_DEBUG

namespace BALL
{
	namespace VIEW
	{
		// Constructor
		EvolutionDockingDialog::EvolutionDockingDialog(QWidget* parent, const char* name)
			throw()
				: DockingAlgorithmDialog(parent),
					Ui_EvolutionDockingDialogData(),
					trans_box_rep_(0),
					trans_box_(0),
					ligand_(0)
		{
			#ifdef BALL_VIEW_DEBUG
				Log.info() << "new EvolutionDockingDialog " << this << std::endl;
			#endif

				setupUi(this);
				setObjectName(name);
			
				// register QWidgets of Dialog with PreferenceEntry
				// entries of them will be generated in the INIFile
				setINIFileSectionName("EVOLUTION_DOCKKING_OPTIONS");
				registerObject_(grid_group);
				registerObject_(grid_filename);
				registerObject_(trans_box_group);
				registerObject_(trans_box_bottom_x);
				registerObject_(trans_box_bottom_y);
				registerObject_(trans_box_bottom_z);
				registerObject_(trans_box_top_x);
				registerObject_(trans_box_top_y);
				registerObject_(trans_box_top_z);
				registerObject_(max_iterations);
				registerObject_(init_population);	
				registerObject_(population);
				registerObject_(survivors);
				registerObject_(force_field_group);
        inifile_section_name_backup_ = "EVOLUTION_DOCKKING_OPTIONS_REDOCK";
		
				connect(cancel_button, SIGNAL(clicked()), this, SLOT(reject()));
				connect(ok_button, SIGNAL( clicked() ), this, SLOT(accept()));
				connect(reset_button, SIGNAL(pressed()), this, SLOT(reset()));
			  connect(grid_radio_button, SIGNAL(clicked()), this, SLOT(enableFileBrowsing()));
				connect(new_grid_radio_button, SIGNAL(clicked()), this, SLOT(disableFileBrowsing()));
				connect(browse_button, SIGNAL(clicked()), this, SLOT(browseGridFile()));
				connect(force_field_button, SIGNAL(clicked()), this, SLOT(showForceFieldOptions()));
				connect(trans_box_button, SIGNAL(clicked()), this, SLOT(showTranslationBox()));
			}
		
		// Copy constructor.
		EvolutionDockingDialog::EvolutionDockingDialog(const EvolutionDockingDialog& ev_dock_dialog)
			throw()
			: DockingAlgorithmDialog(ev_dock_dialog),
				Ui_EvolutionDockingDialogData(),
				trans_box_rep_(0),
				trans_box_(0),
				ligand_(0)
		{}
			
		// Destructor
		EvolutionDockingDialog::~EvolutionDockingDialog()
			throw()
		{
			#ifdef BALL_VIEW_DEBUG
				Log.info() << "Destructing object " << this << " of class EvolutionDockingDialog" << std::endl;
			#endif 
		}
		
		// Assignment operator
		const EvolutionDockingDialog& EvolutionDockingDialog::operator =(const EvolutionDockingDialog& ev_dock_dialog)
			throw()
		{
			if (&ev_dock_dialog != this)
			{
				DockingAlgorithmDialog::operator=(ev_dock_dialog);
			}
			return *this;
		}

		//Read the preferences from an INIFile
		void EvolutionDockingDialog::fetchPreferences(INIFile& file)
			throw()
		{
			DockingAlgorithmDialog::fetchPreferences(file);
			if(!grid_filename->text().isEmpty())
			{
				enableFileBrowsing();
			}
			return;
		}
				
		// Fill options with values of the dialog.
		void EvolutionDockingDialog::getOptions(Options& options)
					throw()
		{
		  try
			{
				if(new_grid_radio_button->isChecked())
				{
					options[EvolutionaryDocking::Option::GRID_FILE] = "";
				}
				else
				{
					options[EvolutionaryDocking::Option::GRID_FILE] = String(ascii(grid_filename->text()));
				}

				options[EvolutionaryDocking::Option::MAX_ITERATIONS] = ascii(max_iterations->text()).toInt();
				options[EvolutionaryDocking::Option::INITIAL_POPULATION] = ascii(init_population->text()).toInt();
				options[EvolutionaryDocking::Option::POPULATION] = ascii(population->text()).toInt();
				options[EvolutionaryDocking::Option::SURVIVORS] = ascii(survivors->text()).toInt();
				
				if(grid_boundary_radio_button->isChecked())
				{
					options[EvolutionaryDocking::Option::TRANSLATION_BOX_BOTTOM_X] = 0.0;
					options[EvolutionaryDocking::Option::TRANSLATION_BOX_BOTTOM_Y] = 0.0;
					options[EvolutionaryDocking::Option::TRANSLATION_BOX_BOTTOM_Z] = 0.0;
					options[EvolutionaryDocking::Option::TRANSLATION_BOX_TOP_X] = 0.0;
					options[EvolutionaryDocking::Option::TRANSLATION_BOX_TOP_Y] = 0.0;
					options[EvolutionaryDocking::Option::TRANSLATION_BOX_TOP_Z] = 0.0;
				}

				if(abs_trans_radio_button->isChecked())
				{
					options[EvolutionaryDocking::Option::TRANSLATION_BOX_BOTTOM_X] = ascii(trans_box_bottom_x->text()).toFloat();
					options[EvolutionaryDocking::Option::TRANSLATION_BOX_BOTTOM_Y] = ascii(trans_box_bottom_y->text()).toFloat();
					options[EvolutionaryDocking::Option::TRANSLATION_BOX_BOTTOM_Z] = ascii(trans_box_bottom_z->text()).toFloat();
					options[EvolutionaryDocking::Option::TRANSLATION_BOX_TOP_X] = ascii(trans_box_top_x->text()).toFloat();
					options[EvolutionaryDocking::Option::TRANSLATION_BOX_TOP_Y] = ascii(trans_box_top_y->text()).toFloat();
					options[EvolutionaryDocking::Option::TRANSLATION_BOX_TOP_Z] = ascii(trans_box_top_z->text()).toFloat();
				}
				else
				{
					GeometricCenterProcessor gcp;
					if (ligand_ == NULL)
					{
						Log.error() << "No ligand! Cannot apply GeometricCenterProcessor! " << __FILE__ << " " << __LINE__ << std::endl;
						return;
					}
					ligand_->apply(gcp);
					Vector3 center = gcp.getCenter();

					options[EvolutionaryDocking::Option::TRANSLATION_BOX_BOTTOM_X] = ascii(trans_box_bottom_x->text()).toFloat()+center.x;
					options[EvolutionaryDocking::Option::TRANSLATION_BOX_BOTTOM_Y] = ascii(trans_box_bottom_y->text()).toFloat()+center.y;
					options[EvolutionaryDocking::Option::TRANSLATION_BOX_BOTTOM_Z] = ascii(trans_box_bottom_z->text()).toFloat()+center.z;
					options[EvolutionaryDocking::Option::TRANSLATION_BOX_TOP_X] = ascii(trans_box_top_x->text()).toFloat()+center.x;
					options[EvolutionaryDocking::Option::TRANSLATION_BOX_TOP_Y] = ascii(trans_box_top_y->text()).toFloat()+center.y;
					options[EvolutionaryDocking::Option::TRANSLATION_BOX_TOP_Z] = ascii(trans_box_top_z->text()).toFloat()+center.z;
				}
			}
		  catch (Exception::InvalidFormat)
			{
				Log.error() << "Conversion from String to float, double or int failed: invalid format! " << __FILE__ << " " << __LINE__ << std::endl;
				return;
			}
		  
		}
	
		// Fill options with values of the force field dialog.
		ForceField* EvolutionDockingDialog::getForceField()
					throw()
		{
			MolecularStructure* mol_struct = MolecularStructure::getInstance(0);
			if (!mol_struct)
			{
				Log.error() << "Error while applying options of AMBER_FF scoring function! " << __FILE__ << " " << __LINE__<< std::endl;
				return NULL;
			}

			if(amber_radio_button->isChecked())
			{
				AmberConfigurationDialog& dialog =	mol_struct->getAmberConfigurationDialog();
				AmberFF& ff = mol_struct->getAmberFF();
				// now the Amber force field gets its options
				dialog.applyTo(ff);
				return &ff;
			}
			else
			{
				CharmmConfigurationDialog& dialog = mol_struct->getCharmmConfigurationDialog();
				CharmmFF& ff = mol_struct->getCharmmFF();
				// now the Charmm force field gets its options
				dialog.applyTo(ff);
				return &ff;
			}
		}
		  
		void EvolutionDockingDialog::setSystem(System* s)
					throw()
		{
			ligand_ = s;
		}
				
		
				
	// --------------------------------- SLOTS ------------------------------------------------
	// ----------------------------------------------------------------------------------------

		/** Is called when radio button for using a created grid is pressed. 
		 	* It enables the browse button and to corresponding line edit.
		 	*/
		void EvolutionDockingDialog::enableFileBrowsing()
		{
			browse_button->setEnabled(true);
			grid_filename->setEnabled(true);
			filename_label->setEnabled(true);
		}

		/** Is called when radio button for using a new grid is pressed. 
		 	* It disables the browse button and to corresponding line edit.
		 	*/
		void EvolutionDockingDialog::disableFileBrowsing()
		{
			browse_button->setEnabled(false);
			grid_filename->setEnabled(false);
			grid_filename->clear();
			filename_label->setEnabled(false);
		}

		/** Is called when browse button is pressed. 
			* It shows a file dialog.
		 	*/
		void EvolutionDockingDialog::browseGridFile()
		{
			// look up the full path of the grid file
			Path p;
			String filename = p.find(ascii(grid_filename->text()));

			if (filename == "")
			{
				filename = ascii(grid_filename->text());
			}
			QString tmp = filename.c_str();
			QString result = QFileDialog::getOpenFileName(0, "Select an grid file", tmp, "*.gr", 0);
			if (!result.isEmpty())
			{
				// store the new filename in the lineedit field
				grid_filename->setText(result);
			}
		}

		/** Is called when force field options button is pressed. 
			* It shows the force field option dialog.
		 	*/
		void EvolutionDockingDialog::showForceFieldOptions()
		{
			MolecularStructure* mol_struct = MolecularStructure::getInstance(0);
			if (!mol_struct)
			{
				Log.error() << "Error while opening force field option dialog! " 
										<< __FILE__ << " " << __LINE__ << std::endl;
				return;
			}

			if(amber_radio_button->isChecked())
			{
				mol_struct->getAmberConfigurationDialog().exec();
			}
			else
			{
				mol_struct->getCharmmConfigurationDialog().exec();
			}
		}

		void EvolutionDockingDialog::showTranslationBox()
		{
			if(grid_boundary_radio_button->isChecked()) return;

			float x_bottom = ascii(trans_box_bottom_x->text()).toFloat();
			float y_bottom = ascii(trans_box_bottom_y->text()).toFloat();
			float z_bottom = ascii(trans_box_bottom_z->text()).toFloat();

			float x_top = ascii(trans_box_top_x->text()).toFloat();
			float y_top = ascii(trans_box_top_y->text()).toFloat();
			float z_top = ascii(trans_box_top_z->text()).toFloat();

			Vector3 point, right, height, depth;

			right = Vector3(x_top-x_bottom, 0, 0);
			height = Vector3(0, y_top-y_bottom, 0);
			depth = Vector3(0, 0, z_top-z_bottom);

			if (abs_trans_radio_button->isChecked())
			{
				point = Vector3(x_bottom, y_bottom, z_bottom);
			}
			else
			{
				GeometricCenterProcessor gcp;
				if (ligand_ == NULL)
				{
					QMessageBox error_message("Error","Please select docking partner 2!", 
																		QMessageBox::Critical,
																		QMessageBox::Ok,
																		QMessageBox::NoButton,
																		QMessageBox::NoButton);
					error_message.exec();
					return;
				}
				ligand_->apply(gcp);
				Vector3 center = gcp.getCenter();
				point = Vector3(x_bottom + center.x, y_bottom + center.y, z_bottom + center.z);
			}

			RepresentationManager& pm = getMainControl()->getRepresentationManager();

			if(!pm.has(*trans_box_rep_))
			{
				// remark: is deleted by main control
				trans_box_rep_ = new Representation;
				trans_box_rep_->setTransparency(90);

				// is deleted if representation is deleted
				trans_box_ = new Box(point,right,height,depth.getLength());
				ColorRGBA bcolor(0,255,190,180);
				trans_box_->setColor(bcolor);
				trans_box_rep_->insert(*trans_box_);

				getMainControl()->insert(*trans_box_rep_);
				getMainControl()->update(*trans_box_rep_);
			}
			else
			{
				Log.error() << "in update Box" << std::endl;
				if (trans_box_ == NULL) Log.error() << "Box pointer NULL" << std::endl;
				trans_box_->setPoint(point);
				trans_box_->setRightVector(right);
				trans_box_->setHeightVector(height);
				trans_box_->setDepth(depth.getLength());
				getMainControl()->update(*trans_box_rep_);
			}
		}
	} // namespace VIEW
} // namespace BALL
