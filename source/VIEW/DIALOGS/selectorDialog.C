// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
#include <BALL/VIEW/DIALOGS/selectorDialog.h>
#include <BALL/VIEW/KERNEL/mainControl.h>
#include <BALL/VIEW/KERNEL/message.h>
#include <BALL/KERNEL/selector.h>

#include <qlineedit.h> 

namespace BALL
{
	namespace VIEW
	{

SelectorDialog::SelectorDialog( QWidget* parent,  const char* name, bool modal, WFlags fl )
    : SelectorDialogData( parent, name, modal, fl )
{
}

SelectorDialog::~SelectorDialog()
{
 // no need to delete child widgets, Qt does it all for us
}

void SelectorDialog::accept()
{
	if (parentWidget() == 0) return;
	MainControl* mc = MainControl::getMainControl(parentWidget());
	if (mc == 0) return;

	Selector s;
	try
	{
		s.setExpression(Expression(line_edit->text().ascii()));
	}
	catch(...)
	{
		mc->setStatusbarText("Invalid regular expression");
		return;
	}

// 	mc->getSelection().clear();

	List<Composite*>::Iterator it = mc->getControlSelection().begin();
	for(; it != mc->getControlSelection().end(); it++)
	{
		(*it)->apply(s);
		List<Atom*>::Iterator ait = s.getSelectedAtoms().begin();
		for (; ait != s.getSelectedAtoms().end(); ait++)
		{
			mc->selectCompositeRecursive(*ait, true);
		}
		mc->updateRepresentationsOf(**it);
	}

	NewSelectionMessage* nm = new NewSelectionMessage;
	mc->sendMessage(*nm);

	mc->setStatusbarText(String("Selected " + String(s.getNumberOfSelectedAtoms()) + " Atoms."));
}

void SelectorDialog::clearSelection()
{
	if (parentWidget() == 0) return;
	MainControl* mc = MainControl::getMainControl(parentWidget());
	if (mc == 0) return;
	
	CompositeManager::CompositeIterator it = mc->getCompositeManager().begin();
	for (; it != mc->getCompositeManager().end(); it++)
	{
//	 	CompositeMessage* cm = new CompositeMessage(**it, CompositeMessage::CHANGED_COMPOSITE);
// 		mc->sendMessage(*cm);
		mc->getSelection().insert(*it);
		mc->deselectCompositeRecursive(*it);
		mc->updateRepresentationsOf(**it, true);
	}
 	mc->getSelection().clear();

 	mc->updateAllRepresentations(true);

	NewSelectionMessage* nm = new NewSelectionMessage;
	mc->sendMessage(*nm);
}

// NAMESPACE
} }
