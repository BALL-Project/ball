// $Id: composite.C,v 1.21 2000/08/28 15:50:29 oliver Exp $

#include <BALL/CONCEPT/composite.h>
#include <BALL/CONCEPT/persistenceManager.h>
#include <BALL/DATATYPE/listSort.mac>
#include <BALL/KERNEL/predicate.h>

using namespace std;

namespace BALL 
{

	UnaryPredicate<Composite> Composite::DEFAULT_UNARY_PREDICATE;
	UnaryProcessor<Composite> Composite::DEFAULT_PROCESSOR;

	// default ctor
	Composite::Composite()
		:	PersistentObject(),
			Object(),
			Selectable(),
			number_of_children_(0),
			parent_(0),
			previous_(0),
			next_(0),
			first_child_(0),
			last_child_(0),
			contains_selection_(false),
			number_of_selected_children_(0),
			number_of_children_containing_selection_(0),
			selection_stamp_(),
			modification_stamp_()
	{
		BALL_BIT_CLEAR_ALL(properties_);
	}

	// copy ctor
	Composite::Composite(const Composite& composite, bool deep)
		:	PersistentObject(composite),
			Object(composite),
			Selectable(composite),
			number_of_children_(0),
			parent_(0),
			previous_(0),
			next_(0),
			first_child_(0),
			last_child_(0),
			contains_selection_(false),
			number_of_selected_children_(0),
			number_of_children_containing_selection_(0),
			selection_stamp_(),
			modification_stamp_()
	{
		if (deep == true)
		{
			composite.clone(*this, Composite::DEFAULT_UNARY_PREDICATE);
		}
		properties_ = composite.properties_;
	}

	// destructor
	Composite::~Composite()
	{
		destroy();
	}

  void Composite::persistentWrite(PersistenceManager& pm, const char* name) const
  {
		pm.writeObjectHeader(this, name);
			pm.writeStorableObject(*(Selectable*)this, "Selectable");

			pm.writePrimitive(number_of_children_, "number_of_children_");
			pm.writeObjectPointer(parent_, "parent_");
			pm.writeObjectPointer(previous_, "previous_");
			pm.writeObjectPointer(next_, "next_");
			pm.writeObjectPointer(first_child_, "first_child_");
			pm.writeObjectPointer(last_child_, "last_child_");
			pm.writePrimitive(properties_, "properties_");
			pm.writePrimitive(contains_selection_, "contains_selection_");
			pm.writePrimitive(number_of_selected_children_, "number_of_selected_children_");
			pm.writePrimitive(number_of_children_containing_selection_, "number_of_children_containing_selection_");
			pm.writeStorableObject(selection_stamp_, "selection_stamp_");
			pm.writeStorableObject(modification_stamp_, "modification_stamp_");
		pm.writeObjectTrailer(name);
	}

  void Composite::persistentRead(PersistenceManager& pm)
  {
		pm.readStorableObject(*(Selectable*)this, "Selectable");

		pm.readPrimitive(number_of_children_, "number_of_children_");
		pm.readObjectPointer(parent_, "parent_");
		pm.readObjectPointer(previous_, "previous_");
		pm.readObjectPointer(next_, "next_");
		pm.readObjectPointer(first_child_, "first_child_");
		pm.readObjectPointer(last_child_, "last_child_");
		pm.readPrimitive(properties_, "properties_");
		pm.readPrimitive(contains_selection_, "contains_selection_");
		pm.readPrimitive(number_of_selected_children_, "number_of_selected_children_");
		pm.readPrimitive(number_of_children_containing_selection_, "number_of_children_containing_selection_");
		pm.readStorableObject(selection_stamp_, "selection_stamp_");
		pm.readStorableObject(modification_stamp_, "modification_stamp_");
	}
 

	Size Composite::getPathLength(const Composite& composite) const
	{
		// if composite equals *this - return 0
		if (&composite == this)
		{
			return 0;
		}

		// first, try to walk upwards from composite
		Size path_size = 1;
		Composite* composite_ptr = composite.parent_;
		for (; composite_ptr != 0; composite_ptr = composite_ptr->parent_, ++path_size)
		{
			if (composite_ptr == this)
			{	
				// if we found a path from composite to *this - return its length
				return path_size;
			}
		}
		
		// if this didn't help, try to walk upwards from *this
		for (composite_ptr = parent_, path_size = 1; composite_ptr != 0; composite_ptr = composite_ptr->parent_,  ++path_size)
		{
			if (composite_ptr == &composite)
			{
				// if we found a path to composite - return
				return path_size;
			}
		}
		
		// no path from *this to composite exists
		return INVALID_SIZE;
	}

	Size Composite::getDepth() const
	{
		// walk up to the root (if possible)
		Size size = 0;
		for (Composite* composite_ptr = parent_;
				 composite_ptr != 0; composite_ptr = composite_ptr->parent_)
		{
			++size;
		}

		return size;
	}

	Composite& Composite::getRoot()
	{
		// walk up to the root
		Composite* composite_ptr = this;
		for (; composite_ptr->parent_ != 0; composite_ptr = composite_ptr->parent_);

		return *composite_ptr;
	}

