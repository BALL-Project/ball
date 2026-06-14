// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: standardPredicates.h,v 1.52.16.2 2007/03/29 10:34:59 bertsch Exp $
//

#ifndef BALL_KERNEL_STANDARDPREDICATES_H
#define BALL_KERNEL_STANDARDPREDICATES_H

#ifndef BALL_DATATYPE_STRINGHASHMAP_H
#	include <BALL/DATATYPE/stringHashMap.h>
#endif

#ifndef BALL_DATATYPE_HASHSET_H
#	include <BALL/DATATYPE/hashSet.h>
#endif

#ifndef BALL_KERNEL_BOND_H
#	include <BALL/KERNEL/bond.h>
#endif

#ifndef BALL_KERNEL_ATOMHANDLE_H
#	include <BALL/KERNEL/atomHandle.h>     // v2.2 H4 7b.17 (D-H3.8): RingFinder keys on handles
#endif

#ifndef BALL_KERNEL_BONDHANDLE_H
#	include <BALL/KERNEL/bondHandle.h>
#endif

#ifndef BALL_KERNEL_EXPRESSION_H
#	include <BALL/KERNEL/expression.h>
#endif

// SMARTSPredicate needs SmartsMatcher (STRUCTURE) + AromaticityProcessor +
// RingPerceptionProcessor (QSAR). v2.2 HCP-1 task 0: re-gated under
// BALL_COLLAPSE_KERNEL_ONLY (Track-B Cluster-B-step-1 had lifted the gate; restored for
// the hierarchy-collapse KERNEL-only build -- the gate lifts again when
// STRUCTURE/QSAR re-open at HCP-3).
#ifndef BALL_COLLAPSE_KERNEL_ONLY
# ifndef BALL_STRUCTURE_SMARTSMATCHER_H
#  include <BALL/STRUCTURE/smartsMatcher.h>
# endif

# ifndef BALL_QSAR_AROMATICITYPROCESSOR_H
#  include <BALL/QSAR/aromaticityProcessor.h>
# endif

# ifndef BALL_QSAR_RINGPERCEPTIONPROCESSOR_H
#  include <BALL/QSAR/ringPerceptionProcessor.h>
# endif
#endif

namespace BALL 
{
	class Atom;

	/**	@name	Predefined Predicates. 
			<b>Note:</b> These predicates do not provide the OCI.
			@see ExpressionPredicate
			
    	\ingroup  Predicates
	*/
	//@{

	/** Default predicate.
			This predicate may be used to implement
			default rules. It is always <b>true</b>.
	 */
	class BALL_EXPORT TruePredicate
		:	public ExpressionPredicate
	{
		public:
			BALL_CREATE(TruePredicate)

			/** Evaluate the predicate for the atom <tt>atom</tt>
					@param atom the atom to test
					@return true - always
			*/
			virtual bool operator () (const Atom& atom) const;
	};

	/** False predicate.
			This predicate may be used to implement
			default rules. It is always <b>false</b>.
	 */
	class BALL_EXPORT FalsePredicate
		:	public ExpressionPredicate
	{
		public:
		BALL_CREATE(FalsePredicate)

		/** Evaluate the predicate for the atom <tt>atom</tt>
				@param atom the atom to test
				@return false - always
		*/
		virtual bool operator () (const Atom& atom) const;
	};

	/** Selection predicate.
			This predicate is <b>true</b>, if the atom is selected, <b>false</b>
			otherwise.
	 */
	class BALL_EXPORT SelectedPredicate
		:	public ExpressionPredicate
	{
		public:
			BALL_CREATE(SelectedPredicate)

			/** Evaluate the predicate for the atom <tt>atom</tt>.
					@param atom the atom to test
					@return true - always
			*/
			virtual bool operator () (const Atom& atom) const;
	};

	/** Predicate for matching atom names. Returns <tt>true</tt>, if the name of
			the atom matches the argument of this predicate. This is an exact
			match.
	 */
	class BALL_EXPORT AtomNamePredicate
		:	public	ExpressionPredicate
	{
		public:
			BALL_CREATE(AtomNamePredicate)

			/** Evaluate the predicate for the atom <tt>atom</tt>.
					@param atom the atom to test
					@return true, if the predicate is true, false otherwise
			*/
			virtual bool operator () (const Atom& atom) const;
	};

