#ifndef BALL_SEQUENCE_SEQUENCECHARACTER_H
#define BALL_SEQUENCE_SEQUENCECHARACTER_H

//////////////////////////////////////////////////////
#ifndef BALL_DATATYPE_STRING_H
#include <BALL/DATATYPE/string.h>
#endif


#ifndef BALL_CONCEPT_COMPOSITE_H
#include <BALL/CONCEPT/composite.h>
#endif

#ifndef BALL_COMMON_EXCEPTION_H
#include <BALL/COMMON/exception.h>
#endif

#ifndef BALL_KERNEL_RESIDUE_H
#include<BALL/KERNEL/residue.h>
#endif
////////////////////////////////////////////////////////////



namespace BALL
{
	class Sequence;

	class BALL_EXPORT SequenceCharacter
	{

		public:

			enum type { GAP , CHAR};



			/////////////////////////////////////////// Constructors and Destructor ////////////////////////////////////////////////////

			/**
			 *Default Constructor
			 */
			SequenceCharacter();

			/**
			 *Copy Constructor
			 *@param sequenceCharacter the SequenceCharacter to construct from
			 *@param origin the Sequence from which the character origins
			 */
			SequenceCharacter(const SequenceCharacter& seq_char); 

			/**
			 *Detailed Constructor
			 *@param character the character of which comprises the new SequenceCharacter
			 *@param origin the sequence from which the character originates
			 *@param type tells whether the character is a GAP a nucleic acid or an amino acid
			 */
			SequenceCharacter(char c, Sequence* ori, type ty,Residue* residue);

			/**
			 *Destructor
			 */					
			~SequenceCharacter();

			////////////////////////////////////////////////////// Getter and Setter ///////////////////////////////////////////////////

			/**
			 *sets the type of the Character
			 *@param type the new type to be set
			 *@throws BALL::InvalidArgument if type is no SequenceCharacterType
			 */
			void setType(const type ty);

			/**
			 *returns the type of the Character
			 *@return the type of the SequenceCharacter
			 */
			type getType() const;

			/**
			 *@param character the character to be set
			 */
			void setChar(const char c);

			/**
			 *@return the stored character
			 */
			char getChar() const;

			/**
			 *sets the Origin of the Character
			 *@param origin the origin to be set
			 */
			void setOrigin(Sequence* ori);

			/**
			 *@return the origin of the character
			 */
			Sequence* getOrigin() const;

			/**
			*sets the corresponding Residue of the Character
			*@param residue pointer to the residue to be setted
			*/
			void setResidue(Residue* residue);

			/**
			*@return the corresponding residue of the character
			*/
			Residue* getResidue() const;
						
			//////////////////////////////////////////////////////////////////////////////// Operators ///////////////////////////////////////////////////
			/**
			 *operator ==
			 */
			bool operator== (const SequenceCharacter& c);

			/**
			 *operator !=
			 **/

			bool operator!= (const SequenceCharacter& c);


			SequenceCharacter& operator = (const SequenceCharacter& c);

			//////////////////////////////////////////////////////////////////// Misc //////////////////////////////////////////////////////////

			/**
			 *@return true if the Character is a Gap
			 * false else
			 */
			bool isGap();

			void reset();
		private:
			type t_;
			Sequence* origin_;
			char character_;
			Residue* res_;
	};

}//namespace BALL
#endif // BALL_SEQUENCE_SEQUENCECHARACTER_H
