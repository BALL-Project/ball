// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: MMFF94Parameters.h,v 1.1.8.1 2007/03/25 21:25:17 oliver Exp $ 
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

namespace BALL 
{

	class Atom;
	class Parameters;

	Position getMMFF94Index(Position atom_type1, Position atom_type2);

	/// hold the maximum number of MMFF94 atom types + 1 (wildcard)
	extern Size MMFF94_number_atom_types;
	
	class MMFF94AtomTypeEquivalences;

#define MMFF94_INVALID_VALUE 99.0

/////////////////////////////////////////////////////////////////////////////
	/**	MMFF94 component parameters base class
      \ingroup  MMFF94
	*/
	class BALL_EXPORT MMFF94ParametersBase
	{
		public:

		BALL_CREATE(MMFF94ParametersBase)

		///	Default constructor.
		MMFF94ParametersBase();

		///	Destructor.
		virtual ~MMFF94ParametersBase() {};

		///	Assignment operator
		const MMFF94ParametersBase& operator = (const MMFF94ParametersBase&)
			 {return *this;};

		///	Clear method
		virtual void clear()
			;

		///
		bool isInitialized() { return is_initialized_;}

		///
		bool readParameters(Parameters& p, const String& section)
			throw(Exception::FileNotFound);

		///
		void setEquivalences(const MMFF94AtomTypeEquivalences& equi) { equiv_ = &equi;}
		
		protected:

		virtual bool setup_(const std::vector<std::vector<String> >&) { return true;};

		bool is_initialized_;

		// nr of needed fields in parameter files
		Size number_expected_fields_;
		const MMFF94AtomTypeEquivalences* equiv_;
	};


	/**	MMFF94 equivalences for atom types (see MMFFDEF.PAR)
      \ingroup  MMFF94
	*/
	class BALL_EXPORT MMFF94AtomTypeEquivalences:
		public MMFF94ParametersBase
	{
		public: 

		///
		MMFF94AtomTypeEquivalences();

		///
		MMFF94AtomTypeEquivalences(const MMFF94AtomTypeEquivalences& to_copy);

		///
		~MMFF94AtomTypeEquivalences();
		
		///
		void clear()
			;

		/** Get an equivalence atom type for the given atom type.
		 		@param number between 1 and 4, 4 is the most general equivalence
				@return Index -1 if no equivalence found
		*/
		Index getEquivalence(Position original, Position number) const;

		protected:

		virtual bool setup_(const std::vector<std::vector<String> >&);
		std::vector<std::vector<Position> > equivalences_;
		std::vector<String> 	 names_;
		std::vector<bool> 	 	 exists_;
	};
	

	/// Class to store the values of an atom type from MMFFPROP.PAR
	struct BALL_EXPORT MMFF94AtomType
	{
		///
		MMFF94AtomType();

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

		/// Is this a valid MMFF94 atom type?
		bool valid;
	};


///////////////////////////////////////////////////////////////////////////
	/**	MMFF94 parameters for atom types (see MMFFPROP.PAR)
      \ingroup  MMFF94
	*/
	class BALL_EXPORT MMFF94AtomTypes:
		public MMFF94ParametersBase
	{
		public:

		///
		MMFF94AtomTypes();

		///
		MMFF94AtomTypes(const MMFF94AtomTypes& to_copy);

		///
		~MMFF94AtomTypes() {};

		///
		const std::vector<MMFF94AtomType>& getAtomTypes() const { return data_;}

		protected:

		virtual bool setup_(const std::vector<std::vector<String> >&);
		std::vector<MMFF94AtomType> data_;
	};

///////////////////////////////////////////////////////////////////////////
	
