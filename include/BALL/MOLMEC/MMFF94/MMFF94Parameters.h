// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: MMFF94Parameters.h,v 1.1.2.1 2005/03/21 16:42:44 amoll Exp $ 
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
		typedef HashMap<Position, HashMap<Position, pair<float, float> > > StretchMap;

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
		bool getParameters(Position atom_type1, Position atom_type2, float& kb, float& r0) const;

		///
		bool readParameters(const String& filename)
			throw(Exception::FileNotFound);
		
		//@}

		protected:

		StretchMap parameters_;
		bool is_initialized_;
	};
} // namespace BALL

#endif // BALL_MOLMEC_MMFF94_H