	/** Predicate for matching atom types. Returns <tt>true</tt>, if the type name
			of the atom matches exactly the argument of this predicate. 
	 */
	class BALL_EXPORT AtomTypePredicate
		:	public	ExpressionPredicate
	{
		public:
			BALL_CREATE(AtomTypePredicate)

			/** Evaluate the predicate for the atom <tt>atom</tt>.
					@param atom the atom to test
					@return true, if the predicate is true, false otherwise
			*/
			virtual bool operator () (const Atom& atom) const;
	};

	/** Predicate for matching elements. Returns <tt>true</tt>, if the element
			symbol of the atom matches exactly the argument  of this predicate.
	 */
	class BALL_EXPORT ElementPredicate
		:	public	ExpressionPredicate
	{
		public:
			BALL_CREATE(ElementPredicate)

			/** Evaluate the predicate for the atom <tt>atom</tt>.
					@param atom the atom to test
					@return true, if the predicate is true, false otherwise
			*/
			virtual bool operator () (const Atom& atom) const;
	};

	/** Predicate for matching residues. Returns <tt>true</tt>, if the name of the 
			residue that the atom belongs to matches exactly the argument of this
			predicate.
	 */
	class BALL_EXPORT ResiduePredicate
		:	public	ExpressionPredicate
	{
		public:
			BALL_CREATE(ResiduePredicate)

			/** Evaluate the predicate for the atom <tt>atom</tt>.
					@param atom the atom to test
					@return true, if the predicate is true, false otherwise
			*/
			virtual bool operator () (const Atom& atom) const;
	};

	/** Predicate for matching residues IDs. Returns <tt>true</tt>, if the ID of
			the residue that the atom belongs to matches exactly the argument of
			this predicate.
	 */
	class BALL_EXPORT ResidueIDPredicate
		:	public	ExpressionPredicate
	{
		public:
			BALL_CREATE(ResidueIDPredicate)

			/** Evaluate the predicate for the atom <tt>atom</tt>.
					@param atom the atom to test
					@return true, if the predicate is true, false otherwise
			*/
			virtual bool operator () (const Atom& atom) const;
	};

	/** Predicate for matching proteins. Returns <tt>true</tt>, if the name of the
			protein that the atom belongs to matches exactly the argument of this
			predicate.
	 */
	class BALL_EXPORT ProteinPredicate
		:	public	ExpressionPredicate
	{
		public:
			BALL_CREATE(ProteinPredicate)

			/** Evaluate the predicate for the atom <tt>atom</tt>.
					@param atom the atom to test
					@return true, if the predicate is true, false otherwise
			*/
			virtual bool operator () (const Atom& atom) const;
	};

	/** Predicate for matching chains. Returns <tt>true</tt>, if the name of the
			chain that the atom belongs to matches exactly the argument of this
			expression.
	 */
	class BALL_EXPORT ChainPredicate
		:	public	ExpressionPredicate
	{
		public:
			BALL_CREATE(ChainPredicate)

			/** Evaluate the predicate for the atom <tt>atom</tt>.
					@param atom the atom to test
					@return true, if the predicate is true, false otherwise
			*/
			virtual bool operator () (const Atom& atom) const;
	};

	/** Predicate for matching secondary structures. Returns <tt>true</tt>, if the
			name of the secondary structure that the atom belongs to matches
			exactly the argument of this predicate.
	 */
	class BALL_EXPORT SecondaryStructurePredicate
		:	public	ExpressionPredicate
	{
		public:
			BALL_CREATE(SecondaryStructurePredicate)

			/** Evaluate the predicate for the atom <tt>atom</tt>.
					@param atom the atom to test
					@return true, if the predicate is true, false otherwise
			*/
			virtual bool operator () (const Atom& atom) const;
	};

	/** Predicate indicating solvent atoms. 
			Returns <tt>true</tt>, if the atom
			belongs to a solvent molecule. This predicate is <b>true</b> if the 
			atom is contained in a molecule with the property  \link Molecule::IS_SOLVENT Molecule::IS_SOLVENT \endlink 
			set. This is usually the case for solvents added automatically by a
			force field.
			@see PeriodicBoundary::addSolvent
	 */
	class BALL_EXPORT SolventPredicate
		:	public	ExpressionPredicate
	{
		public:
		BALL_CREATE(SolventPredicate)

		/** Evaluate the predicate for the atom <tt>atom</tt>.
				@param atom the atom to test
				@return true, if the predicate is true, false otherwise
		*/
		virtual bool operator () (const Atom& atom) const;
	};

