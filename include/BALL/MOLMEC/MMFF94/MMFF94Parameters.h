// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: MMFF94Parameters.h,v 1.1.2.2 2005/03/22 15:41:17 amoll Exp $ 
//

// Molecular Mechanics: MMFF94 force field class

#ifndef BALL_MOLMEC_MMFF94_MMFF94PARAMETERS_H
#define BALL_MOLMEC_MMFF94_MMFF94PARAMETERS_H

#ifndef BALL_COMMON_H
#	include <BALL/common.h>
#endif

#ifndef BALL_DATATYPE_HASHMAP_H
# include <BALL/DATATYPE/hashMap.h>
#endif 

#ifndef BALL_KERNEL_STANDARDPREDICATES_H
# include <BALL/KERNEL/standardPredicates.h>
#endif

using namespace std;

namespace BALL 
{
	/**	MMFF94 force field class.
      \ingroup  MMFF94
	*/
	class MMFF94BondStretchParameters
	{
		public:

		///
		typedef HashMap<Position, pair<float, float> > StretchMap;

		/**	@name Constant Definitions
		*/
		//@{

		BALL_CREATE(MMFF94BondStretchParameters)

		/**	Default constructor.
		*/
		MMFF94BondStretchParameters();

		/**	Destructor.
		*/
		virtual ~MMFF94BondStretchParameters();

		//@}
		/**	@name Assignment
		*/
		//@{

		/**	Assignment operator
		*/
		const MMFF94BondStretchParameters& operator = (const MMFF94BondStretchParameters& param)
			throw();

		/**	Clear method
		*/
		virtual void clear()
			throw();

		///
		bool isInitialized() { return is_initialized_;}

		///
		bool getParameters(const Bond& bond, float& kb, float& r0) const;

		///
		bool readParameters(const String& filename)
			throw(Exception::FileNotFound);
		
		//@}

		protected:

		Position getIndex_(const Position& atom_type1, const Position& atom_type2) const
		{ 
			return atom_type1 * nr_of_atom_types_ + atom_type2;
		}

		/// standard parameters 
		StretchMap parameters_;
		
		/// parameters for optinal single-bond--multiple bond cases (see MMFFPROP.PAR)
		StretchMap parameters_optional_sbmb_;

		bool is_initialized_;

		Size nr_of_atom_types_;

		Sp2HybridizedPredicate isSp2_;
		SpHybridizedPredicate  isSp_;
	};
} // namespace BALL

#endif // BALL_MOLMEC_MMFF94_H
