// $Id: atomBondModelBaseProcessor.h,v 1.2 2000/12/12 16:16:51 oliver Exp $

#ifndef BALL_MOLVIEW_FUNCTOR_ATOMBONDMODELBASEPROCESSOR_H
#define BALL_MOLVIEW_FUNCTOR_ATOMBONDMODELBASEPROCESSOR_H

#ifndef BALL_COMMON_H
#	include <BALL/common.h>
#endif

#ifndef BALL_CONCEPT_COMPOSITE_H
#	include <BALL/CONCEPT/composite.h>
#endif

#ifndef BALL_DATATYPE_LIST_H
#	include <BALL/DATATYPE/list.h>
#endif

#ifndef BALL_DATATYPE_HASHSET_H
#	include <BALL/DATATYPE/hashSet.h>
#endif

#ifndef BALL_KERNEL_ATOM_H
#	include <BALL/KERNEL/atom.h>
#endif

#ifndef BALL_KERNEL_BOND_H
#	include <BALL/KERNEL/bond.h>
#endif

#ifndef BALL_KERNEL_FOREACH_H
#	include <BALL/KERNEL/forEach.h>
#endif

#ifndef BALL_MOLVIEW_FUNCTOR_BASEMODEL_H
#	include <BALL/MOLVIEW/FUNCTOR/baseModel.h>
#endif



namespace BALL
{

	namespace MOLVIEW
	{


		/**
		*/
		class AtomBondModelBaseProcessor
			:
			public BaseModelProcessor
		{
			public:

			/**	@name	 Constructors and Destructors
			*/
			//@{

			AtomBondModelBaseProcessor();

			AtomBondModelBaseProcessor
				(const AtomBondModelBaseProcessor& baseProcessor, bool deep = true);

			virtual ~AtomBondModelBaseProcessor()
				throw();

			virtual void clear()
				throw();

			virtual void destroy()
				throw();
			//@}

			
			/**	@name	Assignment
			*/
			//@{

			void set
				(const AtomBondModelBaseProcessor& processor, bool deep = true);

			AtomBondModelBaseProcessor& operator =
				(const AtomBondModelBaseProcessor& processor);

			void get
				(AtomBondModelBaseProcessor& processor, bool deep = true) const;

			void swap
				(AtomBondModelBaseProcessor& processor);
			//@}

			/**	@name	Accessors
			*/
			//@{
			virtual bool start();
			
			virtual bool finish();

			virtual Processor::Result operator() (Composite& composite);
			//@}

				
			/**	@name	Processor Related Methods
			*/
			//@{
			//@}
				
			/**	@name	Predicates
			*/
			//@{
			//@}

			/**	@name	Debugging and Diagnostics
			*/
			//@{
			virtual bool isValid() const;

			virtual void dump
				(std::ostream& s = std::cout, Size depth = 0) const
				throw();
			//@}

			/**	@name Storers
			*/
			//@{

			virtual void read(std::istream& s);

			virtual void write(std::ostream& s) const;
			//@}


			
			protected:

			void insertAtom_(Atom* atom);

			void clearUsedAtoms_();

			List<Atom*>& getAtomList_();

			HashSet<Atom*>& getAtomSet_();

			// builds bond primitives
			// PRECOND: used atoms are inserted in the operator(Composite &) method
			//          with insertAtom_(Atom*) 
			void buildBondModels_();

			private:

			List<Atom*> used_atoms_;
			HashSet<Atom*> hashed_atoms_;
		};

#			ifndef BALL_NO_INLINE_FUNCTIONS
#				include <BALL/MOLVIEW/FUNCTOR/atomBondModelBaseProcessor.iC>
#			endif

	} // namespace MOLVIEW

} // namespace BALL

#endif // BALL_MOLVIEW_FUNCTOR_ATOMBONDMODELBASEPROCESSOR_H 
