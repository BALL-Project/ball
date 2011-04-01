// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//

#ifndef BALL_KERNEL_FRAGMENT_H
#define BALL_KERNEL_FRAGMENT_H

#ifndef BALL_KERNEL_ATOMCONTAINER_H
#	include <BALL/KERNEL/atomContainer.h>
#endif

namespace BALL 
{
	/**	Fragment class.
			Fragments are used to represent parts of molecules (for example functional
			groups, monomers in larger polymers). \par
			
    	\ingroup KernelContainers 
	*/
	class BALL_EXPORT Fragment
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
		Fragment();
	
		/** Copy constructor 
		*/
		Fragment(const Fragment& fragment, bool deep = true);
	
		/** Constructor 
		*/
		Fragment(const String& name);

		/** Destructor 
		*/
		virtual ~Fragment();

		//@}
		/** @name Persistence 
		*/
		//@{

		/**	Writes a Fragment object to a persistent stream.
		 *	@param pm the persistence manager
		 */
		void persistentWrite(PersistenceManager& pm, const char* name = 0) const;

		/**	Reads a Fragment object from a persistent stream.
		 *	@param pm the persistence manager
		 */
		void persistentRead(PersistenceManager& pm);

		//@}
		/**	@name	Assignment 
		*/
		//@{

		/** Assignment operator.
				@param   fragment the Fragment to be copied (cloned)
				@return  Fragment& - this instance
		*/
		Fragment& operator = (const Fragment& fragment);

		//@}

		/**	Equality operator.
				Two fragments are equal if they have the same handle.
				@see Object::operator ==.
		*/
		bool operator == (const Fragment& fragment) const;

		/**	Inequality operator
				@see operator ==
		*/
		bool operator != (const Fragment& fragment) const;

		/** @name Type predicates */
		//@{
		/// @copydoc Composite::isFragment
		virtual bool isFragment() const { return true; }
		//@}

		/**	@name	Debugging and Diagnostics 
		*/
		//@{

		/** Internal state dump.
				Dump the current internal state to the output ostream 
				<b>  s </b> with dumping depth <b>  depth </b>.
				@param   s - output stream where to output the internal state
				@param   depth - the dumping depth
		*/
		virtual void dump(std::ostream& s = std::cout, Size depth = 0) const;

		//@}
	};

// required for visual studio
#ifdef BALL_COMPILER_MSVC
template class BALL_EXPORT UnaryProcessor<Fragment>;
#endif
} // namespace BALL

#endif // BALL_KERNEL_FRAGMENT_H
