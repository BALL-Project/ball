// $Id: smilesParser.sip,v 1.1 2002/02/03 14:28:24 oliver Exp $


class SmilesParser
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
	//typedef std::pair<ChiralClass, Position> ChiralDef;
	class ChiralDef
	{
		public:
		SmilesParser::ChiralClass first;
		Position second;
	};
	class SPBond
		: Bond
	{
		public:
		SPBond(SmilesParser::SPAtom* /first/, SmilesParser::SPAtom* /second/, Index /order/ = 1);
		SmilesParser::ZEIsomerType getZEType() const;
		void setZEType(SmilesParser::ZEIsomerType /type/);
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
	public:

	typedef std::list<Position> ConnectionList;

	SmilesParser();
	SmilesParser(const SmilesParser& /parser/);
	~SmilesParser();
	void parse(const String& /s/)
		throw(ParseError);
	const System& getSystem() const;
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