	/** Predicate for matching molecules. Returns <tt>true</tt>, if the atom
			belongs to the molecule defined by the expression of this predicate.
	 */
	class BALL_EXPORT MoleculePredicate
		:	public	ExpressionPredicate
	{
		public:
			BALL_CREATE(MoleculePredicate)

			/** Evaluate the predicate for the atom <tt>atom</tt>.
					@param atom the atom to test
					@return true, if the predicate is true, false otherwise
			*/
			virtual bool operator () (const Atom& atom) const;
	};

	/** Predicate indicating backbone atoms. Returns <tt>true</tt>, if the atom
			belongs to the backbone of a protein, i. e. is one of the members of
			the peptide bond.
	 */
	class BALL_EXPORT BackBonePredicate
		:	public	ExpressionPredicate
	{
		public:
			BALL_CREATE(BackBonePredicate)

			/** Evaluate the predicate for the atom <tt>atom</tt>.
					@param atom the atom to test
					@return true, if the predicate is true, false otherwise
			*/
			virtual bool operator () (const Atom& atom) const;
	};

	/**
	 */
	class BALL_EXPORT NucleicAcidPredicate
		:	public	ExpressionPredicate
	{
		public:
			BALL_CREATE(NucleicAcidPredicate)

			/** Evaluate the predicate for the atom <tt>atom</tt>.
					@param atom the atom to test
					@return true, if the predicate is true, false otherwise
			*/
			virtual bool operator () (const Atom& atom) const;
	};

	/** Predicate indicating nucleotide atoms. Returns <tt>true</tt>, if the
			atom belongs to a nucleotide.
	 */
	class BALL_EXPORT NucleotidePredicate
		:	public	ExpressionPredicate
	{
		public:
			BALL_CREATE(NucleotidePredicate)

			/** Evaluate the predicate for the atom <tt>atom</tt>.
					@param atom the atom to test
					@return true, if the predicate is true, false otherwise
			*/
			virtual bool operator () (const Atom& atom) const;
	};

	/** Predicate for atoms being included in rings of a certain number.
	 */
	class BALL_EXPORT InRingPredicate
		:	public	ExpressionPredicate
	{
		public:

			BALL_CREATE(InRingPredicate)

			/// Default constructor;
			InRingPredicate();

			/// Detailed constructor;
			InRingPredicate(Size n);

			/// Destructor;
			~InRingPredicate();

			/** Evaluate the predicate for the atom <tt>atom</tt>.
					@param atom the atom to test
					@return true, if the predicate is true, false otherwise
			*/
			virtual bool operator () (const Atom& atom) const;

	};

	/** Predicate class for atoms bearing a certain number of bonds.
			Arguments of this class are	<b> required </b> to consist 
			of an relational operator and a number between 1 and 8.
			The default is ">0" which means that the atom under siege has to bear
			at least one bond to make this predicate's operator () () return
			true.
	 */
	class BALL_EXPORT NumberOfBondsPredicate
		:	public	ExpressionPredicate
	{
		public:

			/// Default constructor.
			NumberOfBondsPredicate();

			BALL_CREATE(NumberOfBondsPredicate)

			/** Evaluate the predicate for the atom <tt>atom</tt>.
					@param atom the atom to test
					@return true, if the predicate is true, false otherwise
			*/
			virtual bool operator () (const Atom& atom) const;

		protected:
			bool testPredicate_(const Atom& atom, Bond::Order order) const;
	};


	/** Predicate class for atoms bearing single bonds.
			Arguments of this class are	<b> required </b> to consist 
			of an relational operator and a number between 1 and 8.
	 */
	class BALL_EXPORT SingleBondsPredicate
		:	public NumberOfBondsPredicate
	{
		public:
			BALL_CREATE(SingleBondsPredicate)

			/** Evaluate the predicate for the atom <tt>atom</tt>.
					@param atom the atom to test
					@return true, if the predicate is true, false otherwise
			*/
			virtual bool operator () (const Atom& atom) const;
	};


	/** Predicate for atoms bearing double bonds. 
			Arguments of this class are	<b> required </b> to consist 
			of an relational operator and a number between 1 and 8.
	 */
	class BALL_EXPORT DoubleBondsPredicate
		:	public NumberOfBondsPredicate
	{
		public:
			BALL_CREATE(DoubleBondsPredicate)

			/** Evaluate the predicate for the atom <tt>atom</tt>.
					@param atom the atom to test
					@return true, if the predicate is true, false otherwise
			*/
			virtual bool operator () (const Atom& atom) const;

	};