	Composite* Composite::getLowestCommonAncestor(Composite& composite)
	{
		Composite* composite_ptr = 0;

		// determine depth of node A
		const Size size_a = getDepth() + 1;
		Index index_a = 0;
		
		// create an array and store all nodes on the path to the root	
		Composite** composites_a = new Composite* [size_a];
		for (composite_ptr = this; composite_ptr != 0; composite_ptr = composite_ptr->parent_)
		{
			composites_a[index_a++] = composite_ptr;
		}

		// determine the depth of B
		const Size size_b = composite.getDepth() + 1;
		Index index_b = 0;

		// and store all ancestor nodes of B in an array
		Composite** composites_b = new Composite *[size_b];
		for (composite_ptr =& composite; composite_ptr != 0;composite_ptr = composite_ptr->parent_)
		{
			composites_b[index_b++] = composite_ptr;
		}

		// compare all nodes from A and B 
		// if A and B have any common ancestor, the last entry in each array is the root
		// from there we walk backwards in both arrays
		for (composite_ptr = 0, index_a = (Index)(size_a - 1), index_b = (Index)(size_b - 1);
				 index_a >= 0 && index_b >= 0; --index_a, --index_b)
		{
			// as long as both array entries are equal, decrase the indices
			if (composites_a[index_a] == composites_b[index_b])
			{
				composite_ptr = composites_a[index_a];
			} else {
				// if the entries differ, the last equal entry was the lowest
				// common ancestor - exit the loop
				break;
			}
		}

		delete [] composites_a;
		delete [] composites_b;

		return composite_ptr;
	} 

	Composite* Composite::getSibling(Index index)
	{
		Composite* composite_ptr = this;

		if (index < 0)
		{
			// walk "index" steps along the previous_ pointers
			composite_ptr = composite_ptr->previous_;

			for (; ++index < 0 && composite_ptr != 0; 
					 composite_ptr = composite_ptr->previous_);
		} else if (index > 0) {
			// walk "index" steps along the next_ pointers
			composite_ptr = composite_ptr->next_;

			for (; --index > 0 && composite_ptr != 0; 
					 composite_ptr = composite_ptr->next_);
		}

		return composite_ptr;
	}

	void* Composite::clone(Composite& root, UnaryPredicate<Composite>& predicate) const
	{
		// avoid self-cloning
		if (&root == this)	
		{
			return 0;
		}

		// remove old contents
		root.destroy();
		
		// copy the properties
		root.properties_ = properties_;

		// clone everything else
		clone_(*const_cast<Composite*>(this), root, predicate);

		// update the selection of the parent (if it exists)
		if (root.parent_ != 0)
		{
			if (root.containsSelection())
			{
				root.parent_->number_of_children_containing_selection_++;

				if (root.selected_)
				{
					root.parent_->number_of_selected_children_++;
				}

				root.updateSelection_();
			}
		}

		return &root;
	}

	void Composite::select()
	{
		select_(true);
	}

	void Composite::select_(bool update_parent)
	{
		if (!selected_)
		{
			// select all children
			Composite* child = first_child_;
			for (; child != 0; child = child->next_)
			{
				if (!child->selected_)
				{
					// select the child only if it is not already selected
					child->select_(false);
				}
			}
			
			// all children are selected, so this node is selected, too
			number_of_selected_children_ = number_of_children_;
			number_of_children_containing_selection_ = number_of_children_;
			selected_ = true;

			// update the time stamp
			selection_stamp_.stamp();

			// check whether we have to inform the parent
			if (update_parent && (parent_ != 0))
			{
				// we were not selected beforehand, so increase
				// the corresponding counters
				parent_->number_of_selected_children_++;
				if (!contains_selection_)
				{
					// increase the corresponding counter in the parent
					parent_->number_of_children_containing_selection_++;
				}
				contains_selection_ = true;

				// updateSelection updates the time stamp and propagates it upwards...
				parent_->updateSelection_();
			} 
			else 
			{
				contains_selection_ = true;
			}
		}
	}
	
	void Composite::deselect()
	{
		deselect_(true);
	}

	void Composite::deselect_(bool update_parent)
	{
		// if anything is selected, deselect everything below
		if (selected_ || (number_of_selected_children_ > 0)
				|| (number_of_children_containing_selection_ > 0))
		{
			Composite* child = first_child_;
			for (; child != 0; child = child->next_)
			{
				if (child->containsSelection())
				{
					child->deselect_(false);
				}
			}
			
			selected_ = false;
			number_of_selected_children_ = 0;
			number_of_children_containing_selection_ = 0;
			
			// update the time stamp
			selection_stamp_.stamp();

			// check whether we have to inform the parent
			if (update_parent && (parent_ != 0))
			{
				// we were selected beforehand, so decrease
				// the corresponding counters
				parent_->number_of_selected_children_--;
				if (contains_selection_)
				{
					// increase the corresponding counter in the parent
					parent_->number_of_children_containing_selection_--;
				}
				contains_selection_ = false;

				// updateSelection_ updates the time stamp as well
				// and propagates it upwards to the root
				parent_->updateSelection_();
			} 
			else 
			{
				contains_selection_ = false;
			}
		}
	}
	
