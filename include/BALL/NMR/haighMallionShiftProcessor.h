// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: haighMallionShiftProcessor.h,v 1.16.10.6 2007-04-12 13:53:54 anne Exp $
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
		 *  This method reads the parameter section "HaighMallionRingCurrent" 
		 *  and "RingCurrentTargets" and parses its content. The effector residue names are stored in 
		 *  effector_names_, the corresponding intensity factors are stored in intensity_factors_, and 
		 *  the effectors ring atoms are stored in ring_atoms_. 
		 *  The ring current target atom names and factors are stored in target_names_ and  
		 *  target_nucleus_factors_.
		 *  Furthermore the flags H_influenced_by_all_effectors_ (default false)
		 *    										HA_influenced_by_all_effectors_ (default false)
		 *    										use_cut_off_ (default false)
		 *    										cut_off_ (default 0.)
		 *    										project_target_to_ring_plane_  (default false) 
		 *    										all_hydrogens_are_targets_ (default  false)
		 *    										default_hydrogen_target_nucleus_factor_  (default 5.13)
		 *    										negative_ringcurrent_factor_  (default 1.1) are read. 
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
		
		
		/**	Application method.
				Works as a collector:	It stores the systems aromatic rings in 
				a vector of vectors effectors_ and each target (H, N or C) in 
				a vector called targets_.
		*/
		virtual Processor::Result operator () (Composite& atom);
		
			
		/**	Finish method.
				If no targets or effectors of the ring current effect were found during the
				application of the {\tt operator()}, no ring current shift effects are computed.
				Otherwise for all target effector pairs (e t) the current ring shift contribution is
				computed as 
				
				 shift = intensity_factors_[effector_types_[e]] * target_nucleus_factor[t] * geometrical_factor;

				and added to the current shift of the target atom t stored in the named property
				\link PROPERTY__RING_CURRENT_SHIFT PROPERTY__RING_CURRENT_SHIFT \endlink and
				the named property  \link ShiftModule::PROPERTY__SHIFT ShiftModule::PROPERTY__SHIFT \endlink. 
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

		/*_	A flag indicating whether a cutoff for the ring current effect is used. 
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
		
		/*_	A flag indicating whether all hydrogens are targets. 
		 		Set this flag by specifying the option {\tt	all_hydrogen_are_targets = true} in 
				the HaighMallionRingCurrent section of the parameter file.
				Default is false.
		*/
		bool										all_hydrogen_are_targets_;

		/** A flag indicating whether CA shifts should be corrected by the HA shift values.
		 
				Default is false.
			*/
		bool                    correct_CA_shifts_;

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

		/*_ The default factor of negative ring current factors
		 */
		float 									negative_ringcurrent_factor_ ;
		
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

		void 			printParameters_();
		void  		printEffectors_();
		void 			printTargets_();
		void 			postprocessing_();
		
	};
  
} // namespace BALL

