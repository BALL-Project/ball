// $Id: fragment.h,v 1.10 2000/08/30 19:58:08 oliver Exp $

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
			
		/**	@name	Constructors and Destructors */
		//@{

		/** Default constructor */
		Fragment();
	
		/** Copy constructor */
		Fragment(const Fragment& fragment, bool deep = true);
	
		/** Constructor */
		Fragment(const String& name);

		/** Destructor */
		virtual ~Fragment();

		//@}
	
		/** @name Persistence */
		//@{

		/**	Persistent writing.
				Writes a Fragment object to a persistent stream.
				@param pm the persistence manager
		*/
		void persistentWrite(PersistenceManager& pm, const char* name = 0) const;

		/**	Persistent reading.
				Reads a Fragment object from a persistent stream.
				@param pm the persistence manager
		*/
		void persistentRead(PersistenceManager& pm);

		//@}

		/**	@name	Assignment */
		//@{

		/** Assignment operator.
				Assign {\em fragment} to {\em *this} instance.
				@param   fragment the Fragment to be copied (cloned)
				@return  Fragment& - {\em *this} instance
		*/
		Fragment& operator = (const Fragment& fragment);

		//@}


		/**	@name	Debugging and Diagnostics */
		//@{

		/** Internal state dump.
				Dump the current internal state of {\em *this} Fragment to the output ostream 
				{\em s} with dumping depth {\em depth}.
				@param   s - output stream where to output the internal state of {\em *this} Fragment
				@param   depth - the dumping depth
		*/
		virtual void dump(std::ostream& s = std::cout, Size depth = 0) const;
		//@}


		/**	@name	Storers */
		//@{

		/* Persistent stream input and state restorage.
				Read persistent data from the input stream {\em s} and restore the state of {\em *this}.
				\\
				{\bf Note:} Not yet implemented.
				@param  s input stream from where to restore the internal state of {\em *this}
		*/
		virtual void read(std::istream& s);

		/* Persistent stream input and state restorage.
				Read persistent data from the input stream {\em s} and restore the state of {\em *this}.
				\\
				{\bf Note:} Not yet implemented.
				@param  s input stream from where to restore the internal state of {\em *this}
		*/
		virtual void write(std::ostream& s) const;
		//@}
	};

} // namespace BALL

#endif // BALL_KERNEL_FRAGMENT_H
