#ifndef BALL_EXTENSIONS_OPENSIM_OPENSIMTASK_H
#define BALL_EXTENSIONS_OPENSIM_OPENSIMTASK_H
#ifndef BALL_COMMON_GLOBAL_H
# include <BALL/COMMON/global.h>
#endif

#ifndef BALL_CONCEPT_OBJECT_H
# include <BALL/CONCEPT/object.h>
#endif

#ifndef BALL_DATATYPE_HASHMAP_H
# include <BALL/DATATYPE/hashMap.h>
#endif

#include <list>

namespace BALL
{
	class Atom;
	class AtomContainer;
	class Bond;

	namespace VIEW
	{
		class OpenSimTask
		{
			public:

				enum OpenSimTaskTypes
				{
					NEW_COMPOSITE,
					CHANGED_COMPOSITE_HIERARCHY,
					REMOVED_COMPOSITE,
					REPRESENTATION
				};

				OpenSimTaskTypes type;
				AtomContainer* container;
				
				// TODO: this should be all we need
				std::list<Atom*> affected_atoms;
				std::list<Bond*> affected_bonds;
		};
	}
}

#endif
