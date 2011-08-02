
#include <BALL/SEQUENCE/sequenceCharacter.h>
#include <BALL/SEQUENCE/sequence.h>

namespace BALL
{

		
		

			/////////////////////////////////////////// Constructors and Destructor ////////////////////////////////////////////////////

			/**
			 *Default Constructor
			 */
			SequenceCharacter::SequenceCharacter()
				:t(type::CHAR),
				character()
			 { 
					origin=0;
				}

			/**
			 *Copy Constructor
			 *@param sequenceCharacter the SequenceCharacter to construct from
			 */
			SequenceCharacter::SequenceCharacter( SequenceCharacter& seq_char)
			{
				origin = seq_char.getOrigin();
				t= seq_char.getType();
				character=seq_char.getChar();
			} 

			/**
			 *Detailed Constructor
			 *@param character the character of which comprises the new SequenceCharacter
			 *@param origin the sequence from which the character originates
			 *@param type tells whether the character is a GAP a nucleic acid or an amino acid
			 */
			SequenceCharacter::SequenceCharacter(char c, Sequence* ori, type ty)
				:t(ty),
				origin(ori),
				character(c)
				{		}

			/**
			 *Destructor
			 */					
			SequenceCharacter::~SequenceCharacter()
				{ 	}

			


			////////////////////////////////////////////////////// Getter and Setter ///////////////////////////////////////////////////

			/**
			 *sets the type of the Character and if type is GAP char will be automatically setted to '-'
			 *@param type the new type to be set
			 *@throws BALL::InvalidArgument if character is a GAP and type is to be setted to CHAR
			 */
			void SequenceCharacter::setType(SequenceCharacter::type ty)
			{

				//if type is to be setted to GAP the character will also be set to GAP
				if (ty == type::GAP) 
				{ 
					character = '-'; 
				} 
				
				//if type is to be setted to CHAR and character is a GAP throw Exception
				if( (character == '-') && (ty == type::CHAR ) ) 
				{ 
					throw Exception::InvalidArgument(__FILE__,__LINE__, "Don't set the type to Char manually just set the Char you want to change, the rest will be done automatically"); 
				}

				//change the type
				t=ty;

			}

			/**
			 *returns the type of the Character
			 *@return the type of the SequenceCharacter
			 */
			SequenceCharacter::type SequenceCharacter::getType() const
			{
				return t;
			}

			/**
			 *@param character the character to be set
			@throws InvalisArgumentException if Character does not represent a valid AminoAcid or Nucleotide OneLetterCode

			 */
			void SequenceCharacter::setChar(char c)
			{

					switch(c){
							case '-': 
											{
												t = type::GAP; character = c;return;
											}

							case 'B': 
							case 'J':
							case 'O':
							case 'X':
							case 'Z': 
											{
												throw Exception::InvalidArgument(__FILE__,__LINE__,"character is not a valid OneLetterCode");
											}

									}

				character = c;

				t = type::CHAR;
			}

			/**
			 *@return the stored character
			 */
			char SequenceCharacter::getChar() const 
			{
				return character;
			}

			/**
			 *sets the Origin of the Character
			 *@param origin the origin to be set
			 */
			void SequenceCharacter::setOrigin(Sequence::Sequence* ori)
			{
				origin = ori;
			}

			/**
			 *@return the origin of the character
			 */
			Sequence::Sequence* SequenceCharacter::getOrigin() const 
			{
				return origin;
			}
					//////////////////////////////////////////////////////////////////////////////// Operators ///////////////////////////////////////////////////
			/**
			*operator ==
			*/
			bool SequenceCharacter::operator== (SequenceCharacter::SequenceCharacter& c)
			{
				return ( (character== c.getChar()) && (origin == c.getOrigin()) && (t == c.getType()) );
			}
			
			/**
			*operator !=
			**/
			
			bool SequenceCharacter::operator!= (SequenceCharacter::SequenceCharacter& c)
			{
				return !(operator==(c));
			}

			SequenceCharacter& SequenceCharacter::operator = (const SequenceCharacter& c)
			{
				origin = (c.getOrigin());
				t= c.getType();
				character=c.getChar();	

				return *this;
			}
			
			
			//////////////////////////////////////////////////////////////////// Misc //////////////////////////////////////////////////////////

			/**
			 *@return true if the Character is a Gap
			 * false else
			 */
			bool SequenceCharacter::isGap()
			{
				return t == type::GAP;
			}

	
}//namespace BALL