	void Composite::updateSelection_()
	{
		// update the time stamp
		selection_stamp_.stamp();

		// and propagate the time stamp upwards to the root
		Composite* parent(parent_);
		while (parent != 0)
		{
			parent->selection_stamp_.stamp();
			parent = parent->parent_;
		}

		// calculate the new selection flags according 
		// to the current contents of the counters
		bool new_selected = (((number_of_selected_children_ == number_of_children_) && (number_of_children_ > 0))
												 || ((number_of_children_ == 0) && selected_));
		bool new_contains_selection = ((number_of_children_containing_selection_ > 0) || new_selected);
		
		// now check for transitions in the node's state
		if (((selected_ != new_selected) || (new_contains_selection != contains_selection_)) && (parent_ != 0))
		{
			// we have a transition in the selection state
			if (selected_ && !new_selected)
			{
				// this node got deselected
				if (contains_selection_ && !new_contains_selection)
				{
					// it does not even contain a selection any more
					parent_->number_of_selected_children_--;
					parent_->number_of_children_containing_selection_--;
				} 
				else 
				{
					// it still contains selected nodes
					parent_->number_of_selected_children_--;
				}
			} 
			else if (!selected_ && new_selected) 
			{
				// the node was deselected before and now is selected
				if (!contains_selection_)
				{
					// the node didn`t contain selected nodes before
					parent_->number_of_selected_children_++;
					parent_->number_of_children_containing_selection_++;
				} 
				else 
				{
					// the node did contain selected nodes before
					parent_->number_of_selected_children_++;
				}
			} 
			else 
			{
				// selected didn`t change at all
				if (contains_selection_ && !new_contains_selection)
				{
					// there are no more selections left
					parent_->number_of_children_containing_selection_--;
				} 
				else 
				{
					// a child now contains a selection
					parent_->number_of_children_containing_selection_++;
				}
			}

			// store the new flags (have to be set prior to the recursive call 
			// to updateSelection())
			selected_ = new_selected;
			contains_selection_ = new_contains_selection;

			// something changed - 
			// walk up the tree as far as necessary
			parent_->updateSelection_();
		}

		// store the new flags (else case of outer if loop)
		selected_ = new_selected;
		contains_selection_ = new_contains_selection;		
	}

	void Composite::determineSelection_()
	{
		number_of_selected_children_ = 0;
		number_of_children_containing_selection_ = 0;
		
		Composite* composite_ptr = first_child_;
		for (; composite_ptr != 0; composite_ptr = composite_ptr->next_)
		{
			if (composite_ptr->contains_selection_)
			{
				++number_of_children_containing_selection_;
				if (composite_ptr->selected_)
				{
					++number_of_selected_children_;
				}
			}
		}
		
		// set the selction flags and propagate them
		// upwards (if necessary)
		updateSelection_();
	}

	void Composite::prependChild(Composite& composite)
	{
		// avoid self-prepend and prepend of children
		if (&composite == this || isDescendantOf(composite) == true)
		{
			return;
		}
		
		// if it is already the first child, abort
		if (&composite == first_child_)
		{
			return;
		}

		// remove it first, if it has a parent
		if (composite.parent_ != 0)
		{
			composite.parent_->removeChild(composite);
		}

		// insert the new child
		if (first_child_ == 0)
		{ 
			// if there is no child, it's easy
			first_child_ = last_child_ = &composite;
		} 
		else 
		{
			// otherwise insert it into the list
			first_child_->previous_ = &composite;
			composite.next_ = first_child_;
			first_child_ = &composite;
		}

		composite.parent_ = this;
		++number_of_children_;
	
		// update selection counters
		if (composite.containsSelection())
		{
			number_of_children_containing_selection_++;
			
			if (composite.selected_)
			{
				number_of_selected_children_++;
			}
			
			// recursively update the nodes` states
			updateSelection_();
		}
	}

	void Composite::appendChild(Composite& composite)
	{
		// avoid self-appending and appending of parent nodes
		if (&composite == this || isDescendantOf(composite) == true)
		{
			return;
		}
		
		// if it is already the last child, everything is done
		if (&composite == last_child_)
		{
			return;
		}
		
		// if composite has a parent, remove it from there
		if (composite.parent_ != 0)
		{
			composite.parent_->removeChild(composite);
		}

		// insert it
		if (last_child_ == 0)
		{
			// its the only child - easy!
			first_child_ = last_child_ = &composite;
		} 
		else 
		{
			// append it to the list of children
			last_child_->next_ = &composite;
			composite.previous_ = last_child_;
			last_child_ = &composite;
		}

		composite.parent_ = this;
		++number_of_children_;

		// update selection counters
		if (composite.containsSelection())
		{
			number_of_children_containing_selection_++;
			
			if (composite.selected_)
			{
				number_of_selected_children_++;
			}
			
			// recursively update the nodes` states
			updateSelection_();
		}
	}

	bool Composite::insertParent(Composite& parent, Composite& first, Composite& last, bool destroy_parent)
	{
		// return if first and last have different parents, 
		// if either doesn't possess a parent, or if parent equals first or parent
		if (first.parent_ != last.parent_ || first.parent_ == 0 || &first == &parent || &last == &parent)
		{
			return false;
		}

		// if first is already a child of parent, return immediately
		if (first.isDescendantOf(parent) == true)
		{
			return true;
		}

		// destroy the parent's contents
		parent.destroy(destroy_parent);

		// insert the new parent as a child into the
		// "first" node's parent
		Composite* parent_ptr = first.parent_;
		parent.parent_ = parent_ptr;
		parent.first_child_ = &first;
		parent.last_child_ = &last;
		
		if (parent_ptr->first_child_ == &first)
		{
			if (parent_ptr->last_child_ == &last)
			{
				parent_ptr->last_child_ = &parent;
			} 
			else 
			{
				last.next_->previous_ = &parent;
				parent.next_  = last.next_;
				last.next_ = 0;
			}

			parent_ptr->first_child_ = &parent;
		}
		else if (parent_ptr->last_child_ == &last)
		{
			first.previous_->next_ = &parent;
			parent.previous_  = first.previous_;
			first.previous_ = 0;
			
			parent_ptr->last_child_ = &parent;

		} 
		else 
		{

			first.previous_->next_ = &parent;
			parent.previous_ = first.previous_;
			last.next_->previous_ = &parent;
			parent.next_ = last.next_;
			first.previous_ = last.next_ = 0;
		}
				
		for (Composite* composite_ptr = &first; composite_ptr != &last; composite_ptr = composite_ptr->next_)
		{
			++(parent.number_of_children_);
			--(parent_ptr->number_of_children_);
			composite_ptr->parent_ = &parent;
		}

		last.parent_ = &parent;
		++(parent.number_of_children_);

		// update all selection fields recursively
		parent.determineSelection_();

		return true;
	}

