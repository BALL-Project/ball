// $Id: LEF.h,v 1.6 2000/09/18 17:09:26 oliver Exp $

#ifndef BALL_NMR_LEF_H
#define BALL_NMR_LEF_H

#ifndef BALL_NMR_SHIFT_MODULE
#	include<BALL/NMR/shiftModule.h>
#endif

#ifndef BALL_KERNEL_EXPRESSION_H
#	include<BALL/KERNEL/expression.h>
#endif

#ifndef BALL_FORMAT_PARAMETERS_H
#	include<BALL/FORMAT/parameters.h>
#endif

#ifndef BALL_FORMAT_PARAMETERSECTION_H
#	include<BALL/FORMAT/parameterSection.h>
#endif

namespace BALL 
{

	class Atom;
		
	/**	Shift assignment processor implementing LEF.
	*/
	class LEFShiftProcessor
		:	public ShiftModule
	{
		public:
	
		/**@name	Constructors and Destructors
		*/
		//@{

		/**	Default constructor.
		*/
		LEFShiftProcessor()
			throw();
	
		/**	Destructor
		*/
		virtual ~LEFShiftProcessor()
			throw();
	
		//@}

	
		/**@name	Processor specific functions.
		*/
		//@{
		
		/**	Finish method.
				Here, the  shift calculation is carried out.
				There are four constant floats which contain the charges of atoms, named
				{\tt QC}, {\tt QN}, {\tt QO} and {\tt QH}.
				It iterates over every hydrogen in {\tt proton\_list\_}.
				Its position is stored in {\tt proton} and the position of its bounded Atom
				is stored in {\tt bond}. Their difference vector is calculated and stored in
				{\tt prot\_bin}.
				Next, an iteration over every atom in {\tt atom\_list\_} is started.
				If the atom is in another residue then the proton is, the atom´s name is checked,
				and according to that {\tt ladung} is assigned whith the charge of the atom.
				The position of the atom is stored in {\tt atom}. Next the difference vector between
				{\tt proton} and {\tt atom} is calculated and stored in {\tt prot\_atom}.
				Then the scalarproduct of the two difference vectors {\tt prot\_bin} and
				{\tt prot\_atom} is calculated and stored in {\tt sc}.
				After that the Lengths of the vectors are stored in {\tt b\_prot\_bin} and {\tt b\_prot\_atom}.
				Next the cosinus of the angle between these vectors is calculated and stored in 
				{\tt theta}. Then {\tt Efact} is assigned the quotient of {\tt ladung} and the product of both
				vector´s lenghts. {\tt theta} * {\tt Efact} is added to float {\tt Ez}.
				Then the next iteration step starts.
				If {\tt atom\_list\_} has finished {\tt Ez} * {\tt sigmaE} ,which is another constant ,
				is substracted from the actual proton´s chemical shift.
				Then iteration goes on whith next proton of {\tt proton\_list\_}
		*/
		virtual bool finish()
			throw();

		/**	Processor start method
		*/
		virtual bool start()
			throw();

		/**	operator ().
				PDBAtoms are assigned to two different lists, named {\tt proton\_list\_}
				and {\tt atom\_list\_}. In {\tt proton\_list\_} every Hydrogen is stored except
				the ones called "H". That special Hydrogen is stored as well as PDBAtoms named
				"C", "N" and "O" in {\tt atom\_list\_}.
		*/
		virtual Processor::Result operator () (Composite& composite)
			throw();
		//@}

		/**	@name Accessors
		*/
		//@{
		/**	
		*/
		const String& getFilename() const
			throw();
			
		/**	
		*/
		void setFilename(const String& filename)
			throw();
		//@}
	
		protected:
	
		list<Atom*>					proton_list_;	
		list<Atom*>					effector_list_;
		list<Atom*>					atom_list_;
		String							ini_filename_;
		vector<Expression>  first_atom_expressions_;
		vector<Expression>  second_atom_expressions_;
		Parameters					parameters_;
		ParameterSection		parameter_section_;
 	};

} // namespace BALL

#endif // BALL_NMR_SHIFTMODULE_H
