// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: selectorDialog.C,v 1.9 2004/01/18 16:43:58 oliver Exp $
//

#include <BALL/VIEW/DIALOGS/selectorDialog.h>
#include <BALL/VIEW/KERNEL/mainControl.h>
#include <BALL/VIEW/KERNEL/message.h>
#include <BALL/KERNEL/selector.h>

#include <qlineedit.h> 
#include <qmessagebox.h> 

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

			HashSet<Composite*> roots;

			CompositeManager::CompositeIterator it = mc->getCompositeManager().begin();
			for(; it != mc->getCompositeManager().end(); it++)
			{
				(*it)->apply(s);
				List<Atom*>::Iterator ait = s.getSelectedAtoms().begin();
				for (; ait != s.getSelectedAtoms().end(); ait++)
				{
					mc->selectCompositeRecursive(*ait, true);
					roots.insert(&(*ait)->getRoot());
				}
			}

			HashSet<Composite*>::Iterator sit = roots.begin();
			for (; sit != roots.end(); sit++)
			{
				// faster, but doesnt always work:
		//	 	mc->updateRepresentationsOf(**sit, false);
				mc->updateRepresentationsOf(**sit, true, true);
			}

			NewSelectionMessage* nm = new NewSelectionMessage;
			nm->setOpenItems(true);
			mc->sendMessage(*nm);

			mc->setStatusbarText(String("Selected " + String(s.getNumberOfSelectedAtoms()) + " Atoms."));
		}

		void SelectorDialog::clearSelection()
		{
			if (parentWidget() == 0) return;
			MainControl* mc = MainControl::getMainControl(parentWidget());
			if (mc == 0) return;
			
			mc->clearSelection();
		}


		void SelectorDialog::showHelp()
		{
			QMessageBox::information( this, "molview",
					String(
					String("Possible predicates: \n")+
					"true() \t this is always true\n" +
					"false() \t this is always false\n" +
					"selected() \t this is true for already selected atoms\n" +
					"name(string) \t the name of the atom \n" +
					"type(string) \t the type name of the atom\n" +
					"element(char) \t the element (abbreviated by its symbol)\n" +
					"residue(string) \t the name of the residue containing the atom\n" +
					"residueID(int) \t the PDB ID of the residue (usally a number)\n" +
					"protein() \t the name of the protein the atom is contained in\n" +
					"secondaryStruct() \t the name of the secondary structure the atom is contained in\n" +
					"solvent() \t true if the atom is a solvent atom added by BALL\n" +
					"backbone() \t true for backbone atoms\n" +
					"chain() \n" +
					"nucleotide() \n" +
					"inRing() \n" +
					"doubleBonds() \n" +
					"tripleBonds() \n" +
					"aromaticBonds() \n" +
					"numberOfBonds(int) \n" +
					"connectedTo(char) \n" +
					"sp3Hybridized() \n" +
					"sp2Hybridized() \n" +
					"spHybridized() \n" +
					"charge() \n" +
					"isAxial() \n" +
					"is4C1() \n\n" +
					"They can be connected with\n" +
					"AND and OR, grouped with brackets, and each predicate can be negated with '!'\n"
					).c_str(),
					"&OK");
		}

	} // namespace VIEW

} // namespace BALL
