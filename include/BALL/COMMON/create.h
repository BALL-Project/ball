// $Id: create.h,v 1.4 2001/02/04 10:29:43 amoll Exp $

#ifndef BALL_COMMON_CREATE_H
#define BALL_COMMON_CREATE_H

/**	@name	Dynamic Creation and Virtual Construction of Objects
*/
//@{

/**	Virtual construction macro.
		This macro is used to define the virtual {\bf create} method.
		On inclusion of this macro in the public interface of a class,
		the virtual creation method becomes available. The create method's signature is as follows:
		{\tt virtual void* {\bf create}(bool deep = true, bool empty = false) const}\\
		The create method either creates an empty default object of the class ({\tt empty == {\bf true}})
		or a copy of the object. The copy is either deep ({\tt deep == {\bf true}}) or shallow ({\tt deep == {\bf false}}).
		By default, the create methods returns a pointer to a deep copy of the object.
		The use of the create method requires a (public) default constructor (when creating an empty copy)
		or a copy constructor.\\
		The macro also implements a static method {\tt createDefault} that returns a void pointer to
		a new instance of {\tt name}.
		
		@param	name the class name
*/
#define BALL_CREATE_DEEP(name)\
\
	virtual void* create(bool deep = true, bool empty = false) const\
	{\
		void* ptr;\
		if (empty == true)\
		{\
			ptr = (void*)new name;\
		}\
		else\
		{\
			ptr = (void*)new name(*this, deep);\
		}\
		\
		return ptr;\
	}\
	\
	static void* createDefault()\
	{\
		return static_cast<void*>(new name);\
	}

/**	Virtual construction macro.
		This macro is used to define the virtual {\bf create} method for classes that do
		not define a copy constructor taking a second argument (boolean, deep or shallow copy).
		On inclusion of this macro in the public interface of a class,
		the virtual creation method becomes available. The create method's signature is as follows:
		{\tt virtual void* {\bf create}(bool deep = true, bool empty = false) const}\\
		The create method either creates an empty default object of the class ({\tt empty == {\bf true}})
		or a copy of the object.
		The use of the create method requires a (public) default constructor (when creating an empty copy)
		and a copy constructor taking a reference to an object.
		The macro also implements a static method {\tt createDefault} that returns a void pointer to
		a new instance of {\tt name}.
		@param	name the class name
*/
#define BALL_CREATE(name)\
\
	virtual void* create(bool /* deep */ = true, bool empty = false) const\
	{\
		void* ptr;\
		if (empty == true)\
		{\
			ptr = (void*)new name;\
		}\
		else\
		{\
			ptr = (void*)new name(*this);\
		}\
		\
		return ptr;\
	}\
	\
	static void* createDefault()\
	{\
		return static_cast<void*>(new name);\
	}

/**	Virtual cloning method definition macro.
		If the create method has to be implemented by the user, this macro just defines 
		the create method and the createDefault method.
		The function signatures are:
		\begin{verbatim}
			virtual void* create(bool deep = true, bool empty = false) const;
			static void* createDefault();
		\end{verbatim}
*/
#define BALL_DEFINE_CREATE(name)\
\
	virtual void* create(bool deep = true, bool empty = false) const;\
	static void* createDefault();

//@}

#endif // BALL_COMMON_CREATE_H
