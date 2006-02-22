// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: MMFF94Processors.h,v 1.1.2.2 2006/02/22 23:09:38 amoll Exp $ 
//

#ifndef BALL_MOLMEC_MMFF94_PROCESSORS_H
#define BALL_MOLMEC_MMFF94_PROCESSORS_H

#ifndef BALL_MOLMEC_MMFF94_MMFF94PARAMETERS_H
# include <BALL/MOLMEC/MMFF94/MMFF94Parameters.h>
#endif

#ifndef BALL_DATATYPE_HASHSET_H
# include <BALL/DATATYPE/hashSet.h>
#endif

#include <vector>

namespace BALL 
{
	using std::vector;

	class MMFF94ESParameters;

	/**	Assign MMFF94 Charges
      \ingroup  MMFF94
	*/
	class BALL_EXPORT MMFF94ChargeProcessor
		:	public UnaryProcessor<Atom>
	{
		public:

		BALL_CREATE(MMFF94ChargeProcessor)

		///
		MMFF94ChargeProcessor();

		///
    MMFF94ChargeProcessor(const MMFF94ChargeProcessor& cp);
 
		///
		virtual ~MMFF94ChargeProcessor() {};

		///
		const MMFF94ChargeProcessor& operator = (const MMFF94ChargeProcessor& cp)
			throw();

		///
		virtual void clear()
			throw();
		
		///
		virtual bool start();

		///
		virtual Processor::Result operator () (Atom& atom);

		///
		void assignFormalCharge(Atom& atom);
		
		///
		const HashSet<Atom*>& getUnassigedAtoms() { return unassigned_atoms_;}

		///
		void setESParameters(const MMFF94ESParameters& es) { es_parameters_ = &es; }

		protected:

		HashSet<Atom*> 							unassigned_atoms_;
		const MMFF94ESParameters* 	es_parameters_;
	};

} // namespace BALL

#endif // BALL_MOLMEC_MMFF94_PROCESSORS_H
