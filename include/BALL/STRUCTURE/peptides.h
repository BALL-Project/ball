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

		//@}
	 
//////////////////////////////////////////////////////////////////////////
//
//////////////////////////////////////////////////////////////////////////

		/**	Name Converter
		 
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
						String pdb_name = converter.convertName("ALA", atom_name, "BMRB", "PDB");
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
				};

				/// Default values for options
				struct BALL_EXPORT Default
				{
					static const String INIFile;
				};
		
				/** Standard constructor
				*/
				NameConverter();
			
				/// Destructor.
				virtual ~NameConverter() {}
		
				/** Reset the options to default values.
				*/
				void setDefaultOptions();
	
				/** Convert atom name between two naming schemes.
				 *
				 *  If one of the naming schemes is unknown, or the name
				 *  was not found, the old name is returned.
				 */
				String convertName(const String& amino_acid, const String& old_atom_name,
				                   const String& old_naming_scheme, const String& new_naming_scheme) const;

				/** Determine whether the Converter supports the naming scheme scheme_name.
				 *
				 *  @param 	scheme_name	naming scheme in question
				 * 	@return bool - true if the naming scheme is supported, false otherwise 
				 */
				bool supportsNamingScheme(const String& scheme_name) const;


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

				std::vector<String> 								conventions_;
				std::vector< std::vector <String> > conversion_table_;
		};

	}  // namespace Peptides

} // namespace BALL

#endif // BALL_STRUCTURE_PEPTIDES_H_


