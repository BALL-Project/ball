// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: smartsParser.h,v 1.1 2005/04/07 15:57:22 bertsch Exp $
//

#ifndef BALL_STRUCTURE_SMARTES_PARSER_H
#define BALL_STRUCTURE_SMARTES_PARSER_H

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

	/** @name	SMARTS Parser.
	*/
	class SmartsParser
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
		class SPBond 
			:	public Bond
		{
			public:
			virtual ~SPBond() throw();

			SPBond(SPAtom* first, SPAtom* second, Index order = 1);

			ZEIsomerType getZEType() const;
			void setZEType(ZEIsomerType type);

			protected:
			ZEIsomerType	ze_type_;
		};
		
		class SPAtom
			:	public Atom
		{
			public:
				
			SPAtom(const String& symbol, bool in_brackets);
			virtual ~SPAtom() throw();

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
		SmartsParser();
			
		///
		SmartsParser(const SmartsParser& parser);

		///
		virtual ~SmartsParser();
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
			SmartsParser*	current_parser;
			const char*		buffer;
		};
		
		static State state;

		protected:
		System								system_;
		std::vector<SPAtom*>	connections_;
		std::vector<SPAtom*>	all_atoms_;
		static SmartsParser*	current_parser_;
	};
  
} // namespace BALL

#endif // BALL_STRUCTURE_SMARTS_PARSER_H
