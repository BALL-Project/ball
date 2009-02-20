// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: johnsonBoveyShiftProcessor.h,v 1.18 2005/12/23 17:01:56 amoll Exp $
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
	class BALL_EXPORT JohnsonBoveyShiftProcessor
		:	public ShiftModule
	{
		public:

    BALL_CREATE(JohnsonBoveyShiftProcessor)

		/**	@name	Type definitions
		*/
		//@{

		///
		struct BALL_EXPORT Ring
    {
			double	radius;
			double	intensity;
			Size	electrons;
			std::vector<String>	atom_names;

			bool operator == (const Ring& ring) const;
			bool operator != (const Ring& ring) const { return !(*this == ring);}
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
		JohnsonBoveyShiftProcessor();

		/**	Destructor
		*/
		virtual ~JohnsonBoveyShiftProcessor();

		//@}
		/** @name	Processor specific functions.
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
			
		/** Initialize all parameters required
		*/
		virtual void init();
			
		//@}

		protected:

		std::list<Atom*>	proton_list_;	
		std::list<Atom*>	atom_list_;	
		std::list<Residue*>	aromat_list_;
		StringHashMap<Ring>	rings_;
		StringHashMap<Position>	residues_with_rings_;
		std::vector<Expression>	expressions_;
		
		double carlsonEllipticalIntegral1_(double, double, double);	// Carlson elliptical Integral of 1st kind
		double carlsonEllipticalIntegral2_(double, double, double);	// Carlson elliptical Integral of 2nd kind
		double legendreEllipticalIntegral1_(double, double);	// Legendre elliptical Integral of 1st kind
		double legendreEllipticalIntegral2_(double, double);	// Legendre elliptical Integral of 2nd kind
	};
  
} // namespace BALL 
