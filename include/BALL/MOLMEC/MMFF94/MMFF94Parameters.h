// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: MMFF94Parameters.h,v 1.1.2.4 2005/03/24 16:17:38 amoll Exp $ 
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

#ifndef BALL_KERNEL_BOND_H
# include <BALL/KERNEL/bond.h>
#endif
using namespace std;

namespace BALL 
{
	///
	struct MMFF94AtomTypeData
	{
		///
		MMFF94AtomTypeData();

		/// atomic number;
		Position aspec;

		/// necessary number of bonded neighbours
		Position crd;

		/// number of bonds made to this atom type
		Position val;

		/// has pi lone pair electrons
		bool pilp;

		/** double(2) or triple(3) bonds are expected
				1 cases with intermediate hypridization 
		*/
		Position mltb;

		/// is aromatic
		bool arom;

		/// has linear bond
		bool lin;

		/// can have a delocalized single bond
		bool sbmb;
	};


	/**	MMFF94 parameters for bond stretching
      \ingroup  MMFF94
	*/
	class MMFF94AtomTypesContainer
	{
		public:

		///
		MMFF94AtomTypesContainer();

		///
		MMFF94AtomTypesContainer(const MMFF94AtomTypesContainer& to_copy);

		///
		~MMFF94AtomTypesContainer();

		///
		bool readDataSet(const String& filename);
		
		///
		const vector<MMFF94AtomTypeData>& getAtomTypes() const { return data_;}

		///
		bool isInitialized() { return is_initialized_;}

		protected:

		vector<MMFF94AtomTypeData> data_;

		bool is_initialized_;
	};


	/**	MMFF94 parameters for bond stretching
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
		void getOptionalSBMBParameters(const Bond& bond, float& kb, float& r0) const;

		///
		bool readParameters(const String& filename)
			throw(Exception::FileNotFound);
		
		bool hasOptionalSBMBParameter(Position atom_type1, Position atom_type2)
		{
			return parameters_optional_sbmb_.has(getIndex_(atom_type1, atom_type2));
		}

		//@}

		protected:

		Position getIndex_(Position atom_type1, Position atom_type2) const;

		/// standard parameters 
		StretchMap parameters_;
		
		/// parameters for optinal single-bond--multiple bond cases (see MMFFPROP.PAR)
		StretchMap parameters_optional_sbmb_;

		bool is_initialized_;

		Size nr_of_atom_types_;
	};


	/**	MMFF94 parameters for bond bending
      \ingroup  MMFF94
	*/
	class MMFF94BendParameters
	{
		public:

		/// Map with the bond type, force constant and reference angle
		typedef HashMap<Position, vector<float> > BendMap;

		/**	@name Constant Definitions
		*/
		//@{

		BALL_CREATE(MMFF94BendParameters)

		/**	Default constructor.
		*/
		MMFF94BendParameters();

		/**	Destructor.
		*/
		virtual ~MMFF94BendParameters();

		//@}
		/**	@name Assignment
		*/
		//@{

		/**	Assignment operator
		*/
		const MMFF94BendParameters& operator = (const MMFF94BendParameters& param)
			throw();

		/**	Clear method
		*/
		virtual void clear()
			throw();

		///
		bool isInitialized() { return is_initialized_;}

		///
		bool getParameters(Position atom_type1, Position atom_type2, Position atom_type3, float& ka, float& angle) const;

		///
		bool readParameters(const String& filename)
			throw(Exception::FileNotFound);
		
		//@}

		protected:

		Position getIndex_(Position atom_type1, Position atom_type2, Position atom_type3) const;

		/// parameters 
		BendMap parameters_;

		bool is_initialized_;

		Size nr_of_atom_types_;
	};

} // namespace BALL

#endif // BALL_MOLMEC_MMFF94_H