	/** Predicate class for atoms bearing triple bonds.
			Arguments of this class are	<b> required </b> to consist 
			of an relational operator and a number between 1 and 8.
	 */
	class BALL_EXPORT TripleBondsPredicate
		:	public DoubleBondsPredicate
	{
		public:
			BALL_CREATE(TripleBondsPredicate)

			/** Evaluate the predicate for the atom <tt>atom</tt>.
					@param atom the atom to test
					@return true, if the predicate is true, false otherwise
				*/
			virtual bool operator () (const Atom& atom) const;
	};


	/** Predicate class for atoms bearing a certain number of aromatic bonds.
			Arguments of this class are	<b> required </b> to consist 
			of an relational operator and a number between 1 and 8.
			The default is ">0" which means that the atom under siege has to bear
			at least one aromatic bond to make this predicate's operator () () return
			true.

			A bond is defined as aromatic if it has the property Bond::IS_AROMATIC set, or
			if its bond order equals Bond::ORDER__AROMATIC.
	 */
	class BALL_EXPORT AromaticBondsPredicate
		:	public ExpressionPredicate
	{
		public:
			BALL_CREATE(AromaticBondsPredicate)

			/** Evaluate the predicate for the atom <tt>atom</tt>.
					@param atom the atom to test
					@return true, if the predicate is true, false otherwise
			*/
			virtual bool operator () (const Atom& atom) const;

		protected:
			bool testPredicate_(const Atom& atom) const;
	};


	/** Predicate class for atoms being connected to a constellation defined
			by an expression. Syntax:
      \begin{tabular}{cl}
        Bonds  \par
        <tt>-</tt>            & single  \par
   
        <tt>=</tt>            & double  \par
   
        <tt>\#</tt>           & triple  \par
     
        <tt>\symbol{126</tt>} & aromatic  \par
     
        <tt>.</tt>            & any  \par
   
        Elements           &  \par
        <tt>symbol</tt>       & the desired element  \par
   
        <tt>*</tt>            & any element  \par
   
        Groups             &  \par
        <tt>( )</tt>          & symbols enlosed by brackets denote groups  \par
   
      \end{tabular}
	*/
	class BALL_EXPORT ConnectedToPredicate
		:	public	ExpressionPredicate
	{

		public:

			class BALL_EXPORT CTPNode
			{
				public:

					enum BondType
					{
						BONDTYPE__UNINITIALISED = 0,
						BONDTYPE__ANY           = 1,
						BONDTYPE__SINGLE        = 2,
						BONDTYPE__DOUBLE        = 3,
						BONDTYPE__TRIPLE        = 4,
						BONDTYPE__QUADRUPLE     = 5,
						BONDTYPE__AROMATIC      = 6
					};

					typedef std::list<CTPNode*>::iterator Iterator;
					typedef std::list<CTPNode*>::const_iterator ConstIterator;

					/*
					 */
					CTPNode();

					/*
					 */
					CTPNode(const CTPNode& node);

					/*
					 */
					virtual ~CTPNode();

					/*
					*/
					virtual void destroy();

					/*
					 */
					void setParent(CTPNode* parent);

					/*
					 */
					CTPNode* getParent() const;

					/*
					 */
					void addChild(CTPNode* child);

					/*
					 */
					void removeChild(CTPNode* child);

					/*
					 */
					::std::list<CTPNode*>& getChildren();

					Iterator begin();

					ConstIterator begin() const;

					Iterator end();

					ConstIterator end() const;

					/*
					 */
					Size getNumberOfChildren() const;

					/*
					 */
					void setBondType(Size type);

					/*
					 */
					void setBondType(char type);

					/*
					 */
					Size getBondType() const;

					/*
					 */
					char getBondTypeChar() const;

					/*
					 */
					String getSymbol() const;

					/*
					 */
					void setSymbol(const String& symbol);

					/*
					 */
					void setFinished();

					/*
					 */
					void unsetFinished();

					/*
					 */
					bool isFinished() const;

					/*
					 */
					void setLinked();

					/*
					 */
					void unsetLinked();

					/*
					 */
					bool isLinked() const;

					/*
					 */
					void linkWith(CTPNode* partner);

					/*
					 */
					const HashSet<CTPNode*>& getLinkSet() const;


				private: 

					/*
					 */
					String element_symbol_;

					/* Bond type means the bond connecting *to* this node.
					 */
					Size bond_type_;

					/*
					 */
					std::list<CTPNode*> children_;

					/*
					 */
					CTPNode* parent_;

					/*
					 */
					bool finished_;

					/*
					 */
					bool linked_;

					/*
					 */
					HashSet<CTPNode*> link_set_;

					/*
					*/
					struct greater_
					{

						bool operator () (const CTPNode* a, const CTPNode* b) const
						{
							if ((a != 0) && (b != 0))
							{
								if (a->getSymbol() == "*")
								{
									return true;
								}
								else
								{
									return false;
								}
							}
							else
							{
								Log.error() << "greater_: got NULL pointers" << std::endl;
								return false;
							}
						}

					};

			};

		public:
			BALL_CREATE(ConnectedToPredicate)

			ConnectedToPredicate();

			ConnectedToPredicate(const ConnectedToPredicate& predicate);

			virtual ~ConnectedToPredicate();

			virtual void clear();

			/** Evaluate the predicate for the atom <tt>atom</tt>.
					@param atom the atom to test
					@return true, if the predicate is true, false otherwise
			*/
			virtual bool operator () (const Atom& atom) const;
		
			void dump() const;

			void dump(const CTPNode* current) const;

			virtual void setArgument(const String& argument);

		private:
			/*_ The syntax tree
			*/
			CTPNode* tree_;

			/*_ Needed for realising parsing of loops (or links).
			*/
			HashMap<char, std::pair<CTPNode*, CTPNode*> > link_map_;

			/*_ Needed for realising parsing of loops (or links).
			*/
			char link_mark_;

			/*_ Keep it consistent
			*/
			CTPNode* createNewNode_(CTPNode* node);
		
			/*_
			*/
			CTPNode* parse_();

			// Clean up nodes created if parse_ exits with a parse error.
			void cleanUpNodes_(std::vector<CTPNode*>& all_nodes);

			/*_
			*/
			CTPNode* parse_(const String& input);

			bool bondOrderMatch_(const Bond& bond, const CTPNode& node) const;

			bool find_(const Atom& atom, const CTPNode* current,
					HashSet<BondHandle>& visited) const;

	};

