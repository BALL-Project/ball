// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: johnsonBoveyShiftProcessor.h,v 1.17 2004/11/18 19:34:14 oliver Exp $
//

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
	/**	Shift assignment processor implementing Johnson Bovey theory. 
	\ingroup ShiftModulesNMR		
	*/
	class JohnsonBoveyShiftProcessor
		:	public ShiftModule
	{
		public:

    BALL_CREATE(JohnsonBoveyShiftProcessor)

		/**	@name	Type definitions
		*/
		//@{

		///
		struct Ring
    {
			double							radius;
			double							intensity;
			Size								electrons;
			std::vector<String>	atom_names;
		} ;
  
		//@}
		
    /** @name Enums and Constants
    */
    //@{

    /** A symbolic name for the ring current contribution to the chemical shift
        @see ShiftModule::PROPERTY__SHIFT
    */
    static const char* PROPERTY__RING_CURRENT_SHIFT;		
    //@}


 		/** @name	Constructors and Destructors
		*/
		//@{

		/**	Default constructor.
		*/
		JohnsonBoveyShiftProcessor() throw();

		/**	Destructor
		*/
		virtual ~JohnsonBoveyShiftProcessor() throw();

		//@}
		/** @name	Processor specific functions.
		*/
		//@{

		/**	Start method.
		*/
		virtual bool start() throw();

		/**	Finish method.
		*/
		virtual bool finish() throw();

		/**	Application method
		*/
		virtual Processor::Result operator () (Composite& composite) throw();
		
		//@}
		/**	@name Accessors
		*/
		//@{
			
		/** Initialize all parameters required
		*/
		virtual void init() throw();
			
		//@}

		protected:

		std::list<Atom*>				proton_list_;	
		std::list<Atom*>				atom_list_;	
		std::list<Residue*>			aromat_list_;
		StringHashMap<Ring>			rings_;
		StringHashMap<Position>	residues_with_rings_;
		std::vector<Expression>	expressions_;
		
		double carlsonEllipticalIntegral1_(double, double, double);	// Carlson elliptical Integral of 1st kind
		double carlsonEllipticalIntegral2_(double, double, double);	// Carlson elliptical Integral of 2nd kind
		double legendreEllipticalIntegral1_(double, double);	// Legendre elliptical Integral of 1st kind
		double legendreEllipticalIntegral2_(double, double);	// Legendre elliptical Integral of 2nd kind
	};
  
} // namespace BALL 
