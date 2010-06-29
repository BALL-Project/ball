// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//

namespace Peptides
{
%TypeHeaderCode
	#include <BALL/STRUCTURE/peptides.h>

	using namespace BALL;
	using namespace Peptides;
%End

	char OneLetterCode(const String&);
	String ThreeLetterCode(char);
	bool IsOneLetterCode(char);
	bool IsThreeLetterCode(const String&);
	String ThreeLetterToOneLetter(const list<String>&);
	list<String> OneLetterToThreeLetter(const String&);
	String GetSequence(const Protein&);
	String GetSequence(const Chain&);

	class NameConverter
		: Embeddable
	{
		public:
			struct Option
			{
				static const char* INIFile;	
				static const char* PSEUDO_ATOMS_INIFile;	
			};

			struct Default
			{
				static const String INIFile;
				static const String PSEUDO_ATOMS_INIFile;
			};
		
			NameConverter();
			virtual ~NameConverter();
			void setDefaultOptions();

			// methods implemented by the BALL_EMBEDDED macro
			static Size countInstances();
			static Peptides::NameConverter* getInstance(Position);

			bool supportsNamingScheme(const String&) const;

			bool supportsPseudoAtomNamingScheme(const String&) const;
		
			String convertName(const String&, const String&,
												 const String&, const String&) const;
				
			bool matches(const String&, const String&,
									 const String&, const String&, const String&) const;

			vector<String> resolvePseudoAtoms(const String&, const String&, 
																				const String&, const String&) const;

			Options options;

	};

};

