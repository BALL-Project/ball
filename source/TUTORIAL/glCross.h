// $Id: glCross.h,v 1.1 2002/01/12 21:22:45 anker Exp $
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
