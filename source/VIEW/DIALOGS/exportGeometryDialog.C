// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//

#include <BALL/VIEW/DIALOGS/exportGeometryDialog.h>
#include <BALL/VIEW/KERNEL/mainControl.h>
#include <BALL/VIEW/KERNEL/message.h>
#include <BALL/VIEW/KERNEL/common.h>

#include <BALL/VIEW/PRIMITIVES/box.h>
#include <BALL/VIEW/PRIMITIVES/disc.h>
#include <BALL/VIEW/PRIMITIVES/gridVisualisation.h>
#include <BALL/VIEW/PRIMITIVES/label.h>
#include <BALL/VIEW/PRIMITIVES/mesh.h>
#include <BALL/VIEW/PRIMITIVES/simpleBox.h>

#include <QtGui/QFileDialog>
#include <QtGui/QMessageBox>
#include <QtGui/QLineEdit>


namespace BALL
{
	namespace VIEW
	{

ExportGeometryDialog::ExportGeometryDialog(QWidget* parent, const char* name)
	:	QDialog(parent),
		Ui_ExportGeometryDialogData()
{
#ifdef BALL_VIEW_DEBUG
	Log.error() << "new ExportGeometryDialog " << this << std::endl;
#endif

	setupUi(this);

  // signals and slots connections
	QObject::connect( browse_button, SIGNAL( clicked() ), this, SLOT( browseFiles() ) );
  QObject::connect( prototyping_radio, SIGNAL( clicked() ), this, SLOT( protomode() ) );
  QObject::connect( view_radio, SIGNAL( clicked() ), this, SLOT( viewmode() ) );

	setObjectName(name);
}

ExportGeometryDialog::~ExportGeometryDialog()
{
#ifdef BALL_VIEW_DEBUG
	Log.error() << "deleting ExportGeometryDialog " << this << std::endl;
#endif
}



void ExportGeometryDialog::exec()
{
	MainControl* mc = getMainControl();
	if (mc == 0) return;

	RepresentationManager& rm = mc->getRepresentationManager();

	//creation of the listpart of the GUI
	listview->setColumnCount(3);
	listview->setRowCount(rm.getRepresentations().size());
	listview->setHorizontalHeaderItem(0, new QTableWidgetItem());
	listview->setHorizontalHeaderItem(1, new QTableWidgetItem());
	listview->setHorizontalHeaderItem(2, new QTableWidgetItem());
	listview->horizontalHeaderItem(0)->setText(tr("Model"));
	listview->horizontalHeaderItem(1)->setText(tr("Coloring"));
	listview->horizontalHeaderItem(2)->setText(tr("Properties"));
	listview->setColumnWidth(0, 140);
	listview->setColumnWidth(1, 140);
	listview->setColumnWidth(2, 140);

	const ModelInformation& mi = mc->getModelInformation();

	Position row= 0;
	RepresentationList::const_iterator it = rm.getRepresentations().begin();

	//filling of the list: as "for viewing" is the standard configuration all reps are checkable 
	for (; it != rm.getRepresentations().end(); it++)
	{
		const Representation& rep = **it;

		QTableWidgetItem* item = new QTableWidgetItem(rep.getName().c_str());
		item->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled | Qt::ItemIsEnabled | Qt::ItemIsUserCheckable);

		//the same represantations are checked as in BALLView
		if ( (*it)->isHidden() )
		{
			item->setCheckState(Qt::Unchecked);
		}
		else
		{
			item->setCheckState(Qt::Checked);
		}

		listview->setItem(row, 0, item);

		item = new QTableWidgetItem(mi.getColoringName(rep.getColoringMethod()).c_str());
		listview->setItem(row, 1, item);

		item = new QTableWidgetItem(rep.getProperties().c_str());
		listview->setItem(row, 2, item);
		row++;
	}
	split_ = false;
	vrml_ = false;
	stl_ = false;
	raise();
	QDialog::exec();
}


void ExportGeometryDialog::accept()
{
	hide();

	//before we do anything at all we have to store the information which representations are now visible
	//as the export works by switching them on and off => saving base stats
	MainControl* mc = getMainControl();
	RepresentationManager& rm = mc->getRepresentationManager();
	RepresentationList::const_iterator rit;

	Position count = 0;

	rit = rm.getRepresentations().begin();
	for (; rit != rm.getRepresentations().end(); rit++)
	{
		if(!((*rit)->isHidden()))
		{
			basestats[count]=true;
			(*rit)->setHidden(true);
		}
		else
		{
			basestats[count]=false;
		}
		count ++;
	}

//we have to get to know which representations the user wants to export:
	for (Position pos = 0; pos < (Position)listview->rowCount(); pos++)
	{
		const QTableWidgetItem& item = *listview->item(pos, 0);
		if (item.checkState() == Qt::Checked)
		{
			reps[pos] = true;
		}
		else
		{
			reps[pos] = false;
		}
	}

	//now we look whether we have to split them into several files:
	if ((prototyping_radio->isChecked()) || (split_radio->isChecked()))
	{
		split_ = true;
	}
	else
	{
		split_ = false;
	}

	if (vrml_box->isChecked())
	{
		vrml_ = true;
	}
	else
	{
		vrml_ = false;
	}

	if (stl_box->isChecked())
	{
		stl_ = true;
	}
	else
	{
		stl_ = false;
	}
	setFilename(file_edit->displayText());
}