	void Composite::insertBefore(Composite& composite)
	{
		// abort if a self-insertion is requested, there`s no parent at all,
		// or this node is a descendant of composite
		if (parent_ == 0 || &composite == this
				|| isDescendantOf(composite) == true)
		{
			return;
		}

		// if this node is the first child of its parent, this task 
		// reduces to a prependChild
		if (parent_->first_child_ == this)
		{
			parent_->prependChild(composite);

			return;
		}

		// if composite has a parent, remove it from there
		if (composite.parent_ != 0)
		{
			composite.parent_->removeChild(composite);
		}

		// open the linked list and insert composite
		// between this node and its left (previous) neighbour
		previous_->next_ = &composite;
		composite.previous_ = previous_;
		previous_ = &composite;
		composite.next_ = this;
		
		composite.parent_ = parent_;
		++(parent_->number_of_children_);
		
		// update selection counters
		if (composite.containsSelection())
		{
			++(parent_->number_of_children_containing_selection_);
			
			if (composite.selected_)
			{
				++(parent_->number_of_selected_children_);
			}
			
			parent_->updateSelection_();
		}
	}

	void Composite::insertAfter(Composite& composite)
	{
		// abort if there`s no parent, on slef-insertion, or
		// if this node is a descendant of composite
		if (parent_ == 0 || &composite == this
				|| isDescendantOf(composite) == true)
		{
			return;
		}

		// if this node is the last node in its parent`s list
		// the problem reduces to appendChild
		if (parent_->last_child_ == this)
		{
			parent_->appendChild(composite);

			return;
		}

		// if composite has a prent, remove it from there
		if (composite.parent_ != 0)
		{
			composite.parent_->removeChild(composite);
		}
		
		// insert composite in the children`s linked
		// list between this node and its successor (next_)
		next_->previous_ = &composite;
		composite.next_ = next_;
		next_ =& composite;
		composite.previous_ = this;
		
		composite.parent_ = parent_;
		++(parent_->number_of_children_);

		// update selection counters
		if (composite.containsSelection())
		{
			++(parent_->number_of_children_containing_selection_);
			
			if (composite.selected_)
			{
				++(parent_->number_of_selected_children_);
			}
			
			parent_->updateSelection_();
		}
	}

	void Composite::spliceBefore(Composite& composite)
	{
		// avoid self-splicing and the splicing of nodes that are
		// descendants of composite
		if (&composite == this || isDescendantOf(composite) == true)
		{
			return;
		}
		
		// if composite has a parent, remove it from there
		if (composite.parent_ != 0)
		{
			composite.parent_->removeChild(composite);
		}
		
		// assign the new parent to all children of composite
		for (Composite* composite_ptr =  composite.first_child_;
				 composite_ptr != 0; composite_ptr = composite_ptr->next_)
		{
			composite_ptr->parent_ = this;
		}

		// insert all children of composite into the 
		// double linked list of this composite
		if (composite.first_child_ != 0)
		{
			if (first_child_ != 0)
			{
				composite.last_child_->next_ = first_child_;
				first_child_->previous_  = composite.last_child_;
			} 
			else 
			{
				last_child_ = composite.last_child_;
			}
			
			first_child_ = composite.first_child_;
		}

		// update this composite
		number_of_children_ += composite.number_of_children_;
		number_of_selected_children_ += composite.number_of_selected_children_;
		number_of_children_containing_selection_ += composite.number_of_children_containing_selection_;
		updateSelection_();

		// update the (now empty) composite
		composite.number_of_children_ = 0;
		composite.first_child_  = composite.last_child_ = 0;
		composite.number_of_selected_children_ = 0;
		composite.number_of_children_containing_selection_ = 0;
		composite.contains_selection_ = composite.selected_;
	}

