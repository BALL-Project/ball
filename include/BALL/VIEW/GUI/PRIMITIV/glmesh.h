// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: glmesh.h,v 1.12 2003/02/25 13:28:15 sturm Exp $

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
    /** \ingroup ViewGuiPrimitives
     *  @{
     */
		/** GLMesh class.
				An instance of GLMesh represents an instance of the geometric visualization "mesh".
				The class GLMesh is derived from the classes  \link Mesh Mesh \endlink  and
				 \link GLObject GLObject \endlink . Therefore the class glMesh is the graphical extension
				of the geometrical definition of the class mesh.
				The drawing method from  \link GLObject GLObject \endlink  is overridden to visualize the
				mesh. OpenGL code is used for the visualization.
				See these classes for further information.
				<b>Definition:</b> BALL/VIEW/GUI/PRIMITIV/glmesh.h
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
					@return      GLMesh new constructed glMesh
					@see         Mesh
					@see         GLObject
			*/
			GLMesh()
				throw();

			/** Copy constructor with cloning facility.
					Construct new glMesh by copying the glMesh {\em mesh}.
					The copy is either deep (default) or shallow.
					@param       mesh the glMesh to be copied (cloned)
					@param       deep make a deep (=<tt>true</tt>) or shallow (=<tt>false</tt>)
					@return      GLMesh new constructed glMesh copied from {\em mesh}
					@see         Mesh
					@see         GLObject
			*/
			GLMesh(const GLMesh& mesh, bool deep = true)
				throw();

			/** Copy constructor from geometricObject.
					Construct new glMesh by copying the internal values from geometricObject {\em geometric_object}.
					@param       geometric_object the geometricObject which internal value should be copied
					@return      GLMesh new constructed glMesh initialized from {\em geometric_object}
					@see         Mesh
					@see         GLObject
			*/
			GLMesh(const GeometricObject& geometric_object)
				throw();

			//@}
			/** @name Destructors 
			*/
			//@{

			/** Destructor.
					Default destruction of {\em *this} glMesh.
					Calls  \link destroy destroy \endlink .
					@see         destroy
			*/
			virtual ~GLMesh()
				throw();

			/** Explicit default initialization.
					Calls  \link Mesh::clear Mesh::clear \endlink 
					Calls  \link GLObject::clear GLObject::clear \endlink 
					@see  Mesh::clear
					@see  GLObject::clear
			*/
			virtual void clear()
				throw();

			/** Explicit destructor.
					Calls  \link Mesh::destroy Mesh::destroy \endlink 
					Calls  \link GLObject::destroy GLObject::destroy \endlink 
					@see  Mesh::destroy
					@see  GLObject::destroy
			*/
			virtual void destroy()
				throw();

			//@}
			/**	@name	Assignment methods
			*/
			//@{

			/** Assignment.
					Assign the glMesh {\em mesh} to {\em *this} glMesh.
					The copy is either deep (default) or shallow.
					The value of {\em *this} glMesh is initialized to the value of 
					the glMesh {\em mesh}. \par
					@param       mesh the glMesh to be copied
					@param       deep make a deep (=<tt>true</tt>) or shallow (=<tt>false</tt>) copy of {\em mesh}
					@see         Mesh
					@see         GLObject
			*/
			void set(const GLMesh& mesh, bool deep = true)
				throw();

			/** Assignment operator.
					Assign the glMesh {\em mesh} to {\em *this} glMesh.
					The copy is deep.
					Calls  \link set set \endlink .
					The value of {\em *this} glMesh is initialized to the value 
					of the glMesh {\em mesh}. \par
					@param       mesh the glMesh to be copied
					@return      GLMesh& constant reference {\em *this} glMesh
					@see         set
			*/
			const GLMesh& operator = (const GLMesh& mesh)
				throw();

			/** Copying with cloning facility.
					Copy {\em *this} glMesh to the glMesh {\em mesh}.
					The copy is either deep (default) or shallow.
					Calls  \link set set \endlink .
					The value of the glMesh {\em mesh} is initialized to the
					value of {\em *this} glMesh. \par
					@param       mesh the glMesh to be assigned to
					@param       deep make a deep (=<tt>true</tt>) or shallow (=<tt>false</tt>) copy of {\em mesh}
					@see         set
			*/
			void get(GLMesh& mesh, bool deep = true) const
				throw();

			/** Swapping of Mesh's.
					Swap the value of {\em *this} Mesh with the Mesh {\em mesh}.
					@param       mesh the Mesh being swapped with {\em *this} Mesh 
					@see         GLMesh
			*/
			void swap(GLMesh& mesh)
				throw();

			protected:

			//@}
			/** @name Graphical interface methods
			*/
			//@{
			
			/** Creation of the graphical representation.
					Overridden method from  \link GLObject GLObject \endlink . Uses the geometrical properties
					defined in class  \link Mesh Mesh \endlink  to create the graphical representation
					of the mesh. This method is used internally from the render engine
					of the  \link Scene Scene \endlink .
					This method draws the mesh of the surface defined by  \link Surface Surface \endlink  as
					triangles. OpenGL triangle strips are used for the implementation.
					The parameter {\em with_names} indicates whether the openGL command 
					{\em glLoadName} must be used for naming the graphical object 
					(necessary for picking mode in the scene).
					@param     with_names flag if the graphical objects must have a name
					@return    bool <tt>true</tt> if successful,	<tt>false</tt> otherwise
					@see       GLObject::draw
			*/
			virtual bool draw(bool with_names = false)
				throw();

			/** Export method.
					This method handles the export of {\em *this} glMesh into another
					format (eg. POVRAY, VRML) \par
				  <b>Note:</b> Not yet implemented.
					@return    bool <tt>true</tt> if successful,	<tt>false</tt> otherwise
			*/
			virtual bool extract()
				throw();

			//@}
		};
  /** @} */
	} // namespace VIEW
} // namespace BALL

#endif // BALL_VIEW_GUI_PRIMITIV_GLMESH_H
