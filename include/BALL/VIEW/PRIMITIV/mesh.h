// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: mesh.h,v 1.14 2003/02/21 16:08:05 anhi Exp $

#ifndef BALL_VIEW_PRIMITIV_MESH_H
#define BALL_VIEW_PRIMITIV_MESH_H

#ifndef BALL_COMMON_H
#	include <BALL/common.h>
#endif

#ifndef BALL_MATHS_SURFACE_H
#	include <BALL/MATHS/surface.h>
#endif

#ifndef BALL_VIEW_DATATYPE_COLOR_H
#	include <BALL/VIEW/DATATYPE/color.h>
#endif

#ifndef BALL_VIEW_KERNEL_COLOREXTENSION1_H
#	include <BALL/VIEW/KERNEL/colorExtension1.h>
#endif

#ifndef BALL_VIEW_KERNEL_GEOMETRICOBJECT_H
# include <BALL/VIEW/KERNEL/geometricObject.h>
#endif

namespace BALL
{
	
	namespace VIEW
	{

		/** Mesh class.			
				An instance of Mesh represents an instance of the geometric representation "mesh".
				A mesh has the following properties. 
				\begin{itemize}
				  \item color - the color of the mesh
				\end{itemize}
				The class Mesh is derived from the classes  \link GeometricObject GeometricObject \endlink ,  \link ColorExtension ColorExtension \endlink 
				and  \link Surface Surface \endlink . The data structures defining the mesh are implemented in
				the class  \link Surface Surface \endlink . Therefore one can use {\em *this} mesh in the
				same fashion as  \link Surface Surface \endlink .  \par
				<b>Definition:</b> BALL/VIEW/PRIMITIV/mesh.h
		*/
		class Mesh
			: public GeometricObject,
				public ColorExtension,
			  public Surface
		{
			public:

			/**	@name	Constructors
			*/	
			//@{

			/** Default Constructor.
					Construct new mesh.
					The properties of {\em *this} mesh are set to:
					\begin{itemize}
  				  \item color - to the color black
					\end{itemize}
					@return      Mesh new constructed mesh
					@see         GeometricObject
					@see         ColorExtension
					@see         Surface
			*/
			Mesh()
				throw();

			/** Copy constructor with cloning facility.
					Construct new mesh by copying the mesh {\em mesh}.
					The copy is either deep (default) or shallow.
					@param       mesh the mesh to be copied (cloned)
					@param       deep make a deep (=<tt>true</tt>) or shallow (=<tt>false</tt>)
					@return      Mesh new constructed mesh copied from {\em mesh}
					@see         GeometricObject
					@see         ColorExtension
					@see         Surface
			*/
			Mesh(const Mesh& mesh, bool deep = true)
				throw();

			/** Copy constructor from geometricObject.
					Construct new mesh by copying the internal values from geometricObject 
					{\em geometric_object}.
					@param       geometric_object the geometricObject which internal value should be copied
					@return      Mesh new constructed mesh initialized from {\em geometric_object}
					@see         GeometricObject
					@see         ColorExtension
					@see         Surface
			*/
			Mesh(const GeometricObject& geometric_object)
				throw();

			//@}
			/** @name Destructors */
			//@{

			/** Destructor.
					Default destruction of {\em *this} mesh.
					Calls  \link destroy destroy \endlink .
					@see         destroy
			*/
			virtual ~Mesh()
				throw();

			/** Explicit default initialization.
					Calls  \link GeometricObject::clear GeometricObject::clear \endlink 
					Calls  \link ColorExtension::clear ColorExtension::clear \endlink 
					@see  GeometricObject::clear
					@see  ColorExtension::clear
			*/
			virtual void clear()
				throw();

			/** Explicit destructor.
					Calls  \link GeometricObject::destroy GeometricObject::destroy \endlink 
					Calls  \link ColorExtension::destroy ColorExtension::destroy \endlink 
					@see  GeometricObject::destroy
					@see  ColorExtension::destroy
			*/
			virtual void destroy()
				throw();

			//@}	
			/**	@name	Assignment methods
			*/
			//@{

			/** Assignment.
					Assign the mesh {\em mesh} to {\em *this} mesh.
					The copy is either deep (default) or shallow.
					The value of {\em *this} mesh is initialized to the value of 
					the mesh {\em mesh}. \par
					@param       mesh the mesh to be copied
					@param       deep make a deep (=<tt>true</tt>) or shallow (=<tt>false</tt>) copy of {\em mesh}
					@see         Mesh
			*/
			void set(const Mesh& mesh, bool deep = true)
				throw();

			/** Assignment operator.
					Assign the mesh {\em mesh} to {\em *this} mesh.
					The copy is deep.
					Calls  \link set set \endlink .
					The value of {\em *this} mesh is initialized to the value 
					of the mesh {\em mesh}. \par
					@param       mesh the mesh to be copied
					@return      Mesh& {\em *this} mesh
					@see         set
			*/
			const Mesh& operator = (const Mesh& mesh)
				throw();

			/** Copying with cloning facility.
					Copy {\em *this} mesh to the mesh {\em mesh}.
					The copy is either deep (default) or shallow.
					Calls  \link set set \endlink .
					The value of the mesh {\em mesh} is initialized to the
					value of {\em *this} mesh. \par
					@param       mesh the mesh to be assigned to
					@param       deep make a deep (=<tt>true</tt>) or shallow (=<tt>false</tt>) copy of {\em mesh}
					@see         set
			*/
			void get(Mesh& mesh, bool deep = true) const
				throw();

			/** Swapping of mesh's.
					Swap the value of {\em *this} mesh with the mesh {\em mesh}.
					@param       mesh the mesh being swapped with {\em *this} mesh 
			*/
			void swap(Mesh& mesh)
				throw();

			//@}
			/**	@name	debuggers and diagnostics
			*/
			//@{

			/** Internal state and consistency self-validation.
					Initiate self-validation of the internal state and data structure consistencies
					of {\em *this} mesh.
					If the internal state of {\em *this} mesh is correct (self-validated) and 
					consistent <tt>true</tt> is returned, <tt>false</tt> otherwise. 
					Calls  \link GeometricObject::isValid GeometricObject::isValid \endlink .
					@return			bool <tt>true</tt> if the internal state of {\em *this} mesh is correct (self-validated) and consistent, <tt>false</tt> otherwise
					@see        GeometricObject::isValid
			*/
			virtual bool isValid() const
				throw();

			/** Internal value dump.
					Dump the current value of {\em *this} mesh to 
					the output ostream {\em s} with dumping depth {\em depth}.
					Calls  \link GeometricObject::dump GeometricObject::dump \endlink .
					Calls  \link ColorExtension::dump ColorExtension::dump \endlink .
					@param   s output stream where to output the value of {\em *this} mesh
					@param   depth the dumping depth
					@see     GeometricObject::dump
					@see     ColorExtension::dump
			*/
			virtual void dump(std::ostream&  s = std::cout, Size depth = 0) const
				throw();

			//@}
			/** @name Attributes
			 */
			//@{
			
			/** The colors belonging to each vertex. If this list has *less* entries than
					there are vertices in this mesh, we take the first element to color the whole
					mesh. If it is *empty*, we use the color white.
			 */
			vector<ColorRGBA> colorList;
			//@}

			protected:

			/** Export method.
					This method handles the export of {\em *this} mesh into another
					format (eg. POVRAY, VRML)	 \par
				  <b>Note:</b> Not yet implemented.
					@return    bool <tt>true</tt> if successful,	<tt>false</tt> otherwise
			*/
			virtual bool extract()
				throw();
		};

	} // namespace VIEW

} // namespace BALL

#endif // BALL_VIEW_PRIMITIV_MESH_H
