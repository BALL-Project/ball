// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//

#include <BALL/VIEW/DIALOGS/assignBondOrderResultsDialog.h>
#include <BALL/VIEW/KERNEL/mainControl.h>
#include <BALL/STRUCTURE/sdGenerator.h>
#include <BALL/VIEW/KERNEL/message.h>

#include <QtGui/QPushButton>
#include <QtGui/QTreeWidget>

#include <sstream>

namespace BALL
{
	namespace VIEW
	{
		AssignBondOrderResultsDialog::AssignBondOrderResultsDialog(QWidget* parent, const char* name)
			: QDialog(parent),
				Ui_AssignBondOrderResultsDialogData(),
				ModularWidget(name),
				bond_order_processor_(0),
				root_(NULL),
				activated_item_(NULL)
		{
			#ifdef BALL_VIEW_DEBUG
				Log.error() << "new AssignBondOrderResultsDialog " << this << std::endl;
			#endif

			setupUi(this);
			setObjectName(name);

			// register the widget with the MainControl
			ModularWidget::registerWidget(this);
			hide();
			connect(addAsNew_button, SIGNAL(clicked()), this, SLOT(addSolutionToStructures()));
			connect(cancel_button, SIGNAL(clicked()), this, SLOT(hide()));
			connect(computeNextSolution_button, SIGNAL(clicked()), this, SLOT(computeNextSolution()));
			connect(queries, SIGNAL(itemActivated(QTreeWidgetItem*, int)), this, SLOT(switchView(QTreeWidgetItem*, int)));
		}

		AssignBondOrderResultsDialog::~AssignBondOrderResultsDialog()
		{
#ifdef BALL_VIEW_DEBUG
			Log.error() << "deleting AssignBondOrderResultsDialog " << this << std::endl;
#endif

			clearEntries();
		}

		void AssignBondOrderResultsDialog::setProcessor(AssignBondOrderProcessor* abop)
		{
			clearEntries();
			bond_order_processor_ = abop;
		}

		void AssignBondOrderResultsDialog::addSolutionToStructures()
		{
			// is this item connected with a system?
			if (sd_systems_.find(activated_item_) != sd_systems_.end())
			{
				System* org_system = sd_systems_.find(activated_item_)->second;
				System* new_system = new System(*org_system);

				new_system->setName(solution_systems_[activated_item_]->getName());

				// try to set correct coordinates
				if (new_system->countAtoms() == bond_order_processor_->getAtomContainer()->countAtoms())
				{
					AtomIterator at2 = new_system->beginAtom();
					for (AtomIterator at =  bond_order_processor_->getAtomContainer()->beginAtom(); +at && +at2; ++at, ++at2)
					{
						if (at->getName() == at2->getName())
						{
							at2->setPosition(at->getPosition());
						}
					}
				}

				// inform BV
				getMainControl()->insert(*new_system);
				getMainControl()->update(*new_system);

				// highlight as before
				list<Composite*> sel;
				Composite* to_highlight = bond_order_processor_->getAtomContainer()->getParent();

				if (to_highlight)
				{
					sel.push_back(to_highlight);
					ControlSelectionMessage* msg = new ControlSelectionMessage();
					msg->setSelection(sel);
					notify_(msg);
				}
			}
			else
			{
				Log.error()<< "Invalid state! " << __FILE__ << " " << __LINE__ << std::endl;
			}
		}

		void AssignBondOrderResultsDialog::computeNextSolution()
		{
			if (bond_order_processor_->computeNextSolution())
			{
				// add this solution the results
				// generate the tree entry
				QString name = bond_order_processor_->getSolution(0).getName().c_str();

				// This is a hack
				if (name.trimmed() == "")
				{
					name = "<Molecule>";
				}
				Size num_of_sol = bond_order_processor_->getNumberOfComputedSolutions();

				std::ostringstream stream_name;
				stream_name.setf(std::ios_base::fixed);
				stream_name.precision(2);

				stream_name << ascii(name) << "_sol_" << num_of_sol << "_" << bond_order_processor_->getTotalPenalty(num_of_sol-1);
				String sol_text = stream_name.str();

				std::ostringstream stream_description;
				stream_description.setf(std::ios_base::fixed);
				stream_description.precision(2);

				stream_description << "\n" << ascii(name)
				                             << ":\n\n      solution : " << num_of_sol
				                             <<    "\n      penalty  : " << bond_order_processor_->getTotalPenalty(num_of_sol-1);
				// 			     << "\n  charge     : " << bond_order_processor_->getTotalCharge(num_of_sol-1);

				String description = stream_description.str();

				QTreeWidgetItem* current_item = new QTreeWidgetItem(root_, QStringList(sol_text.c_str()));
				descriptions_[current_item] = description;
				solution_number_[current_item] = num_of_sol-1;

				System S = bond_order_processor_->getSolution(num_of_sol-1);
				solution_systems_[current_item] = new System(S);

				SDGenerator sdg(true);
				try {
					sdg.generateSD(S);
				} catch (...)
				{
				}

				sd_systems_[current_item] = new System(S);

				queries->setCurrentItem(current_item);
				current_item->setSelected(true);
				queries->update();

				// now update the view
				switchView(current_item, num_of_sol-1);
			}	
			else
			{
				Log.info() << "There are no further solutions!" << std::endl;
				setStatusbarText(tr("There are no further solutions!"), true);

			}
		}

