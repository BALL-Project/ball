// $Id: rtti.h,v 1.3 1999/12/30 20:30:32 oliver Exp $

#ifndef BALL_COMMON_RTTI_H
#define BALL_COMMON_RTTI_H

#ifndef BALL_CONFIG_CONFIG_H
#	include <BALL/CONFIG/config.h>
#endif


#include <string>
#include <typeinfo>
using std::string;

namespace BALL 
{


#	ifdef	__GNUC__
	// EGCS produces a nearly unreadable name mangling that requires 
	// further interpretation
	namespace GNUDemangling 
	{
		string demangle(string s);
	}
#	endif

	/**	Returns a unique name for a class.
			This name contains no blanks. It is usually derived by substituting all
			blanks in the name (as returned by RTTI::getName()) with underscores ("\_").
			In the case of {\tt egcs}, however a name demangling decodes the string first.
			This function is needed for object persistence.
			@param		type_info t the {\tt type\_info} structure as returned by {\tt typeid}
			@return		string the coverted class name
	*/ 
	string streamClassName(const std::type_info& t);

	/**	Simplified RunTime Type Identification.
			ANSI C++ provides support for runtime type identification (RTTI). However, the support
			is very basic. The template functions of the RTTI namespace  provide a more 
			readable support for RTTI. It defines
			predicates such as \Ref{isKindOf} that simplify tests on the hereditary relationship of
			different objects.\\
			To use the RTTI template functions, parametrize it with the type you are interested in.
			For example, to find out whether a given molecule is a protein, the following code
			can be used:\\
\begin{verbatim}
	Molecule& m =...;
	...
	if (RTTI::isKindOf<Protein>(m)) 
	{
		// perform some protein specific operations
	} else {
		// this is only a molecule...
	}
\end{verbatim}
	*/
	namespace RTTI
	{

		/**	Return a reference to a static default instance of the corresponding class.
				This method is basically intended to provide a default object for certain operations
				that require an instance of a certain class without really using this instance.
				It is mainly used inside the RTTI class.
		*/
		template <typename T>
		const T& getDefault() 
		{
			static T t;
			return t;
		}

		/**	Return a void pointer to a new instance of the class.
				Use this method to provide an easy factoy for obejct of a certain class.
				The main use of this function lies in obejct persistence. The \Ref{PersistenceManager}
				needs a function for the dynamic creation of objects.
		*/
		template <typename T>
		void* getNew()
		{
			return (void*)new T;
		}

		/**	Return the name of the class.
				This method returns the name of the class as given by {\tt typeid(<class instance>.name())}.
				No additional name demangling and whitespace substitution are performed.
				@see	
		*/
		template <typename T>
		const char* getName()
		{
			return typeid(getDefault<T>()).name();
		}

		/**
		*/
		template <typename T>
		const char* getStreamName()
		{
			static string s("");
			static bool is_set = false;

			if (!is_set)
			{
				is_set = true;
				s = streamClassName(typeid(getDefault<T>()));
			}

			return s.c_str();
		}

		/**
		*/
		template <typename T, typename U>
		bool isBaseOf(const U& u) 
		{
			static T t;
			T*	pt	= &t;
			U* U_ptr = dynamic_cast<U*>(pt);
			return (U_ptr != (U*)(&t)) && (U_ptr != 0);
		}

		/**
		*/
		template <typename T, typename U>
		bool isKindOf(const U&  u)
		{
			return (0 != dynamic_cast<const T*>(&u));
		}

		/**
		*/
		template <typename T, typename U>
		T* castTo(const U& u)
		{
			return const_cast<T*>(dynamic_cast<const T*>(&u));
		}

		/**
		*/
		template <typename T, typename U>
		bool isInstanceOf(const U& u)
		{
			T		t;
			U* 	U_ptr = const_cast<U*>(&u);
			if ((dynamic_cast<T*>(U_ptr) != 0) && (dynamic_cast<U*>(&t) != 0))
			{
				return true;
			}
			
			return false;
		}

		/**
		*/
		template <typename T, typename U>
		bool isDerivedFrom(const U& u)
		{
			return (isKindOf<T>(u) && !isInstanceOf<T>(u));
		}

	} // namespace RTTI

} // namespace BALL

#endif // BALL_COMMON_RTTI_H