	/**	MMFF94 parameters for bond stretching (see MMFFBOND.PAR)
      \ingroup  MMFF94
	*/
	class BALL_EXPORT MMFF94StretchParameters:
		public MMFF94ParametersBase
	{
		public:

		///
		struct BondData
		{
			BondData();

			double kb_normal;
			double r0_normal;
			bool  standard_bond_exists;
			
			/// parameters for optinal single-bond--multiple bond cases (see MMFFPROP.PAR)
			double kb_sbmb;
			double r0_sbmb;
			bool  sbmb_exists;
			bool  empirical;
		};

		///
		struct EmpiricalBondData
		{
			double kb;
			double r0;
		};
	
		///
		typedef HashMap<Position, BondData> StretchMap;
		typedef HashMap<Position, EmpiricalBondData> EmpiricalStretchMap;

		BALL_CREATE(MMFF94StretchParameters)

		///	Default constructor.
		MMFF94StretchParameters();

		///	Destructor.
		virtual ~MMFF94StretchParameters();

		///	Assignment operator
		const MMFF94StretchParameters& operator = (const MMFF94StretchParameters& param)
			;

		///	Clear method
		virtual void clear()
			;

		///
		bool assignParameters(Position type1, Position type2, BondData& data) const;

		///
		bool readEmpiricalParameters(Parameters& p, const String& section);

		///
		const StretchMap& getBondParameters() const { return parameters_;}
		
		///
		const EmpiricalStretchMap& getEmpiricalParameters() const { return empirical_parameters_;}

		static double radii[];
		static double electronegatives[];

		protected:

		virtual bool setup_(const std::vector<std::vector<String> >&);

		/// standard parameters 
		StretchMap parameters_;
		mutable StretchMap buffered_parameters_;
		EmpiricalStretchMap empirical_parameters_;
	};

///////////////////////////////////////////////////////////////////////////

	/**	MMFF94 parameters for bond bending (see MMFFANG.PAR)
      \ingroup  MMFF94
	*/
	class BALL_EXPORT MMFF94BendParameters
		: public MMFF94ParametersBase
	{
		public:

		/// Map with the force constant and reference angle
		typedef HashMap<Position, std::pair<double, double> > BendMap;

		BALL_CREATE(MMFF94BendParameters)

		///	Default constructor.
		MMFF94BendParameters();

		///	Destructor.
		virtual ~MMFF94BendParameters();

		///	Assignment operator
		const MMFF94BendParameters& operator = (const MMFF94BendParameters& param)
			;

		///	Clear method
		virtual void clear()
			;

		///
		bool assignParameters(Position bend_type,
											 Position atom_type1, Position atom_type2, Position atom_type3, 
											 double& ka, double& angle) const;

		protected:

		virtual bool setup_(const std::vector<std::vector<String> >&);

		Position getIndex_(Position bend_type, Position atom_type1, Position atom_type2, Position atom_type3) const;

		/// parameters 
		BendMap parameters_;
		mutable BendMap buffered_parameters_;
	};


////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////

	/**	MMFF94 parameters for stretch-bend interactions (see MMFFSTBN.PAR)
      \ingroup  MMFF94
	*/
	class BALL_EXPORT MMFF94StretchBendParameters
		: public MMFF94ParametersBase
	{
		public:

		/// Map with the force constant and reference angle
		typedef HashMap<Position, std::pair<double, double> > StretchBendMap;

		BALL_CREATE(MMFF94StretchBendParameters)

		///	Default constructor.
		MMFF94StretchBendParameters();

		///	Destructor.
		virtual ~MMFF94StretchBendParameters();

		///	Assignment operator
		const MMFF94StretchBendParameters& operator = (const MMFF94StretchBendParameters& param)
			;

		///	Clear method
		virtual void clear()
			;

		///
		bool assignParameters(Position stretch_bend_type, const Atom& atom1, const Atom& atom2, const Atom& atom3, 
											 double& kba_ijk, double& kba_kji) const;

		/// read parameters for stretch-bends and for assignment by periodic table row
		bool readEmpiricalParameters(Parameters& p, const String& section)
			throw(Exception::FileNotFound);
		
		//@}

		protected:

		virtual bool setup_(const std::vector<std::vector<String> >&);

		Position getIndex_(Position stretch_bend_type,
											 Position atom_type1, Position atom_type2, Position atom_type3) const; 

		Position getIndexByRow_(Position r1, Position r2, Position r3) const;

		/// parameters 
		StretchBendMap parameters_;
		mutable StretchBendMap buffered_parameters_;
		StretchBendMap parameters_by_row_;
	};

////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////
	
	/**	MMFF94 parameters for torsions (see MMFFTOR.PAR)
      \ingroup  MMFF94
	*/
	class BALL_EXPORT MMFF94TorsionParameters
		: public MMFF94ParametersBase
	{
		public:

		/// Map with the force constant and reference angle
		typedef HashMap<String, std::vector<double> > TorsionsMap;

		BALL_CREATE(MMFF94TorsionParameters)

		///	Default constructor.
		MMFF94TorsionParameters();

		///	Destructor.
		virtual ~MMFF94TorsionParameters();

		///	Assignment operator
		const MMFF94TorsionParameters& operator = (const MMFF94TorsionParameters& param)
			;

		///	Clear method
		virtual void clear()
			;

		///
		bool assignParameters(Position type_index,
											 Index at1, Index at2, Index at3, Index at4,
											 double& v1, double& v2, double& v3) const;

		protected:

		virtual bool setup_(const std::vector<std::vector<String> >&);

		String getIndex_(Position type, Position atom_type1, Position atom_type2, Position atom_type3, Position atom_type4) const;

		/// parameters 
		TorsionsMap parameters_;
		mutable TorsionsMap buffered_parameters_;
		static Position ic_[5];
		static Position lc_[5];
	};

////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////

