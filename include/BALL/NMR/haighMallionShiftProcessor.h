// $Id: haighMallionShiftProcessor.h,v 1.5 2000/09/26 19:26:46 oliver Exp $

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
		
	/**	Shift assignment processor implementing Haigh Mallion theory.
	*/
	class HaighMallionShiftProcessor
		:	public ShiftModule
	{
		public:

		BALL_CREATE(HaighMallionShiftProcessor)
		
    /** @name Enums and Constants
    */
    //@{

    /** A symbolic name for the ring current contribution to the chemical shift
        @see ShiftModule::PROPERTY__SHIFT
    */
    static const char* PROPERTY__RING_CURRENT_SHIFT;
    //@}
 
		/**@name	Constructors and Destructors
		*/
		//@{

		/**	Default constructor.
		*/
		HaighMallionShiftProcessor() throw();
		
		/**	Destructor
		*/
		virtual ~HaighMallionShiftProcessor() throw();
		
		//@}

		
		/**@name	Processor specific functions.
		*/
		//@{
				
		/**	Finish method.
				here the work is done :
				the funcion iterates over all systems's protons by iterating through _proton_list.
				then for each proton every ring in _aromat_list is accessed.
				the actual ring's ringplane and it's area is calculated. 
				
				Iterating over every bond of the aromatic ring, triangle areas are calculated with:
				the bond's two atoms and the actual proton's projection onto the ringplane.
				It is important to state that these areas are signed areas, depending on wether there is 
				a right handed system between the plane's normal vector and the difference vectors from the projection
				to the two bond's atoms.
				Next the protons distances to the actual two bond's atoms are calculated and stored in a_eins and a_zwei.
				Then ts is calculated : ts = f * (1/a_eins^3 + 1/a_zwei) with f is the actual tringle area.
				For every bond of the ring this partial sum is calculated , added and stored in gs.
				Just some constant factor B and the chemical shift using Haigh Mallions Model is done.
				Important note:
				Distances are expressed in terms of the actual ring's diameter.
				Triangle areas are expressed in terms of the actual ring's area.
		*/
		virtual bool finish() throw();

		/**	Application method
				works as a collector :
				it stores the systems aromatic rings in a list called _aromat_list and
				each proton in a list called _proton_list
		*/
		virtual Processor::Result operator () (Composite& atom) throw();
		//@}

		/**	@name	Accessors
		*/
		//@{

		/**	Initialization method
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

