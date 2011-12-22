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

	//typedef std::pair<ChiralClass, Position> ChiralDef;
	class ChiralDef
	{
		public:
		SmilesParser::ChiralClass first;
		Position second;
	};

	class SPAtom
		: Atom
	{
		public:
		SPAtom(const String& /symbol/, bool /in_brackets/);
		~SPAtom() throw();
		Size getDefaultValence() const;
		Size countRealValences() const;
		Size getIsotope() const;
		void setIsotope(Size /isotope/);
		Index getFormalCharge() const;
		void setFormalCharge(Index /charge/);
		const SmilesParser::ChiralDef& getChirality() const;
		void setChirality(const SmilesParser::ChiralDef& /chirality/);
		bool isAromatic() const;
		void setAromatic(bool /is_aromatic/);
		bool isInBrackets() const;
		void setInBrackets(bool /in_brackets/);
	};

	class SPBond
		: Bond
	{
		public:
		~SPBond() throw();
		SPBond(SmilesParser::SPAtom* /first/, SmilesParser::SPAtom* /second/, Index /order/ = 1);
		SmilesParser::ZEIsomerType getZEType() const;
		void setZEType(SmilesParser::ZEIsomerType /type/);
	};

	public:

	typedef std::list<Position> ConnectionList;

	SmilesParser();
	SmilesParser(const SmilesParser& /parser/);
	~SmilesParser();
	void parse(const String& /s/)
		throw(ParseError);
	System getSystem() const;
	//SPAtom* createAtom(const String& /symbol/, bool /in_bracket/ = false);
	//void createBonds(SPAtom* /atom/, const ConnectionList* /list/);
	//void createBond(SPAtom* /left/, SPAtom* /right/, Index /order/);
	//void addMissingHydrogens();
	//struct State
	//{
	//	Size char_count;
	//	SmilesParser* current_parser;
	//	const char* buffer;
	//};
	//static State state;
};
