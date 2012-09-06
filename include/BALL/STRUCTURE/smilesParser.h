// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//

#ifndef BALL_STRUCTURE_SMILES_PARSER_H
#define BALL_STRUCTURE_SMILES_PARSER_H

#ifndef BALL_COMMON_H
#	include <BALL/common.h>
#endif

#ifndef BALL_KERNEL_ATOM_H
#	include <BALL/KERNEL/atom.h>
#endif

#ifndef BALL_KERNEL_BOND_H
#	include <BALL/KERNEL/bond.h>
#endif

#ifndef BALL_KERNEL_SYSTEM_H
#	include <BALL/KERNEL/system.h>
#endif

namespace BALL 
{

	/** @name	SMILES Parser.
			A simple parser for SMILES strings. Output is a system, the atom coordinates
			however are all zero! The system contains only connectivity data (topology) of
			the molecule. At this time, there's no support in BALL to convert this 2D data
			automatically to 3D data.
			<br><br>
			<b>Status:</b> <b>experimental</b> \par
			\ingroup StructureMatching
	*/
	class BALL_EXPORT SmilesParser
	{
		public:
		enum ZEIsomerType
		{
			NONE,
			Z,
			E
		};

		enum ChiralClass
		{
	    NONCHIRAL,
			TH,
			AL,
			SP,
			TB,
			OH
		};

		enum
		{
			MAX_CONNECTIONS = 100
		};

		typedef std::pair<ChiralClass, Position> ChiralDef;

		class SPAtom;
		class BALL_EXPORT SPBond 
			:	public Bond
		{
			public:
			virtual ~SPBond() ;

			SPBond(SPAtom* first, SPAtom* second, Index order = 1);

			ZEIsomerType getZEType() const;
			void setZEType(ZEIsomerType type);

			protected:
			ZEIsomerType	ze_type_;
		};
		
		class BALL_EXPORT SPAtom
			:	public Atom
		{
			public:
				
			SPAtom(const String& symbol, bool in_brackets);
			virtual ~SPAtom() ;

			Size getDefaultValence() const;
			Size countRealValences() const;

			Size getIsotope() const { return isotope_; }
			void setIsotope(Size isotope) { isotope_ = isotope; };

			Index getFormalCharge() const { return formal_charge_; }
			void setFormalCharge(Index charge) { formal_charge_ = charge; }
			
			const ChiralDef& getChirality() const { return chirality_; }
			void setChirality(const ChiralDef& chirality) { chirality_ = chirality; }
			
			bool isAromatic() const { return is_aromatic_; }
			void setAromatic(bool is_aromatic) { is_aromatic_ = is_aromatic; };
			bool isInBrackets() const { return in_brackets_; }
			void setInBrackets(bool in_brackets) { in_brackets_ = in_brackets; };

			protected:
			Size				isotope_;
			Index				formal_charge_;
			ChiralDef		chirality_;
			bool				is_aromatic_;
			bool				in_brackets_;
		};
		
		typedef std::list<Position> ConnectionList;

		/**	@name Constructors and Destructors
		*/
		//@{

		///
		SmilesParser();
			
		///
		SmilesParser(const SmilesParser& parser);

		///
		virtual ~SmilesParser();
		//@}
		
		/**	@name	Parsing
		*/
		//@{
		/**	Parse a SMILES string.
		*/
		void parse(const String& s)
			throw(Exception::ParseError);

		/**	Return the parsed system
		*/
		const System& getSystem() const;
		//@}
		
		/**	@name Methods required by the underlying YACC parser
		*/
		//@{
		///
		SPAtom* createAtom(const String& symbol, bool in_bracket = false);

		///
		void createBonds(SPAtom* atom, const ConnectionList* list);		

		///
		void createBond(SPAtom* left, SPAtom* right, Index order);

		/// 
		void addMissingHydrogens();
		//@}
		

		struct State
		{
			Size					char_count;
			SmilesParser*	current_parser;
			const char*		buffer;
		};
		
		static State state;

		protected:
		System								system_;
		std::vector<SPAtom*>	connections_;
		std::vector<SPAtom*>	all_atoms_;
		static SmilesParser*	current_parser_;
	};
  
} // namespace BALL

#endif // BALL_STRUCTURE_SMILES_PARSER_H
