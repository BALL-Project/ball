// $Id: tutorial1.C,v 1.3 2000/03/24 11:33:58 anker Exp $
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
#include <BALL/KERNEL/PSE.h>

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
	// and set its element to oxygen (PSE[Element::O])
	Atom* oxygen = new Atom;
	oxygen->setElement(PSE[Element::O]);

	// now we create two hydrogen atoms...
	Atom* hydrogen1 = new Atom;
	Atom* hydrogen2 = new Atom;
	hydrogen1->setElement(PSE[Element::H]);
	hydrogen2->setElement(PSE[Element::H]);

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

	// bond_vector is a vector and is set to the
	// difference of atom positions of oxygen and hydrogen1
	Vector3 bond_vector = oxygen->getPosition() - hydrogen1->getPosition();

	// Vector3::getLength prints the length of the vector
	cout << "bond distance: " << bond_vector.getLength() << endl;

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
	HINFile outfile("water.hin", ios::out);
	outfile << *S;
	outfile.close();

	// We delete the system. This also deletes the molecules
	// and the atoms therein
	delete S;
}
