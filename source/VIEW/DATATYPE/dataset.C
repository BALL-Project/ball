// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//

#include <BALL/VIEW/DATATYPE/dataset.h>
#include <BALL/VIEW/WIDGETS/datasetControl.h>
#include <BALL/VIEW/KERNEL/common.h>
#include <BALL/VIEW/KERNEL/mainControl.h>
#include <BALL/VIEW/KERNEL/message.h>
#include <BALL/CONCEPT/molecularInformation.h>

#include <QtWidgets/QFileDialog>
#include <QtCore/QStringList>

using namespace std;

namespace BALL
{

	namespace VIEW
	{

		Dataset::Dataset()
			: composite_(0)
		{
		}

		Dataset::Dataset(const Dataset& ds)
			: composite_(ds.composite_),
				name_(ds.name_),
				type_(ds.type_)
		{
		}

		Dataset::~Dataset()
		{
			#ifdef BALL_VIEW_DEBUG
				cout << "Destructing object " << (void *)this << " of class " << RTTI::getName<Dataset>() << endl;
			#endif 
		}

		void Dataset::clear()
		{
			composite_ = 0;
			name_ = "";
			type_ = "";
		}

		void Dataset::set(const Dataset& ds)
		{
			composite_ = ds.composite_;
			name_ = ds.name_;
			type_ = ds.type_;
		}

		const Dataset& Dataset::operator = (const Dataset& v)
		{
			set(v);
			return *this;
		}

		void Dataset::dump(std::ostream& s, Size depth) const
		{
			BALL_DUMP_STREAM_PREFIX(s);
			
			BALL_DUMP_DEPTH(s, depth);
			BALL_DUMP_HEADER(s, this, this);

			BALL_DUMP_STREAM_SUFFIX(s);
		}

		////////////////////////////////////////////////////////////////////////
		// Controller:
		
		DatasetController::DatasetController()
			: QObject(),
				Embeddable("DatasetController"),
				type_("undefined type"),
				file_formats_(),
				control_(0)
		{
			registerThis();
		}

		DatasetController::DatasetController(DatasetController& dc)
			: QObject(),
				Embeddable(dc),
				type_(dc.type_),
				file_formats_(dc.file_formats_),
				control_(dc.control_)
		{
			registerThis();
		}

		DatasetController::~DatasetController()
		{
		}


		bool DatasetController::write(Dataset* /*set*/, String /*filetype*/, String /*filename*/)
		{
			Log.error() << "DatasetController::write() should have been overloaded in derived class!" <<std::endl;
			return false;
		}

		Dataset* DatasetController::open(String /*filetype*/, String /*filename*/)
		{
			Log.error() << "DatasetController::open() should have been overloaded in derived class!" <<std::endl;
			return 0;
		}

		bool DatasetController::insertDataset(Dataset* set)
		{
			if (getDatasetControl() == 0)
			{
				BALLVIEW_DEBUG
				Log.error() << "DatasetController not bound to Dataset!" << std::endl;
				return false;
			}

			if (set == 0)
			{
				BALLVIEW_DEBUG
				return false;
			}

			QStringList sl;
			sl << set->getName().c_str();
			if (set->getComposite() != 0)
			{
				MolecularInformation mi;
				mi.visit(*set->getComposite());
				sl << mi.getName().c_str();
			}
			else
			{
				sl << "";
			}
			sl << set->getType().c_str();

			DatasetControl* dc = getDatasetControl();
 			QTreeWidgetItem* item = dc->addRow(sl);
			item_to_dataset_[item] = set;
			dataset_to_item_[set]  = item;

			// Make sure that DatasetControl dockwidget is visible after loading a data set. Else people will wonder what happened to the data set they just tried to load ...
			dc->show();
			dc->raise();

			return true;
		}

