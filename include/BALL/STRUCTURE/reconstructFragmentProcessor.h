// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: reconstructFragmentProcessor.h,v 1.2 2002/05/12 14:03:22 oliver Exp $

#ifndef BALL_STRUCTURE_RECONSTRUCFRAGMENTPROCESSOR_H
#define BALL_STRUCTURE_RECONSTRUCFRAGMENTPROCESSOR_H

#ifndef BALL_COMMON_H
#	include <BALL/common.h>
#endif

#ifndef BALL_DATATYPE_TRIPLE_H
#	include <BALL/DATATYPE/triple.h>
#endif

#ifndef BALL_MATCH_MATRIX44_H
#	include <BALL/MATHS/matrix44.h>
#endif

#ifndef BALL_KERNEL_FRAGMENT_H
#	include <BALL/KERNEL/fragment.h>
#endif

namespace BALL 
{

	class FragmentDB;

	/** ReconstructFragmentProcessor.
			
	*/
	class ReconstructFragmentProcessor 
		: public UnaryProcessor<Fragment> 
	{	
		
		public:

		/** @name Constructors and Destructors
		*/
		//@{

		/**	Default constructor.
				Create a new ReconstructFragmentProcessor, which is not yet 
				bound to a \Ref{FragmentDB}, i.e. which cannot yet reconstruct
				fragments.
		*/
		ReconstructFragmentProcessor();

		/** Detailed constructor.
				Create a new ReconstructFragmentProcessor, which is 
				bound to the \Ref{FragmentDB} {\tt db}.
		*/
		ReconstructFragmentProcessor(const FragmentDB& db);

		/**	Copy Constructor.
		*/
		ReconstructFragmentProcessor(const ReconstructFragmentProcessor& db);

		/** Destructor 
		*/
		virtual ~ReconstructFragmentProcessor();

		//@}			
		/** @name Processor-related methods 
		*/
		//@{

		/**	Processor start method
		*/
		virtual bool start();
		
		/**	Processor finish method
		*/
		virtual bool finish();
		
		/** Processor applicaiton method.
				Tries to reconstruct the current fragment from a matching
				fragment from the current \Ref{FragmentDB}.
		*/
		virtual Processor::Result operator() (Fragment&);

		//@}

		/** @name Accessors
		*/
		//@{
		
		/** Returns number of recently inserted hydrogens
		*/
		Size getNumberOfInsertedAtoms() const;

		/**	Set the fragment database
		*/
		void setFragmentDB(const FragmentDB& fragment_db);

		/**	Return the current fragment database.
		*/
		const FragmentDB* getFragmentDB() const;

		//@}

		/**@name	Static methods.
		*/
		//@{
		/**	Map three vectors onto each other.
				This method computes the transformation which 
				move the three points given in {\tt v1}, {\tt v2}, and {\tt v3} 
				onto the the three reference points {\tt w1}, {\tt w2}, and {\tt w3}.
				{\tt v1} is always mapped exactly onto {\tt w1}, and the plane spanned by
				{\tt v1}, {\tt v2}, and {\tt v3} is always mapped onto  the plane spanned
        by the three reference points. Furthermore, {\tt v2} is mapped such that
				the vector connecting {\tt v1} and {\tt v2} is parallel to the vector 
				connecting {\tt w1} and {\tt w2} after the transformation.
				Well-deinfed, but less meaningful transformations are obtained if
				any of the two point sets is co-linear.
		*/
		static Matrix4x4 mapThreeVectors
			(const Vector3& w1, const Vector3& w2, const Vector3& w3,
			 const Vector3& v1, const Vector3& v2, const Vector3& v3);

		/**	Search for the next atoms suitable as reference points.
				This method performs a BFS-like search for atoms in the same
				residue the {\tt ref_center_atom} is in. It will consider
				only atoms contained in the hash set {\tt allowed}.

		*/
		static Triple<bool, const Atom*, const Atom*>
		getTwoReferenceAtoms(const Atom& ref_center_atom, const HashSet<const Atom*>& allowed)
			throw();

		/**
		*/
		static Size reconstructFragment(Fragment& frag, const Fragment& tplate);
		//@}

		protected:
		
		/*_ @name Protected attributes
		*/
		//_@{

		/*_	Pointer to the fragment DB 
		*/	
		const FragmentDB*	fragment_db_;
		
		/*_ Number of atoms inserted during the last application  of the processor
		*/
		Size	number_of_inserted_atoms_;
		//_@}
	};

} // namespace BALL

#endif // BALL_STRUCTURE_RECONSTRUCFRAGMENTPROCESSOR_H_


