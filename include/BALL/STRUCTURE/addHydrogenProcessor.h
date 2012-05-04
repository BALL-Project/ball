// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//

#ifndef BALL_STRUCTURE_ADDHYDROGENPROCESSOR_H
#define BALL_STRUCTURE_ADDHYDROGENPROCESSOR_H

#ifndef BALL_CONCEPT_PROCESSOR_H
#include <BALL/CONCEPT/processor.h>
#endif

#ifndef BALL_MATHS_VECTOR3_H
#include <BALL/MATHS/vector3.h> 
#endif

#include <vector>

namespace BALL
{
	class Residue;
	class Atom;
	class Composite;

	/** Saturate atoms with hydrogen atoms.
	 		Only works with main group elements.
			The formal charge of the atoms are taken into account.
			The placement of the hydrogen atoms only depends on
			the direct neighbour atoms.
			No additional optimization of the atom placement is done.
			The optimal bond lengths are calculated by
			a modified Schomaker-Stevenson rule 
			(adapted from the MMFF94 force field).
			<br>
			Usage:<br>
			\code
			RingPerceptionProcessor rpp;
			vector<vector<Atom*> > rings;
			rpp.calculateSSSR(rings, system);
			rings = rpp.getAllSmallRings();
			AddHydrogenProcessor ap;
			ap.setRings(rings);
			system.apply(ap);
			\endcode
			\ingroup StructureMiscellaneous
	*/
  class BALL_EXPORT AddHydrogenProcessor
		:	public UnaryProcessor<Composite>
  {
 
		public:

    ///
    AddHydrogenProcessor();
		
    ///
		virtual ~AddHydrogenProcessor();
		
    ///
		AddHydrogenProcessor(const AddHydrogenProcessor& hbp);

		virtual bool start();
		
    ///
    virtual Processor::Result operator() (Composite &composite);

		///
		Size getConnectivity(Atom& atom);

		///
		void setRings(const vector<vector<Atom*> >& rings);

		///
		Size getNumberOfAddedHydrogens() { return nr_hydrogens_;}
		
		protected:
			Processor::Result placePeptideBondH_(Residue* res) const;

		Size countBondOrders(Atom& atom);
		void addHydrogen_(Atom& atom, Vector3 position);
		inline bool isRingAtom_(Atom& atom);
		vector<Atom*> getPartners_(Atom& atom);
		Vector3 getNormal_(const Vector3& v);
		inline bool normalize_(Vector3& v);
		bool hasMultipleBond_(Atom& atom);
		// get the ideal bond length for the given element with hydrogen atoms
		float getBondLength_(Position element);

		HashSet<Atom*> ring_atoms_;
		Position atom_nr_;
		Atom*    last_atom_;
		Size 		 nr_hydrogens_;
  }; //class AddHydrogenProcessor

} //namesspace BALL

#endif // BALL_STRUCTURE_ADDHYDROGENPROCESSOR_H
