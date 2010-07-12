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

				struct BondStruct
				{
					const Bond* bond;
					Handle atom_one_handle;
					Handle atom_two_handle;

					BondStruct(const Bond* bond_,Handle atom_one_handle_,Handle atom_two_handle_);

					BondStruct();
				};

				OpenSimTask(OpenSimTaskTypes type_, AtomContainer* container_,
								HashMap<Handle, const Atom*> handle_to_atom,
								HashMap<const Atom*, Handle> atom_to_handle,
								HashMap<Handle, BondStruct> handle_to_bond);

				OpenSimTask();

				OpenSimTaskTypes type;
				AtomContainer* container;
				
				HashMap<Handle, const Atom*> handle_to_atom_;
				HashMap<Handle, BondStruct> handle_to_bond_;
				
		};
	}
}

#endif
