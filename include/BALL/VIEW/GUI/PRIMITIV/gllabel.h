// $Id: gllabel.h,v 1.7 2001/07/16 14:49:30 amoll Exp $

#ifndef BALL_VIEW_GUI_PRIMITIV_GLLABEL_H
#define BALL_VIEW_GUI_PRIMITIV_GLLABEL_H

#ifndef BALL_COMMON_H
#	include <BALL/common.h>
#endif

#ifndef BALL_VIEW_GUI_KERNEL_GLOBJECT_H
#	include <BALL/VIEW/GUI/KERNEL/glObject.h>
#endif

#ifndef BALL_VIEW_PRIMITIV_LABEL_H
#	include <BALL/VIEW/PRIMITIV/label.h>
#endif

#include <qpainter.h>
#include <qfont.h>
#include <qbitmap.h>
#include <qimage.h>

namespace BALL
{
	
	namespace VIEW
	{

		/** GLLabel class.
				An instance of GLLabel represents an instance of the geometric
				visualization "label".
				The class GLLabel is derived from the classes \Ref{Label} and
				\Ref{GLObject}. Therefore the class glLabel is the graphical extension
				of the geometrical definition of the class label.
				The drawing method from \Ref{GLObject} is overridden to visualize the
				label. OpenGL code is used for the visualization.
				See these classes for further information.
				{\bf Definition:} \URL{BALL/VIEW/GUI/PRIMITIV/gllabel.h}
		*/
		class GLLabel
			: public Label,
				public GLObject
		{
			public:

			/**	@name	Constructors
			*/	
			//@{

			/** Default Constructor.
					Construct new glLabel with text "unkown" and font "helvetica".
					@return      GLLabel new constructed glLabel
					@see         Label
					@see         GLObject
			*/
			GLLabel()
				throw();

			/** Copy constructor with cloning facility.
					Construct new glLabel by copying the glLabel {\em label}.
					The copy is either deep (default) or shallow.
					@param       label the glLabel to be copied (cloned)
					@param       deep make a deep (={\tt true}) or shallow (={\tt false})
					@return      GLLabel new constructed glLabel copied from {\em label}
					@see         Label
					@see         GLObject
			*/
			GLLabel(const GLLabel& label, bool deep = true)
				throw();

			/** Copy constructor from geometricObject.
					Construct new glLabel by copying the internal values from geometricObject 
					{\em geometric_object}. The text of {\em *this} label is set to
					"unkown" and the font is set to "helvetica".
					@param       geometric_object the geometricObject which internal value should be copied
					@return      GLLabel new constructed glLabel initialized from {\em geometric_object}
					@see         Label
					@see         GLObject
			*/
			GLLabel(const GeometricObject& geometric_object)
				throw();

			//@}

			/** @name Destructors 
			*/
			//@{

			/** Destructor.
					Default destruction of {\em *this} glLabel.
					Calls \Ref{destroy}.
					@see         destroy
			*/
			virtual ~GLLabel()
				throw();

			/** Explicit default initialization.
					Set the font of {\em *this} label to "helvetica".
					Calls \Ref{Label::clear}
					Calls \Ref{GLObject::clear}
					@see  Label::clear
					@see  GLObject::clear
			*/
			virtual void clear()
				throw();

			/** Explicit destructor.
					Calls \Ref{Label::destroy}
					Calls \Ref{GLObject::destroy}
					@see  Label::destroy
					@see  GLObject::destroy
			*/
			virtual void destroy()
				throw();
			//@}

			/**	@name	Assignment methods
			*/
			//@{

			/** Assignment.
					Assign the glLabel {\em label} to {\em *this} glLabel.
					The copy is either deep (default) or shallow.
					The value of {\em *this} glLabel is initialized to the value of 
					the glLabel {\em label}.\\
					@param       label the glLabel to be copied
					@param       deep make a deep (={\tt true}) or shallow (={\tt false}) copy of {\em label}
					@see         Label
					@see         GLObject
			*/
			void set(const GLLabel& label, bool deep = true)
				throw();

			/** Assignment operator.
					Assign the glLabel {\em label} to {\em *this} glLabel.
					The copy is deep.
					Calls \Ref{set}.
					The value of {\em *this} glLabel is initialized to the value 
					of the glLabel {\em label}.\\
					@param       label the glLabel to be copied
					@return      GLLabel& constant reference {\em *this} glLabel
					@see         set
			*/
			const GLLabel& operator = (const GLLabel& label)
				throw();

			/** Copying with cloning facility.
					Copy {\em *this} glLabel to the glLabel {\em label}.
					The copy is either deep (default) or shallow.
					Calls \Ref{set}.
					The value of the glLabel {\em label} is initialized to the
					value of {\em *this} glLabel.\\
					@param       label the glLabel to be assigned to
					@param       deep make a deep (={\tt true}) or shallow (={\tt false}) copy of {\em label}
					@see         set
			*/
			void get(GLLabel& label, bool deep = true) const
				throw();

			/** Swapping of Label's.
					Swap the value of {\em *this} Label with the Label {\em label}.
					@param       label the Label being swapped with {\em *this} Label 
					@see         GLLabel
			*/
			void swap(GLLabel& label)
				throw();
			//@}

			/**	@name	Inspectors, Mutators, Accessors
			*/
			//@{
			/** Change the name of the glLabel.
					Change the name of {\em *this} glLabel represented by the parameter {\em text}.
					Store the old text for internal purpose.
					Calls \Ref{Label::setText}.
					@param       text the new text of {\em *this} glLabel
					@see         Label::setText
			*/
			void setText(const String text)
				throw();

			/** Change the font of the glLabel.
					Change the font of {\em *this} glLabel represented by the parameter
					{\em font}. This method uses QFont from the qt - library.
					@param       font the new font of {\em *this} glLabel
					@see         getFont
			*/
			void setFont(const QFont& font)
				throw();

			/** Non-mutable inspection of the font of {\em *this} glLabel.
					Access the constant reference of the font of {\em *this} glLabel.
					There is no mutable inspection of the font because the glLabel
					must be informed of font changes. If the font changes the glLabel
					must be redrawn.
					@return      QFont& constant reference to the font of {\em *this} glLabel
					@see         setFont
			*/
			const QFont& getFont() const
				throw();

			//@}


			protected:

			/** @name Graphical interface methods
			*/
			//@{
			
			/** Creation of the graphical representation.
					Overridden method from \Ref{GLObject}. Uses the geometrical properties
					defined in class \Ref{Label} to create the graphical representation
					of the label. This method is used internally from the render engine
					of the \Ref{Scene}.
					If the render engine calls this method for the first time a bitmap
					representing the text of the glLabel will be created with the
					chosen font. If the font or the text of {\em *this} glLabel changes
					a new bitmap will be created.
					The parameter {\em with_names} indicates whether the openGL command 
					{\em glLoadName} must be used for naming the graphical object 
					(necessary for picking mode in the scene).
					@param     with_names flag if the graphical objects must have a name
					@return    bool {\tt true} if successful,	{\tt false} otherwise
					@see       GLObject::draw
			*/
			virtual bool draw(bool with_names = false)
				throw();

			/** Export method.
					This method handles the export of {\em *this} glLabel into another
					format (eg. POVRAY, VRML)\\
				  {\bf Note:} Not yet implemented.
					@return    bool {\tt true} if successful,	{\tt false} otherwise
			*/
			virtual bool extract()
				throw();

			//@}
  		
		  private:

			void generateBitmap_();

			String getOldText_() const;

			const QFont& getOldFont_() const;

			QFont actual_font_;
			QFont old_font_;

			String old_text_;

			int width_;
			int height_;

			GLubyte* text_array_;
		};

#		ifndef BALL_NO_INLINE_FUNCTIONS
#			include <BALL/VIEW/GUI/PRIMITIV/gllabel.iC>
#		endif
		
	} // namespace VIEW

} // namespace BALL

#endif // BALL_VIEW_GUI_PRIMITIV_GLLABEL_H
