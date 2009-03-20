// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: mesh.h,v 1.9.16.1 2007-03-25 21:26:09 oliver Exp $
//

#ifndef BALL_VIEW_PRIMITIV_MESH_H
#define BALL_VIEW_PRIMITIV_MESH_H

#ifndef BALL_MATHS_SURFACE_H
#	include <BALL/MATHS/surface.h>
#endif

#ifndef BALL_VIEW_KERNEL_GEOMETRICOBJECT_H
# include <BALL/VIEW/KERNEL/geometricObject.h>
#endif

#ifndef BALL_VIEW_DATATYPE_COLOREXTENSIONS_H
# include <BALL/VIEW/DATATYPE/colorExtensions.h>
#endif

namespace BALL
{
	namespace VIEW
	{           
		/** Mesh class.			
				An instance of Mesh represents an instance of the geometric representation "mesh".
				\par
				The class Mesh is derived from the classes GeometricObject
				and Surface. The data structures defining the mesh are implemented in
				the class Surface. Therefore one can use this mesh in the
				same fashion as Surface. \par
				The colors belonging to each vertex. If this list has *less* entries than
				there are vertices in this mesh, we take the first element to color the whole
				mesh. If it is *empty*, we use the color white.
				\ingroup ViewPrimitives
		*/
		class BALL_VIEW_EXPORT Mesh
			: public GeometricObject,
			  public Surface,
				public MultiColorExtension
		{
			public:

			BALL_CREATE(Mesh)

			/**	@name	Constructors
			*/	
			//@{

			/** Default Constructor.
			*/
			Mesh();

			/** Copy constructor 
			*/
			Mesh(const Mesh& mesh);

			//@}
			/** @name Destructors */
			//@{

			/** Destructor.
			*/
			virtual ~Mesh();

			/** Explicit default initialization.
					Calls GeometricObject::clear
			*/
			virtual void clear();

			//@}	
			/**	@name	Assignment methods
			*/
			//@{

			/** Assignment.
			*/
			void set(const Mesh& mesh);

			/** Assignment operator.
					Calls set.
			*/
			const Mesh& operator = (const Mesh& mesh);

			/** Swapping of mesh's.
					Swap the value of this mesh with the mesh <b> mesh</b>.
					\param       mesh the mesh being swapped with this mesh 
			*/
			void swap(Mesh& mesh);

			//@}
			/**	@name	debuggers and diagnostics
			*/
			//@{

			/** Internal state and consistency self-validation.
					Calls GeometricObject::isValid.
			*/
			virtual bool isValid() const;

			/** Internal value dump.
					Dump the current value of this mesh to 
					the output ostream <b> s</b> with dumping depth <b> depth</b>.
					Calls GeometricObject::dump.
					\param   s output stream where to output the value of this mesh
					\param   depth the dumping depth
			*/
			virtual void dump(std::ostream&  s = std::cout, Size depth = 0) const;

			///
			bool binaryWrite(const String& filename);

			///
			bool binaryRead(const String& filename);

			// Method to get all vertices from a geometric object
			virtual void getVertices(vector<Vector3>& vertices) const;

			//@}
		};
  
} } // namespaces

#endif // BALL_VIEW_PRIMITIV_MESH_H
