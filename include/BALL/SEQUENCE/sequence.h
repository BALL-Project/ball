// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//

#ifndef BALL_SEQUENCE_SEQUENCE_H
#define BALL_SEQUENCE_SEQUENCE_H

#ifndef BALL_DATATYPE_STRING_H
	#include <BALL/DATATYPE/string.h>
#endif

#ifndef BALL_KERNEL_ATOMCONTAINER_H
	#include <BALL/KERNEL/atomContainer.h>
#endif


#include <Eigen/Dense>


namespace BALL
{
	class SequenceIterator;
	class SequenceCharacter;

	class BALL_EXPORT Sequence
	{

		public:

			//////////////////////////////////Constructors and Deconstructor//////////////////////////////////////////

			/**
			 * Default Constructor
			 */
			Sequence();

			/**
			 * Detailed Constructor
			 * @param name the name of the Sequence
			 * @param origin the composite from which the sequence originates
			 */
			Sequence(const String& name, AtomContainer* origin);

			/**
			 * Detailed Constructor
			 * @param name the name of the Sequence
			 * @param sequence  the sequence given in one letter code
			 */
			Sequence(const String& name, const String& sequence);

			/**
			 * Copy Constructor
			 */
			Sequence(const Sequence& seq);

			/**
			 * Deconstrcutor
			 **/
			~Sequence();

			/////////////////////////////////////Getter and Setter/////////////////////////////////////////////////////////////

			/**
			 * @param name the new name to be set
			 * @throws Exception::GeneralException if an invalid name was specified
			 */
			void setName(const String& name);

			/**
			 * @return the name of the sequence
			 */
			String getName()const;

			/**
			 *@param origin the origin which is to be setted
			 */
			void setOrigin(AtomContainer* origin);

			/**
			 * @return the origin of the Sequence
			 */
			AtomContainer* getOrigin() const;

			///////////////////////////////////////////////////////////// Operators //////////////////////////////////////////////////////////////////////

			/**
			 * operator==
			 */
			bool operator== (const Sequence& seq);

			/**
			 * operator!=
			 */
			bool operator!= (const Sequence& seq);

			/**
			 * assignment operator
			 */
			Sequence& operator= (const Sequence& seq);


			/////////////////////////////////////////////////Misc///////////////////////////////////////////

			/**
			 * @return a SequenceIterator which points to the first character of the Sequence
			 */
			SequenceIterator begin();

			/**
			 * @return a SequenceIterator which points to exactly one position after the last character of the Sequence
			 */
			SequenceIterator end();

			/**
			 * @return String which contains the Sequence
			 **/
			String getStringSequence();

			/**
			* @return an Eigen Array of SequenceCharacters containing the Sequence
			*/
			Eigen::Array< SequenceCharacter, 1, Eigen::Dynamic> getArraySequence();

		private:

			String         name_;
			AtomContainer* origin_;
	};
}
#endif // BALL_SEQUENCE_SEQUENCE_H
