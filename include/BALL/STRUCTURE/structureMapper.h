// $Id: structureMapper.h,v 1.9 2001/06/21 08:58:41 amoll Exp $

#ifndef BALL_STRUCTURE_STRUCTUREMAPPER_H
#define BALL_STRUCTURE_STRUCTUREMAPPER_H

#ifndef BALL_COMMON_H
#	include <BALL/common.h>
#endif

#ifndef BALL_CONCEPT_PROCESSOR_H
#	include <BALL/CONCEPT/processor.h>
#endif

#ifndef BALL_STRUCTURE_GEOMETRICTRANSFORMATIONS_H
#	include <BALL/STRUCTURE/geometricTransformations.h>
#endif

#ifndef BALL_DATATYPE_STRING_H
#	include <BALL/DATATYPE/string.h>
#endif

#ifndef BALL_KERNEL_PROTEIN_H
#	include <BALL/KERNEL/protein.h>
#endif

#ifndef BALL_KERNEL_ATOM_H
#	include <BALL/KERNEL/atom.h>
#endif

#ifndef BALL_MATHS_MATRIX44_H
#	include <BALL/MATHS/matrix44.h>
#endif

#include <vector>
#include <map>

namespace BALL 
{

	using std::vector;
	using std::map;

	/**	Structure mapping class.
			{\bf Definition:} \URL{BALL/STRUCTURE/structureMapper.h}
	*/
	class StructureMapper
		:	public TransformationProcessor
	{
		public:

		struct AtomPairStruct 
		{
			Atom*	first;
			Atom*	second;
		};
		
		typedef struct AtomPairStruct	AtomPairType;

		/*_	The list representing a bijection between	selected atoms of A and B
		*/
		typedef vector < AtomPairType >	AtomBijection;
		
		/**	@name	Constructors and Destructors
		*/
		//@{
		
		/**	Default constructor
		*/
		StructureMapper();

		/**	Constructor
		*/
		StructureMapper(Composite& A, Composite& B);

		/**	Destructor
		*/
		~StructureMapper();

		//@}

		/**	Assign the two objects to be mapped
		*/	
		void set(Composite& A, Composite& B);
		
		/**	Calculate the root mean squared deviation
		*/
		double calculateRMSD();
		
		/**	Calculate the transformation to map the first of two isomorphous 
				Composite objects onto the second
		*/
		bool calculateTransformation();

		/**	Calculate a reasonable bijection for the mapping of A and B
		*/
		void calculateDefaultBijection();


		/**	Calculates a bijection to map two arrays of fragments onto each other.
		*/
		StructureMapper::AtomBijection& calculateFragmentBijection(const vector<Fragment*> &A,
																															 const vector<Fragment*> &B);

		/**	Maps two fragments onto each other
		*/
		bool mapFragments(const vector<Fragment*>& A, const vector<Fragment*>& B, 
											Matrix4x4* transformation,
											double upper_bound = 8.0,	double lower_bound = 2.5);

		Matrix4x4 matchPoints_(const Vector3& w1,	const Vector3& w2, const Vector3& w3,	
													 const Vector3& v1,	const Vector3& v2, const Vector3& v3);

		Matrix4x4 matchPointsOptimally_(const Vector3& w1, const Vector3& w2, const Vector3& w3,
																		const Vector3& v1, const Vector3& v2,	const Vector3& v3);

		Matrix4x4 matchBackboneAtoms(const Residue* r1, const Residue* r2);

		int mapResidues(const list<Residue*>& l1, const list<Residue*>& l2);
	 
		vector<vector<Fragment*> >& searchPattern_(vector<Fragment*> &pattern, Composite &composite,
																				 			 double max_rmsd = 4.0,	   double max_center_tolerance = 2.0,
																				  		 double upper_bound = 8.0, double lower_bound = 4.0);

		Matrix4x4 mapProteins_(Protein& P1, Protein& P2, 
													 map<String, int>&	type_map,
													 int&								no_matched_ca,
													 double&						rmsd,
													 double							upper_bound = 8.0,
													 double							lower_bound = 4.0,
													 double							tolerance = 0.6);

		protected:
			
		Size countFragments_(const Composite& composite) const;
		

		/*_	The first of two composites - the "original" 
		*/
		Composite*	A_;

		/*_	The second composites - the "copy" to be mapped 
		*/
		Composite*	B_;
		
		/*_	The current atom bijection (needed for the caclulation of the rmsd 
		*/
		StructureMapper::AtomBijection bijection_;
		
		/*_	The rmsd of the last mapping executed 
		*/
		double	rmsd_;
	};

#	ifndef	BALL_NO_INLINE_FUNCTIONS
#		include <BALL/STRUCTURE/structureMapper.iC>
#	endif

} // namespace BALL

#endif // BALL_STRUCTURE_STRUCTUREMAPPER_H
