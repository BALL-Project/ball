// $Id: fragment.h,v 1.2 1999/12/28 18:38:44 oliver Exp $

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
			groups, monomers in larger polymers).
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

		///
		Fragment();
	
		///
		Fragment(const Fragment& fragment, bool deep = true);
	
		///
		Fragment(const String& name);

		///
		virtual ~Fragment();

		///
		virtual void clear();
	
		///
		virtual void destroy();
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

		///
		Fragment& operator = (const Fragment& fragment);

		//@}


		/**	@name	Debugging and Diagnostics */
		//@{

		///
		virtual bool isValid() const;

		///
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
