// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id:

#include <BALL/VIEW/DIALOGS/atomOverview.h>
#include <BALL/VIEW/KERNEL/mainControl.h>
#include <BALL/VIEW/KERNEL/common.h>
#include <BALL/KERNEL/PTE.h>

namespace BALL
{
	namespace VIEW
	{

AtomOverview::AtomOverview(QWidget* parent, const char* name)
	throw()
	:	QDialog(parent),
		Ui_AtomOverviewData(),
		parent_(0)
{
#ifdef BALL_VIEW_DEBUG
	Log.error() << "new AtomOverview " << this << std::endl;
#endif
	setupUi(this);

	setObjectName(name);
	setModal(true);
	setSizeGripEnabled(true);
	processor_.setTable(table);

	connect(table, SIGNAL(itemChanged(QTableWidgetItem*)), this, SLOT(itemChanged(QTableWidgetItem*)));
	connect(table, SIGNAL(itemActivated(QTableWidgetItem*)), this, SLOT(itemActivated(QTableWidgetItem*)));

	hide();
}

AtomOverview::~AtomOverview()
	throw()
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
	table->setColumnCount(5);
	table->setHorizontalHeaderItem(0, new QTableWidgetItem("Atom"));
	table->setHorizontalHeaderItem(1, new QTableWidgetItem("Element"));
	table->setHorizontalHeaderItem(2, new QTableWidgetItem("Type"));
	table->setHorizontalHeaderItem(3, new QTableWidgetItem("Radius"));
	table->setHorizontalHeaderItem(4, new QTableWidgetItem("Charge"));
	parent_->apply(processor_);
}


void AtomOverview::accept()
{
	QDialog::accept();
	VIEW::getMainControl()->update(*parent_);
	VIEW::getMainControl()->setStatusbarText("Applied values", true);
}


AtomOverview::OverviewProcessor::OverviewProcessor()
	throw()
	: UnaryProcessor<Composite>(),
	  table_(0)
{
}

AtomOverview::OverviewProcessor::OverviewProcessor(const OverviewProcessor&)
	throw()
	: UnaryProcessor<Composite>(),
		table_(0)
{
}

AtomOverview::OverviewProcessor::~OverviewProcessor()
	throw()
{
}

Processor::Result AtomOverview::OverviewProcessor::operator() (Composite& composite)
{
	if (RTTI::isKindOf<Atom>(composite))
	{
		Atom* atom = dynamic_cast<Atom*>(&composite);
		Position r = table_->rowCount();
		table_->setRowCount(r + 1);

		Position c = 0;
		String s = atom->getFullName(Atom::ADD_VARIANT_EXTENSIONS_AND_ID);
		QTableWidgetItem* item = new QTableWidgetItem(s.c_str());
		table_->setItem(r, c, item);
		item->setCheckState(Qt::Checked);
		if (!atom->isSelected()) item->setCheckState(Qt::Unchecked);

		s = atom->getElement().getSymbol();
		c++;
		table_->setItem(r, c, new QTableWidgetItem(s.c_str()));
		if (atom->getElement().isUnknown()) table_->item(c, r)->setBackgroundColor(Qt::red);

		s = atom->getType();
		c++;
		table_->setItem(r, c, new QTableWidgetItem(s.c_str()));
 		if (atom->getType() == -1) table_->item(r, c)->setBackgroundColor(Qt::red);

		s = atom->getRadius();
		c++;
		table_->setItem(r, c, new QTableWidgetItem(s.c_str()));
		if (atom->getRadius() <= 0.0) table_->item(r, c)->setBackgroundColor(Qt::red);

		s = atom->getCharge();
		c++;
		table_->setItem(r, c, new QTableWidgetItem(s.c_str()));
		if (atom->getCharge() == 0.0) table_->item(r, c)->setBackgroundColor(Qt::red);
	}

	return Processor::CONTINUE;
}

void AtomOverview::itemChanged(QTableWidgetItem* item)
{
	if (item == 0) return;
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
		if (c == 2)
		{
			Index i = s.toInt();
			if (i < -1) restoreItem_(item);
			return;
		}

		float f = s.toFloat();

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
	if (item == 0) return;
	item->setText(item_backup_.c_str());
}

void AtomOverview::itemActivated(QTableWidgetItem* item)
{
Log.error() << "#~~#   2 "             << " "  << __FILE__ << "  " << __LINE__<< std::endl;
	if (item == 0) return;
	item_backup_ = ascii(item->text());
}

} } // namespaces
