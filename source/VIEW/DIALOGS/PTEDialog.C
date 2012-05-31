// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
#include <BALL/VIEW/DIALOGS/PTEDialog.h>
#include <BALL/KERNEL/PTE.h>
#include <BALL/VIEW/WIDGETS/scene.h>

#include <QtGui/QToolTip>
#include <QtGui/QPushButton>

namespace BALL
{
	namespace VIEW
	{

		PTEDialog::PTEDialog(QWidget* parent)
			: QDialog(parent),
				Ui_PTEDialogData()
		{
			setupUi(this);

			connectChilds_(this);

			vector<String> elements;
			Element element;

			for (Position nr = 1; ; nr++) 
			{
				element = PTE.getElement(nr);
				if (element == Element::UNKNOWN) break;

				elements.push_back(element.getSymbol());
			}

			sort(elements.begin(), elements.end());
			
			for (Position nr = 0; nr < elements.size(); nr++)
			{
				element_box->addItem(elements[nr].c_str());
			}

			connect(element_box, SIGNAL(activated(int)), this, SLOT(elementChoosen_()));

			Scene* scene = Scene::getInstance(0);
			if (scene == 0)
			{
				Log.error() << "Expected a Scene, but found none!" << std::endl;
				return;
			}	

			int an = scene->getEditElementType();

			String symb = PTE[an].getSymbol();
			element_box->setCurrentIndex(element_box->findText(symb.c_str()));
		}


		PTEDialog::~PTEDialog()
		{
		}


		void PTEDialog::newElementType(int elementNumber)
		{
			Scene* scene = Scene::getInstance(0);

			if (scene == 0)
			{
				Log.error() << "Expected an Scene, but found none!" << std::endl;
			}
			else
			{
				scene->setEditElementType(elementNumber);
			}
		}

		QString PTEDialog::atomProperties_(int number)
		{
			Element e = PTE[number];
			String result("Element: ");
			result += e.getName();
			result += "\nSymbol: ";
			result += e.getSymbol();
			result += "\nAtomic Weight: ";
			result += String(e.getAtomicWeight()).c_str();
			result += "\nAtomic Radius: ";
			result += String(e.getAtomicRadius()).c_str();
			result += "\nCovalent Radius: ";
			result += String(e.getCovalentRadius()).c_str();
			result += "\nVan der Waals Radius: ";
			result += String(e.getVanDerWaalsRadius()).c_str();
			result += "\nElectronegativity: ";
			result += String(e.getElectronegativity()).c_str();
			result += "\n"; 

			QString r(result.c_str());

			return r;
		}

		void PTEDialog::elementClicked_()
		{
			QObject* w = sender();
			if (w == 0) return;
			QPushButton* bt = dynamic_cast<QPushButton*>(w);
			if (bt == 0) return;

			String element = ascii(bt->text());

			Element e = PTE[element];
			newElementType(e.getAtomicNumber());
			accept();
		}

		void PTEDialog::elementChoosen_()
		{
			int an = PTE[(ascii(element_box->currentText()))].getAtomicNumber();
			newElementType(an);
			accept();
		}


		void PTEDialog::connectChilds_(QObject* o)
		{
			QPushButton* w = dynamic_cast<QPushButton*>(o);
			if (w != 0) 
			{
				String s = ascii(w->text());
				if (s.size() > 2) return;
				Position an = PTE[s].getAtomicNumber();
				w->setToolTip(atomProperties_(an));
				connect(w, SIGNAL(released()), this, SLOT(elementClicked_()));
				return;
			}

			// iterate over all buttons in the button group
			QObjectList ol = o->children();
			QObjectList::iterator it = ol.begin();
			for (; it != ol.end(); it++)
			{
				connectChilds_(*it);
			}
		}

	} // VIEW and BALL namespace:
}