		void AssignBondOrderResultsDialog::finished()
		{
			hide();
		}

		void AssignBondOrderResultsDialog::createEntries()
		{
			// before creating new entries, remove the old ones!
			clearEntries();

			if (bond_order_processor_)
			{
				// generate the tree entries

				// first the root element
				QList<QTreeWidgetItem *> query_results;
				QString name = bond_order_processor_->getSolution(0).getName().c_str();

				// This is a hack
				if (name.trimmed() == "")
				{
					name = "<Molecule>";
				}

				QTreeWidgetItem* query_item = new QTreeWidgetItem((QTreeWidget*)0, QStringList(name));

				query_results.append(query_item);
				queries->insertTopLevelItems(0, query_results);
				queries->expandItem(query_item);

				// store the root 
				root_ = query_item;

				QTreeWidgetItem* first_item_in_list = NULL;
				// now create for each solution a child
				Size num_of_sol = bond_order_processor_->getNumberOfComputedSolutions();
				for (Size i=0; i < num_of_sol; i++)
				{
					std::ostringstream stream_name;
					stream_name.setf(std::ios_base::fixed);
					stream_name.precision(2);

					stream_name << ascii(name) << "_sol_" << i+1 << "_" << bond_order_processor_->getTotalPenalty(i);
					String sol_text = stream_name.str();

					std::ostringstream stream_description;
					stream_description.setf(std::ios_base::fixed);
					stream_description.precision(2);

					stream_description << "\n  " << ascii(name)
					                   << ":\n\n      solution : " << i+1
					                   <<  "\n      penalty  : " << bond_order_processor_->getTotalPenalty(i);
					//                 << "\n  charge   : " << bond_order_processor_->getTotalCharge(i);

					String description = stream_description.str();

					QTreeWidgetItem* current_item = new QTreeWidgetItem(query_item, QStringList(sol_text.c_str()));

					if (i==0)
						first_item_in_list = current_item;

					descriptions_[current_item] = description;
					solution_number_[current_item] = i;

					System S = bond_order_processor_->getSolution(i);
					solution_systems_[current_item] = new System(S);

					// The SDGenerator sometimes crashes ...
					try {
						// place the atoms in a plane
						SDGenerator sdg(true);
						sdg.generateSD(S);
					} catch (...)
					{
					}
					// create the 2D image
					sd_systems_[current_item] = new System(S);

					queries->setCurrentItem(current_item);
					//current_item->setSelected(true);
					//queries->update();
				}

				//switch View to the first solution
				queries->setCurrentItem(first_item_in_list);
				first_item_in_list->setSelected(true);

				bond_order_processor_->resetBondOrders();
				switchView(first_item_in_list, 0);
			}
		}

		void AssignBondOrderResultsDialog::clearEntries()
		{
			std::map<QTreeWidgetItem*, System*>::iterator it = sd_systems_.begin();
			for (; it != sd_systems_.end(); it++)
			{
				delete it->second;
			}

			it = solution_systems_.begin();
			for (; it != solution_systems_.end(); it++)
			{
				delete it->second;
			}

			sd_systems_.clear();
			solution_systems_.clear();
			solution_number_.clear();
			descriptions_.clear();
			sd_widget->clear();
			queries->clear();
		}

		void AssignBondOrderResultsDialog::show()
		{
			QDialog::show();
			raise();
		}

		void AssignBondOrderResultsDialog::switchView(QTreeWidgetItem* item, int /*column*/)
		{
			// is this item connected with a system?
			if (sd_systems_.find(item) != sd_systems_.end())
			{
				activated_item_ = item;

				sd_widget->plot(*sd_systems_[item], false);
				if (applyToSelected_checkBox->isChecked())
				{
					bond_order_processor_->apply(solution_number_[item]);
					getMainControl()->update(*(bond_order_processor_->getAtomContainer()));
				}
			}

			// do we have a description for it?
			if (descriptions_.find(item) != descriptions_.end())
			{
				text_field->setText(QString(descriptions_[item].c_str()));
				addAsNew_button->setEnabled(true);
			}
			else
			{
				addAsNew_button->setEnabled(false);
			}
		}

		void AssignBondOrderResultsDialog::initializeWidget(MainControl&)
		{
		}
	}
}