	void  Composite::spliceAfter(Composite& composite)
	{
		// avoid self-splicing and the splicing of nodes that are
		// descendants of composite
		if (&composite == this || isDescendantOf(composite) == true)
		{
			return;
		}
		
		// if composite has a parent, remove it from there
		if (composite.parent_ != 0)
		{
			composite.parent_->removeChild(composite);
		}

		// assign the new parent to all children of composite
		for (Composite* composite_ptr = composite.first_child_;
				 composite_ptr != 0; composite_ptr = composite_ptr->next_)
		{
			composite_ptr->parent_ = this;
		}
		
		// insert all children of composite into the 
		// double linked list of this composite
		if (composite.first_child_ != 0)
		{
			if (first_child_ != 0)
			{
				last_child_->next_ = composite.first_child_;
				composite.first_child_->previous_  = last_child_;
			} 
			else 
			{
				first_child_ = composite.first_child_;
			}

			last_child_ = composite.last_child_;
		}

		// update this composite
		number_of_children_ += composite.number_of_children_;
		number_of_selected_children_ += composite.number_of_selected_children_;
		number_of_children_containing_selection_ += composite.number_of_children_containing_selection_;
		updateSelection_();

		// update the (now empty) composite
		composite.number_of_children_ = 0;
		composite.first_child_  = composite.last_child_ = 0;
		composite.number_of_selected_children_ = 0;
		composite.number_of_children_containing_selection_ = 0;
		composite.contains_selection_ = composite.selected_;
	}

	void Composite::splice(Composite& composite)
	{
		// avoid self-splicing and splicing of descendants of composite
		if (&composite == this|| isDescendantOf(composite) == true)
		{
			return;
		}
		
		// if composite is the first child of this composite or
		// composite is a child of this composite,
		// the problem is a special case of spliceBefore()...
		if (&composite == first_child_ || composite.parent_ != this)
		{
			spliceBefore(composite);
			return;
		} 
		else if (&composite == last_child_)
		{
			// ... or spliceAfter()
			spliceAfter(composite);
			return;
		}

		// set the parent of each of composite`s children
		for (Composite* composite_ptr  = composite.first_child_;
				 composite_ptr != 0; composite_ptr = composite_ptr->next_)
		{
			composite_ptr->parent_ = this;
		}
		
		// now insert all children of composite
		if (composite.first_child_ != 0)
		{
			if (first_child_ != 0)
			{
				composite.first_child_->previous_  = composite.previous_;

				if (composite.previous_ != 0)
				{
					composite.previous_->next_ = composite.first_child_;
				}

				composite.last_child_->next_ = composite.next_;

				if (composite.next_ != 0)
				{
					composite.next_->previous_ = composite.last_child_;
				}

			} 
			else 
			{
				first_child_ = composite.first_child_;
				last_child_ = composite.last_child_;
			}
		}
		

		// remove composite from this
		composite.parent_ = composite.previous_ = composite.next_ 
			= composite.last_child_ = composite.first_child_  = 0;

		// update counters		
		// - 1 because composite was a child of this, too
		number_of_children_ += composite.number_of_children_ - 1;
		number_of_selected_children_ += composite.number_of_selected_children_;
		number_of_children_containing_selection_ += composite.number_of_children_containing_selection_;
		if (composite.selected_)
		{
			--number_of_selected_children_;
		} 
		if (composite.contains_selection_)
		{
			-- number_of_children_containing_selection_;
		}

		composite.number_of_children_ = 0;
		composite.number_of_selected_children_ = 0;
		composite.number_of_children_containing_selection_ = 0;
		composite.contains_selection_ = composite.selected_;

		// clean up everything
		updateSelection_();
	}

	bool Composite::removeChild(Composite& child)
	{
		// avoid self-removal and removal of ancestors
		if (&child == this || isDescendantOf(child) == true)
		{
			return false;
		}
		
		
		Composite* parent_ptr = child.parent_;

		// if child has no parent, we cannot remove it
		if (parent_ptr != this)
		{
			return false;
		}
			
		// remove child from the list of children
		if (first_child_ == &child)
		{
			first_child_ = first_child_->next_;

			if (first_child_ != 0)
			{
				first_child_->previous_ = 0;
			} 
			else 
			{
				last_child_ = 0;
			}
			
			child.next_ = 0;
		} 
		else if (last_child_ == &child)
		{
			last_child_ = child.previous_;
			last_child_->next_ = child.previous_ = 0;

		} 
		else
		{
			child.previous_->next_ = child.next_;
			
			child.next_->previous_ = child.previous_;
			
			child.previous_ = child.next_ = 0;
		}

		// delete the child`s parent pointer
		child.parent_ = 0;
		
		// adjust some counters
		--number_of_children_;
		if (child.contains_selection_)
		{
			--number_of_children_containing_selection_;
			if (child.selected_)
			{
				--number_of_selected_children_;
			}
		}

		// update the selection
		updateSelection_();

		return true;
	}

	void Composite::clear()
	{
		Composite* next_ptr = 0;
		Composite* composite_ptr = first_child_;
				
		for (; composite_ptr != 0;)
		{
			next_ptr = composite_ptr->next_;
		
			if (composite_ptr->isAutoDeletable() == true)
			{
				delete composite_ptr;
			} 
			else
			{
				composite_ptr->previous_ = composite_ptr->next_ = composite_ptr->parent_ = 0;
				composite_ptr->clear();
			}
			
			composite_ptr = next_ptr;
		}

		// clear pointers
		first_child_ = last_child_ = 0;

		// clear properties
		BALL_BIT_CLEAR_ALL(properties_);

		// update counters and selection
		number_of_children_ = 0;
		number_of_selected_children_ = 0;
		number_of_children_containing_selection_ = 0;
		contains_selection_ = selected_;
		updateSelection_();
	}

	void Composite::destroy()
	{
		if (parent_ != 0)
		{
			parent_->removeChild(*this);
		}

		clear();
	}

	void Composite::destroy(bool virtual_flag)
	{
		if (parent_ != 0)
		{
			parent_->removeChild(*this);
		}

		if (virtual_flag == true)
		{
			clear();
		} 
		else 
		{
			Composite::clear();
		}
	}

