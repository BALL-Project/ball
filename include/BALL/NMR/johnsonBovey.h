// $Id: johnsonBovey.h,v 1.7 2000/09/16 07:43:16 oliver Exp $

#ifndef BALL_COMMON_H
#	include <BALL/common.h>
#endif

#ifndef BALL_KERNEL_EXPRESSION_H
#	include <BALL/KERNEL/expression.h>
#endif

#ifndef BALL_NMR_SHIFTMODULE_H
#	include <BALL/NMR/shiftModule.h>
#endif

#ifndef BALL_DATATYPE_STRINGHASHMAP_H
#	include <BALL/DATATYPE/stringHashMap.h>
#endif

#ifndef BALL_FORMAT_PARAMETERS_H
#	include <BALL/FORMAT/parameters.h>
#endif

#ifndef BALL_FORMAT_PARAMETERSECTION_H
#	include <BALL/FORMAT/parameterSection.h>
#endif

#include <list>

namespace BALL 
{
		
	/**@name	Johnson Bovey Model
	*/
	//@{		

	/**	Shift assignment processor implementing Johnson Bovey theory.
	*/
	class JohnsonBoveyShift
		:	public ShiftModule
	{
		public:

		/**	@name	Type definitions
		*/
		//@{
		typedef struct
    {
			Size						number_of_atoms;
			float						radius;
			Size						electrons;
			vector<String>	atom_names;
		} Ring;
  
		//@}
		
		/**@name	Constructors and Destructors
		*/
		//@{

		/**	Default constructor.
		*/
		JohnsonBoveyShift();

		/**	Destructor
		*/
		virtual ~JohnsonBoveyShift();

		//@}


		/**@name	Processor specific functions.
		*/
		//@{

		/**	Start method.
		*/
		virtual bool start();

		/**	Finish method.
		*/
		virtual bool finish();

		/**	Application method
		*/
		virtual Processor::Result operator () (Composite& composite);
		//@}

		/**	@name Accessors
		*/
		//@{
		/**
		*/
		const String& getFilename() const;

		/**
		*/
		void setFilename(const String& filename);
		//@}

		protected:

		std::list<PDBAtom*>			proton_list_;	
		std::list<PDBAtom*>			atom_list_;	
		std::list<Residue*>			aromat_list_;
		String**								asrings_;
		String									ini_filename_;
		Parameters							parameters_;
		ParameterSection				parameter_section_;
		StringHashMap<Ring>			rings_;
		StringHashMap<Position>	residues_with_rings_;
		vector<Expression>			expressions_;
		
		float carlsonEllipticalIntegral1_(float, float, float);	// Carlson elliptical Integral of 1st kind
		float carlsonEllipticalIntegral2_(float, float, float);	// Carlson elliptical Integral of 2nd kind
		float legendreEllipticalIntegral1_(float, float);	// Legendre elliptical Integral of 1st kind
		float legendreEllipticalIntegral2_(float, float);	// Legendre elliptical Integral of 2nd kind
	};

	//@}

} // namespace BALL 
