// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//

#ifndef BALL_STRUCTURE_PEPTIDES_H
#define BALL_STRUCTURE_PEPTIDES_H

#ifndef BALL_COMMON_H
#	include <BALL/common.h>
#endif

#ifndef BALL_KERNEL_PROTEIN_H
#	include <BALL/KERNEL/protein.h>
#endif

#ifndef BALL_DATATYPE_OPTIONS_H
# include <BALL/DATATYPE/options.h>
#endif

#ifndef BALL_CONCEPT_EMBEDDABLE_H
# include <BALL/CONCEPT/embeddable.h>
#endif

namespace BALL 
{

	namespace Peptides
	{
		/**	@name	Type definitions
		\ingroup StructureMiscellaneous
		*/
		//@{

		/// An amino acid sequence in three-letter code
		typedef std::list<String> ThreeLetterAASequence;

		/// An amino acid sequence in one-letter code
		typedef String OneLetterAASequence;
		//@}

		/**	@name Amino acid sequence conversion
		\ingroup StructureMiscellaneous
		*/
		//@{

		/**	Convert a three-letter amino acid name to the one-letter abbreviation.
				If the string given is not the three-letter code of one of the twenty
				amino acids, <tt>'?'</tt> is returned. This method is not case sensitive.
		*/
		BALL_EXPORT char OneLetterCode(const String& aa);

		/**	Convert a one-letter amino acid abbreviation to its three-letter code.
				If the character given is not the code for one of the twenty amino acids,
				<tt>'UNK'</tt> is returned.
				This method is not case sensitive. The strings returned are all upper cases.
		*/
		BALL_EXPORT String ThreeLetterCode(char aa);

		/** Determine whether the given character is a valid amino acid abbreviation.		 
		  
		 	  @param 	aa   amino acid abbreviation in question
				@return bool - true if aa is a valid amino acid abbreviation, false otherwise 
		 */
		BALL_EXPORT bool IsOneLetterCode(char aa);
		
		/** Determine whether the given string is a valid amino acid three-letter code. 	
		 		
				@param 	aa   amino acid string in question
				@return bool - true if aa is a valid amino acid three-letter code, false otherwise 
		 */
		BALL_EXPORT bool IsThreeLetterCode(const String& aa);

		/**	Convert an amino acid sequence from three-letter code to one-letter code.
				Call  \link OneLetterCode OneLetterCode \endlink  for each of the amino acids in the sequence.
		*/
		BALL_EXPORT OneLetterAASequence ThreeLetterToOneLetter(const ThreeLetterAASequence& sequence);
	
		/**	Convert an amino acid sequence from one-letter code to three-letter code.
				Call  \link ThreeLetterCode ThreeLetterCode \endlink  for each of the characters in the string.
		*/
		BALL_EXPORT ThreeLetterAASequence OneLetterToThreeLetter(const OneLetterAASequence& sequence);

		/**	Get amino acid sequence from an protein.
				This method iterates over all residues of the protein, calls
				 \link OneLetterCode OneLetterCode \endlink  for each of the residue names, and returns
				the concatenation of all results.
		*/
		BALL_EXPORT OneLetterAASequence GetSequence(const Protein& protein);	
		
		/**	Get amino acid sequence from a chain.
				This method iterates over all residues of the chain, calls
				 \link OneLetterCode OneLetterCode \endlink  for each of the residue names, and returns
				the concatenation of all results.
		*/
		BALL_EXPORT OneLetterAASequence GetSequence(const Chain& chain);

		/**	Get amino acid sequence from an iterator.
				This method iterates over all residues starting with it, calls
				 \link OneLetterCode OneLetterCode \endlink  for each of the residue names, and returns
				the concatenation of all results.
		*/
		BALL_EXPORT OneLetterAASequence GetSequence(ResidueConstIterator it);
		//@}
	 
//////////////////////////////////////////////////////////////////////////
//
//////////////////////////////////////////////////////////////////////////

		/** Name Converter class.
		   
			 \brief This class provides conversion of atom names between naming schemata.
		 
				Converts hydrogen atom names between naming systems.
				The original version of this table was created by Charles Hoogstraten.
				The default ini file allows conversion between 
						BMRB, SC, PDB, UCSF, MSI, XPLOR, SYBYL*, MIDAS*, and	DIANA.

				If no new atom name can be found the old name is returned.
				\par
	 	 	
				<br>
	 	  	Example code: <br> 
	 	  	\code
					NameConverter converter();
					if (   converter.supportsNamingScheme("BMRB")
					    && converter.supportsNamingScheme("PDB"))
					{
						...
						String atom_name = "HB2";
						String pdb_name  = converter.convertName("ALA", atom_name, "BMRB", "PDB");
						...
					}	
					if (   converter.supportsPseudoAtomNamingScheme("NMRSTAR") 
					    && converter.supportsPseudoAtomNamingScheme("PDB"))
					{	
						std::vector<String> result = converter.resolvePseudoAtoms("R", "HB", "NMRSTAR", "PDB"); 
						cout << "pseudo atom HB of residue R can be matched to " << result.size() << " PDB atoms." << endl;
						

						Atom* atom = ...;
    				if (converter.matches(atom->getResidue()->getName(), atom->getName(), "PDB", "HG2", "NMRSTAR"))
   	 				{
       				cout << "atom " << atom->getName() << " is a HG2 pseudo atom." << endl;
    				}
					}
 	 	  		
 				\endcode
		*/	

