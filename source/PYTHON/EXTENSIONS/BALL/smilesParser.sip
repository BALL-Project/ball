// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//

class SmilesParser
{
%TypeHeaderCode
	#include <BALL/STRUCTURE/smilesParser.h>
	#include <BALL/KERNEL/atom.h>
	using namespace BALL;
%End

	public:
//	enum ZEIsomerType
//	{
//		NONE,
//		Z,
//		E
//	};

//	enum ChiralClass
//	{
//		NONCHIRAL,
//		TH,
//		AL,
//		SP,
//		TB,
//		OH
//	};

//	enum
//	{
//		MAX_CONNECTIONS = 100
//	};
//
	//typedef std::pair<ChiralClass, Position> ChiralDef;
//	class ChiralDef
//	{
//		public:
//		SmilesParser::ChiralClass first;
//		Position second;
//	};

//	class SPAtom
//		: Atom
//	{
//		public:
//		SPAtom(const String&, bool);
//		~SPAtom() throw();
//		Size getDefaultValence() const;
//		Size countRealValences() const;
//		Size getIsotope() const;
//		void setIsotope(Size);
//		Index getFormalCharge() const;
//		void setFormalCharge(Index);
//		const SmilesParser::ChiralDef& getChirality() const;
//		void setChirality(const SmilesParser::ChiralDef&);
//		bool isAromatic() const;
//		void setAromatic(bool);
//		bool isInBrackets() const;
//		void setInBrackets(bool);
//	};

//	class SPBond
//		: Bond
//	{
//		public:
//		~SPBond() throw();
//		SPBond(SmilesParser::SPAtom*, SmilesParser::SPAtom*, Index order = 1);
//		SmilesParser::ZEIsomerType getZEType() const;
//		void setZEType(SmilesParser::ZEIsomerType);
//	};

	public:

	typedef std::list<Position> ConnectionList;

	SmilesParser();
	SmilesParser(const SmilesParser&);
	~SmilesParser();
	void parse(const String&)
		throw(ParseError);
	System getSystem() const;
	//SPAtom* createAtom(const String&, bool in_bracket = false);
	//void createBonds(SPAtom*, const ConnectionList*);
	//void createBond(SPAtom*, SPAtom*, Index);
	//void addMissingHydrogens();
	//struct State
	//{
	//	Size char_count;
	//	SmilesParser* current_parser;
	//	const char* buffer;
	//};
	//static State state;
};