	void Composite::swap(Composite& composite)
	{
		if (&composite == this || isAncestorOf(composite) == true
				|| isDescendantOf(composite) == true)
		{
			return;
		}

		// adjust first/last pointers of the parents (if necessary)
		if (parent_ != 0)
		{
			if (parent_->first_child_ == this)
			{
				parent_->first_child_ = &composite;
			} 
			if (parent_->last_child_ == this) 
			{
				parent_->last_child_ = &composite;
			}
		}
		if (composite.parent_ != 0)
		{
			if (composite.parent_->first_child_ == this)
			{
				composite.parent_->first_child_ = &composite;
			} 
			if (composite.parent_->last_child_ == this) 
			{
				composite.parent_->last_child_ = &composite;
			}
		}
		std::swap(parent_, composite.parent_);

		// adjust the next_ and previous_ pointers pointing to this and composite
		if (previous_ != 0)
		{
			previous_->next_ = &composite;
		}
		if (next_ != 0)
		{
			next_->previous_ = &composite;
		}
		if (composite.previous_ != 0)
		{
			composite.previous_->next_ = this;
		}
		if (composite.next_ != 0)
		{
			composite.next_->previous_ = this;
		}
		std::swap(previous_, composite.previous_);
		std::swap(next_, composite.next_);

		// adjust pointers to and from the composite`s children
		Composite* composite_ptr = composite.first_child_;
		for (; composite_ptr != 0; composite_ptr = composite_ptr->next_)
		{
			composite_ptr->parent_ = this;
		}
		composite_ptr = first_child_;
		for (; composite_ptr != 0; composite_ptr = composite_ptr->next_)
		{
			composite_ptr->parent_ = &composite;
		}
		std::swap(number_of_children_, composite.number_of_children_);
		std::swap(first_child_, composite.first_child_);
		std::swap(last_child_, composite.last_child_);

		// swap all other attributes
		std::swap(number_of_children_, composite.number_of_children_);
		std::swap(number_of_selected_children_, composite.number_of_selected_children_);
		std::swap(number_of_children_containing_selection_, composite.number_of_children_containing_selection_);
		std::swap(contains_selection_, composite.contains_selection_);
		std::swap(properties_, composite.properties_);
		Selectable::swap(composite);

		// if the two parents are different, we have to update
		// the selection information
		if (parent_ != 0)
		{
			parent_->determineSelection_();
		}
		if (composite.parent_ != 0)
		{
			composite.parent_->determineSelection_();
		}
	}

	bool Composite::isDescendantOf(const Composite& composite) const
	{
		for (Composite* composite_ptr = parent_;
				 composite_ptr != 0; composite_ptr = composite_ptr->parent_)
		{
			if (composite_ptr == &composite)
			{
				return true;
			}
		}
		
		return false;
	}

	bool Composite::isHomomorph(const Composite& composite) const
	{
		if (this == &composite)
		{
			return true;
		}

		if (number_of_children_ != composite.number_of_children_)
		{
			return false;
		}

		Composite* ptr_a = first_child_;

		Composite *ptr_b = composite.first_child_;

		for (; ptr_a != 0 && ptr_b != 0;
				 ptr_a = ptr_a->next_, ptr_b = ptr_b->next_)
		{
			if (*ptr_a != *ptr_b)
			{
				return false;
			}
		}

		return true;
	}

	bool Composite::isValid() const
	{
		Composite* composite_ptr = first_child_;
		Size number_of_children = 0;
		
		for (; composite_ptr != 0; composite_ptr = composite_ptr->next_)
		{
			if (!composite_ptr->isValid())
			{
#				ifdef BALL_DEBUG      
					Log.error() << "Composite::isValid: INVALID: substructure is invalid" << endl;
#				endif      
				
				return false;
			}
			
			if (composite_ptr->parent_ != this)
			{
#				ifdef BALL_DEBUG      
					Log.error() << "Composite::isValid: INVALID: parent of " << composite_ptr->getHandle()
											<< " should be " << getHandle()
											<< " but is " << composite_ptr->parent_->getHandle() << endl;
#				endif      
				
				return false;
			}
			
			++number_of_children;
			
			if (composite_ptr->next_ == 0)
			{
				if (composite_ptr != last_child_)
				{
#					ifdef BALL_DEBUG      

						Log.error() << "INVALID: last child of " << getHandle()
												<< " should be " << last_child_->getHandle()
												<< " but is " << composite_ptr->getHandle() << endl;
#					endif      
		
					return false;
				}
				
				if (number_of_children != number_of_children_)
				{
#					ifdef BALL_DEBUG      
						Log.error() << "INVALID: number of children " << getHandle()
												<< " should be " << number_of_children_
												<< " but is " << number_of_children << endl;
#					endif      
		
					return false;
				}

				break;
			}
		}
		
		number_of_children = 0;
		
		for (composite_ptr = last_child_; composite_ptr != 0;
				 composite_ptr = composite_ptr->previous_)
		{
			if (!composite_ptr->isValid())
			{
#				ifdef BALL_DEBUG      
					Log.error() << "INVALID: substructure is invalid" << endl;
#				endif      
				
				return false;
			}
			
			++number_of_children;
			
			if (composite_ptr->previous_ == 0)
			{
				if (composite_ptr != first_child_)
				{
#					ifdef BALL_DEBUG      
						Log.error() << "INVALID: first child of " << getHandle()
												<< " should be " << first_child_->getHandle()
												<< " but is " << composite_ptr->getHandle() << endl;
#					endif      
		
					return false;
				}
				
				if (number_of_children != number_of_children_)
				{
#					ifdef BALL_DEBUG      
						Log.error() << "INVALID: number of children of " << getHandle()
												<< " should be " << number_of_children_
												<< " but is " << number_of_children << endl;
#					endif      
		
					return false;
				}

				break;
			}
		}
		
		return true;
	}

