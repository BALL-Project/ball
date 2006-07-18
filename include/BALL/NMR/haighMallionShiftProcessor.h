// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: haighMallionShiftProcessor.h,v 1.16.10.3 2006/07/18 18:42:44 anne Exp $
//

#ifndef BALL_COMMON_H
#	include <BALL/common.h>
#endif

#ifndef BALL_CONCEPT_PROCESSOR_H
#	include <BALL/CONCEPT/processor.h>
#endif

#ifndef BALL_NMR_SHIFTMODULE_H
#	include<BALL/NMR/shiftModule.h>
#endif

#ifndef BALL_DATATYPE_STRINGHASHMAP_H
# include <BALL/DATATYPE/stringHashMap.h>
#endif

#include <list>

namespace BALL 
{
	/**	Shift assignment processor implementing Haigh Mallion theory. 
		\ingroup ShiftModulesNMR	
	*/
	class BALL_EXPORT HaighMallionShiftProcessor
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
		HaighMallionShiftProcessor();
		
		/**	Default copy constructor.
		*/
		HaighMallionShiftProcessor(const HaighMallionShiftProcessor& processor);

		/**	Destructor.
		*/
		virtual ~HaighMallionShiftProcessor();
		
		
		//@}
		/**	@name	Accessors
		*/
		//@{

		/**	Initialization method.
		*/
		virtual void init();

		//@}
		

		//@}
		/** @name	Processor specific functions.
		*/
		//@{
		
		/**	Processor start method.
				This method clears the targets and effectors lists.
				It fails if no parameters were assigned.
				@return bool, <b>false</b> if <tt>parameters_ == 0</tt>
		*/
		virtual bool start();
		
		
		/**	Application method
				Works as a collector:	It stores the systems aromatic rings in 
				a vector of vectors effectors_ and each target (H, N or C) in 
				a vector called targets_
		*/
		virtual Processor::Result operator () (Composite& atom);
		
			
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
		virtual bool finish();

		
		//@}
		

		protected:
		
		/*_	A flag indicating whether Hydrogens are effected by all effectors or 
		 		just by effectors of the same residue.
				Set this flag by specifying the option {\tt	H_influenced_by_all_effectors = true} in 
				the HaighMallionRingCurrent section of the parameter file.
				Default is false.
		*/
		bool										H_influenced_by_all_effectors_;
	
		/*_	A flag indicating whether HA Hydrogens are effected by all effectors or 
		 		just by effectors of the same residue.
				Set this flag by specifying the option {\tt	HA_influenced_by_all_effectors = true} in 
				the HaighMallionRingCurrent section of the parameter file.
				Default is false.
		*/
		bool										HA_influenced_by_all_effectors_;

		/*_	A fag indicating whether a cutoff for the ring current effect is used. 
		 		Set this flag by specifying the option {\tt		use_cut_off = true} in 
				the HaighMallionRingCurrent section of the parameter file.
				Default is false.
		*/
		bool										use_cut_off_;


		/*_	A cut off value for the ring current effect computation via Haigh Mallion.
		  	Set this flag by specifying the option {\tt		cut_off = 15.} in 
				the HaighMallionRingCurrent section of the parameter file.
				Default is 15.*15.
		*/
		float										cut_off2_;					
		
		/*_	A fag indicating whether all hydrogens are targets. 
		 		Set this flag by specifying the option {\tt	all_hydrogen_are_targets = true} in 
				the HaighMallionRingCurrent section of the parameter file.
				Default is false.
		*/
		bool										all_hydrogen_are_targets_;


		/*_ A flag indicating whether for the computation of the ring current effect
		 		the target has to be projected onto the effector ring plane or not.
		 		Set this flag by specifying the option {\tt	project_target_to_ring_plane = true} in 
				the HaighMallionRingCurrent section of the parameter file.
				Default is false.
		 */
		bool 										project_target_to_ring_plane_;

		/*_ The values of the target nucleus factors F.
		 */
		StringHashMap<float>		target_nucleus_factors_;

		/*_ The default value of the target nucleus factors F for hydrogen atoms.
		 */
		float										default_hydrogen_target_nucleus_factor_;

		/*_	The effector residues stored as a vector of atoms the collected by {\tt operator ()}.
		*/	
		std::vector< std::vector<Atom*> >			effectors_;
		
		/*_	The effector residue types stored as a vector of strings collected by {\tt operator ()}.
				This is necessary to allow, e.g., TRP1 and TRP2 as two different kinds of rings.
		*/	
		std::vector<String>								effector_types_;

		/*_	The effector names collected from the ini-file by {\tt init ()}.
		*/
		std::vector<BALL::String> 				effector_names_;
		
		/*_	The intensities of the effecting residues collected from the ini-file by {\tt init ()}.
		*/
		StringHashMap<float>							intensity_factors_;
		
		/*_	The ring atom names of the effecting residues collected from the ini-file by {\tt init ()}.
		*/
		std::vector< std::vector<BALL::String> > 		ring_atoms_;
		
		/*_	The targeted atom names collected by {\tt init ()}.
		*/
		std::vector< BALL::String>							target_names_;
	
		/*_	The targeted atoms collected by {\tt operator ()}.
		*/
		std::vector<Atom* >										targets_;
	
		private:
		void 			printParameters_() throw();
		void  		printEffectors_() throw();
		void 			printTargets_() throw();

	};
  
} // namespace BALL

