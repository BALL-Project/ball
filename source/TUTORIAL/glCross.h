// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: glCross.h,v 1.1.2.1 2003/01/07 13:23:15 anker Exp $
// molview tutorial example
// ------------------------
// create a geometric primitive

// BALLVIEW includes
#include <BALL/VIEW/KERNEL/geometricObject.h>
#include <BALL/VIEW/KERNEL/colorExtension1.h>
#include <BALL/VIEW/KERNEL/radius.h>
#include <BALL/VIEW/KERNEL/vertex1.h>
#include <BALL/VIEW/GUI/glObject.h>

namespace BALL
{

	namespace VIEW
	{

		class GLCross: 
			public GeometricObject,
			public ColorExtension,
			public Radius,
			public Vertex,
			public GLObject
		{
			public:

				GLCross()
					throw();
				virtual ~GLCross()
					throw();

			protected:

				virtual bool draw(bool with_names = false)
					throw();
		};

	}

}
