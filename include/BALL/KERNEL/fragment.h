// $Id: fragment.h,v 1.16 2001/07/15 16:14:22 oliver Exp $

#ifndef BALL_KERNEL_FRAGMENT_H
#define BALL_KERNEL_FRAGMENT_H

#ifndef BALL_COMMON_H
#	include <BALL/common.h>
#endif

#ifndef BALL_KERNEL_ATOMCONTAINER_H
#	include <BALL/KERNEL/atomContainer.h>
#endif

#ifndef BALL_KERNEL_FRAGMENTITERATOR_H
#	include <BALL/KERNEL/fragmentIterator.h>
#endif

namespace BALL 
{

	/**	Fragment class.
			Fragments are used to represent parts of molecules (for example functional
			groups, monomers in larger polymers).\\
			{\bf Definition:}\URL{BALL/KERNEL/fragment.h}
	*/
	class Fragment
		: public AtomContainer
	{
		public:

		BALL_CREATE_DEEP(Fragment)
		
		/**	@name	Enums 
		*/
		//@{

		///
		enum Property
		{
			///
			NUMBER_OF_PROPERTIES = AtomContainer::NUMBER_OF_PROPERTIES
		};

		//@}		
		/**	@name	Constructors and Destructors 
		*/
		//@{

		/** Default constructor 
		*/
		Fragment()
			throw();
	
		/** Copy constructor 
		*/
		Fragment(const Fragment& fragment, bool deep = true)
			throw();
	
		/** Constructor 
		*/
		Fragment(const String& name)
			throw();

		/** Destructor 
		*/
		virtual ~Fragment()
			throw();

		//@}
		/** @name Persistence 
		*/
		//@{

		/**	Writes a Fragment object to a persistent stream.
				@param pm the persistence manager
		*/
		void persistentWrite(PersistenceManager& pm, const char* name = 0) const
			throw(Exception::GeneralException);

		/**	Reads a Fragment object from a persistent stream.
				@param pm the persistence manager
		*/
		void persistentRead(PersistenceManager& pm)
			throw(Exception::GeneralException);

		//@}
		/**	@name	Assignment 
		*/
		//@{

		/** Assignment operator.
				@param   fragment the Fragment to be copied (cloned)
				@return  Fragment& - this instance
		*/
		const Fragment& operator = (const Fragment& fragment)
			throw();

		//@}

		/**	Equality operator.
				Two fragments are equal if they have the same handle.
				@see Object::operator ==.
		*/
		bool operator == (const Fragment& fragment) const
			throw();

		/**	Inequality operator
				@see operator ==
		*/
		bool operator != (const Fragment& fragment) const
			throw();

		/**	@name	Debugging and Diagnostics 
		*/
		//@{

		/** Internal state dump.
				Dump the current internal state to the output ostream 
				{\em s} with dumping depth {\em depth}.
				@param   s - output stream where to output the internal state
				@param   depth - the dumping depth
		*/
		virtual void dump(std::ostream& s = std::cout, Size depth = 0) const
			throw();

		//@}
	};

} // namespace BALL

#endif // BALL_KERNEL_FRAGMENT_H
