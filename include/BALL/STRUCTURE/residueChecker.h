// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: residueChecker.h,v 1.19 2003/08/26 08:04:53 oliver Exp $
//

#ifndef BALL_STRUCTURE_RESIDUECHECKER_H
#define BALL_STRUCTURE_RESIDUECHECKER_H

#ifndef BALL_MATHS_COMMON_H
#	include <BALL/MATHS/common.h>
#endif

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
			The individual tests can be enabled/disabled using the \link enable enable \endlink
			and \link disable disable \endlink methods. Please use the enum
			\link TestTypes TestTypes \endlink to refer to the appropriate tests.
			By default all tests are enabled.
			
			The following tests are currently implemented:
			<ul>
				<li> 
					missing atoms in a residue
				</li>
				<p>
					This test checks whether all atoms contained in the fragment database for
					this type of residue are present in each of the residues.
				</p>
				<li>
					extra atoms
				</li>
				<p>
					This test checks whether all atoms contained in the current residue are also	
					present in the template from the fragment database.
				</p>
				<li>
					correct elements
				</li>
				<p>
					Check that all atoms in a residue are of the same element as the corresponding 
					atoms in the template from the fragment database.
				</p>
				<li>
					suspect bond lengths
				</li>
				<p>
					Check that the bond lengths encountered in each of the residues agree within 15% with
					the bond length in the template from the reference database.
				</p>
				<li>
					non-integral net charge
				</li>
				<p>
					Check that the sum of charges of a residue is integral, i.e. the sum of all charges
					is an integer (+/- 0.05 elementary charges).
				</p>
				<li>
					large charges
				</li>
				<p>
					Check that the none of the atoms bears a charge with an absolute value above
					4.0 elementary charges.
				</p>
				<li>
					large net charge
				</li>
				<p>
					Check that the net charge of the residue is between -2 and +2 elementary charges.
				</p>
				<li>
					NaN atom positions
				</li>
				<p>
					Check whether any atom has NaN (not a number) as any of its corrdinates. This is a frequent source of	
					error after failing simulations, minimizations, etc.
				</p>
				<li>
					overlapping atoms
				</li>
				<p>
					Check whether any pair of atoms within a residue is closer than 0.5 Angstrom.
				</p>
				<li>
					duplicate atom names
				</li>
				<p>
					Check whether two atoms within the residue have the same name.
				</p>
				<li>
					unknown residues
				</li>
				<p>
					If unknown residues are encountered (i.e. residues for which no reference template
					exists in the fragment database), an error occurs. If this test is disabled,
					the tests requiring a template (missing atoms, extra atoms, suspectr bond lengths, elements) will
					not be executed for unknown residues.
			</ul>
	\ingroup StructureMiscellaneous		
	*/
	class ResidueChecker
		:	public UnaryProcessor<Residue>
	{
		public:

		BALL_CREATE(ResidueChecker)

		/**	Enums
		*/
		//@{
			
		/**	Enum to refer to the individual tests.
				The values of this enum are used by \link enable enable \endlink,
				\link disable disable \endlink, and \link isEnabled isEnabled \endlink.
		*/
		enum TestType
		{
			///
			MISSING_ATOMS = 0,
			///
			EXTRA_ATOMS,
			///
			ELEMENTS, 
			///
			SUSPECT_BOND_LENGTHS,
			///
			NON_INTEGRAL_NET_CHARGE,
			///
			LARGE_CHARGES,
			///
			LARGE_NET_CHARGE,
			///
			NAN_POSITIONS,
			///
			OVERLAPPING_ATOMS,
			///
			DUPLICATE_ATOM_NAMES,
			/// 
			UNKNOWN_RESIDUES,

			NUMBER_OF_TESTS
		};
		//@}

		/**	Constructors and Destructors
		*/
		//@{

		/**	Default constructor
		*/
		ResidueChecker();

		/** Detailed constructor
		*/
		ResidueChecker(FragmentDB& fragment_db);

		/**	Copy constructor
		*/
		ResidueChecker(const ResidueChecker& residue_checker);

		/**	Destructor
		*/
		virtual ~ResidueChecker();

		//@}
		/**	Accessors
		*/
		//@{
		/**	Enable a specific test
		*/
		void enable(TestType t) throw();

		/**	Disable a specific test
		*/
		void disable(TestType t) throw();
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

		/**	Check whether a specific test is enabled.
		*/
		bool isEnabled(TestType t) const throw();

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
		BitVector		tests_;
	}; 
  					
} // namespace BALL

#endif // BALL_STRUCTURE_RESIDUE_CHECKER
