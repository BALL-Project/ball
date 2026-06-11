// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// BALL 2.0 — PropertyManager JSON helpers (K0.6.5b).
//

#include <BALL/KERNEL/propertyJson.h>
#include <BALL/CONCEPT/property.h>
#include <BALL/COMMON/logStream.h>
#include <BALL/DATATYPE/bitVector.h>
#include <BALL/EXTERNAL/nlohmann_json.hpp>

// v2.2 H4 commit 6.b: ContainerHandleBase + AtomHandle overloads need
// the handle headers + atom.h for the bridge dispatch.
#include <BALL/KERNEL/containerHandle.h>
#include <BALL/KERNEL/atomHandle.h>
#include <BALL/KERNEL/atomContainer.h>
#include <BALL/KERNEL/atom.h>
#include <BALL/KERNEL/moleculeStore.h>

#include <string>

namespace BALL
{
namespace detail
{

void properties_to_json(const PropertyManager& pm, void* out_json)
{
	using nlohmann::json;
	json& out = *static_cast<json*>(out_json);
	out = json::object();

	// --- Named properties -----------------------------------------------
	const Size n = pm.countNamedProperties();
	if (n > 0)
	{
		json named = json::array();
		for (Position i = 0; i < n; ++i)
		{
			const NamedProperty& np = pm.getNamedProperty(i);
			json entry;
			entry["name"] = np.getName();
			switch (np.getType())
			{
				case NamedProperty::BOOL:
					entry["type"]  = "BOOL";
					entry["value"] = np.getBool();
					break;
				case NamedProperty::INT:
					entry["type"]  = "INT";
					entry["value"] = np.getInt();
					break;
				case NamedProperty::UNSIGNED_INT:
					entry["type"]  = "UNSIGNED_INT";
					entry["value"] = np.getUnsignedInt();
					break;
				case NamedProperty::FLOAT:
					entry["type"]  = "FLOAT";
					entry["value"] = np.getFloat();
					break;
				case NamedProperty::DOUBLE:
					entry["type"]  = "DOUBLE";
					entry["value"] = np.getDouble();
					break;
				case NamedProperty::STRING:
					entry["type"]  = "STRING";
					entry["value"] = std::string(np.getString().c_str());
					break;
				case NamedProperty::OBJECT:
				case NamedProperty::SMART_OBJECT:
				case NamedProperty::NONE:
				default:
					// Non-scalar: skip the value; emit a marker so the key
					// survives but the pointer doesn't migrate (per header
					// doc). Log once-per-property so callers know to switch
					// to typed scalars if they want round-trip fidelity.
					entry["type"]  = "OBJECT";
					Log.warn() << "propertyJson: NamedProperty '" << np.getName()
						<< "' has non-serialisable type; emitting placeholder"
						<< std::endl;
					break;
			}
			named.push_back(std::move(entry));
		}
		out["named"] = std::move(named);
	}

	// --- Indexed (BitVector) properties --------------------------------
	// Emitted ONLY if any bit is set, to keep the common-case (no
	// indexed properties) JSON compact.
	const BitVector& bv = pm.getBitVector();
	const Size bv_size = bv.getSize();
	json bits = json::array();
	for (Index i = 0; i < static_cast<Index>(bv_size); ++i)
	{
		if (bv[i]) bits.push_back(static_cast<int>(i));
	}
	if (!bits.empty())
	{
		json indexed;
		indexed["size"] = bv_size;
		indexed["bits"] = std::move(bits);
		out["indexed"]  = std::move(indexed);
	}
}

void json_to_properties(PropertyManager& pm, const void* in_json)
{
	using nlohmann::json;
	const json& in = *static_cast<const json*>(in_json);

	// Wipe existing state on the destination PropertyManager so a load
	// into a pre-populated object doesn't union the two property sets.
	// CRITICAL: use non-virtual dispatch on PropertyManager::clear() —
	// the virtual override on Atom / Molecule / etc. wipes much more
	// than the property bag (Atom::clear nukes name/position/etc.).
	// We only want the named_properties_ + bit_vector_ reset.
	pm.PropertyManager::clear();

	if (!in.is_object()) return;   // empty or missing → nothing to restore

	if (in.contains("named") && in["named"].is_array())
	{
		for (const auto& entry : in["named"])
		{
			if (!entry.is_object() || !entry.contains("name") || !entry.contains("type"))
				continue;
			const std::string name = entry["name"].get<std::string>();
			const std::string type = entry["type"].get<std::string>();
			if      (type == "BOOL")
				pm.setProperty(name, entry["value"].get<bool>());
			else if (type == "INT")
				pm.setProperty(name, entry["value"].get<int>());
			else if (type == "UNSIGNED_INT")
				pm.setProperty(name, entry["value"].get<unsigned int>());
			else if (type == "FLOAT")
				pm.setProperty(name, entry["value"].get<float>());
			else if (type == "DOUBLE")
				pm.setProperty(name, entry["value"].get<double>());
			else if (type == "STRING")
				pm.setProperty(name, String(entry["value"].get<std::string>().c_str()));
			else if (type == "OBJECT")
				pm.setProperty(name);   // creates a NONE-typed entry (key survives)
			// Unknown type strings (future-minor) silently skipped.
		}
	}

	if (in.contains("indexed") && in["indexed"].is_object())
	{
		const json& idx = in["indexed"];
		if (idx.contains("bits") && idx["bits"].is_array())
		{
			for (const auto& bit : idx["bits"])
				pm.setProperty(static_cast<Property>(bit.get<int>()));
		}
	}
}

// v2.2 H4 commit 6.b (D-H4.6 R2): ContainerHandleBase + AtomHandle
// overloads. Implementation note: both forward to the v0
// PropertyManager& path via the dual-existence bridge (container_back_ptr
// for ContainerHandleBase, AtomHandle::getAtom() for AtomHandle). This
// guarantees byte-identical wire format with the v0 path -- the JSON
// round-trip test added in H4 commit 1 catches any drift. At H4 commit
// 12 the bridge is removed and the implementations are rewritten to
// read property_columns_ + sparse bag directly. The wire format
// remains unchanged.

void properties_to_json(const ContainerHandleBase& h, void* out_json)
{
	using nlohmann::json;
	json& out = *static_cast<json*>(out_json);
	out = json::object();
	if (!h.isValid()) return;
	AtomContainer* c = h.getStore()->container_back_ptr(h.getStoreIndex());
	if (c == nullptr) return;
	properties_to_json(static_cast<const PropertyManager&>(*c), out_json);
}

void json_to_properties(ContainerHandleBase& h, const void* in_json)
{
	if (!h.isValid()) return;
	AtomContainer* c = h.getStore()->container_back_ptr(h.getStoreIndex());
	if (c == nullptr) return;
	json_to_properties(static_cast<PropertyManager&>(*c), in_json);
}

void properties_to_json(const AtomHandle& h, void* out_json)
{
	using nlohmann::json;
	json& out = *static_cast<json*>(out_json);
	out = json::object();
	Atom* a = h.getAtom();
	if (a == nullptr) return;
	properties_to_json(static_cast<const PropertyManager&>(*a), out_json);
}

void json_to_properties(AtomHandle& h, const void* in_json)
{
	Atom* a = h.getAtom();
	if (a == nullptr) return;
	json_to_properties(static_cast<PropertyManager&>(*a), in_json);
}

} // namespace detail
} // namespace BALL
