// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: addHydrogenProcessor.h,v 1.1.2.2 2006/10/24 12:47:34 amoll Exp $
//

#ifndef BALL_STRUCTURE_ADDHYDROGENPROCESSOR_H
#define BALL_STRUCTURE_ADDHYDROGENPROCESSOR_H

#ifndef BALL_CONCEPT_PROCESSOR_H
#include <BALL/CONCEPT/processor.h>
#endif

#ifndef BALL_MATHS_VECTOR3_H
#include <BALL/MATHS/vector3.h> 
#endif

#ifndef BALL_KERNEL_ATOM_H
#include <BALL/KERNEL/atom.h>
#endif

#include <vector>

namespace BALL
{

	///
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
		
    ///
    virtual Processor::Result operator() (Composite &composite);

		///
		Size getConnectivity(Atom& atom);
		Size countBondOrders(Atom& atom);
		void addHydrogen(Atom& atom, Vector3 position);
		
		protected:

		bool hasMultipleBond(Atom& atom);
		inline bool isRingAtom_(Atom& atom);
		vector<Atom*> getPartners_(Atom& atom);
		void addHydrogen_(Atom& atom, Vector3 position);
		Vector3 getNormal_(const Vector3& v);
		inline bool normalize_(Vector3& v);
		bool hasMultipleBond_(Atom& atom);

		HashSet<Atom*> ring_atoms_;

  }; //class AddHydrogenProcessor

} //namesspace BALL

#endif // BALL_STRUCTURE_ADDHYDROGENPROCESSOR_H
