// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: mesh.h,v 1.5 2003/11/29 15:56:27 amoll Exp $
//

#ifndef BALL_VIEW_PRIMITIV_MESH_H
#define BALL_VIEW_PRIMITIV_MESH_H

#ifndef BALL_MATHS_SURFACE_H
#	include <BALL/MATHS/surface.h>
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
				  - color - the color of the mesh
				\par
				The class Mesh is derived from the classes GeometricObject
				and Surface. The data structures defining the mesh are implemented in
				the class Surface. Therefore one can use this mesh in the
				same fashion as Surface. \par
				\ingroup ViewPrimitives
		*/
		class Mesh
			: public GeometricObject,
			  public Surface
		{
			public:

			/**	@name	Constructors
			*/	
			//@{

			/** Default Constructor.
					The properties of this mesh are set to:
  				  - color - to the color black
			*/
			Mesh()
				throw();

			/** Copy constructor 
			*/
			Mesh(const Mesh& mesh)
				throw();

			//@}
			/** @name Destructors */
			//@{

			/** Destructor.
			*/
			virtual ~Mesh()
				throw();

			/** Explicit default initialization.
					Calls GeometricObject::clear
			*/
			virtual void clear()
				throw();

			//@}	
			/**	@name	Assignment methods
			*/
			//@{

			/** Assignment.
			*/
			void set(const Mesh& mesh)
				throw();

			/** Assignment operator.
					Calls set.
			*/
			const Mesh& operator = (const Mesh& mesh)
				throw();

			/** Swapping of mesh's.
					Swap the value of this mesh with the mesh <b> mesh</b>.
					\param       mesh the mesh being swapped with this mesh 
			*/
			void swap(Mesh& mesh)
				throw();

			//@}
			/**	@name	debuggers and diagnostics
			*/
			//@{

			/** Internal state and consistency self-validation.
					Calls GeometricObject::isValid.
			*/
			virtual bool isValid() const
				throw();

			/** Internal value dump.
					Dump the current value of this mesh to 
					the output ostream <b> s</b> with dumping depth <b> depth</b>.
					Calls GeometricObject::dump.
					\param   s output stream where to output the value of this mesh
					\param   depth the dumping depth
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

		};
  
} } // namespaces

#endif // BALL_VIEW_PRIMITIV_MESH_H
