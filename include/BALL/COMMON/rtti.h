// $Id: rtti.h,v 1.15 2001/06/24 14:13:39 oliver Exp $

#ifndef BALL_COMMON_RTTI_H
#define BALL_COMMON_RTTI_H

#ifndef BALL_CONFIG_CONFIG_H
#	include <BALL/CONFIG/config.h>
#endif


#include <string>
#include <typeinfo>
using ::std::string;

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
			blanks in the name (as returned by RTTI::getName()) with underscores ("_").
			In the case of {\tt gcc}, however a name demangling decodes the string first.
			This function is needed for object persistence.
			@param	t the {\tt type_info} structure as returned by {\tt typeid}
			@return	string the coverted class name
	*/ 
	string streamClassName(const ::std::type_info& t);

	/**	@name Simplified RunTime Type Identification.
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
	//@{

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
				Use this method to provide an easy factory for objects of a certain class.
				The main use of this function lies in object persistence. The \Ref{PersistenceManager}
				needs a function for the dynamic creation of objects.
		*/
		template <typename T>
		void* getNew()
		{
			return static_cast<void*>(new T);
		}

		/**	Return the name of the class.
				This method returns the name of the class as given by {\tt typeid(<class instance>.name())}.
				No additional name demangling and whitespace substitution are performed.
		*/
		template <typename T>
		const char* getName()
		{
			return typeid(getDefault<T>()).name();
		}

		/**	Return a void pointer that is unique for each class.
		*/
		template <typename T>
		void* getClassID()
		{
			static char dummy;
			return (void*)&dummy;
		}

		/**	Return the demangled class name.
				The class name is demangled (as far as possible) and in the
				resulting string blanks are substituted by underscores, so the
				name can be read from a stream as one string.
				The typical usage is something like
				\begin{verbatim}
					String class_name = RTTI::getStreamName<Residue>();
					...
				\end{verbatim}
		*/
		template <typename T>
		const char* getStreamName()
		{
			// define portable names for the portable
			// types (some platforms use Size, some unsigned int, 
			// SUN CC even unsigned  for the Size type)
			if ((typeid(T) == typeid(Size)) 
					|| (typeid(T) == typeid(Position))
					|| (typeid(T) == typeid(HashIndex))
					|| (typeid(T) == typeid(Property))
					|| (typeid(T) == typeid(Handle)))
			{
				return "BALL::Size";
			}
			if ((typeid(T) == typeid(Index))
					|| (typeid(T) == typeid(ErrorCode))
					|| (typeid(T) == typeid(Distance)))
			{
				return "BALL::Index";
			}
			if (typeid(T) == typeid(::std::string))
			{
				return "::std::string";
			}
			if (typeid(T) == typeid(PointerSizeInt))
			{
				return "BALL::PointerSizeInt";
			}
			if (typeid(T) == typeid(bool))
			{
				return "bool";
			}
			if (typeid(T) == typeid(float))
			{
				return "float";
			}
			if (typeid(T) == typeid(char))
			{
				return "char";
			}
			if (typeid(T) == typeid(unsigned char))
			{
				return "unsigned_char";
			}
			if (typeid(T) == typeid(double))
			{
				return "double";
			}
			static string s("");
			static bool is_set = false;

			if (!is_set)
			{
				is_set = true;
				s = streamClassName(typeid(getDefault<T>()));
			}

			return s.c_str();
		}

		/**	Return true if {\tt u} is a kind of T.
				If {\tt u} is an instance of a class derived from T,
				this predicate returns true:\\
				\begin{verbatim}
					Protein p;

					// return true, since Protein is derived from Molecule
					bool is_molecule = RTTI::isKindOf<Molecule>(p);
				\end{verbatim}
		*/
		template <typename T, typename U>
		bool isKindOf(const U&  u)
		{
			return (0 != dynamic_cast<const T*>(&u));
		}

		/**	Cast an object to a specialized class.
				{\bf Example:}\\
				\begin{verbatim}
					Composite* composite = ...;
					...
				
					// check whether the composite is also an atom
					if (RTTI::isKindOf<Atom>(composite))
					{
						// perform some atom specific actions
						Atom* atom = RTTI::castTo<Atom>(*composite);
						...
						atom->setCharge(0);
						...
					}
				\end{verbatim}
		*/
		template <typename T, typename U>
		T* castTo(const U& u)
		{
			return const_cast<T*>(dynamic_cast<const T*>(&u));
		}

		/**	Return {\bf true} if a given object is an instance of a given class.
				If {\tt u} is an instance of {\tt T}, this predicate returns {\bf true}.
				If {\tt u} is an instance of a class that is derived from {\tt T} or
				a base class of {\tt T}, it returns false.
		*/
		template <typename T, typename U>
		bool isInstanceOf(const U& u)
		{
			T		t;
			return (typeid(u) == typeid(t));
		}
	//@}

	} // namespace RTTI

} // namespace BALL

#endif // BALL_COMMON_RTTI_H
