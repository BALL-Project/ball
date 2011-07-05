#ifndef BALL_SEQUENCE_SEQUENCE_H
#define BALL_SEQUENCE_SEQUENCE_H
////////////////////////////////////////////////////////////////////////////////
#ifndef BALL_DATATYPE_STRING_H
	#include <BALL/DATATYPE/string.h>
#endif

#ifndef BALL_CONCEPT_COMPOSITE_H
	#include <BALL/CONCEPT/composite.h>
#endif

///////////////////////////////////////////////////////////////////////////////////////////////////////

class SequenceIterator;

namespace BALL
{

	class BALL_EXPORT Sequence
	{

		public:


			//////////////////////////////////Constructors and Deconstructor//////////////////////////////////////////


			/**
			 *Default Constructor
			 */
			Sequence();

			/**
			 *Detailed Constructor
			 *@param name the name of the Sequence
			 *@param origin the composite from which the sequence originates
			 */
			Sequence(String& seq_name, Composite* seq_origin);

			/**
			 *Copy Constructor
			 */
			Sequence(Sequence& seq);

			/**
			 *Deconstrcutor
			 **/
			~Sequence();

			/////////////////////////////////////Getter and Setter/////////////////////////////////////////////////////////////

			/**
			 * @param name the new name to be set
			 * @throws Exception::GeneralException if an invalid name was specified
			 */
			void setName(String& seq_name);

			/**
			 *@return the name of the sequence
			 */
			String& getName();


			/**
			 *@param origin the origin which is to be setted
			 */
			void setOrigin(Composite* seq_origin);

			/**
			 *@return the origin of the Sequence
			 */
			Composite* getOrigin();	
			
			///////////////////////////////////////////////////////////// Operators //////////////////////////////////////////////////////////////////////
			
			/**
			*operator==
			*/
			bool operator== (Sequence& seq);

			/**
			*operator!=
			*/
			bool operator!= (Sequence& seq);
			
			/**
			*assignment operator
			*/
			Sequence& operator= (Sequence& seq);
			
			
			/////////////////////////////////////////////////Misc///////////////////////////////////////////

			/**
			 *@return a SequenceIterator which points to the first character of the Sequence
			 */
			SequenceIterator& begin();				

			/**
			 *@return a SequenceIterator which points to exactly one position after the last character of the Sequence
			 */
			SequenceIterator& end();

			/**
			 *@return String which contains the Sequence
			 **/
			String& getSequence();

		private:

			String name;

			Composite* origin;

	};

}//namespace BALL
#endif // BALL_SEQUENCE_SEQUENCE_H
