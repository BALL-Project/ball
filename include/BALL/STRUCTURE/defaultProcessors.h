// $Id: defaultProcessors.h,v 1.8 2001/07/16 00:33:49 amoll Exp $

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

#ifndef BALL_DATATYPE_STRINGHASHMAP_H
#	include <BALL/DATATYPE/stringHashMap.h>
#endif

namespace BALL 
{

	/** @name	Standard processors
	*/
	//@{

	/**	Clears the charge on all atoms
	*/
	class ClearChargeProcessor
		:	public UnaryProcessor<Atom> 
	{
		public:

		/// Sets the charge to zero.
		virtual Processor::Result operator()(Atom& atom);
	};


	/**	Clears the radius of all atoms
	*/
	class ClearRadiusProcessor
		:	public UnaryProcessor<Atom>
	{
		public:

		/// Sets the radius to zero..
		virtual Processor::Result operator()(Atom& atom);
	};


	/**	Assigns the radius to each atom.
			This processor reads a radius table from a file	and assigns each 
			atom a radius according to its name and the name of the fragment 
			the atom is contained	in.
	*/
	class AssignRadiusProcessor
		:	public UnaryProcessor<Atom> 
	{
		public:

		/// Default constructor
		AssignRadiusProcessor();

		/** Detailled constructor.
				If the file can not be found in the actual path, FileNotFound is thrown.
		*/
		AssignRadiusProcessor(const String& filename)
			throw(Exception::FileNotFound);

		/** Start Method.
		 * 	The number of errors and the numbers of assignments are reset to 0.
		 * 	The radius data from the file is extracted.
		 * 	@return bool, allways true
		 */
		virtual bool start();

		/** Finish method.
		 * 	Allways returns true and does nothing.
		 */
		virtual bool finish();

		/** Applicator method
		 *  The full names of all atoms in the container are compared to the
		 *  atomnames form the file. For all matching atoms, the radii from the
		 *  file are set.
		 *  If for an atom from the container no matching atom from the file can be found,
		 *  a warning is displayed and the number of errors increases.
		 *  If in the file, the is a nonmatching atom, nothing happens.
		 *  \\
		 *  The matching of the atoms from the file with the atom of the container works like
		 *  this:\\
		 *  1.) The original atomnames are tested. \\
		 *  2.) The full name of the atoms are compared. \\
		 *  3.) Wild card matching: {\tt  "*:" + atom_name} \\
		 *  @see Residue
		 */
		virtual Processor::Result operator()(Atom& atom);

		/**	Set the filename to read the charges from.
		 *  If the file can not be found in the actual path, FileNotFound is thrown.
		 */
		void setFilename(const String& filename)
			throw(Exception::FileNotFound);

		/**	Return the current filename
		*/
		String& getFilename();
		
		/**	Return the number of assigned atoms.
		*/
		Size 	getNumberOfAssignments();

		/**	Return the number of unassignable atoms.
		 * 	Only the atoms form the container, which cannot be matched, count as errors.
		 * 	The unmatched atoms from the file dont care.
		 */
		Size 	getNumberOfErrors();


		protected:

		//_ Extract the data from the file.
		bool buildTable_()
			throw(Exception::FileNotFound);

		String									filename_;
		StringHashMap<float>		table_;
		Size 										number_of_errors_;
		Size										number_of_assignments_;
	};


	/**	Assigns a charge to each atom.
			This processor reads a charge table from a file	and assigns each 
			atom a charge according to its name and the name of the fragment 
			the atom is contained in.
	*/
	class AssignChargeProcessor
		: public AssignRadiusProcessor 
	{
		public:

		/// Default constructor
		AssignChargeProcessor();

		/** Detailled constructor.
		 * 	If the file can not be found in the actual path, FileNotFound is thrown.
		 */
		AssignChargeProcessor(const String& filename)
			throw(Exception::FileNotFound);
		
    /** Start Method.
		 *  The number of errors and the numbers of assignments are reset to 0.
		 *  The charge data from the file is extracted.
		 *  @return bool, always true
		 */
		virtual bool start();

		/** Applicator method.
 		 *	This method works like its counterpart in AssignRadiusProcessor, but for charges.
 		 *	@see AssignRadiusProcessor::operator()
 		 */
		virtual Processor::Result operator () (Atom& atom);

		/**	Returns the net assigned charge for all atoms.
		*/
		float getTotalCharge();


		protected:
		
		float		total_charge_;
	};

	//@}

} // namespace BALL

#endif // BALL_STRUCTURE_DEFAULTPROCESSORS_H