	void Composite::dump(std::ostream& s, Size depth) const
	{
		BALL_DUMP_STREAM_PREFIX(s);

		BALL_DUMP_DEPTH(s, depth);
    BALL_DUMP_HEADER(s, this, this)

		Object::dump(s, depth);
		Selectable::dump(s, depth);
		
		BALL_DUMP_DEPTH(s, depth);
		s << "  parent: "
			<< ((parent_) 
					? parent_->getHandle() 
					: (INVALID_HANDLE)) << endl;
		
		BALL_DUMP_DEPTH(s, depth);
		s << "  first: "
				 << ((first_child_)
			 ? first_child_->getHandle()
			 : INVALID_HANDLE) << endl;
		
		BALL_DUMP_DEPTH(s, depth);
		s << "  last: "
				 << ((last_child_)
			 ? last_child_->getHandle()
			 : INVALID_HANDLE) << endl;
		
		BALL_DUMP_DEPTH(s, depth);
		s << "  previous: "
				 << ((previous_)
			 ? previous_->getHandle()
			 : INVALID_HANDLE) << endl;
		
		BALL_DUMP_DEPTH(s, depth);
		s << "  next: "
				 << ((next_)
			 ? next_->getHandle()
			 : INVALID_HANDLE) << endl;
		
		BALL_DUMP_DEPTH(s, depth);
		s << "  number of children:" << number_of_children_ << endl;

		BALL_DUMP_DEPTH(s, depth);
		s << "  contains_selection_:" << contains_selection_ << endl;

		BALL_DUMP_DEPTH(s, depth);
		s << "  number of selected children:" << number_of_selected_children_ << endl;

		BALL_DUMP_DEPTH(s, depth);
		s << "  number of children containing selection:" << number_of_children_containing_selection_ << endl;
		
		BALL_DUMP_DEPTH(s, depth);
		s << "  children:" << endl;
		
		for (Composite* composite_ptr = first_child_;
				 composite_ptr != 0; composite_ptr = composite_ptr->next_)
		{
			BALL_DUMP_DEPTH(s, depth);
			s << "    [" << ((composite_ptr->previous_) 
											 ? composite_ptr->previous_->getHandle() 
											 : INVALID_HANDLE) << "] <- "
				<< "[" << composite_ptr->getHandle() << "] -> "
				<< "[" << ((composite_ptr->next_) 
									 ? composite_ptr->next_->getHandle() 
									 : INVALID_HANDLE) << "]" << endl;
			
			composite_ptr->dump(s, depth + 1);
		}

		BALL_DUMP_STREAM_SUFFIX(s);
	}

	Size Composite::getHeight_(Size size, Size& max_height) const
	{
		if (++size > max_height)
		{
			max_height = size;
		}
		
		for (Composite* composite_ptr = first_child_;
				 composite_ptr != 0; composite_ptr = composite_ptr->next_)
		{
			if (composite_ptr->first_child_ != 0)
			{
				composite_ptr->getHeight_(size, max_height);
			}
		}
		
		return max_height;
	}
		
	Size Composite::countDescendants_() const
	{
	
		// walk over all children and recursively call countDescendants
		Size number_of_descendants = 1;
		for (Composite* composite_ptr = first_child_;
				 composite_ptr != 0; 
				 composite_ptr = composite_ptr->next_)
		{
			number_of_descendants += composite_ptr->countDescendants_();
		}
		
		return number_of_descendants;
	}

	Size Composite::count(const KernelPredicateType& predicate) const
	{
		// iterate over the node itself and all its descendants
		// and count hits of the predicate
		Size hits = 0;
		SubcompositeIterator	sub_it = beginSubcomposite();
		for (; sub_it != endSubcomposite(); ++sub_it)
		{
			if (predicate(*sub_it))
			{
				hits++;
			}
		}
		
		return hits;
	}

	void Composite::clone_(Composite& parent, Composite& stack, UnaryPredicate<Composite>& predicate) const
	{
		Composite* cloned_ptr = 0;

		for (Composite* composite_ptr = parent.first_child_;
				 composite_ptr != 0; composite_ptr = composite_ptr->next_)
		{
			if (predicate(*composite_ptr) == true)
			{
				cloned_ptr = (Composite*)composite_ptr->create(false);

				stack.appendChild(*cloned_ptr);
				
				cloned_ptr->properties_ = composite_ptr->properties_;

				if (composite_ptr->first_child_ != 0)
				{
					clone_(*composite_ptr, *cloned_ptr, predicate);
				}
				
			}	
			else 
			{
				if (composite_ptr->first_child_ != 0)
				{
					clone_(*composite_ptr, stack, predicate);
				}
			}
		}

		// create selection information
		stack.determineSelection_();		
	}
				
	Composite* Composite::setCurrentPreorderIteratorPosition_
		(Composite& composite,
		 Composite::CompositeIteratorPosition_& position,
		 bool subcomposite)
	{
		return setCurrentPreorderForward_(composite, position, subcomposite);
	}

