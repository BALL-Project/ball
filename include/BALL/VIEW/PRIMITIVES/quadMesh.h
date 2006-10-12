// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: quadMesh.h,v 1.1.2.2 2006/10/12 20:00:06 amoll Exp $
//

#ifndef BALL_VIEW_PRIMITIV_QUADMESH_H
#define BALL_VIEW_PRIMITIV_QUADMESH_H

#ifndef BALL_VIEW_KERNEL_GEOMETRICOBJECT_H
# include <BALL/VIEW/KERNEL/geometricObject.h>
#endif

namespace BALL
{
	namespace VIEW
	{           
		/** QuadMesh class.			
				An instance of QuadMesh represents an mesh with quadruples.
				A mesh has the following properties. 
				  - color - the color of the mesh
				\par
				\ingroup ViewPrimitives
		*/
		class BALL_VIEW_EXPORT QuadMesh
			: public GeometricObject
		{
			public:

			///
			struct Quadruple
			{
				///
				Quadruple(Position p1, Position p2, Position p3, Position p4) 
				{
					q1 = p1; q2 = p2; q3 = p3; q4 = p4;
				}

				///
				Position q1, q2, q3, q4;
			};

			BALL_CREATE(QuadMesh)

			/** Default Constructor.
					The properties of this mesh are set to:
  				  - color - to the color black
			*/
			QuadMesh()
				throw();

			/// Copy constructor 
			QuadMesh(const QuadMesh& mesh)
				throw();

			///
			~QuadMesh()
				throw() {}

			///
			vector<ColorRGBA>& getColors() { return colors;}
			
			/** The colors belonging to each vertex. If this list has *less* entries than
					there are vertices in this mesh, we take the first element to color the whole
					mesh. If it is *empty*, we use the color white.
			 */
			vector<Vector3>		vertex;
			vector<Vector3>		normal;
			vector<Quadruple>	quad;
			vector<ColorRGBA> colors;
		};
  
} } // namespaces

#endif // BALL_VIEW_PRIMITIV_MESH_H
