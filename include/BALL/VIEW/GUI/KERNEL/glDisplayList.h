// $Id: glDisplayList.h,v 1.6 2001/07/16 14:49:30 amoll Exp $

#ifndef BALL_VIEW_GUI_KERNEL_GLDISPLAYLIST_H
#define BALL_VIEW_GUI_KERNEL_GLDISPLAYLIST_H

#ifndef BALL_COMMON_H
#	include <BALL/common.h>
#endif

#include <GL/gl.h>

namespace BALL
{

	namespace VIEW
	{

		/** GLDisplayList class.
				The class GLDisplayList is a container for graphical objects.
				Graphical objects can be drawn directly to the screen or the can be
				stored in a display list for faster drawing afterwards. This class
				provides the means for storing graphical objects in a own display list.
				This class is a wrapper class for the OpenGL display list mechanims.
				It defines some useful methods for easy creation of a display list and
				handles errors that can occur when using display lists. See the OpenGL
				defintion of display lists for further information concerning display lists.\\
				{\bf Definition:} \URL{BALL/VIEW/GUI/KERNEL/glDisplayList.h}
		*/
		class GLDisplayList
		{
			public:


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
					{\em DISPLAYLIST_NOT_DEFINED}.
			*/
			static const GLList DISPLAYLIST_NOT_DEFINED;
			//@}

			/**	@name	Exceptions
			*/
			//@{
			
			/** NestedDisplayList exception class.
					This exception will be thrown whenever a display list is tried to be
					defined inside another definition of a display list.
					@see         GeneralException			
			*/
			class NestedDisplayList:	public Exception::GeneralException
			{
				public:

				NestedDisplayList(const char* file, int line) 
					throw();
			};

			/** NoDisplayListAvailable exception class.
					This exception will be thrown when a display list should be internally
					allocated but there is no more memory available.
					@see         GeneralException			
			*/
			class NoDisplayListAvailable:	public Exception::GeneralException
			{
				public:

				NoDisplayListAvailable(const char* file, int line)
					throw();
			};

			/** DisplayListRedeclaration exception class.
					This exception will be thrown when a display list that is already defined
					is tried to be redefined without being destroyed before.
					@see         GeneralException			
			*/
			class DisplayListRedeclaration:	public Exception::GeneralException
			{
				public:

				DisplayListRedeclaration(const char* file, int line)
					throw();
			};
			//@}

			/**	@name	Constructors
			*/	
			//@{

			/** Default Constructor.
					Construct new glDisplayList.
					There is no copy constructor because the OpenGL display lists
					cannot be copied.
					The state of {\em *this} glDisplayList is set to:
					\begin{itemize}
					  \item display list empty
						\item use compile mode instead of compile and execute mode
					\end{itemize}
					@return      GLDisplayList new constructed glDisplayList
			*/
			GLDisplayList()
					throw();

			//@}

			/** @name Destructors 
			*/
			//@{

			/** Destructor.
					Default destruction of {\em *this} glDisplayList.
					Calls \Ref{destroy}.
					@see         destroy
			*/
			virtual ~GLDisplayList()
					throw();

			/** Explicit default initialization.
					Resets the mode of {\em *this} glDisplayList to compile.
			*/
			virtual void clear()
					throw();

			/** Explicit destructor.
					If {\em *this} has already a display list then that display list
					will be deleted.
					This method can be used for clearing {\em *this} glDisplayList before
					compiling other graphical objects into this display list.
			*/
			virtual void destroy()
					throw();
			//@}

			/**	@name	Accessors: inspectors and mutators 
			*/
			//@{

			/** Begin the display list.
					If graphical objects should be put into a display list they must be put
					between a {\em startDefinition} and {\em endDefinition} command.
					This method indicates the start of a display list. Every object drawn after
					this method will be compiled into {\em *this} glDisplayList.
					@exception   NestedDisplayList thrown whenever a nested display list definition is tried.
					@exception   NoDisplayListAvailable thrown whenever no memory for the display list is available.
					@exception   DisplayListRedeclaration thrown whenever {\em *this} glDisplayList is tried to be redefined before \Ref{destroy} is called.
					@see         endDefinition
			*/
			void startDefinition()
				throw(NestedDisplayList, NoDisplayListAvailable, DisplayListRedeclaration);