	Composite& Composite::getFirstPreorderIteratorPosition_(Composite& composite)
	{
		return composite;
	}
		
	Composite& Composite::getLastPreorderIteratorPosition_(Composite& composite)
	{
		Composite* composite_ptr = &composite;
		
		for (;	composite_ptr->last_child_ != 0; composite_ptr = composite_ptr->last_child_);
				
		return *composite_ptr;
	}
			
	Composite& Composite::setLastPreorderIteratorPosition_
		(Composite& composite, CompositeIteratorPosition_ &position, bool subcomposite)
	{
		if (subcomposite == true)
		{
			position.empty_stack_ = composite.parent_;
		}
		else
		{
			position.empty_stack_ = 0;
		}
				
		Composite *composite_ptr =& composite;
		
		for (; composite_ptr->last_child_ != 0; composite_ptr = composite_ptr->last_child_);

		position.stack_ = composite_ptr->parent_;
		position.current_ = composite_ptr;
		position.continue_ = false;
		position.traversing_forward_ = true;
		
		return *composite_ptr;
	}
			
	Composite* Composite::getNextPreorderIteratorPosition_(CompositeIteratorPosition_ &position)
	{
		if (position.traversing_forward_ == false)
		{
			setCurrentPreorderForward_(*(position.current_), position, (position.empty_stack_ != 0));
			
			return getNextPreorderIteratorPosition_(position);
		}
		
		if (position.continue_ == false)
		{
			if (position.current_->parent_ == position.empty_stack_)
			{
				if (position.current_->isCollapsed() == true)
				{
					return (position.current_ = 0);
				}
				else
				{
					position.current_ = position.current_->first_child_;
				}
			} 
			else 
			{
				position.current_ = position.current_->next_;
			}

			if (position.stack_ == position.empty_stack_)
			{
				return (position.current_ = 0);
			}
			
			if (position.current_ == 0)
			{
				position.current_ = position.stack_;
				position.stack_ = position.stack_->parent_;

				if (position.stack_	== position.empty_stack_)
				{
					return (position.current_ = 0);
				}

				if (position.current_->next_ == 0)
				{
					return getNextPreorderIteratorPosition_(position);
				} 
				else 
				{
					position.current_ = position.current_->next_;
				}
			}
		} 
		else 
		{
			position.stack_ = position.current_;
			position.current_ = position.current_->first_child_;
		}

		if (position.current_->first_child_ != 0 && position.current_->isCollapsed() == false)
		{
			position.continue_ = true;
			return position.current_;
		}

		position.continue_ = false;

		return position.current_;
	}

	Composite* Composite::getPreviousPreorderIteratorPosition_(CompositeIteratorPosition_& position)
	{
		if (position.traversing_forward_ == true)
		{
			setCurrentPreorderBackward_(*(position.current_), position, (position.empty_stack_ != 0));
			
			return getPreviousPreorderIteratorPosition_(position);
		}

		if (position.continue_ == false)
		{
			if (position.current_->parent_ == position.empty_stack_
					|| position.current_->isCollapsed() == true)
			{
				return (position.current_ = 0);
			} 
			else 
			{
				position.current_ = position.current_->previous_;
			}
			
			if (position.stack_ == position.empty_stack_)
			{
				return (position.current_ = 0);
			}
			
			while (position.current_ != 0
						 && position.current_->last_child_ != 0
						 && position.current_->isCollapsed() == false)
			{
				position.stack_ = position.current_;	
				position.current_ = position.current_->last_child_;
			}
			
			if (position.current_ == 0)
			{
				position.current_ = position.stack_;
				position.stack_ = position.stack_->parent_;
				return position.current_;
			}
		} 
		else 
		{
			position.stack_ = position.current_;	
			position.current_ = position.current_->last_child_;
		}
				
		if (position.current_->last_child_ != 0
				&& position.current_->isCollapsed() == false)
		{
			position.continue_ = true;
			return position.current_;
		}

		position.continue_ = false;
		return position.current_;
	}

	Composite* Composite::setCurrentPreorderForward_
		(Composite& composite, CompositeIteratorPosition_& position,
		 bool subcomposite)
	{
		if (subcomposite == true)
		{
			position.empty_stack_ = composite.parent_;
		}
		else
		{
			position.empty_stack_ = 0;
		}
		
		if (composite.parent_ == position.empty_stack_)
		{
			position.stack_ = &composite;
			position.continue_ = false;
		} 
		else 
		{
			position.stack_ = composite.parent_;	
			position.continue_ = (composite.first_child_ != 0);
		}

		position.current_ = &composite;
		position.traversing_forward_ = true;
		
		return &composite;
	}

	Composite* Composite::setCurrentPreorderBackward_
		(Composite& composite, CompositeIteratorPosition_& position,
		 bool subcomposite)
	{
		if (subcomposite == false)
		{
			position.empty_stack_ = 0;
		}
		
		if (composite.parent_ == position.empty_stack_)
		{
			position.stack_ = &composite;
		}
		else
		{
			position.stack_ = composite.parent_;
		}
		
		position.current_ = &composite;
		position.continue_ = false;
		position.traversing_forward_ = false;
		
		return &composite;
	}

#	ifdef BALL_NO_INLINE_FUNCTIONS
#		include <BALL/CONCEPT/composite.iC>
#	endif

} // namespace BALL
