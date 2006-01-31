// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: MMFF94Parameters.h,v 1.1.2.12 2006/01/31 01:13:32 amoll Exp $ 
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

//   #ifndef BALL_KERNEL_STANDARDPREDICATES_H
# include <BALL/KERNEL/standardPredicates.h>
//   #endif

using namespace std;

namespace BALL 
{

	Position getMMFF94Index(Position atom_type1, Position atom_type2);

	/// hold the maximum number of MMFF94 atom types + 1 (wildcard)
	extern Size MMFF94_number_atom_types;

	/**	MMFF94 equivalences for atom types (see MMFFDEF.PAR)
      \ingroup  MMFF94
	*/
	class MMFF94AtomTypeEquivalences
	{
		public: 

		///
		MMFF94AtomTypeEquivalences();

		///
		MMFF94AtomTypeEquivalences(const MMFF94AtomTypeEquivalences& to_copy);

		///
		~MMFF94AtomTypeEquivalences();
		
		///
		bool readParameters(const String& filename);

		/** Get an equivalence atom type for the given atom type.
		 		@param number between 1 and 4, 4 is the most general equivalence
				@return Index -1 if no equivalence found
		*/
		Index getEquivalence(Position original, Position number) const;

		///
		bool isInitialized() { return is_initialized_;}

		protected:

		vector<vector<Position> > equivalences_;
		vector<String> 	 names_;
		vector<bool> 	 	 exists_;

		bool is_initialized_;
	};
	

	/** Class to store the values of an atom type from MMFFPROP.PAR
	*/
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


	/**	MMFF94 parameters for atom types (see MMFFPROP.PAR)
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
		bool readParameters(const String& filename);
		
		///
		const vector<MMFF94AtomTypeData>& getAtomTypes() const { return data_;}

		///
		bool isInitialized() { return is_initialized_;}

		protected:

		vector<MMFF94AtomTypeData> data_;

		bool is_initialized_;
	};


	/////////////////////////////////////////////////////////////////////////////
	
	/**	MMFF94 parameters for bond stretching (see MMFFBOND.PAR)
      \ingroup  MMFF94
	*/
	class MMFF94StretchParameters
	{
		public:

		///
		struct BondData
		{
			BondData();

			float kb_normal;
			float r0_normal;
			bool  standard_bond_exists;
			
			/// parameters for optinal single-bond--multiple bond cases (see MMFFPROP.PAR)
			float kb_sbmb;
			float r0_sbmb;
			bool  sbmb_exists;
			bool  emperical;
		};

		///
		struct EmpericalBondData
		{
			float kb;
			float r0;
		};
	
		///
		typedef HashMap<Position, BondData> StretchMap;
		typedef HashMap<Position, EmpericalBondData> EmpericalStretchMap;

		BALL_CREATE(MMFF94StretchParameters)

		///	Default constructor.
		MMFF94StretchParameters();

		///	Destructor.
		virtual ~MMFF94StretchParameters();

		///	Assignment operator
		const MMFF94StretchParameters& operator = (const MMFF94StretchParameters& param)
			throw();

		///	Clear method
		virtual void clear()
			throw();

		///
		bool isInitialized() { return is_initialized_;}

		///
		StretchMap::ConstIterator getParameters(const Bond& bond) const;

		///
		StretchMap::ConstIterator getEmpericalParameters(const Bond& bond);

		///
		bool readParameters(const String& filename)
			throw(Exception::FileNotFound);
		
		///
		bool readEmpericalParameters(const String& filename);

		///
		const StretchMap& getBondParameters() { return parameters_;}

		/// Calculate the radius value per Schomaker-Stevenson Rule
		float calculateR0(const Bond& bond);

		///
		float calculateStretchConstant(const Bond& bond, float r0);

		protected:

		/// standard parameters 
		StretchMap parameters_;
		EmpericalStretchMap emperical_parameters_;
		
		bool is_initialized_;

		static float radii_[];
		static float electronegatives_[];

		SpHybridizedPredicate  sp1_;
		Sp2HybridizedPredicate sp2_;
		Sp2HybridizedPredicate sp3_;
	};

///////////////////////////////////////////////////////////////////////////

	/**	MMFF94 parameters for bond bending (see MMFFANG.PAR)
      \ingroup  MMFF94
	*/
	class MMFF94BendParameters
	{
		public:

		/// Map with the force constant and reference angle
		typedef HashMap<Position, pair<float, float> > BendMap;

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
		bool getParameters(Position bend_type,
											 Position atom_type1, 
											 Position atom_type2, 
											 Position atom_type3, float& ka, float& angle) const;

		///
		bool readParameters(const String& filename)
			throw(Exception::FileNotFound);
		
		//@}

		protected:

		Position getIndex_(Position bend_type,
											 Position atom_type1, 
											 Position atom_type2, 
											 Position atom_type3) const;

		/// parameters 
		BendMap parameters_;

		bool is_initialized_;
	};


////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////

	
	/**	MMFF94 parameters for stretch-bend interactions (see MMFFSTBN.PAR)
      \ingroup  MMFF94
	*/
	class MMFF94StretchBendParameters
	{
		public:

		/// Map with the force constant and reference angle
		typedef HashMap<Position, pair<float, float> > StretchBendMap;

		/**	@name Constant Definitions
		*/
		//@{

		BALL_CREATE(MMFF94StretchBendParameters)

		/**	Default constructor.
		*/
		MMFF94StretchBendParameters();

		/**	Destructor.
		*/
		virtual ~MMFF94StretchBendParameters();

		//@}
		/**	@name Assignment
		*/
		//@{

		/**	Assignment operator
		*/
		const MMFF94StretchBendParameters& operator = (const MMFF94StretchBendParameters& param)
			throw();

		/**	Clear method
		*/
		virtual void clear()
			throw();

		///
		bool isInitialized() { return is_initialized_;}

		///
		bool getParameters(Position stretch_bend_type,
											 const Atom& atom1, 
											 const Atom& atom2, 
											 const Atom& atom3, 
											 float& kba_ijk, float& kba_kji) const;

		/// read parameters for stretch-bends and for assignment by periodic table row
		bool readParameters(const String& filename, const String& by_row_filename)
			throw(Exception::FileNotFound);
		
		//@}

		protected:

		Position getIndex_(Position stretch_bend_type,
											 Position atom_type1, 
											 Position atom_type2, 
											 Position atom_type3) const;

		Position getIndexByRow_(Position r1, Position r2, Position r3) const;

		/// parameters 
		StretchBendMap parameters_;
		StretchBendMap parameters_by_row_;

		bool is_initialized_;
	};


} // namespace BALL

#endif // BALL_MOLMEC_MMFF94_MMFF94PARAMETERS_H
