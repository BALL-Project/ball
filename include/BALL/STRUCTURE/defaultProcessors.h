// $Id: defaultProcessors.h,v 1.3 2000/02/15 21:10:05 oliver Exp $

#ifndef BALL_STRUCTURE_DEFAULTPROCESSORS_H
#define BALL_STRUCTURE_DEFAULTPROCESSORS_H

#ifndef BALL_COMMON_H
#	include <BALL/common.h>
#endif

#ifndef BALL_KERNEL_atom_H
#	include <BALL/KERNEL/atom.h>
#endif

#ifndef BALL_MATHS_VECTOR3_H
#	include <BALL/MATHS/vector3.h>
#endif

#ifndef BALL_CONCEPT_PROCESSOR_H
#	include <BALL/CONCEPT/processor.h>
#endif

#ifndef BALL_DATATYPE_STRING_H
#	include <BALL/DATATYPE/string.h>
#endif

#ifndef BALL_DATATYPE_STRINGHASHMAP_H
#	include <BALL/DATATYPE/stringHashMap.h>
#endif

namespace BALL 
{

	/**@name	Standard processors
	*/
	//@{

	/**	Clears the charge on all atoms
	*/
	class ClearChargeProcessor
		:	public UnaryProcessor<Atom> 
	{
		public:

		virtual Processor::Result operator()(Atom& atom);
	};


	/**	Clears the radius of all atoms
	*/
	class ClearRadiusProcessor
		:	public UnaryProcessor<Atom>
	{
		public:
		virtual Processor::Result operator()(Atom& atom);
	};


	/**	Assigns the radius to each atom.
			This processor reads a radius table from a file
			and assigns each atom a raidus according to its
			name and the name of the fragment the atom is contained
			in.
	*/
	class AssignRadiusProcessor
		:	public UnaryProcessor<Atom> 
	{

		public:

		AssignRadiusProcessor();

		AssignRadiusProcessor(const String& filename);

		virtual bool start();

		virtual bool finish();

		virtual Processor::Result operator()(Atom& atom);

		/**	Set the filename to read the charges from
		*/
		void setFilename(const String& filename);

		/**	Return the current filename
		*/
		String& getFilename();
		
		/**	Return the number of assigned atoms
		*/
		unsigned long getNumberOfAssignments();

		/**	Return the number of unassignable atoms
		*/
		unsigned long getNumberOfErrors();


		protected:

		bool buildTable_();

		String									filename_;

		StringHashMap<float>		table_;

		unsigned long						number_of_errors_;
		unsigned long						number_of_assignments_;
	};


	/**	Assigns a charge to each atom.
			This processor reads a charge table from a file
			and assigns each atom a charge according to its
			name and the name of the fragment the atom is contained
			in.
	*/
	class AssignChargeProcessor
		: public AssignRadiusProcessor 
	{

		public:
		AssignChargeProcessor();

		AssignChargeProcessor(const String& filename);

		virtual bool start();
		
		virtual Processor::Result operator () (Atom& atom);


		/**	Returns the net assigned charge
		*/
		float getTotalCharge();


		protected:
		
		float		total_charge_;
	};

	//@}

} // namespace BALL

#endif // BALL_STRUCTURE_DEFAULTPROCESSORS_H
