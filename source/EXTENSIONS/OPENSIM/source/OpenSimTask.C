#include <OpenSimTask.h>

namespace BALL
{
	namespace VIEW
	{
		OpenSimTask::BondStruct::BondStruct(const Bond* bond_,Handle atom_one_handle_,Handle atom_two_handle_)
		{
			bond = bond_;
			atom_one_handle_ = atom_one_handle;
			atom_two_handle_ = atom_two_handle;
		}

		OpenSimTask::BondStruct::BondStruct()
		{
		}

		OpenSimTask::OpenSimTask(OpenSimTaskTypes type_, AtomContainer* container_,
		                         HashMap<Handle, const Atom*> handle_to_atom,
														 HashMap<const Atom*, Handle> atom_to_handle,
														 HashMap<Handle, BondStruct> handle_to_bond)
		{
			type = type_;
			container = container_;
			handle_to_atom_ = handle_to_atom;
			handle_to_bond_ = handle_to_bond;
		}

		OpenSimTask::OpenSimTask()
		{
		}
	}
}
