// $Id: singleton.h,v 1.6 2001/05/29 16:34:12 anker Exp $

#ifndef BALL_CONCEPT_SINGLETON_H
#define BALL_CONCEPT_SINGLETON_H

#ifndef BALL_COMMON_H
#	include <BALL/common.h>
#endif

namespace BALL 
{

	/**	Generic Singleton Class.
			\\
			{\bf Definition:} \URL{BALL/CONCEPT/singleton.h}
	*/
	template <class T>
	class Singleton
	{
		public:

		/**	@name Constructors and Destructor
		*/
		//@{
		
		/** Destructor
		*/
		~Singleton()
			throw()
		{
			--countReferences_();
		}
		//@}

		/** @name Accessors
		*/
		//@{

		/** Count the references.
				@return the number of references
		*/
		static Size countReferences()
			throw();

		/**
		*/
		T* operator -> ()
			throw();

		/**
		*/
		const T* operator -> () const
			throw();

		/**
		*/
		T& getExemplary()
			throw();

		/**
		*/
		const T& getExemplary() const
			throw();

		/**
		*/
		operator T & ()
			throw();

		/**
		*/
		operator T * ()
			throw();


		protected:

		/*_
		*/
		Singleton()
			throw()
		{
			++countReferences_();
		}

		/*_
		*/
		const Singleton& operator = (const Singleton &)
			throw();


		private:

		/*_
		*/
		static Size &countReferences_()
			throw();
	};

	template <class T>
	inline Size Singleton<T>::countReferences()
		throw()
	{
		return countReferences_();
	}

	template <class T> 
	T* Singleton<T>::operator -> ()
		throw()
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
		throw()
	{
		return ((Singleton *)this)->operator->();
	}

	template <class T>
	inline T& Singleton<T>::getExemplary()
		throw()
	{
		return *operator->();
	}

	template <class T> 
	inline const T& Singleton<T>::getExemplary() const
		throw()
	{
		return ((Singleton *)this)->operator->();
	}
		
	template <class T>
	inline Singleton<T>::operator T& ()
		throw()
	{
		return *operator->();
	}

	template <class T>
	inline Singleton<T>::operator T * ()
		throw()
	{
		return operator->();
	}

	template <class T>
	Size& Singleton<T>::countReferences_()
		throw()
	{
		static Size references = 0;
		return references;
	}

} // namespace BALL

#endif // BALL_CONCEPT_SINGLETON_H