	/** Predicate indicating sp hybridized atoms.
	 */
	class BALL_EXPORT SpHybridizedPredicate
		:	public	ExpressionPredicate
	{
		public:
			BALL_CREATE(SpHybridizedPredicate)

			/** Evaluate the predicate for the atom <tt>atom</tt>.
					@param atom the atom to test
					@return true, if the predicate is true, false otherwise
			*/
			virtual bool operator () (const Atom& atom) const;
	};

	/** Predicate indicating sp2 hybridized atoms.
	 */
	class BALL_EXPORT Sp2HybridizedPredicate
		:	public	ExpressionPredicate
	{
		public:
			BALL_CREATE(Sp2HybridizedPredicate)

			/** Evaluate the predicate for the atom <tt>atom</tt>.
					@param atom the atom to test
					@return true, if the predicate is true, false otherwise
			*/
			virtual bool operator () (const Atom& atom) const;
	};


	/** Predicate indicating sp3 hybridized atoms.
	 */
	class BALL_EXPORT Sp3HybridizedPredicate
		:	public	ExpressionPredicate
	{
		public:
			BALL_CREATE(Sp3HybridizedPredicate)

			/** Evaluate the predicate for the atom <tt>atom</tt>.
					@param atom the atom to test
					@return true, if the predicate is true, false otherwise
			*/
			virtual bool operator () (const Atom& atom) const;
	};

	/** Charge predicate
	 */
	class BALL_EXPORT ChargePredicate
		:	public	ExpressionPredicate
	{
		public:
		BALL_CREATE(ChargePredicate)

		/** Return true if the charge is in the indicated range.
				The argument of this predicate is either a number,
				in which case the charge of the atom has to be equal to 
				that number (down to the specified accuracy for floating 
				point comparisons,  \link EPSILON EPSILON \endlink ), or an operator followed	
				by a number. Possible operators are: <tt><</tt>, <tt>></tt>, <tt>>=</tt>, <tt> <= </tt>, <tt> = </tt>.
				@param atom the atom to test
				@return true, if the predicate is true, false otherwise
		*/
		virtual bool operator () (const Atom& atom) const;
	};