	void ExportGeometryDialog::browseFiles()
	{
		QString fname = QFileDialog::getSaveFileName(0, tr("Export as 3D file"), getMainControl()->getWorkingDir().c_str(), "*.*");
		if (fname == QString::null)
		{
			return;
		}
		setFilename(fname);
		file_edit->setText(fname);
	}

	bool ExportGeometryDialog::export_vrml()
	{
		return vrml_;
	}

	bool ExportGeometryDialog::export_stl()
	{
		return stl_;
	}

	bool ExportGeometryDialog::split()
	{
		return split_;
	}

	void ExportGeometryDialog::protomode()
	{
		MainControl* mc = getMainControl();
		if (mc == 0) return;

		RepresentationManager& rm = mc->getRepresentationManager();

		const ModelInformation& mi = mc->getModelInformation();

		Position row= 0;
		bool printable;
		RepresentationList::const_iterator it = rm.getRepresentations().begin();
		for (; it != rm.getRepresentations().end(); it++)
		{
			const Representation& rep = **it;
			printable = true;

			list<GeometricObject*>::const_iterator git = (*it)->getGeometricObjects().begin();
			for (; git != (*it)->getGeometricObjects().end(); git++)
			{
				//now we test if the geo object is printable
				//all geometric objects that are just helpers as the grid but are not exported are counted as printable
				// mesh (5) is the only real printable representation
				//
				// TODO: what about spheres, e.g.??
				if (! (	 RTTI::isKindOf<Box> (**git)
							 ||RTTI::isKindOf<Disc>(**git)
							 ||RTTI::isKindOf<GridVisualisation>(**git)
							 ||RTTI::isKindOf<Label>(**git)
							 ||RTTI::isKindOf<Mesh>(**git)
							 ||RTTI::isKindOf<SimpleBox>(**git)
							)
					 )
				{
					printable=false;
					break;
				}

			}

			if (printable)
			//item is the same as normal
			{

				QTableWidgetItem* item = new QTableWidgetItem(rep.getName().c_str());
				item->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled | Qt::ItemIsEnabled | Qt::ItemIsUserCheckable);
				if ( (*it)->isHidden() )
				{
					item->setCheckState(Qt::Unchecked);
				}
				else
				{
					item->setCheckState(Qt::Checked);
				}

				listview->setItem(row, 0, item);

				item = new QTableWidgetItem(mi.getColoringName(rep.getColoringMethod()).c_str());
				listview->setItem(row, 1, item);

				item = new QTableWidgetItem(rep.getProperties().c_str());
				listview->setItem(row, 2, item);
				row++;
			}
			else
			{
				QTableWidgetItem* item = new QTableWidgetItem(rep.getName().c_str());
				item->setFlags( Qt::ItemIsEnabled | Qt::ItemIsEnabled );
				item->setCheckState(Qt::Unchecked);

				listview->setItem(row, 0, item);

				item = new QTableWidgetItem(mi.getColoringName(rep.getColoringMethod()).c_str());
				item->setFlags( Qt::ItemIsEnabled | Qt::ItemIsEnabled );
				listview->setItem(row, 1, item);

				item = new QTableWidgetItem(rep.getProperties().c_str());
				item->setFlags( Qt::ItemIsEnabled | Qt::ItemIsEnabled );
				listview->setItem(row, 2, item);
				row++;
			}
		}
		raise();
	}

	void ExportGeometryDialog::viewmode()
	{
		MainControl* mc = getMainControl();
		if (mc == 0) return;

		RepresentationManager& rm = mc->getRepresentationManager();

		const ModelInformation& mi = mc->getModelInformation();

		Position row= 0;
		RepresentationList::const_iterator it = rm.getRepresentations().begin();
		for (; it != rm.getRepresentations().end(); it++)
		{
			const Representation& rep = **it;

			QTableWidgetItem* item = new QTableWidgetItem(rep.getName().c_str());
			item->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled | Qt::ItemIsEnabled | Qt::ItemIsUserCheckable);
			if ( (*it)->isHidden() )
			{
				item->setCheckState(Qt::Unchecked);
			}
			else
			{
				item->setCheckState(Qt::Checked);
			}

			listview->setItem(row, 0, item);

			item = new QTableWidgetItem(mi.getColoringName(rep.getColoringMethod()).c_str());
			listview->setItem(row, 1, item);

			item = new QTableWidgetItem(rep.getProperties().c_str());
			listview->setItem(row, 2, item);
			row++;
		}
		raise();

	}
} } // namespaces
