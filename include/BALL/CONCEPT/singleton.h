// $Id: singleton.h,v 1.4 2000/07/16 19:27:46 oliver Exp $

#ifndef BALL_CONCEPT_SINGLETON_H
#define BALL_CONCEPT_SINGLETON_H

#ifndef BALL_COMMON_H
#	include <BALL/common.h>
#endif

namespace BALL 
{

	
	/**	Generic Singleton Class.
			{\bf Definition:} \URL{BALL/CONCEPT/singleton.h}
	*/
	template <class T>
	class Singleton
	{
		public:

		/**	@name Constructors and Destructors
		*/
		//@{
		
		/**
		*/
		~Singleton()
		{
			--countReferences_();
		}
		//@}

		static Size countReferences();

		T* operator -> ();

		const T* operator -> () const;

		T& getExemplary();

		const T& getExemplary() const;
	
		operator T & ();

		operator T * ();

		protected:

		Singleton()
		{
			++countReferences_();
		}

		Singleton& operator = (const Singleton &);

		private:

		static Size &countReferences_();
	};

	template <class T>
	inline Size Singleton<T>::countReferences()
	{
		return countReferences_();
	}

	template <class T> 
	T* Singleton<T>::operator -> ()
	{
		static T* exemplary = 0;
		
		if (exemplary == 0)
		{
			exemplary = new T;
		}

		return exemplary;
	}

	template <class T>
	inline const T*  Singleton<T>::operator -> () const
	{
		return ((Singleton *)this)->operator->();
	}

	template <class T>
	inline T& Singleton<T>::getExemplary()
	{
		return *operator->();
	}

	template <class T> 
	inline const T& Singleton<T>::getExemplary() const
	{
		return ((Singleton *)this)->operator->();
	}
		
	template <class T>
	inline Singleton<T>::operator T& ()
	{
		return *operator->();
	}

	template <class T>
	inline Singleton<T>::operator T * ()
	{
		return operator->();
	}

	template <class T>
	Size& Singleton<T>::countReferences_()
	{
		static Size references = 0;
		
		return references;
	}

} // namespace BALL

#endif // BALL_CONCEPT_SINGLETON_H
