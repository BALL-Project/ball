// $Id: singleton.h,v 1.1 1999/08/26 07:53:17 oliver Exp $

#ifndef BALL_CONCEPT_SINGLETON_H
#define BALL_CONCEPT_SINGLETON_H

#ifndef BALL_COMMON_H
#	include <BALL/common.h>
#endif

namespace BALL {

	
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
		~Singleton(void)
		{
			--countReferences_();
		}
		//@}

		static Size countReferences(void);

		T* operator -> (void);

		const T* operator -> (void) const;

		T& getExemplary(void);

		const T& getExemplary(void) const;
	
		operator T & (void);

		operator T * (void);

		protected:

		Singleton(void)
		{
			++countReferences_();
		}

		Singleton &operator = (const Singleton &);

		private:

		static Size &countReferences_(void);
	};

	template <class T>
	inline Size Singleton<T>::countReferences(void)
	{
		return countReferences_();
	}

	template <class T> 
	T* Singleton<T>::operator -> (void)
	{
		static T* exemplary = 0;
		
		if (exemplary == 0)
		{
			exemplary = new T;
		}

		return exemplary;
	}

	template <class T>
	inline const T*  Singleton<T>::operator -> (void) const
	{
		return ((Singleton *)this)->operator->();
	}

	template <class T>
	inline T& Singleton<T>::getExemplary(void)
	{
		return *operator->();
	}

	template <class T> 
	inline const T& Singleton<T>::getExemplary(void) const
	{
		return ((Singleton *)this)->operator->();
	}
		
	template <class T>
	inline Singleton<T>::operator T& (void)
	{
		return *operator->();
	}

	template <class T>
	inline Singleton<T>::operator T * (void)
	{
		return operator->();
	}

	template <class T>
	Size& Singleton<T>::countReferences_(void)
	{
		static Size references = 0;
		
		return references;
	}

} // namespace BALL

#endif // BALL_CONCEPT_SINGLETON_H
