// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//


#ifndef BALL_SEQUENCE_SEQUENCE_H
#define BALL_SEQUENCE_SEQUENCE_H
////////////////////////////////////////////////////////////////////////////////
#ifndef BALL_DATATYPE_STRING_H
	#include <BALL/DATATYPE/string.h>
#endif

#ifndef BALL_KERNEL_ATOMCONTAINER_H
	#include <BALL/KERNEL/atomContainer.h>
#endif


/////////////////////////////////////////////////////////////////////7
#include <Eigen/Dense>

///////////////////////////////////////////////////////////////////////////////////////////////////////


namespace BALL
{
	class SequenceIterator;

	class SequenceCharacter;
	     
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
			Sequence(String& seq_name, AtomContainer* seq_origin);

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
			void setName(const String& seq_name);

			/**
			 *@return the name of the sequence
			 */
			String& getName();


			/**
			 *@param origin the origin which is to be setted
			 */
			void setOrigin(AtomContainer* seq_origin);

			/**
			 *@return the origin of the Sequence
			 */
			AtomContainer* getOrigin();	

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
			SequenceIterator begin();				

			/**
			 *@return a SequenceIterator which points to exactly one position after the last character of the Sequence
			 */
			SequenceIterator end();

			/**
			 *@return String which contains the Sequence
			 **/
			String getStringSequence();

			/**
			*@return an Eigen Array of SequenceCharacters containing the Sequence
			*/
			Eigen::Array< SequenceCharacter ,Eigen::Dynamic,1> getArraySequence();
			

		private:

			String name;
			AtomContainer* origin;
			
	};

}//namespace BALL
#endif // BALL_SEQUENCE_SEQUENCE_H