	/** Axial predicate.
			This predicate tries to tell whether a C1 of a sugar is in axial
			position. 
	 */
	class BALL_EXPORT AxialPredicate
		:	public	ExpressionPredicate
	{
		public:
		BALL_CREATE(AxialPredicate)

		/** Axial predicate needed for the determination of Glycam parameters.
				This is no generally applicable predicate.
				Return true if the atom is a C in a 6 membered ring containing
				and is connected to an H which is standing perpendicular
				on the plane containing the atom itself and those two Cs of the
				ring which are each one atom away.
				@param atom the atom to test
				@return true, if the predicate is true, false otherwise
		*/
		virtual bool operator () (const Atom& atom) const;
	};

	/** Predicate for determining whether a sugar ring is in 4C1
			conformation.
	 */
	class BALL_EXPORT Conformation4C1Predicate
		:	public	ExpressionPredicate
	{
		public:
		BALL_CREATE(Conformation4C1Predicate)

		/** 
				@param atom the atom to test
				@return true, if the predicate is true, false otherwise
		*/
		virtual bool operator () (const Atom& atom) const;
	};

	/** Helper class for all predicates that need ring information.
	*/
	class BALL_EXPORT RingFinder
	{
		public:

			// BALL_CREATE(RingFinder)

			/// Default constructor
			RingFinder();

			/** Detailed constructor. Initialize this instance with the size of
					the ring we want to find. Default setting is 0 which means that
					any ring size will match.
			*/
			RingFinder(Size n);

			/// Destructor
			virtual ~RingFinder();

			/** Return true, if atom is in a ring.
			*/
			bool operator () (const Atom& atom);

			/** Depth first search for finding rings.
			*/
			bool dfs(const Atom& atom, const Size limit);

			/** Set the ring size we want to find. Ring sizes below 3 will always
					return false, except if the size is set to 0, then any ring size
					will match. 
			*/
			void setRingSize(Size n);

			/** Return the hashset containing all visited bonds.
			*/
			const HashSet<BondHandle>& getVisitedBonds() const;

			/** Return the vector of ring atoms.
			*/
			const std::vector<AtomHandle>& getRingAtoms() const;

		private:

			/**
			*/
			const Atom* first_atom_;
			
			/**
			*/
			Size n_;

			/**
			*/
			bool exact_;

			/*_  v2.2 H4 7b.17 (D-H3.8): stable-id handles, not raw pointers.
			*/
			HashSet<BondHandle> visited_bonds_;

			/*_  v2.2 H4 7b.17 (D-H3.8): stable-id handles, not raw pointers.
			*/
			std::vector<AtomHandle> ring_atoms_;

	};

	// SMARTSPredicate holds SmartsMatcher (STRUCTURE) + AromaticityProcessor +
	// RingPerceptionProcessor (QSAR) BY VALUE, so the whole class is gated
	// under BALL_COLLAPSE_KERNEL_ONLY (v2.2 HCP-1 task 0; restored from Track-B's lifted
	// gate -- returns when STRUCTURE/QSAR re-open at HCP-3).
#ifndef BALL_COLLAPSE_KERNEL_ONLY
	/** Predicate for using smarts
	 */
	class BALL_EXPORT SMARTSPredicate
		:	public ExpressionPredicate
	{
		public:

		///
		SMARTSPredicate();

		///
		SMARTSPredicate(const SMARTSPredicate& pred);

		///
		virtual ~SMARTSPredicate();

		BALL_CREATE(SMARTSPredicate)

		/**
				@param atom the atom to test
				@return true, if the predicate is true, false otherwise
		*/
		virtual bool operator () (const Atom& atom) const;

 		mutable SmartsMatcher matcher_;
		mutable Molecule* last_molecule_;
		mutable AromaticityProcessor arom_proc_;
		mutable RingPerceptionProcessor ring_proc_;
		// when was the aromaticity lastly calculated for a given molecule:
		static HashMap<Molecule*, TimeStamp> call_time_map_;
		static Molecule dummy_molecule_;
		// v2.2 H3d.A (D-H3.8): matches_ is a per-call cache survival
		// across the (last_molecule_ + mod-time) test; key by
		// MoleculeStore::StableId so a slot recycle between calls
		// (compact() / atom release) cannot turn a cache hit into a
		// stale-pointer dereference. last_match_store_ pairs with
		// matches_ -- captured at populate time, used to resolve sids
		// back during lookup.
		mutable HashSet<MoleculeStore::StableId> matches_;
		mutable MoleculeStore* last_match_store_ = nullptr;
	};
#endif // BALL_COLLAPSE_KERNEL_ONLY


	//@}	
} // namespace BALL

#endif // BALL_KERNEL_STANDARDPREDICATES_H
