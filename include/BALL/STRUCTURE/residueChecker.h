// $Id: residueChecker.h,v 1.4 2000/08/30 19:58:26 oliver Exp $


#ifndef BALL_STRUCTURE_RESIDUECHECKER_H
#define BALL_STRUCTURE_RESIDUECHECKER_H

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

		BALL_CREATE_DEEP(ResidueChecker)

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

		/**	@name	Predicates
		*/
		//@{

		/**	Return true if the last application did not produce any warning.
				The getStatus method may be called after applying the ResidueChecker 
				to a kernel object. 
				@return bool {\bf true} if no warnings occured, {\bf false} otherwise
		*/
		bool getStatus() const;
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
		bool				status_;
	}; 
					
} // namespace BALL

#endif // BALL_STRUCTURE_RESIDUE_CHECKER