		QMenu* DatasetController::buildContextMenu(QTreeWidgetItem* item)
		{
			if (!item_to_dataset_.has(item)) return 0;

			QMenu* menu = new QMenu(control_);
			menu->addAction("Save as...", this, SLOT(write()));
			menu->addAction("Delete", this, SLOT(deleteDataset()));
			return menu;
		}

		vector<Dataset*> DatasetController::getDatasets()
		{
			vector<Dataset*> result;
			HashMap<Dataset*, QTreeWidgetItem*>::Iterator it = dataset_to_item_.begin();
			for (; it != dataset_to_item_.end(); ++it)
			{
				result.push_back((*it).first);
			}

			return result;
		}

		vector<Dataset*> DatasetController::getSelectedDatasets()
		{
			vector<Dataset*> result;

			if (getDatasetControl() == 0)
			{
				BALLVIEW_DEBUG
				Log.error() << "DatasetController not bound to Dataset!" << std::endl;
				return result;
			}

			DatasetControl::ItemList il = getDatasetControl()->getSelectedItems();
			DatasetControl::ItemList::iterator it = il.begin();
			for (; it != il.end(); it++)
			{
				if (item_to_dataset_.has(*it))
				{
					result.push_back(item_to_dataset_[*it]);
				}
			}

			return result;
		}

		Dataset* DatasetController::getSelectedDataset()
		{
			vector<Dataset*> v = getSelectedDatasets();
			if (v.size() != 1) return 0;
			
			if (getDatasetControl()->getSelectedItems().size() == 1)
			{
				return v[0];
			}

			return 0;
		}


		bool DatasetController::deleteDatasets()
		{
			bool ok = true;

			vector<Dataset*> sets = getSelectedDatasets();
			for (Position p = 0; p < sets.size(); p++)
			{
				Dataset* dp = sets[p];
				if (dp == 0)
				{
					BALLVIEW_DEBUG
					return false;
				}

				ok &= deleteDataset(dp);
			}

			return ok;
		}

		bool DatasetController::deleteDataset()
		{
			bool ok = true;
			vector<Dataset*> v =  getSelectedDatasets();
			for (Position p = 0; p < v.size(); p++)
			{
				Dataset* set = v[p];
				if (set == 0) return false;

				ok &= deleteDataset(set);
			}

			return ok;
		}

		bool DatasetController::deleteDataset(Dataset* set)
		{
			if (getDatasetControl() == 0)
			{
				BALLVIEW_DEBUG
				Log.error() << "DatasetController not bound to Dataset!" << std::endl;
				return false;
			}

			if (set == 0)
			{
				BALLVIEW_DEBUG
				return false;
			}

			if (!dataset_to_item_.has(set))
			{
				return false;
			}

 			QTreeWidgetItem* item = dataset_to_item_[set];
			dataset_to_item_.erase(set);
			item_to_dataset_.erase(item);

			getMainControl()->sendMessage(*(new DatasetMessage(set, DatasetMessage::REMOVE)));

			delete item;
			deleteDataset_(set);

			return true;
		}


		bool DatasetController::createMenuEntries()
		{
			if (getDatasetControl() == 0)
			{
				BALLVIEW_DEBUG
				Log.error() << "DatasetController not bound to Dataset!" << std::endl;
				return false;
			}
			
			String temp_type(type_);
			temp_type.substitute(" ", "_");
			String description = "Shortcut|File|Open|Dataset|" + temp_type;
			
			QAction* action = getDatasetControl()->insertMenuEntry(MainControl::FILE_OPEN, tr(type_.c_str()), this, 
												                                     SLOT(open()), description, QKeySequence(),
																														 tr(""), UIOperationMode::MODE_ADVANCED);
			actions_for_one_set_.insert(action);

			return true;
		}

		String DatasetController::getFileTypes_()
		{
			String result(type_);
			result += " (";
			for (Position p = 0; p < file_formats_.size(); p++)
			{
				result += " *.";
				result += file_formats_[p];
			}

			result += ")";
			return result;
		}


