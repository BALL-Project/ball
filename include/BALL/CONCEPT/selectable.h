// $Id: selectable.h,v 1.5 2000/02/17 00:30:38 oliver Exp $

#ifndef BALL_CONCEPT_SELECTABLE_H
#define BALL_CONCEPT_SELECTABLE_H

#ifndef BALL_COMMON_H
#	include <BALL/common.h>
#endif

#ifndef BALL_CONCEPT_PERSISTENTOBJECT_H
#	include <BALL/CONCEPT/persistentObject.h>
#endif

#define BALL_SELECTABLE_DEFAULT_SELECTION   false

namespace BALL 
{

	/** Selectable Concept.
			Selectable implements the ability of objects to be selected.
			It is used in the BALL kernel to mark parts of the kernel
			objects for special operations. In principle, Selectable
			provides a boolean flag and defines an interface to access
			this flag.
			{\bf Definition:} \URL{BALL/CONCEPT/selectable.h}
	*/
	class Selectable
	{
		public:

		/**	@name	Constructors and Destructors 
		*/
		//@{

		/**	Default Constructor.
				Creates a new selectable object and sets its state to unselected.
		*/
		Selectable();
			
		/**	Copy constructor.
				Creates a copy of selectable object.
				@param	selectable the Selectable object to be copied
				@param	deep ignored
		*/
		Selectable(const Selectable& selectable, bool deep = true);
			
		/**	Destructor.
				The destructor has no functionality.
		*/
		virtual ~Selectable();

		/**	Clear the selection flag.
				Clear resets the selection flag to unselected.
		*/
		void clear();
		
		/**	Clear the selection flag.
				This method does the same as the clear method.
		*/
		void destroy();

		//@}

		/**	@name	Assignment 
		*/
		//@{

		/**	Assign the contents of another selectable object to this object.
				@param selectable the object whose contents are to be copied
				@param deep ignored
		*/
		void set(const Selectable& selectable, bool deep = true);

		/**	Assignment operator.
				Assigns the contents of another Selectable object
				to this obejct.
				@param selectable the object to be copied
		*/
		Selectable& operator = (const Selectable& selectable);
		
		/**	Copy the contents od this object into another.
				@param selectable the object to be assigned to
				@param deep ignored
		*/
		void get(Selectable& selectable, bool deep = true) const;

		/**	Swap the contents of two objects.
				@param selectable the obejct to swap contents with
		*/
		void swap(Selectable& selectable);
		//@}

		/**	Accessors
		*/
		//@{
		
		/**	Select the object.
				The internal flag is set to {\bf true}.
		*/
		virtual void select();

		/**	Deselect the object.
				The internal flag is set to {\bf false}.
		*/
		virtual void deselect();
		//@}

		/**	@name Predicates 
		*/
		//@{

		/**	Get the object state.
				@return bool {\bf true}, if the object is selected, {\bf false} otherwise
		*/
		bool isSelected() const;

		//@}
	
		/**	@name Storers 
		*/
		//@{

		///
		friend ::std::ostream&	operator << (::std::ostream& s, const Selectable& selectable);

		///
		friend ::std::istream&	operator >> (::std::istream& s, Selectable& selectable);

		//@}


		/**	@name Storable Model
		*/
		//@{

		/**	Persistent stream writing.
				This method writes a boolean variable to the
				persistent stream using the {\tt writePrimitive} method
				of the PersistenceManager.
				@param pm the persistence manager
		*/
		void write(PersistenceManager& pm) const;

		/** Persistent stream reading.
				This method reads a boolean variable from the
				persistent stream using the {\tt readPrimitive} method
				of the PersistenceManager.
				@param pm the persistence manager
		*/
		bool read(PersistenceManager& pm);

		//@}

		/**	@name Debugging and Diagnostics 
		*/
		//@{
		
		///
		virtual void dump(::std::ostream& s = std::cout, Size depth = 0L) const;
		//@}
	
		protected:

		bool selected_;
	};

#	ifndef BALL_NO_INLINE_FUNCTIONS
#		include <BALL/CONCEPT/selectable.iC>
#	endif

} // namespace BALL

#endif // BALL_CONCEPT_SELECTABLE_H