		class BALL_EXPORT NameConverter
			: public Embeddable
		{
    	public:
      	BALL_EMBEDDABLE(NameConverter, Embeddable)

				/** @name Constant Definitions
				*/
				//@{
				
				static const String	VALUE_NA;
			
				/// Option names
				struct BALL_EXPORT Option
				{	
					/** The conversion table file
					*/
					static const char* INIFile;	
					
					/** The pseudo atoms conversion table file
					*/
					static const char* PSEUDO_ATOMS_INIFile;	

				};

				/// Default values for options
				struct BALL_EXPORT Default
				{
					static const String INIFile;
					static const String PSEUDO_ATOMS_INIFile;
				};
		
				/** Standard constructor
				*/
				NameConverter();
			
				/// Destructor.
				virtual ~NameConverter() {}
		
				/** Reset the options to default values.
				*/
				void setDefaultOptions();

				/** Determine whether the Converter supports the naming scheme scheme_name.
				 *
				 *  @param 	scheme_name	naming scheme in question
				 * 	@return bool - true if the naming scheme is supported, false otherwise 
				 */
				bool supportsNamingScheme(const String& scheme_name) const;

				/** Determine whether the Converter supports the naming scheme scheme_name for pseudo atoms.
				 *
				 *  @param 	scheme_name	naming scheme in question
				 * 	@return bool - true if the naming scheme is supported, false otherwise 
				 */
				bool supportsPseudoAtomNamingScheme(const String& scheme_name) const;
	
				/** Convert atom name between two naming schemes.
				 *
				 *  If one of the naming schemes is unknown, or the name
				 *  was not found, the old name is returned.
				 *		
				 *  @return String - the converted name. 
				 */
				String convertName(const String& amino_acid, const String& old_atom_name,
													 const String& old_naming_scheme, const String& new_naming_scheme) const;
					
				/** Test whether two names for an atom are equivalent in different naming schemes.
				 *   
				 *	The match can either be according the the conversion as defined in 
				 *	the \link INIFile INIFile \endlink
				 * 	 or one atom is a pseudo atom and matches the other one according the 
				 *	\link PSEUDO_ATOMS_INIFile PSEUDO_ATOMS_INIFile \endlink.
				 *
				 * 	@return bool - true if the atom names can be matched, false otherwise. 
				 */
				bool matches(const String& amino_acid, const String& old_atom_name,
										 const String& old_naming_scheme, const String& new_atom_name,
				             const String& new_naming_scheme) const;

				/** Resolve pseudo atoms among naming schemes.
				 *
				 *  If the input is a pseudo atom in the given naming scheme, convert it into the list of
				 *  atoms in the desired naming scheme.
				 *
				 *  @param amino_acid the amino acid type of the atom
				 *  @param atom_name the atom name to test
				 *  @param old_naming_scheme the naming scheme of the input
				 *  @param new_naming_scheme the desired naming scheme
				 *  @return const std::vector<String>& - the list of atoms this atom resolves to
				 */
				std::vector<String> resolvePseudoAtoms(const String& amino_acid, const String& atom_name, 
				                                       const String& old_naming_scheme, const String& new_naming_scheme) const;

				/** @name Public Attributes
				*/
				//@{
				/// options
				Options options;
				//@}

			protected:	
				
				/** Read the Conversion table from file.
				 */
				void readConversionTable_()
					throw (Exception::FileNotFound());
			
				/** Read the pseudo atoms conversion table from file.
				 */
				void readPseudoAtomsConversionTable_()
					throw (Exception::FileNotFound());

				std::vector<String> 								conventions_;
				std::vector< std::vector <String> > conversion_table_;	
				
				std::vector<String> 								pseudo_conventions_;
				std::vector< std::vector <std::vector <String> > > pseudo_conversion_table_;

				//TODO: unse a StringHashMap of amino acid names to simplify the access
		};

	}  // namespace Peptides

} // namespace BALL

#endif // BALL_STRUCTURE_PEPTIDES_H_


