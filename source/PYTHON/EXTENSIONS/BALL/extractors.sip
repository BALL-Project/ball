// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id$
//
// Author:
//   Oliver Kohlbacher
//

%ModuleCode
	#include <list>
	#include <BALL/KERNEL/extractors.h>
	using namespace std;
	using namespace BALL;
%End

list<Atom*>                 atoms(const AtomContainer&, const String& expression = String());
//AtomList                  all_atoms(const Composite&);
list<PDBAtom*>              PDBAtoms(const AtomContainer&, const String& expression = String());
list<Bond*>                 bonds(const AtomContainer&, bool selected_only = false);
list<Bond*>                 bonds(const Atom&);
list<AtomContainer*>        atomContainers(const AtomContainer&, bool selected_only = false);
list<Fragment*>             fragments(const AtomContainer&, bool selected_only = false);
list<Molecule*>             molecules(const AtomContainer&, bool selected_only = false);
list<Residue*>              residues(const AtomContainer&, bool selected_only = false);
list<SecondaryStructure*>   secondaryStructures(const AtomContainer&, bool selected_only = false);
list<Chain*>                chains(const AtomContainer&, bool selected_only = false);
list<Protein*>              proteins(const AtomContainer&, bool selected_only = false);
list<Nucleotide*>           nucleotides(const AtomContainer&, bool selected_only = false);
list<NucleicAcid*>          nucleicAcids(const AtomContainer&, bool selected_only = false);

// this is the most useful class in the whole of BALL: it is here to get around some %$&%$/ sip bugs!
class BALLCoreSipExtractors
{
%TypeHeaderCode
	#include <BALL/KERNEL/extractors.h>

	using namespace BALL;
	namespace BALL {
		class BALLCoreSipExtractors
		{
			public:
		};
	}
%End

	public:
		list<Atom*>  __atoms();
%MethodCode
	// do nothing
%End

		list<PDBAtom*>  __PDBAtoms();
%MethodCode
	// do nothing
%End

		list<Bond*> __bonds();
%MethodCode
	// do nothing
%End

		list<AtomContainer*> __atomContainers();
%MethodCode
	// do nothing
%End

		list<Fragment*> __fragments();
%MethodCode
	// do nothing
%End

		list<Molecule*> __molecules();
%MethodCode
	// do nothing
%End

		list<Residue*> __residues();
%MethodCode
	// do nothing
%End

		list<SecondaryStructure*> __secondaryStructures();
%MethodCode
	// do nothing
%End

		list<Chain*> __chains();
%MethodCode
	// do nothing
%End

		list<Protein*> __proteins();
%MethodCode
	// do nothing
%End

		list<Nucleotide*> __nucleotides();
%MethodCode
	// do nothing
%End

		list<NucleicAcid*> __nucleicAcids();
%MethodCode
	// do nothing
%End
};