			/** End the display list.
					This method is the end command for a display list definition.
					@see         startDefinition
			*/
			void endDefinition()
				throw();

			/** Draw the display list.
					If this method is called the graphical representation of {\em *this}
					will be drawn. Precondition: {\em *this} glDisplayList has a graphical
					representation defined before by \Ref{startDefinition}
					and \Ref{endDefinition}.
					@see         startDefinition
					@see         endDefinition
			*/
			void draw()
				throw();

			/** Compile mode switch.
					Before the definition of the graphical objects between
					\Ref{startDefinition} and \Ref{endDefinition}
					the compile mode can be set.
					If this method is used {\em *this} glDisplayList will only compile
					the graphical representation of the used objects into a display list.
					@see         useCompileAndExecuteMode
					@see         startDefinition
					@see         endDefinition
			*/
			void useCompileMode()
				throw();

			/** Compile and Execute mode switch.
					Before the definition of the graphical objects between
					\Ref{startDefinition} and \Ref{endDefinition}
					the compile and execute mode can be set.
					If this method is used {\em *this} glDisplayList will compile the used
					objects into a display list and simultaneously execute their graphical
					representation.
					@see         useCompileMode
					@see         startDefinition
					@see         endDefinition
			*/
			void useCompileAndExecuteMode()
				throw();
			//@}

			/**	@name	Predicates
			*/
			//@{

			/** Compile mode test.
					Tests if {\em *this} glDisplayList is set to compile only.
					@return  bool {\tt true} if {\em *this} glDisplayList is set to compile only, {\tt false} otherwise
					@see         useCompileMode				
			*/
			bool isCompileMode() const
				throw();

			/** Compile and Execute mode test.
					Tests if {\em *this} glDisplayList is set to compile and execute.
					@return  bool {\tt true} if {\em *this} glDisplayList is set to compile and execute, {\tt false} otherwise
					@see         useCompileAndExecuteMode				
			*/
			bool isCompileAndExecuteMode() const
				throw();
			//@}

			/**	@name	debuggers and diagnostics
			*/
			//@{
			/** Internal state and consistency self-validation.
					Initiate self-validation of the internal state and data structure 
					consistencies	of {\em *this} glDisplayList.
					If the internal state of {\em *this} glDisplayList is correct
					(self-validated) and consistent {\tt true} is returned,
					{\tt false} otherwise. 
					{\em *this} glDisplayList is valid if a display list is already defined.
					@return			bool {\tt true} if the internal state of {\em *this} glDisplayList is correct (self-validated) and consistent,
					 						{\tt false} otherwise
			*/
			virtual bool isValid() const
				throw();

			/** Internal value dump.
					Dump the current state of {\em *this} glDisplayList to 
					the output ostream {\em s} with dumping depth {\em depth}.
					@param   s output stream where to output the state of {\em *this} glDisplayList
					@param   depth the dumping depth
			*/
			virtual void dump(std::ostream& s = std::cout, Size depth = 0) const
				throw();
			//@}

			/**	@name	Storers
			*/
			//@{

			/** Persistent stream output and state restorage.
  			  Read persistent glDisplayList data from the input stream {\em s} and 
				  restore the state of {\em *this}.\\
				  {\bf Note:} Not yet implemented.
				  @param       s input stream from where to restore the internal state of {\em *this} glDisplayList
			*/
			virtual void read(std::istream& s)
				throw();

			/** Persistent stream output and state storage.
  			  Write persistent glDisplayList data to the output stream {\em s} and 
				  store the state of {\em *this}.\\
				  {\bf Note:} Not yet implemented.
				  @param       s output stream to where to store the internal state of {\em *this} glDisplayList
			*/
			virtual void write(std::ostream& s) const
				throw();
			//@}

			
			private:

			/* compile switch */
			bool compile_;

			/* display list */
			GLList GL_list_;
		};

#		ifndef BALL_NO_INLINE_FUNCTIONS
#			include <BALL/VIEW/GUI/KERNEL/glDisplayList.iC>
#		endif

	} // namespace VIEW

} // namespace BALL
		

#endif // BALL_VIEW_GUI_KERNEL_GLDISPLAYLIST_H
