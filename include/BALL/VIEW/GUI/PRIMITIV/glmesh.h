// $Id: glmesh.h,v 1.5 2001/02/11 13:04:39 hekl Exp $

#ifndef BALL_VIEW_GUI_PRIMITIV_GLMESH_H
#define BALL_VIEW_GUI_PRIMITIV_GLMESH_H

#ifndef BALL_COMMON_H
#	include <BALL/common.h>
#endif

#ifndef BALL_VIEW_GUI_KERNEL_GLOBJECT_H
#	include <BALL/VIEW/GUI/KERNEL/glObject.h>
#endif

#ifndef BALL_VIEW_PRIMITIV_MESH_H
#	include <BALL/VIEW/PRIMITIV/mesh.h>
#endif


namespace BALL
{
	
	namespace VIEW
	{

		/** GLMesh class.
				
				{\bf Framework:} BALL/VIEW/GUI/PRIMITIV\\
				{\bf Definition:} \URL{BALL/VIEW/GUI/PRIMITIV/glmesh.h}
				\\

				An instance of GLMesh represents an instance of the geometric
				visualization "mesh".
				The class GLMesh is derived from the classes \Ref{Mesh} and
				\Ref{GLObject}. Therefore the class glMesh is the graphical extension
				of the geometrical definition of the class mesh.
				The drawing method from \Ref{GLObject} is overridden to visualize the
				mesh. OpenGL code is used for the visualization.
				See these classes for further information.

				@memo    GLMesh class (BALL VIEW gui primitiv framework)
				@author  $Author: hekl $
				@version $Revision: 1.5 $
				@date    $Date: 2001/02/11 13:04:39 $
		*/
		class GLMesh
			: public Mesh,
				public GLObject
		{
			public:

			/**	@name	Constructors
			*/	
			//@{

			/** Default Constructor.
					Construct new glMesh.

					@return      GlMesh - new constructed glMesh
					@see         Mesh::Mesh
					@see         GLObject::GLObject
			*/
			GLMesh();

			/** Copy constructor with cloning facility.
					Construct new glMesh by copying the glMesh {\em mesh}.
					The copy is either deep (default) or shallow.

					@param       mesh the glMesh to be copied (cloned)
					@param       deep make a deep (={\tt true}) or shallow (={\tt false})
					@return      GlMesh - new constructed glMesh copied from {\em mesh}
					@see         Mesh::Mesh
					@see         GLObject::GLObject
			*/
			GLMesh(const GLMesh& mesh, bool deep = true);

			/** Copy constructor from geometricObject.
					Construct new glMesh by copying the internal values from geometricObject 
					{\em geometric_object}.

					@param       geometric_object the geometricObject which internal value should be copied
					@return      GlMesh - new constructed glMesh initialized from {\em geometric_object}
					@see         Mesh::Mesh
					@see         GLObject::GLObject
			*/
			GLMesh(const GeometricObject& geometric_object);

			//@}

			/** @name Destructors */
			//@{

			/** Destructor.
					Default destruction of {\em *this} glMesh.
					Calls \Ref{GlMesh::destroy}.
					@see         GlMesh::destroy
			*/
			virtual ~GLMesh()
				throw();

			/** Explicit default initialization.
					Calls \Ref{Mesh::clear}
					Calls \Ref{GLObject::clear}

					@see  Mesh::clear
					@see  GLObject::clear
			*/
			virtual void clear()
				throw();

			/** Explicit destructor.
					Calls \Ref{Mesh::destroy}
					Calls \Ref{GLObject::destroy}

					@see  Mesh::destroy
					@see  GLObject::destroy
			*/
			virtual void destroy()
				throw();
			//@}


			protected:

			/** @name Graphical interface methods
			*/
			//@{
			
			/** Creation of the graphical representation.
					Overridden method from \Ref{GLObject}. Uses the geometrical properties
					defined in class \Ref{Mesh} to create the graphical representation
					of the mesh. This method is used internally from the render engine
					of the \Ref{Scene}.
					This method draws the mesh of the surface defined by \Ref{Surface} as
					triangles. OpenGL triangle strips are used for the implementation.
					The parameter {\em with_names} indicates whether the openGL command 
					{\em glLoadName} must be used for naming the graphical object 
					(necessary for picking mode in the scene).
					
					@param     with_names flag if the graphical objects must have a name
					@return    bool - {\tt true} if successful,	{\tt false} otherwise
					@see       GLObject::draw
			*/
			virtual bool draw(bool with_names = false);

			/** Experimental method.
					Please avoid using this method.
			*/
			virtual bool drawUserDefined();

			/** Export method.
					This method handles the export of {\em *this} glMesh into another
					format (eg. POVRAY, VRML)
				  \\
				  {\bf Note:} Not yet implemented.

					@return    bool - {\tt true} if successful,	{\tt false} otherwise
			*/
			virtual bool extract()
				throw();

			//@}
		};

#		ifndef BALL_NO_INLINE_FUNCTIONS
#			include <BALL/VIEW/GUI/PRIMITIV/glmesh.iC>
#		endif
		
	} // namespace VIEW

} // namespace BALL

#endif // BALL_VIEW_GUI_PRIMITIV_GLMESH_H
