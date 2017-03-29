// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: tutorial1.C,v 1.7 2005/12/23 17:03:21 amoll Exp $
// tutorial example 1
// ------------------
// build two water molecules and write them to a file

// needed for cout
#include <iostream>

// the BALL kernel classes
#include <BALL/KERNEL/atom.h>
#include <BALL/KERNEL/bond.h>
#include <BALL/KERNEL/molecule.h>
#include <BALL/KERNEL/system.h>
#include <BALL/KERNEL/PTE.h>

// reading and writing of HyperChem files
#include <BALL/FORMAT/HINFile.h>

// the TranslationProcessor
#include <BALL/STRUCTURE/geometricTransformations.h>

// we use the BALL namespace  and the std namespace (for cout and endl)
using namespace BALL;
using namespace std;



int main()
{
	// we create a new atom caleld oxygen
	// and set its element to oxygen (PTE[Element::O])
	Atom* oxygen = new Atom;
	oxygen->setElement(PTE[Element::O]);

	// now we create two hydrogen atoms...
	Atom* hydrogen1 = new Atom;
	Atom* hydrogen2 = new Atom;
	hydrogen1->setElement(PTE[Element::H]);
	hydrogen2->setElement(PTE[Element::H]);

	// ...and move them to approximately correct positions
 	hydrogen1->setPosition(Vector3(-0.95, 0.00, 0.0));
 	hydrogen2->setPosition(Vector3( 0.25, 0.87, 0.0));

	// We create our water molecule...
	Molecule* water = new Molecule;

	// ...and insert the three atoms into the molecule.
	water->insert(*oxygen);
	water->insert(*hydrogen1);
	water->insert(*hydrogen2);

	// Then we build the two O-H bonds
	oxygen->createBond(*hydrogen1);
	oxygen->createBond(*hydrogen2);

	// Some statics: Molecule::countAtoms() writes the number of
	// atoms, Atom::countBonds() the number of bonds the atom shares
	cout << "# of atoms in water: " << water->countAtoms() << endl;
	cout << "# of bonds in O: " << oxygen->countBonds() << endl;
	cout << "# of atoms in H1: " << hydrogen1->countBonds() << endl;
	cout << "# of atoms in H2: " << hydrogen2->countBonds() << endl;

	// compute the bond length
  float distance = oxygen->getDistance(*hydrogen1);

  cout << "bond length: " << distance << endl;

	// Now we copy our molecule using a copy constructor.
	Molecule* water2 = new Molecule(*water);

	// A translation processor moves the second molecule
	// 5 Angstrom along the x axis
	TranslationProcessor translation(Vector3(5, 0, 0));
	water2->apply(translation);

	// We insert our two molecules into a system
	System* S = new System;
	S->insert(*water);
	S->insert(*water2);

	// and write this system to a HyperChem file
	HINFile outfile("water.hin", File::MODE_OUT);
	outfile << *S;
	outfile.close();

	// We delete the system. This also deletes the molecules
	// and the atoms therein
	delete S;
}
