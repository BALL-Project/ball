// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//

#ifndef BALL_STRUCTURE_RECONSTRUCFRAGMENTPROCESSOR_H
#define BALL_STRUCTURE_RECONSTRUCFRAGMENTPROCESSOR_H

#include <list>

#ifndef BALL_COMMON_H
#	include <BALL/common.h>
#endif

#ifndef BALL_DATATYPE_TRIPLE_H
#	include <BALL/DATATYPE/triple.h>
#endif

#ifndef BALL_MATHS_MATRIX44_H
#	include <BALL/MATHS/matrix44.h>
#endif

#ifndef BALL_KERNEL_FRAGMENT_H
#	include <BALL/KERNEL/fragment.h>
#endif

namespace BALL 
{
	class FragmentDB;

	/** ReconstructFragmentProcessor.
	\ingroup StructureMiscellaneous		
	*/
	class BALL_EXPORT ReconstructFragmentProcessor 
		: public UnaryProcessor<Fragment> 
	{	
		
		public:

		/** @name Constructors and Destructors
		*/
		//@{

		/**	Default constructor.
				Create a new ReconstructFragmentProcessor, which is not yet 
				bound to a  \link FragmentDB FragmentDB \endlink , i.e. which cannot yet reconstruct
				fragments.
		*/
		ReconstructFragmentProcessor();

		/** Detailed constructor.
				Create a new ReconstructFragmentProcessor, which is 
				bound to the  \link FragmentDB FragmentDB \endlink  <tt>db</tt>.
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
				fragment from the current  \link FragmentDB FragmentDB \endlink .
		*/
		virtual Processor::Result operator() (Fragment&);

		//@}

		/** @name Accessors
		*/
		//@{
		
		/** Returns a list of inserted atoms.
		*/
		list<Atom*>& getInsertedAtoms();

		/** Returns number of recently inserted atoms.
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
		// ???? This should be replaced by the corresponding method in StructureMapper
		// We don't need two implementations with the same code.
		/**	Map three vectors onto each other.
				This method computes the transformation which 
				move the three points given in <tt>v1</tt>, <tt>v2</tt>, and <tt>v3</tt> 
				onto the the three reference points <tt>w1</tt>, <tt>w2</tt>, and <tt>w3</tt>.
				<tt>v1</tt> is always mapped exactly onto <tt>w1</tt>, and the plane spanned by
				<tt>v1</tt>, <tt>v2</tt>, and <tt>v3</tt> is always mapped onto  the plane spanned
        by the three reference points. Furthermore, <tt>v2</tt> is mapped such that
				the vector connecting <tt>v1</tt> and <tt>v2</tt> is parallel to the vector 
				connecting <tt>w1</tt> and <tt>w2</tt> after the transformation.
				Well-defined, but less meaningful transformations are obtained if
				any of the two point sets is co-linear.
		*/
		static Matrix4x4 mapThreeVectors
			(const Vector3& w1, const Vector3& w2, const Vector3& w3,
			 const Vector3& v1, const Vector3& v2, const Vector3& v3);

		/**	Search for the next atoms suitable as reference points.
				This method performs a BFS-like search for atoms in the same
				residue the <tt>ref_center_atom</tt> is in. It will consider
				only atoms contained in the hash set <tt>allowed</tt>.

		*/
		static Triple<bool, const Atom*, const Atom*>
		getTwoReferenceAtoms(const Atom& ref_center_atom, const HashSet<const Atom*>& allowed)
			;

		/**
		*/
		static list<Atom*> reconstructFragment(Fragment& frag, const Fragment& tplate);
		//@}

		protected:
		
		/*_ @name Protected attributes
		*/
		//_@{

		/*_	Pointer to the fragment DB 
		*/	
		const FragmentDB*	fragment_db_;
		
		/*_ List of atoms inserted during the last application of the processor.
		*/
		list<Atom*> inserted_atoms_;
		//_@}
	};
  
} // namespace BALL

#endif // BALL_STRUCTURE_RECONSTRUCFRAGMENTPROCESSOR_H
