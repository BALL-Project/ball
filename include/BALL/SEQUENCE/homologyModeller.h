// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//


#ifndef BALL_SEQUENCE_HOMOLOGYMODELLER_H
#define BALL_SEQUENCE_HOMOLOGYMODELLER_H
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

	class Alignment;
	class Protein;

	class BALL_EXPORT HomologyModeller
	{

		public:


			//////////////////////////////////Constructors and Deconstructor//////////////////////////////////////////


			/**
			 *Default Constructor
			 */
			HomologyModeller();

			/**
			 *Detailed Constructor
			 *@param name_ the name_ of the Sequence
			 *@param origin_ the composite from which the sequence origin_ates
			 */
		//	HomologyModeller(dcsd);

			/**
			 *Copy Constructor
			 */
			HomologyModeller(const HomologyModeller& modeller);
			/**
			 *Deconstrcutor
			 **/
			~HomologyModeller();


			/**
			* creates a Model for the proteins contained in the alignment
			*Note: at the moment a model is only created wehn the aligmment is perfect, that means the sequence is identically with the one in the template
			*/
			bool createModel(Alignment& alignment, Protein& templ);


		private:

					
	};

}//name_space BALL
#endif // BALL_SEQUENCE_SEQUENCE_H
