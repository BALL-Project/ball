// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: glDisplayList.h,v 1.8.16.1 2007/03/25 21:26:14 oliver Exp $

#ifndef BALL_VIEW_RENDERING_GLDISPLAYLIST_H
#define BALL_VIEW_RENDERING_GLDISPLAYLIST_H

#ifndef BALL_COMMON_H
#	include <BALL/common.h>
#endif

#include <QtOpenGL/qgl.h>

namespace BALL
{
	namespace VIEW
	{

/** GLDisplayList class.
		The class GLDisplayList is a container for graphical objects.
		Graphical objects can be drawn directly to the screen or they can be
		stored in a display list for faster drawing afterwards. This class
		provides the means for storing graphical objects in a own display list.
		This class is a wrapper class for the OpenGL display list mechanims.
		It defines some useful methods for easy creation of a display list and
		handles errors that can occur when using display lists. See the OpenGL
		defintion of display lists for further information concerning display lists.
		\ingroup ViewRendering
*/
class BALL_VIEW_EXPORT GLDisplayList
{
	public:

	BALL_CREATE(GLDisplayList)

	/**	@name	Type Definitions.
	*/
	//@{

	/** GLList definition.
			A display list is stored as a number. For convienence a type is declared
			that represents such a display list.
	*/
	typedef unsigned long GLList;

	//@}
	/** @name Predefined static values.
	*/
	//@{
	
	/** Empty display list.
			A predefined value for an empty display list is defined as
			<b> DISPLAYLIST_NOT_DEFINED</b>.
	*/
	static const GLList DISPLAYLIST_NOT_DEFINED;

	//@}
	/**	@name	Exceptions
	*/
	//@{
	
	/** NestedDisplayList exception class.
			This exception will be thrown whenever a display list is tried to be
			defined inside another definition of a display list.
			\see         GeneralException			
	*/
	class BALL_VIEW_EXPORT NestedDisplayList:	public Exception::GeneralException
	{
		public:

		NestedDisplayList(const char* file, int line) ;
	};

	/** NoDisplayListAvailable exception class.
			This exception will be thrown when a display list should be internally
			allocated but there is no more memory available.
			\see         GeneralException			
	*/
	class BALL_VIEW_EXPORT NoDisplayListAvailable:	public Exception::GeneralException
	{
		public:

		NoDisplayListAvailable(const char* file, int line);
	};

	/** DisplayListRedeclaration exception class.
			This exception will be thrown when a display list that is already defined
			is tried to be redefined without being destroyed before.
			\see         GeneralException			
	*/
	class BALL_VIEW_EXPORT DisplayListRedeclaration:	public Exception::GeneralException
	{
		public:

		DisplayListRedeclaration(const char* file, int line);
	};

	//@}
	/**	@name	Constructors
	*/	
	//@{

	/** Default Constructor.
			There is no copy constructor because the OpenGL display lists cannot be copied.
			The state of this glDisplayList is set to:
				- display list empty
				- use compile mode instead of compile and execute mode
			\par
	*/
	GLDisplayList();

	//@}
	/** @name Destructors 
	*/
	//@{

	/** Destructor.
			Calls clear()
	*/
	virtual ~GLDisplayList();

	/** Explicit default initialization.
			Resets the mode of this glDisplayList to compile.
	*/
	virtual void clear();

	//@}
	/**	@name	Accessors: inspectors and mutators 
	*/
	//@{

	/** Begin the display list.
			If graphical objects should be put into a display list they must be put
			between a <b>startDefinition</b> and <b>endDefinition</b> command.
			This method indicates the start of a display list. Every object drawn after
			this method will be compiled into this glDisplayList.
			\exception   BALL::VIEW::GLDisplayList::NestedDisplayList thrown whenever a nested display list definition is tried.
			\exception   BALL::VIEW::GLDisplayList::NoDisplayListAvailable thrown whenever no memory for the display list is available.
			\exception   BALL::VIEW::GLDisplayList::DisplayListRedeclaration thrown whenever this glDisplayList is tried to redefine before
									 destroy is called.
			\see         endDefinition
	*/
	void startDefinition();

	/** End the display list.
			This method is the end command for a display list definition.
			\see         startDefinition
	*/
	void endDefinition();

	/** Draw the display list.
			If this method is called the graphical representation of this
			will be drawn. Precondition: this glDisplayList has a graphical
			representation defined before by startDefinition
			and endDefinition.
			\see         startDefinition
			\see         endDefinition
	*/
	void draw();

	/** Compile mode switch.
			Before the definition of the graphical objects between
			startDefinition and endDefinition
			the compile mode can be set.
			If this method is used this glDisplayList will only compile
			the graphical representation of the used objects into a display list.
			\see         useCompileAndExecuteMode
			\see         startDefinition
			\see         endDefinition
	*/
	void useCompileMode();

	/** Compile and Execute mode switch.
			Before the definition of the graphical objects between
			startDefinition and endDefinition
			the compile and execute mode can be set.
			If this method is used this glDisplayList will compile the used
			objects into a display list and simultaneously execute their graphical
			representation.
			\see         useCompileMode
			\see         startDefinition
			\see         endDefinition
	*/
	void useCompileAndExecuteMode();

	//@}
	/**	@name	Predicates
	*/
	//@{

	/** Compile mode test.
			Tests if this glDisplayList is set to compile only.
			\see         useCompileMode				
	*/
	bool isCompileMode() const;

	/** Compile and Execute mode test.
			Tests if this glDisplayList is set to compile and execute.
			\see         useCompileAndExecuteMode				
	*/
	bool isCompileAndExecuteMode() const;

	//@}
	/**	@name	debuggers and diagnostics
	*/
	//@{
	/** Internal state and consistency self-validation.
			A glDisplayList is valid if a display list is already defined.
	*/
	virtual bool isValid() const
		{ return GL_list_ != 0; }

	/** Internal value dump.
			Dump the current state to 
			the output ostream <b> s</b> with dumping depth <b> depth</b>.
			\param   s output stream where to output the state of this glDisplayList
			\param   depth the dumping depth
	*/
	virtual void dump(std::ostream& s = std::cout, Size depth = 0) const;

	//@}

	private:

	/* compile switch */
	bool compile_;

	/* display list */
	GLList GL_list_;
};

#	ifndef BALL_NO_INLINE_FUNCTIONS
#		include <BALL/VIEW/RENDERING/glDisplayList.iC>
#	endif

	} // namespace VIEW
} // namespace BALL

#endif // BALL_VIEW_RENDERING_GLDISPLAYLIST_H
