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
			 *@param name_ the name_ of the Sequence
			 *@param origin_ the composite from which the sequence origin_ates
			 */
			Sequence(String& name, AtomContainer* origin);

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
			 * @param name_ the new name_ to be set
			 * @throws Exception::GeneralException if an invalid name_ was specified
			 */
			void setName(const String& name);

			/**
			 *@return the name_ of the sequence
			 */
			String& getName();


			/**
			 *@param origin_ the origin_ which is to be setted
			 */
			void setOrigin(AtomContainer* origin);

			/**
			 *@return the origin_ of the Sequence
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

			String name_;
			AtomContainer* origin_;
			
	};

}//name_space BALL
#endif // BALL_SEQUENCE_SEQUENCE_H