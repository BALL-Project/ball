// $Id: fragment.h,v 1.5 2000/04/25 16:52:25 amoll Exp $

#ifndef BALL_KERNEL_FRAGMENT_H
#define BALL_KERNEL_FRAGMENT_H

#ifndef BALL_COMMON_H
#	include <BALL/common.h>
#endif

#ifndef BALL_KERNEL_BASEFRAGMENT_H
#	include <BALL/KERNEL/baseFragment.h>
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
		: public BaseFragment
	{
		public:

		BALL_CREATE(Fragment)

		
		/**	@name	Enums 
		*/
		//@{

		///
		enum Property
		{
			///
			NUMBER_OF_PROPERTIES = BaseFragment::NUMBER_OF_PROPERTIES
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

		/// Persistent writing
		void persistentWrite(PersistenceManager& pm, const char* name = 0) const;

		/// Persistent reading
		void persistentRead(PersistenceManager& pm);

		//@}

		/**	@name	Assignment */
		//@{

		/** Assignment operator.
				Assign the Fragment {\em fragment} to {\em *this} fragment.
				The assignment is either deep or shallow (default).
				@param   fragment the Fragment to be copied (cloned)
				@return  Fragment& - {\em *this} Fragment
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

		///
		virtual void read(std::istream& s);

		///
		virtual void write(std::ostream& s) const;
		//@}
	};

} // namespace BALL

#endif // BALL_KERNEL_FRAGMENT_H
