// $Id: haighMallionShiftProcessor.h,v 1.7 2001/03/02 22:19:33 amoll Exp $

#ifndef BALL_COMMON_H
#	include <BALL/common.h>
#endif

#ifndef BALL_CONCEPT_PROCESSOR_H
#	include <BALL/CONCEPT/processor.h>
#endif

#ifndef BALL_NMR_SHIFT_MODULE_H
#	include<BALL/NMR/shiftModule.h>
#endif

#include <list>

namespace BALL 
{
		
	/**	Shift assignment processor implementing Haigh Mallion theory. \\
			{\bf Definition}\URL{BALL/NMR/haighMallionShiftProcessor.h}
	*/
	class HaighMallionShiftProcessor
		:	public ShiftModule
	{
		public:

		BALL_CREATE(HaighMallionShiftProcessor)
		
    /** @name Enums and Constants
    */
    //@{

    /** A symbolic name for the ring current contribution to the chemical shift.
        @see ShiftModule::PROPERTY__SHIFT
    */
    static const char* PROPERTY__RING_CURRENT_SHIFT;
    //@}
 
		/** @name	Constructors and Destructors
		*/
		//@{

		/**	Default constructor.
		*/
		HaighMallionShiftProcessor() throw();
		
		/**	Destructor.
		*/
		virtual ~HaighMallionShiftProcessor() throw();
		
		//@}
		/** @name	Processor specific functions.
		*/
		//@{
				
		/**	Finish method.
				Here the work is done:
				The funcion iterates over all systems's protons by iterating through proton_list_.
				Then for each proton every ring in aromat_list_ is accessed.
				The actual ring's ringplane and it's area is calculated. 
				Iterating over every bond of the aromatic ring, the triangle areas are calculated with:
				The bond's two atoms and the actual proton's projection onto the ringplane.
				It is important to state that these areas are signed areas, depending on wether there is 
				a right handed system between the plane's normal vector and the difference vectors 
				from the projection	to the two bond's atoms.
				Next the protons distances to the actual two bond's atoms are calculated and stored 
				in a_first and a_second.
				Then ts is calculated : $ ts = f * (1/a_{first}^3 + 1/a_{second}) $ with f as the actual tringle area.
				For every bond of the ring this partial sum is calculated , added and stored in gs.
				Just some constant factor B and the chemical shift using Haigh Mallions Model is done.
				Important note:
				Distances are expressed in terms of the actual ring's diameter.
				Triangle areas are expressed in terms of the actual ring's area.
		*/
		virtual bool finish() throw();

		/**	Application method
				Works as a collector:	It stores the systems aromatic rings in 
				a list called aromat_list_ and each proton in a list called proton_list_
		*/
		virtual Processor::Result operator () (Composite& atom) throw();

		//@}
		/**	@name	Accessors
		*/
		//@{

		/**	Initialization method.
		*/
		virtual void init()
			throw();

		//@}
		
		private:
		
		std::list<Atom*> proton_list_;	
		std::list<Residue*> aromat_list_;
		String **asrings_;

		static const float big_loop_radius_;
		static const float little_loop_radius_;
		static const float B_;
	};

} // namespace BALL