		bool DatasetController::write()
		{
			Dataset* set = getSelectedDataset();
			if (set == 0) return 0;

			QString file = QFileDialog::getSaveFileName(0,
													QString("Save as ") + type_.c_str(),
													getDatasetControl()->getWorkingDir().c_str(),
													getFileTypes_().c_str());

			if (file == QString::null) return false;

			vector<String> fields;
			String s(ascii(file));
			s.split(fields, ".");
			if (fields.size() == 0) 
			{
				BALLVIEW_DEBUG
				return false;
			}

			return write(set, fields[fields.size() - 1], ascii(file));
		}

		bool DatasetController::open()
		{
			QString file = QFileDialog::getOpenFileName(0,
													QString("Open ") + type_.c_str(),
													getDatasetControl()->getWorkingDir().c_str(),
													getFileTypes_().c_str());

			if (file == QString::null) return false;

			vector<String> fields;
			String s(ascii(file));
			s.split(fields, ".");
			if (fields.size() == 0) 
			{
				BALLVIEW_DEBUG
				return false;
			}

			Dataset* set = open(fields[fields.size() - 1], ascii(file));
			if (set == 0) return false;

			getMainControl()->sendMessage(*(new DatasetMessage(set, DatasetMessage::ADD)));

			return true;
		}

		QAction* DatasetController::insertMenuEntry_(Position pid, const QString& name, const char* slot, 
		                                             const String& description, QKeySequence accel, 
																								 const QString& menu_hint, UIOperationMode::OperationMode minimal_mode)
		{
			if (getDatasetControl() == 0) return 0;
			QAction* action = getDatasetControl()->insertMenuEntry(pid, name, this, 
			                                                       slot, description, accel,
																														 menu_hint, minimal_mode);

			actions_.push_back(action);
			actions_for_one_set_.insert(action);
			return action;
		}

		String DatasetController::getNameFromFileName_(String filename)
		{
			Position pos = 0;
			for (Position p = 0; p < filename.size(); p++)
			{
				if (filename[p] == FileSystem::PATH_SEPARATOR) pos = p;
			}
			
			if (pos) pos++;
			return filename.getSubstring(pos);
		}

		void DatasetController::setStatusbarText(const String& text, bool important)
		{
			if (getDatasetControl() == 0) return;

			getDatasetControl()->setStatusbarText(text, important);
		}

		void DatasetController::setStatusbarText(const QString& text, bool important)
		{
			if (getDatasetControl() == 0) return;

			getDatasetControl()->setStatusbarText(text, important);
		}

		void DatasetController::checkMenu(MainControl& mc)
		{
			Size selected = getSelectedDatasets().size();
			Size all_selected = getDatasetControl()->getSelectionSize();

			bool other_selection = selected != all_selected;

			for (Position p = 0; p < actions_.size(); p++)
			{
				if (actions_[p])
				{
					QAction& action = *actions_[p];

					if (other_selection || mc.isBusy() || selected == 0)
					{
						action.setEnabled(false);
						continue;
					}

					if (selected > 1 && actions_for_one_set_.has(&action))
					{
						action.setEnabled(false);
						continue;
					}

					action.setEnabled(true);
				}
			}
		}

		bool DatasetController::handle(DatasetMessage* msg)
		{
			DatasetMessage::Type type = msg->getType();
			if (type == DatasetMessage::ADD)
			{
				insertDataset(msg->getDataset());
				return true;
			}

			if (type == DatasetMessage::REMOVE)
			{
				deleteDataset(msg->getDataset());
				return true;
			}

			return false;
		}
		
		bool DatasetController::hasDataset(Dataset* set)
		{
			return dataset_to_item_.has(set);
		}

		bool DatasetController::hasItem(QTreeWidgetItem* item)
		{
			return item_to_dataset_.has(item);
		}

		Dataset* DatasetController::getDataset(QTreeWidgetItem* item)
		{
			if (!item_to_dataset_.has(item)) return 0;
			return item_to_dataset_[item];
		}

	} // namespace VIEW
} // namespace BALL
