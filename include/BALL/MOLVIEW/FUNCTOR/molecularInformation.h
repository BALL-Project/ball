// $Id: molecularInformation.h,v 1.2 2000/10/05 22:32:14 oliver Exp $

#ifndef BALL_MOLVIEW_FUNCTOR_MOLECULARINFORMATION_H
#define BALL_MOLVIEW_FUNCTOR_MOLECULARINFORMATION_H

#ifndef BALL_COMMON_H
#	include <BALL/common.h>
#endif

#ifndef BALL_CONCEPT_COMPOSITE_H
#	include <BALL/CONCEPT/composite.h>
#endif

#ifndef BALL_KERNEL_ATOM_H
#	include <BALL/KERNEL/atom.h>
#endif

#ifndef BALL_KERNEL_CHAIN_H
#	include <BALL/KERNEL/chain.h>
#endif

#ifndef BALL_KERNEL_FRAGMENT_H
#	include <BALL/KERNEL/fragment.h>
#endif

#ifndef BALL_KERNEL_MOLECULE_H
#	include <BALL/KERNEL/molecule.h>
#endif

#ifndef BALL_KERNEL_PROTEIN_H
# include <BALL/KERNEL/protein.h>
#endif

#ifndef BALL_KERNEL_RESIDUE_H
#	include <BALL/KERNEL/residue.h>
#endif

#ifndef BALL_KERNEL_SYSTEM_H
# include <BALL/KERNEL/system.h>
#endif
        
#ifndef BALL_MOLVIEW_COMMON_COMMON_H
#	include <BALL/MOLVIEW/COMMON/common.h>
#endif

#ifndef BALL_VIEW_FUNCTOR_INFORMATION_H
#	include <BALL/VIEW/FUNCTOR/information.h>
#endif

using namespace BALL::VIEW;

namespace BALL
{

	namespace MOLVIEW
	{
		
		/**
		*/
		class MolecularInformation
			: public Information
		{
			public:
			
			/**	@name	Enums
			 */
			//@{
			///
			enum Type
			{
				///
				TYPE__UNKNOWN              = -1,
				///
				TYPE__SYSTEM               = 0,
				///
				TYPE__PROTEIN              = 1,
				///
				TYPE__MOLECULE             = 2,
				///
				TYPE__CHAIN                = 3,
				///
				TYPE__FRAGMENT             = 4,
				///
				TYPE__RESIDUE              = 5,
				///
				TYPE__SECONDARY_STRUCTURE  = 6,
				///
				TYPE__ATOM                 = 7
			};
			//@}

			/**	@name	Type definitions
			*/
			//@{
				
			//@}

			/**	@name	Constructors and Destructors
			*/
			//@{

			MolecularInformation();

			MolecularInformation
				(const MolecularInformation& info, bool deep = true);

			virtual ~MolecularInformation();

			virtual void clear();

			virtual void destroy();
			//@}

			/**	@name	Asignment
			*/
			//@{

			void set
				(const MolecularInformation& info, bool deep = true);

			MolecularInformation&  operator =
				(const MolecularInformation& info);

			void get
				(MolecularInformation&  info, bool deep = true) const;

			void swap
				(MolecularInformation&  info);
			//@}

			
			/**	@name	Accessors
			*/
			//@{
			Type getType() const;

			virtual void visit(Composite& composite);
			//@}
			

			/**	@name	Debugging and Diagnostics
			*/
			//@{

			virtual bool isValid() const;

			virtual void dump
				(std::ostream& s = std::cout, Size depth = 0) const;
			//@}

			protected:
			
  		private:

			void getType_(Composite& composite);
			void getTypeName_(Composite& composite);
			void getName_(Composite& composite);

			Type type_;
		};

			
#			ifndef BALL_NO_INLINE_FUNCTIONS
#				include <BALL/MOLVIEW/FUNCTOR/molecularInformation.iC>
#			endif

	} // namespace MOLVIEW

} // namespace BALL

#endif // BALL_MOLVIEW_FUNCTOR_MOLECULARINFORMATION_H
