// $Id: residueChecker.h,v 1.1 1999/09/17 11:16:37 oliver Exp $

#ifndef BALL_COMMON_H
#	include <BALL/common.h>
#endif

#ifndef BALL_STRUCTURE_FRAGMENT_DB_H
#	include <BALL/STRUCTURE/fragmentDB.h>
#endif

namespace BALL
{

	/**	Residue Consistency Checker.
			This processor checks residues for internal consistency.
			It identifies missing or superfluous atoms, suspect bond lengths
			and non-integral or extremely large charges on the residue.
			Warnings are emitted to \Ref{Log}.
	*/
	class ResidueChecker
		:	public	UnaryProcessor<Residue>
	{
		public:

		/**	Constructors and Destructors
		*/
		//@{

		BALL_CREATE(ResidueChecker)

		/**	Default constructor
		*/
		ResidueChecker();

		/**
		*/
		ResidueChecker(FragmentDB& fragment_db);

		/**	Copy constructor
		*/
		ResidueChecker(const ResidueChecker& resdiue_checker, bool deep = true);

		/**	Destructor
		*/
		virtual ~ResidueChecker();
		//@}

		/**	Processor related methods
		*/
		//@{

		/**
		*/
		Processor::Result operator () (Residue& residue);

		/**
		*/
		bool start();

		/**
		*/
		bool finish();
		//@}
			
		protected:

		FragmentDB*	fragment_db_;
	}; 
					
} // namespace BALL
