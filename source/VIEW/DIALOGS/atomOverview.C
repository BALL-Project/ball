// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id:

#include <BALL/VIEW/DIALOGS/atomOverview.h>
#include <BALL/VIEW/KERNEL/mainControl.h>
#include <BALL/VIEW/KERNEL/common.h>
#include <BALL/VIEW/KERNEL/message.h>
#include <BALL/KERNEL/PTE.h>

namespace BALL
{
	namespace VIEW
	{

AtomOverview::AtomOverview(QWidget* parent, const char* name)
	:	QDialog(parent),
		Ui_AtomOverviewData(),
		parent_(0),
		only_selection_(false)
{
#ifdef BALL_VIEW_DEBUG
	Log.error() << "new AtomOverview " << this << std::endl;
#endif
	setupUi(this);

	setObjectName(name);
	setModal(true);
	setSizeGripEnabled(true);
	processor_.setTable(table);
	apply_processor_.setTable(table);
	apply_processor_.setMainControl(VIEW::getMainControl());

	connect(table, SIGNAL(itemChanged(QTableWidgetItem*)), this, SLOT(itemChanged(QTableWidgetItem*)));
	connect(table, SIGNAL(itemClicked(QTableWidgetItem*)), this, SLOT(itemActivated(QTableWidgetItem*)));

	hide();
}

AtomOverview::~AtomOverview()
{
#ifdef BALL_VIEW_DEBUG
	Log.error() << "deleting AtomOverview " << this << std::endl;
#endif
}

void AtomOverview::setParent(AtomContainer* ac) 
{ 
	parent_ = ac;

	if (parent_ == 0 ||
			VIEW::getMainControl() == 0)
	{
		BALLVIEW_DEBUG;
		return;
	}
	
	table->clear();
	table->setColumnCount(6);
	table->setHorizontalHeaderItem(0, new QTableWidgetItem("Atom"));
	table->setHorizontalHeaderItem(1, new QTableWidgetItem("Element"));
	table->setHorizontalHeaderItem(2, new QTableWidgetItem("Type"));
	table->setHorizontalHeaderItem(3, new QTableWidgetItem("Radius"));
	table->setHorizontalHeaderItem(4, new QTableWidgetItem("Charge"));
	table->setHorizontalHeaderItem(5, new QTableWidgetItem("FCharge"));
	ignore_ = true;
	processor_.showOnlySelection(only_selection_);
	parent_->apply(processor_);
	ignore_ = false;
	showMaximized();
}


void AtomOverview::accept()
{
	QDialog::accept();
	apply_processor_.showOnlySelection(only_selection_);
	parent_->apply(apply_processor_);
	VIEW::getMainControl()->update(*parent_);
	VIEW::getMainControl()->sendMessage(*new NewSelectionMessage());
	VIEW::getMainControl()->setStatusbarText(tr("Applied values"), true);
}


AtomOverview::OverviewProcessor::OverviewProcessor()
	: UnaryProcessor<Composite>(),
	  table_(0)
{
}

AtomOverview::OverviewProcessor::OverviewProcessor(const OverviewProcessor&)
	: UnaryProcessor<Composite>(),
		table_(0)
{
}

AtomOverview::OverviewProcessor::~OverviewProcessor()
{
}

Processor::Result AtomOverview::OverviewProcessor::operator() (Composite& composite)
{
	if (RTTI::isKindOf<Atom>(composite))
	{
		Atom* atom = dynamic_cast<Atom*>(&composite);
		if (only_selection_ && !atom->isSelected()) return Processor::CONTINUE;

		Position r = table_->rowCount();
		table_->setRowCount(r + 1);

		// Name
		Position c = 0;
		String s = atom->getFullName(Atom::ADD_VARIANT_EXTENSIONS_AND_ID);
		QTableWidgetItem* item = new QTableWidgetItem(s.c_str());
		table_->setItem(r, c, item);
		item->setCheckState(Qt::Checked);
		if (!atom->isSelected()) item->setCheckState(Qt::Unchecked);

		// Element
		s = atom->getElement().getSymbol();
		c++;
		table_->setItem(r, c, new QTableWidgetItem(s.c_str()));
		if (atom->getElement().isUnknown()) table_->item(c, r)->setBackgroundColor(Qt::red);

		// Type
		s = atom->getType();
		c++;
		table_->setItem(r, c, new QTableWidgetItem(s.c_str()));
 		if (atom->getType() == -1) table_->item(r, c)->setBackgroundColor(Qt::red);

		// Radius
		float f = atom->getRadius();
		s = String(f);
		c++;
		table_->setItem(r, c, new QTableWidgetItem(s.c_str()));
		if (f <= 0.0 || f > 4.0) table_->item(r, c)->setBackgroundColor(Qt::red);

		// Charge
		f = atom->getCharge();
		s = String(f);
		c++;
		table_->setItem(r, c, new QTableWidgetItem(s.c_str()));
		
		// calculate min and max possible charge
		Position p = (Position) atom->getElement().getGroup();
		Index min = -8;
		Index max = 8;

		// HauptGruppe
		if (p< 3 || p > 13)
		{
			if (p > 12) p-= 10;
			min = -p;
			max = 8 - p;
		}

		if (f > (float) max ||
				f < (float) min)
		{
			table_->item(r, c)->setBackgroundColor(Qt::red);
		}
		
		// FCharge
		f = atom->getFormalCharge();
		s = String(f);
		c++;
		table_->setItem(r, c, new QTableWidgetItem(s.c_str()));
	
	}

	return Processor::CONTINUE;
}

void AtomOverview::itemChanged(QTableWidgetItem* item)
{
	if (ignore_ || (item == 0)) return;
	Index c = table->column(item);

	String s = ascii(item->text());
	if (c == 1)
	{
		Element e = PTE.getElement(s);
		if (e.isUnknown()) restoreItem_(item);
		return;
	}

	try
	{
		// Atom Type
		if (c == 2)
		{
			Index i = s.toInt();
			if (i < -1) restoreItem_(item);
			return;
		}

		float f = s.toFloat();

		// Radius
		if (c == 3 && (f <= 0.0))
		{
			restoreItem_(item);
			return;
		}

		// c == 4: charge already cared by toFloat!
	}
	catch(...)
	{
		restoreItem_(item);
	}
}

void AtomOverview::restoreItem_(QTableWidgetItem* item)
{
	if (item == 0 || table->column(item) <= 0) return;
	item->setText(item_backup_.c_str());
}

void AtomOverview::itemActivated(QTableWidgetItem* item)
{
	if (item == 0 || table->column(item) <= 0) 
	{
		return;
	}

	item_backup_ = ascii(item->text());
}


AtomOverview::ApplyProcessor::ApplyProcessor()
	: UnaryProcessor<Composite>(),
	  table_(0)
{
}

AtomOverview::ApplyProcessor::ApplyProcessor(const ApplyProcessor&)
	: UnaryProcessor<Composite>(),
		table_(0)
{
}

AtomOverview::ApplyProcessor::~ApplyProcessor()
{
}

Processor::Result AtomOverview::ApplyProcessor::operator() (Composite& composite)
{
	if (RTTI::isKindOf<Atom>(composite))
	{
		Atom* atom = dynamic_cast<Atom*>(&composite);

		if (only_selection_ && !atom->isSelected()) return Processor::CONTINUE;

		String s;

		bool selected = (table_->item(row_, 0)->checkState() == Qt::Checked);
		if (selected != atom->isSelected())
		{
			if (selected) main_control_->selectCompositeRecursive(atom, true);
			else 					main_control_->deselectCompositeRecursive(atom, true);
		}

		s = ascii(table_->item(row_, 1)->text());
		atom->setElement(PTE.getElement(s));

		s = ascii(table_->item(row_, 2)->text());
		atom->setType(s.toInt());

		s = ascii(table_->item(row_, 3)->text());
		atom->setRadius(s.toFloat());

		s = ascii(table_->item(row_, 4)->text());
		atom->setCharge(s.toFloat());

		s = ascii(table_->item(row_, 5)->text());
		atom->setFormalCharge(s.toInt());

		row_ ++;
	}

	return Processor::CONTINUE;
}

} } // namespaces