	/**	MMFF94 parameters for out of plane bending (see MMFFOOR.PAR)
      \ingroup  MMFF94
	*/
	class BALL_EXPORT MMFF94PlaneParameters
		: public MMFF94ParametersBase
	{
		public:

		/// Map with the force constant 
		typedef HashMap<String, double> PlaneMap;

		BALL_CREATE(MMFF94PlaneParameters)

		///	Default constructor.
		MMFF94PlaneParameters();

		///	Destructor.
		virtual ~MMFF94PlaneParameters();

		/// Assignment operator
		const MMFF94PlaneParameters& operator = (const MMFF94PlaneParameters& param)
			;

		///	Clear method
		virtual void clear()
			;

		///
		bool assignParameters(Index at1, Index at2, Index at3, Index at4, double& v) const;

		protected:

		virtual bool setup_(const std::vector<std::vector<String> >&);

		String getIndex_(Position atom_type1, Position atom_type2, Position atom_type3, Position atom_type4) const;

		/// parameters 
		PlaneMap parameters_;
		mutable PlaneMap buffered_parameters_;
	};

////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////

	/**	MMFF94 parameters for VDW (see MMFFVDW.PAR)
      \ingroup  MMFF94
	*/
	class BALL_EXPORT MMFF94VDWParameters
		: public MMFF94ParametersBase
	{
		public:

		struct VDWEntry
		{
			double alpha_i;
			double ni;
			double ai;
			double gi;
			short  donor_acceptor;
			bool   valid;
		};

		BALL_CREATE(MMFF94VDWParameters)

		///	Default constructor.
		MMFF94VDWParameters();

		///	Destructor.
		virtual ~MMFF94VDWParameters();

		/// Assignment operator
		const MMFF94VDWParameters& operator = (const MMFF94VDWParameters& param)
			;

		///	Clear method
		virtual void clear()
			;

		///
		double getR(Position atom_type) const;

		/// Retrieve vdW parameters for a single atom type
		const VDWEntry& getParameters(Index at) const;

		///
		bool assignParameters(Position at1, Position at2, double& rij, double& rij_7_, double& eij) const;

		protected:

		virtual bool setup_(const std::vector<std::vector<String> >&);

		/// parameters 
		std::vector<VDWEntry> parameters_;

		// R star ii for all individual atom types
		mutable std::vector<double> rs_;

		// R star ij for pairs of atom types
		mutable std::vector<double> rij_;
		// R star ij ^ 7 for pairs of atom types
		mutable std::vector<double> rij_7_;
		// R star ij and for pairs of atom types
		mutable std::vector<double> eij_;
		// bools if indivual value was already calculated
		mutable std::vector<bool> 	 calculated_;
	};


////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////

	/**	MMFF94 parameters for partial charges for electrostatics (see MMFFCHG.PAR MMFFPBCI.PAR)
      \ingroup  MMFF94
	*/
	class BALL_EXPORT MMFF94ESParameters
		: public MMFF94ParametersBase
	{
		public:

		BALL_CREATE(MMFF94ESParameters)

		///	Default constructor.
		MMFF94ESParameters();

		///	Destructor.
		virtual ~MMFF94ESParameters();

		/// Assignment operator
		const MMFF94ESParameters& operator = (const MMFF94ESParameters& param)
			;

		///	Clear method
		virtual void clear()
			;

		/** Get the partial charge increment for the two atom types and the bond type.
		 		The increment is for the atom with atom type at1.
				99 is return if no increment could be calculated.
		*/
		double getPartialCharge(Position at1, Position at2, Position bt) const;
		
		///
		bool readEmpiricalParameters(Parameters& p, const String& section)
			throw(Exception::FileNotFound);

		///
		double getPhi(Index atom_type) const;

		///
		double getPBCI(Index atom_type) const;

		protected:

		virtual bool setup_(const std::vector<std::vector<String> >&);
		Position getIndex_(Position at1, Position at2, Position bt) const;

		/// parameters 
		std::vector<double> parameters_, phis_, pbcis_;
	};


} // namespace BALL

#endif // BALL_MOLMEC_MMFF94_MMFF94PARAMETERS_H
