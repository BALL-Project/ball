// $Id: POVRenderer.h,v 1.2 2002/12/12 09:33:55 oliver Exp $

#ifndef BALL_VIEW_GUI_FUNCTOR_POVRENDERER_H
#define BALL_VIEW_GUI_FUNCTOR_POVRENDERER_H

#ifndef BALL_VIEW_GUI_FUNCTOR_EXTERNALRENDERER_H
# include <BALL/VIEW/GUI/FUNCTOR/externalRenderer.h>
#endif

#ifndef BALL_SYSTEM_FILE_H
# include <BALL/SYSTEM/file.h>
#endif

namespace BALL
{
	namespace VIEW
	{
		/** POVRenderer class.
		 		This class walks over all the geometric primitives in a \Ref{Scene}
				and exports them into a data file in the POVRay 1.5 format, which can
				be used to render the same scene externally.
				{\bf Definition:} \URL{BALL/VIEW/GUI/FUNCTOR/POVRenderer.h}\\
			*/
		class POVRenderer : public ExternalRenderer
		{
			public:

			/** @name Constructors and Destructors.
			 */
			//@{

			/// Default constructor.
			POVRenderer()
				throw();

			/** Detailed constructor.
			 		@param name The name of the file we will create
			 */
			POVRenderer(const String& name)
				throw(Exception::FileNotFound);
			
			/// Destructor.
			virtual ~POVRenderer()
				throw();

			/// Clear method.
			virtual void clear()
				throw();

			/// Destroy method.
			virtual void destroy()
				throw();
	
			//@}

			/** @name Accessors
			 */
			//@{

			/** Sets the name of the file we will create.
			 		@param name The file name
			 */
			void setFileName(const String& name)
				throw(Exception::FileNotFound);

			/** Converts a \Ref{ColorRGBA} into a \Ref{String} in POVRay format.
			 */
			String POVColorRGBA(const ColorRGBA& input)
				throw();

			/** Converts a \Ref{Vector3} into a \Ref{String} in POVRay format.
			 */
			String POVVector3(const Vector3& input)
				throw();

			//@}
			
			/** @name Processor specific methods
			 */
			//@{
			/** Start method. 
			    This method creates the file and writes the header.
			 */
			virtual bool start()
				throw();

			/** Finish method.
			 		This method writes the ending of the file and closes it.
			 */
			virtual bool finish()
				throw();

			/** Operator ().
			 		Traverses the \Ref{Composite} tree with the start \Ref{Composite} {\em composite}
					and searches for \Ref{GeometricObject} objects that are also of kind \Ref{GLObject}.
					The data contained in the \Ref{GeometricObject} is then written into the output file
					according to its type.
					@param composite the \Ref{Composite} to be searched for \Ref{GLObject} objects
					@return Processor::Result the result of {\em *this} ExternalRenderer
			 */
			virtual Processor::Result operator() (Composite& composite)
				throw();
			//@}

			protected:
				
				File outfile_;

		};

	} // namespace BALL
} // namespace VIEW


#endif // BALL_VIEW_GUI_FUNCTOR_POVRENDERER_H
