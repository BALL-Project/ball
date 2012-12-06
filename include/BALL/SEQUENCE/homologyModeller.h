// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//

#ifndef BALL_SEQUENCE_HOMOLOGYMODELLER_H
#define BALL_SEQUENCE_HOMOLOGYMODELLER_H

#ifndef BALL_DATATYPE_STRING_H
# include <BALL/DATATYPE/string.h>
#endif

#ifndef BALL_KERNEL_ATOMCONTAINER_H
# include <BALL/KERNEL/atomContainer.h>
#endif


namespace BALL
{
	class Alignment;
	class Protein;

	class BALL_EXPORT HomologyModeller
	{

		public:

			/**
			 *Default Constructor
			 */
			HomologyModeller();

			/**
			 *Deconstrcutor
			 **/
			~HomologyModeller();


			/**
			* build a model for the query protein based on the alignment to other structures
			*  
			* Note: at the moment a model for query is only created solely based on the first structure in the alignment
			*       make sure, that the naming conventions are compatible e.g. use frag_db.normalize_names);
							if the alignment contains gaps, the corresponding residues will have no reasonable positions
							if there is not total sequence identity, individual atoms have no reasonable positions
			*/
			bool createSimpleModel(Alignment& alignment, Protein& query);

			// bool modelGaps(Protein& query);

			///
			bool fixSkippedAtomsByRotamerLibrary(Protein& query, String property_name = "HOMOLOGY_MODELLER_SKIPPED");
	};

}
#endif // BALL_SEQUENCE_HOMOLOGYMODELLER_H
