// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: residueChecker.h,v 1.16 2003/03/26 15:59:28 anhi Exp $

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
			Atom positions are checked as well: a warning is printed, if any of the 
			atom's coordinates is not a number (NaN).
			Warnings are emitted to  \link Log Log \endlink .  \par
	\ingroup StructureMiscellaneous		
	*/
	class ResidueChecker
		:	public	UnaryProcessor<Residue>
	{
		public:

		BALL_CREATE_DEEP(ResidueChecker)

		/**	Constructors and Destructors
		*/
		//@{

		/**	Default constructor
		*/
		ResidueChecker();

		/** Detailled constructor
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
				@return bool <b>true</b> if no warnings occured, <b>false</b> otherwise
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

		/**@name	Explicit tests
		*/
		//@{
		/**
		*/
		bool checkAtomPositions(const Residue& res, const String& res_name)
			throw();

		/**
		*/
		bool checkCharge(const Residue& res, const String& res_name)
			throw();

		/**
		*/
		bool checkCompleteness(const Residue& res, const Residue& reference, const String& res_name)
			throw();

		/**
		*/
		bool checkTemplate(const Residue& res, const Residue& reference, const String& res_name)
			throw();
			
		//@}
			
		protected:

		FragmentDB*	fragment_db_;
		bool				status_;
	}; 
  					
} // namespace BALL

#endif // BALL_STRUCTURE_RESIDUE_CHECKER
