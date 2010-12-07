// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//

#ifndef BALL_COMMON_CREATE_H
#define BALL_COMMON_CREATE_H

//@{

/**	Virtual construction macro.
		This macro is used to define the virtual <b>create</b> method.
		On inclusion of this macro in the public interface of a class,
		the virtual creation method becomes available. The create method's signature is as follows:
		<tt>virtual void* <b>create</b>(bool deep = true, bool empty = false) const</tt> \par
		The create method either creates an empty default object of the class (<tt>empty == <b>true</b></tt>)
		or a copy of the object. The copy is either deep (<tt>deep == <b>true</b></tt>) or shallow (<tt>deep == <b>false</b></tt>).
		By default, the create methods returns a pointer to a deep copy of the object.
		The use of the create method requires a (public) default constructor (when creating an empty copy)
		or a copy constructor. \par
		The macro also implements a static method <tt>createDefault</tt> that returns a void pointer to
		a new instance of <tt>name</tt>.
		
		@param	name the class name
  \ingroup Common
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
		This macro is used to define the virtual <b>create</b> method for classes that do
		not define a copy constructor taking a second argument (boolean, deep or shallow copy).
		On inclusion of this macro in the public interface of a class,
		the virtual creation method becomes available. The create method's signature is as follows:
		<tt>virtual void* <b>create</b>(bool deep = true, bool empty = false) const</tt> \par
		The create method either creates an empty default object of the class (<tt>empty == <b>true</b></tt>)
		or a copy of the object.
		The use of the create method requires a (public) default constructor (when creating an empty copy)
		and a copy constructor taking a reference to an object.
		The macro also implements a static method <tt>createDefault</tt> that returns a void pointer to
		a new instance of <tt>name</tt>.
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
		\verbatim
			virtual void* create(bool deep = true, bool empty = false) const;
			static void* createDefault();
		\endverbatim
*/
#define BALL_DEFINE_CREATE(name)\
\
	virtual void* create(bool deep = true, bool empty = false) const;\
	static void* createDefault();

//@}

#endif // BALL_COMMON_CREATE_H
