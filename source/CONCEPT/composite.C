// $Id: composite.C,v 1.1 1999/08/26 08:02:35 oliver Exp $

#include <BALL/CONCEPT/composite.h>
#include <BALL/CONCEPT/persistenceManager.h>
#include <BALL/DATATYPE/listSort.mac>
#include <BALL/KERNEL/predicate.h>

namespace BALL 
{

	UnaryPredicate<Composite> Composite::DEFAULT_UNARY_PREDICATE;
	UnaryProcessor<Composite> Composite::DEFAULT_PROCESSOR;

	Composite::Composite()
		:	PersistentObject(),
			number_of_children_(0),
			first_(0),
			parent_(0),
			previous_(0),
			next_(0),
			first_child_(0),
			last_child_(0)
	{
		BALL_BIT_CLEAR_ALL(properties_);
	}

	Composite::Composite(const Composite& composite, bool deep)
		:	PersistentObject(composite),
			number_of_children_(0),
			first_(0),
			parent_(0),
			previous_(0),
			next_(0),
			first_child_(0),
			last_child_(0)
	{
		properties_ = composite.properties_;
		
		if (deep == true)
		{
			composite.clone(*this, Composite::DEFAULT_UNARY_PREDICATE);
		}
	}

	Composite::~Composite()
	{
		destroy();
	}


  void Composite::persistentWrite(PersistenceManager& pm, const char* name) const
  {
		pm.writeObjectHeader(this, name);
			pm.writePrimitive(number_of_children_, "number_of_children_");
			pm.writeObjectPointer(first_, "first_");
			pm.writeObjectPointer(parent_, "parent_");
			pm.writeObjectPointer(previous_, "previous_");
			pm.writeObjectPointer(next_, "next_");
			pm.writeObjectPointer(first_child_, "first_child_");
			pm.writeObjectPointer(last_child_, "last_child_");
			pm.writePrimitive(properties_, "properties_");
		pm.writeObjectTrailer(name);
	}

  void Composite::persistentRead(PersistenceManager& pm)
  {
		pm.readPrimitive(number_of_children_, "number_of_children_");
		pm.readObjectPointer(first_, "first_");
		pm.readObjectPointer(parent_, "parent_");
		pm.readObjectPointer(previous_, "previous_");
		pm.readObjectPointer(next_, "next_");
		pm.readObjectPointer(first_child_, "first_child_");
		pm.readObjectPointer(last_child_, "last_child_");
		pm.readPrimitive(properties_, "properties_");
	}
 

	Size Composite::getPathLength(const Composite& a, const Composite& b)
	{
		if (&a == &b)
		{
			return 0;
		}

		Size path_size = 1;
		Composite* composite_ptr = a.parent_;

		for (; composite_ptr != 0; composite_ptr = composite_ptr->parent_, ++path_size)
		{
			if (composite_ptr == &b)
			{
				return path_size;
			}
		}

		for (composite_ptr = b.parent_, path_size = 1; composite_ptr != 0; composite_ptr = composite_ptr->parent_,  ++path_size)
		{
			if (composite_ptr == &a)
			{
				return path_size;
			}
		}
		
		return INVALID_SIZE;
	}

	Size  Composite::getDepth(const Composite& composite)
	{
		Size size = 0;

		for (Composite *composite_ptr = composite.parent_;
				 composite_ptr != 0; composite_ptr = composite_ptr->parent_)
		{
			++size;
		}

		return size;
	}

	Size Composite::getLevel(const Composite& composite)
	{
		if (composite.parent_ == 0)
		{
			return 0;
		}

		Size levelsize = 1;
				
		for (Composite *composite_ptr = composite.parent_;
				 composite_ptr->parent_ != 0; composite_ptr = composite_ptr->parent_, ++levelsize);

		return levelsize;
	}

	Composite& Composite::getRoot(Composite& composite)
	{
		Composite *composite_ptr = (Composite *)&composite;
		
		for (; composite_ptr->parent_ != 0; composite_ptr = composite_ptr->parent_);

		return *composite_ptr;
	}

	Composite* Composite::getLowestCommonAncestor(Composite& composite)
	{
		Composite *composite_ptr = 0;

		const Size size_a = getDepth() + 1;
		Index index_a = 0;
		Composite** composites_a = new Composite *[size_a];
		for (composite_ptr = this; composite_ptr != 0; composite_ptr = composite_ptr->parent_)
		{
			composites_a[index_a++] = composite_ptr;
		}

		const Size size_b = composite.getDepth() + 1;
		Index index_b = 0;
		Composite** composites_b = new Composite *[size_b];
		for (composite_ptr =& composite; composite_ptr != 0;composite_ptr = composite_ptr->parent_)
			composites_b[index_b++] = composite_ptr;

		for (composite_ptr = 0, index_a = size_a - 1, index_b = size_b - 1;
				 index_a >= 0 && index_b >= 0; --index_a, --index_b)
		{
			if (composites_a[index_a] == composites_b[index_b])
			{
				composite_ptr = composites_a[index_a];
			}
		}

		delete [] composites_a;
		delete [] composites_b;

		return composite_ptr;
	} 

	Composite* Composite::getPreviousSibling(Index index)
	{
		if (index <= 0)
		{
			return 0;
		}
		
		Composite *composite_ptr = previous_;

		for (; --index > 0 && composite_ptr != 0; composite_ptr = composite_ptr->previous_);

		return composite_ptr;
	}

	Composite* Composite::getNextSibling(Index index)
	{
		if (index <= 0)
		{
			return 0;
		}
		
		Composite *composite_ptr = next_;

		for (; --index > 0 && composite_ptr != 0; composite_ptr = composite_ptr->next_);

		return composite_ptr;
	}

	void* Composite::clone(Composite& root, UnaryPredicate<Composite>& predicate) const
	{
		if (&root == this)	
		{
			return 0;
		}

		root.destroy();

		root.properties_ = properties_;

		clone_(*(Composite *)this, root, predicate);

		return &root;
	}

	void  Composite::prependChild(Composite& composite)
	{
		if (&composite == this || isDescendantOf(composite) == true)
		{
			return;
		}
		
		if (&composite == first_child_)
		{
			return;
		}

		if (composite.parent_ != 0)
		{
			composite.parent_->removeChild(composite);
		}

		if (first_child_ == 0)
		{
			first_child_ = last_child_ = &composite;
		} else {
			first_child_->previous_ = &composite;
			composite.next_ = first_child_;
			first_child_ = &composite;
		}

		composite.parent_ = this;

		++number_of_children_;
	}

	void Composite::appendChild(Composite& composite)
	{
		if (&composite == this || isDescendantOf(composite) == true)
		{
			return;
		}
		
		if (&composite == last_child_)
		{
			return;
		}
		
		if (composite.parent_ != 0)
		{
			composite.parent_->removeChild(composite);
		}

		if (last_child_ == 0)
		{
			first_child_ = last_child_ =& composite;
		} else {
			last_child_->next_ =& composite;
			composite.previous_ = last_child_;
			last_child_ =& composite;
		}

		composite.parent_ = this;

		++number_of_children_;
	}

	bool Composite::insertParent(Composite &parent, Composite &first, Composite &last, bool destroy_parent)
	{
		if (first.parent_ != last.parent_ || first.parent_ == 0
				|| last.parent_ == 0 || &first == &parent || &last == &parent)
		{
			return false;
		}

		if (first.isDescendantOf(parent) == true)
		{
			return true;
		}

		parent.destroy(destroy_parent);

		Composite *parent_ptr = first.parent_;

		parent.parent_ = parent_ptr;
		parent.first_child_ = &first;
		parent.last_child_ = &last;
		
		if (parent_ptr->first_child_ == &first)
		{
			if (parent_ptr->last_child_ == &last)
			{
				parent_ptr->last_child_ = &parent;

			} else {

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

		} else {

			first.previous_->next_ = &parent;

			parent.previous_ = first.previous_;

			last.next_->previous_ = &parent;

			parent.next_ = last.next_;

			first.previous_ = last.next_ = 0;
		}
				
		for (Composite *composite_ptr = &first; composite_ptr != &last; composite_ptr = composite_ptr->next_)
		{
			++(parent.number_of_children_);
			--(parent_ptr->number_of_children_);
			composite_ptr->parent_ = &parent;
		}

		last.parent_ = &parent;
		++(parent.number_of_children_);

		return true;
	}

	void  Composite::insertBefore(Composite& composite)
	{
		if (parent_ == 0 || &composite == this
				|| isDescendantOf(composite) == true)
		{
			return;
		}

		if (parent_->first_child_ == this)
		{
			parent_->prependChild(composite);

			return;
		}

		if (composite.parent_ != 0)
		{
			composite.parent_->removeChild(composite);
		}

		previous_->next_ =& composite;
		composite.previous_ = previous_;
		previous_ =& composite;
		composite.next_ = this;
		
		composite.parent_ = parent_;
		++(parent_->number_of_children_);
	}

	void Composite::insertAfter(Composite& composite)
	{
		if (parent_ == 0 || &composite == this
				|| isDescendantOf(composite) == true)
		{
			return;
		}

		if (parent_->last_child_ == this)
		{
			parent_->appendChild(composite);

			return;
		}

		if (composite.parent_ != 0)
		{
			composite.parent_->removeChild(composite);
		}
		
		next_->previous_ =& composite;
		composite.next_ = next_;
		next_ =& composite;
		composite.previous_ = this;
		
		composite.parent_ = parent_;
		++(parent_->number_of_children_);
	}

	void Composite::spliceBefore(Composite& composite)
	{
		if (&composite == this || isDescendantOf(composite) == true)
		{
			return;
		}
		
		if (composite.parent_ != 0)
		{
			composite.parent_->removeChild(composite);
		}
		
		for (Composite *composite_ptr =  composite.first_child_;
				 composite_ptr != 0; composite_ptr = composite_ptr->next_)
		{
			composite_ptr->parent_ = this;
		}

		if (composite.first_child_ != 0)
		{
			if (first_child_ != 0)
			{
				composite.last_child_->next_ = first_child_;

				first_child_->previous_  = composite.last_child_;

			} else {

				last_child_ = composite.last_child_;
			}
			
			first_child_ = composite.first_child_;
		}

		number_of_children_ += composite.number_of_children_;

		composite.number_of_children_ = 0;
		composite.first_child_  = composite.last_child_ = 0;
	}

	void  Composite::spliceAfter(Composite& composite)
	{
		if (&composite == this || isDescendantOf(composite) == true)
		{
			return;
		}
		
		if (composite.parent_ != 0)
		{
			composite.parent_->removeChild(composite);
		}

		for (Composite *composite_ptr = composite.first_child_;
				 composite_ptr != 0; composite_ptr = composite_ptr->next_)
		{
			composite_ptr->parent_ = this;
		}
		
		if (composite.first_child_ != 0)
		{
			if (first_child_ != 0)
			{
				last_child_->next_ = composite.first_child_;

				composite.first_child_->previous_  = last_child_;

			} else {

				first_child_ = composite.first_child_;
			}

			last_child_ = composite.last_child_;
		}

		number_of_children_ += composite.number_of_children_;

		composite.number_of_children_ = 0;
		composite.first_child_ = composite.last_child_ = 0;
	}

	void Composite::splice(Composite& composite)
	{
		if (&composite == this|| isDescendantOf(composite) == true)
		{
			return;
		}
		
		if (&composite == first_child_ || composite.parent_ != this)
		{
			spliceBefore(composite);

			return;

		} else if (&composite == last_child_)
		{
			spliceAfter(composite);

			return;
		}
		
		for (Composite *composite_ptr  = composite.first_child_;
				 composite_ptr != 0; composite_ptr = composite_ptr->next_)
		{
			composite_ptr->parent_ = this;
		}
		
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

			} else {
				first_child_ = composite.first_child_;
				last_child_ = composite.last_child_;
			}
		}
		
		// composite has this as parent so subtract 1 !!!
		number_of_children_ += composite.number_of_children_ - 1;
		
		composite.number_of_children_ = 0;
		composite.parent_  = composite.previous_ = composite.next_ 
			= composite.last_child_ = composite.first_child_  = 0;
	}

	void  Composite::splitBefore(Composite &childcomposite, Composite& composite)
	{
		if (&composite == this || isParentOf(childcomposite) == false
				|| composite.isAncestorOf(*this) == true)
		{
			return;
		}
		
		composite.destroy();
		composite.first_child_ = &childcomposite;
		composite.last_child_ = last_child_;
		
		if (first_child_ == &childcomposite)
		{
			first_child_ = last_child_ = 0;

		} else  {

			last_child_ = childcomposite.previous_;
			childcomposite.previous_->next_ = 0;
			childcomposite.previous_ = 0;
		}
		
		for (Composite *composite_ptr = &childcomposite;
				 composite_ptr != 0; composite_ptr = composite_ptr->next_)
		{
			--number_of_children_;
			++(composite.number_of_children_);
			composite_ptr->parent_ =& composite;
		}
	}

	void  Composite::splitAfter(Composite &childcomposite, Composite& composite)
	{
		if (&composite == this || isParentOf(childcomposite) == false
				|| composite.isAncestorOf(*this) == true)
		{
			return;
		}
		
		composite.destroy();
		composite.first_child_ = first_child_;
		composite.last_child_ = &childcomposite;
		
		if (last_child_ == &childcomposite)
		{
			first_child_ = last_child_  = 0;

		} else {

			first_child_ = childcomposite.next_;
			childcomposite.next_->previous_ = 0;
			childcomposite.next_ = 0;
		}
		
		for (Composite *composite_ptr = &childcomposite;
				 composite_ptr != 0; composite_ptr = composite_ptr->previous_)
		{
			--number_of_children_;
			++(composite.number_of_children_);
			composite_ptr->parent_ =& composite;
		}
	}

	bool Composite::removeAggregates()
	{
		AggregateCompositeItem *next = 0;
		
		for (AggregateCompositeItem *aggregate_composite = first_; aggregate_composite != 0; aggregate_composite = next)
		{
			next = aggregate_composite->composite_next_;
			
			delete aggregate_composite;
		}

		return true;
	}

	bool Composite::removeChild(Composite& composite)
	{
		if (&composite == this || isDescendantOf(composite) == true)
		{
			return false;
		}
		
		Composite *parent_ptr = composite.parent_;
		
		if (parent_ptr == 0)
		{
			return false;
		}
		
		if (parent_ptr != this)	
		{
			return parent_ptr->removeChild(composite);
		}
		
		if (first_child_ ==& composite)
		{
			first_child_ = first_child_->next_;

			if (first_child_ != 0)
			{
				first_child_->previous_ = 0;

			} else {

				last_child_ = 0;
			}
			
			composite.next_ = 0;

		} else if (last_child_ ==& composite)
		{
			last_child_ = composite.previous_;
			last_child_->next_ = composite.previous_ = 0;

		} else
		{
			composite.previous_->next_ = composite.next_;
			
			composite.next_->previous_ = composite.previous_;
			
			composite.previous_ = composite.next_ = 0;
		}
		
		composite.parent_ = 0;
		--number_of_children_;

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

			} else
			{
				composite_ptr->previous_ = composite_ptr->next_ = composite_ptr->parent_ = 0;
				
				composite_ptr->clear();
			}
			
			composite_ptr = next_ptr;
		}

		number_of_children_ = 0;
		first_child_ = last_child_ = 0;
		BALL_BIT_CLEAR_ALL(properties_);
	}

	void Composite::destroy()
	{
		if (parent_ != 0)
		{
			parent_->removeChild(*this);
		}

		AggregateCompositeItem *next = 0;
		
		for(AggregateCompositeItem *aggregate_composite = first_; aggregate_composite!= 0;)
		{
			next = aggregate_composite->composite_next_;
			delete aggregate_composite;			
			aggregate_composite = next;
		}
		
		clear();
	}

	void Composite::destroy(bool virtual_flag)
	{
		if (parent_ != 0)
		{
			parent_->removeChild(*this);
		}

		AggregateCompositeItem* next = 0;
		
		for(AggregateCompositeItem* aggregate_composite = first_; aggregate_composite != 0;)
		{
			next = aggregate_composite->composite_next_;
			delete aggregate_composite;
			aggregate_composite = next;
		}

		if (virtual_flag == true)
		{
			clear();

		} else {

			Composite::clear();
		}
	}

	void Composite::swap(Composite& composite)
	{
		if (&composite == this
				|| isAncestorOf(composite) == true
				|| isDescendantOf(composite) == true)
		{
			return;
		}

		if (parent_ == 0 && composite.parent_ == 0)
		{
			return;
		}

		if (parent_ == composite.parent_)
		{
			bool first_child_found = false;
			bool last_child_found = false;

			if (parent_->first_child_ == this)
			{
				parent_->first_child_ =& composite;
				first_child_found = true;
			}

			if (parent_->last_child_ == this)
			{
				parent_->last_child_ =& composite;
				last_child_found = true;
			}
		
			if (first_child_found == false && parent_->first_child_ ==& composite)
			{
				parent_->first_child_ = this;
			}

			if (last_child_found == false && parent_->last_child_ ==& composite)
			{
				parent_->last_child_ = this;
			}
		}
		else
		{
			if (parent_ != 0)
			{
				if (parent_->first_child_ == this)
				{
					parent_->first_child_ =& composite;
				}

				if (parent_->last_child_ == this)
				{
					parent_->last_child_ =& composite;
				}
			}

			if (composite.parent_ != 0)
			{
				if (composite.parent_->first_child_ ==& composite)
				{
					composite.parent_->first_child_ = this;
				}
				
				if (composite.parent_->last_child_ ==& composite)
				{
					composite.parent_->last_child_ = this;
				}
			}
		}
		
		Composite *tmp_ptr = parent_;
		
		parent_ = composite.parent_;
		composite.parent_ = tmp_ptr;
		
		if (previous_ ==& composite)
		{
			if (next_ != 0)
			{
				next_->previous_ =& composite; 
			}
			
			if (composite.previous_ != 0)
			{
				composite.previous_->next_ = this; 
			}
			
			composite.next_ = next_;
			previous_ = composite.previous_;
			next_ =& composite;
			composite.previous_ = this;
		}
		else if (next_ ==& composite)
		{
			if (previous_ != 0)
			{
				previous_->next_ =& composite; 
			}
			
			if (composite.next_ != 0)
			{
				composite.next_->previous_ = this; 
			}
			
			composite.previous_ = previous_;
			next_ = composite.next_;
			previous_ =& composite;
			composite.next_ = this;

		} else
		{
			if (previous_ != 0)
			{
				previous_->next_ =& composite; 
			}
			
			if (next_ != 0)
			{
				next_->previous_ =& composite; 
			}
			
			if (composite.next_ != 0)
			{
				composite.next_->previous_ = this; 
			}
			
			if (composite.previous_ != 0)
			{
				composite.previous_->next_ = this; 
			}
			
			tmp_ptr = composite.previous_;
			composite.previous_ = previous_;
			previous_ = tmp_ptr;
			
			tmp_ptr = composite.next_;
			composite.next_ = next_;
			next_ = tmp_ptr;
		}
	}

	void Composite::sortChildren(const Comparator<Composite *> &comparator)
	{
		BALL_IMPLEMENT_BALANCED_2_WAY_SINGLE_LINKED_LIST_MERGE_SORT
			(Composite, next_, first_child_, last_child_, comparator);
		
		Composite *previous_ptr = 0;
		
		for (Composite *composite_ptr = first_child_; composite_ptr != 0;
				 composite_ptr = composite_ptr->next_)
		{
			composite_ptr->previous_ = previous_ptr;
			previous_ptr = composite_ptr;
		}
	}

	bool Composite::isPreceedingSiblingOf(const Composite& composite) const
	{
		for (Composite *composite_ptr = next_;
				 composite_ptr != 0; composite_ptr = composite_ptr->next_)
		{
			if (composite_ptr ==& composite)
				return true;
		}
		
		return false;
	}

	bool Composite::isSucceedingSiblingOf(const Composite& composite) const
	{
		for (Composite *composite_ptr = previous_;
				 composite_ptr != 0; composite_ptr = composite_ptr->previous_)
		{
			if (composite_ptr ==& composite)
			{
				return true;
			}
		}

		return false;
	}

	bool Composite::isDescendantOf(const Composite& composite) const
	{
		for (Composite *composite_ptr = parent_;
				 composite_ptr != 0; composite_ptr = composite_ptr->parent_)
		{
			if (composite_ptr ==& composite)
				return true;
		}
		
		return false;
	}

	bool Composite::isHomomorph(const Composite& composite) const
	{
		if (this ==& composite)
			return true;

		if (number_of_children_ != composite.number_of_children_)
			return false;

		Composite *acomposite_ptr = first_child_;

		Composite *bcomposite_ptr = composite.first_child_;

		for (; acomposite_ptr != 0 && bcomposite_ptr != 0;
				 acomposite_ptr = acomposite_ptr->next_, bcomposite_ptr = bcomposite_ptr->next_)
		{
			if (*acomposite_ptr != *bcomposite_ptr)
				return false;
		}

		return true;
	}

	bool Composite::isValid() const
	{
		AggregateCompositeItem *last = 0;
		AggregateCompositeItem* aggregate_composite = first_;
		
		for(; aggregate_composite!= 0; aggregate_composite = aggregate_composite->composite_next_)
		{
			last = aggregate_composite;

			if (aggregate_composite->composite_ != this)
			{
#				ifdef BALL_DEBUG      

				Log.level(LogStream::ERROR) << "INVALID: composite of root " << aggregate_composite->getHandle()
						 << " should be " << getHandle()
						 << " but is " << aggregate_composite->composite_->getHandle() << endl;

#	endif      
				
				return false;
			}
		}
		
		for( aggregate_composite= last; aggregate_composite != 0; aggregate_composite = aggregate_composite->composite_previous_)
		{
			if (aggregate_composite->composite_previous_ == 0)
				break;
		}
		
		if (aggregate_composite!= first_)
		{
#			ifdef BALL_DEBUG      

				Log.level(LogStream::ERROR) << "INVALID: first root of " << getHandle()
						 << " should be " << first_->getHandle()
						 << " but is " << aggregate_composite->getHandle() << endl;

#			endif      
			
			return false;
		}
		
		Composite *composite_ptr = first_child_;
		Size number_of_children = 0;
		
		for (; composite_ptr != 0; composite_ptr = composite_ptr->next_)
		{
			if (!composite_ptr->isValid())
			{
#				ifdef BALL_DEBUG      

				Log.level(LogStream::ERROR) << "INVALID: substructure is invalid" << endl;

#				endif      
				
				return false;
			}
			
			if (composite_ptr->parent_ != this)
			{
#				ifdef BALL_DEBUG      

				Log.level(LogStream::ERROR) << "INVALID: parent of " << composite_ptr->getHandle()
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

					Log.level(LogStream::ERROR) << "INVALID: last child of " << getHandle()
							 << " should be " << last_child_->getHandle()
							 << " but is " << composite_ptr->getHandle() << endl;

#					endif      
		
					return false;
				}
				
				if (number_of_children != number_of_children_)
				{
#					ifdef BALL_DEBUG      

					Log.level(LogStream::ERROR) << "INVALID: number of children " << getHandle()
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

				Log.level(LogStream::ERROR) << "INVALID: substructure is invalid" << endl;

#				endif      
				
				return false;
			}
			
			++number_of_children;
			
			if (composite_ptr->previous_ == 0)
			{
				if (composite_ptr != first_child_)
				{
#					ifdef BALL_DEBUG      

					Log.level(LogStream::ERROR) << "INVALID: first child of " << getHandle()
							 << " should be " << first_child_->getHandle()
							 << " but is " << composite_ptr->getHandle() << endl;

#					endif      
		
					return false;
				}
				
				if (number_of_children != number_of_children_)
				{
#					ifdef BALL_DEBUG      

					Log.level(LogStream::ERROR) << "INVALID: number of children of " << getHandle()
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

	void Composite::dump(ostream &s,
		 unsigned long depth) const
	{
		BALL_DUMP_STREAM_PREFIX(s);

		Object::dump(s, depth);
		
		BALL_DUMP_DEPTH(s, depth);
		s << "  parent aggregates: " << endl;
		for(AggregateCompositeItem* aggregate_composite = first_; aggregate_composite!= 0;
				aggregate_composite= aggregate_composite->composite_next_)
		{
			aggregate_composite->dump(s, depth + 1);
		}

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
		s << "  children:" << endl;
		
		for (Composite *composite_ptr = first_child_;
				 composite_ptr != 0;
				 composite_ptr = composite_ptr->next_)
		{
			BALL_DUMP_DEPTH(s, depth);
			cout << "    [" << ((composite_ptr->previous_) 
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

	bool Composite::applyAggregateCompositeItem(UnaryProcessor<AggregateCompositeItem>& processor)
	{
		if (processor.start() == false)
			return false;

		Processor::Result result;

		for (AggregateCompositeItem* aggregate_composite = first_; aggregate_composite != 0;
					aggregate_composite = aggregate_composite->composite_next_)
		{
			result = processor(*aggregate_composite);

			if (result <= Processor::BREAK)
				return (result == Processor::BREAK) ? true : false;
		}

		return processor.finish();
	}

	bool Composite::applyParentAggregate(UnaryProcessor<Aggregate>& processor)
	{
		if (processor.start() == false)
			return false;

		Processor::Result result;
		
		for (AggregateCompositeItem* aggregate_composite = first_;
				  aggregate_composite != 0; aggregate_composite = aggregate_composite->composite_next_)
		{
			result = processor(*aggregate_composite->aggregate_);

			if (result <= Processor::BREAK)
				return (result == Processor::BREAK) ? true : false;
		}

		return processor.finish();
	}

	bool Composite::applyAncestor(UnaryProcessor<Composite>& processor)
	{
		if (processor.start() == false)
			return false;

		Processor::Result result;

		for (Composite *composite_ptr = parent_;
				 composite_ptr != 0; composite_ptr = composite_ptr->parent_)
		{
			result = processor(*composite_ptr);
		
			if (result <= Processor::BREAK)
				return (result == Processor::BREAK) ? true : false;
		}

		return processor.finish();
	}

	bool Composite::applyChild(UnaryProcessor<Composite>& processor)
	{
		return processor.start() && applyChildNostart_(processor) && processor.finish();
	}

	bool Composite::applyChildNostart_(UnaryProcessor<Composite>& processor)
	{
		if (isCollapsed() == true)
			return true;
		
		Processor::Result result;
		
		for (Composite *composite_ptr = first_child_;
				 composite_ptr != 0; composite_ptr = composite_ptr->next_)
		{
			result = processor(*composite_ptr);
		
			if (result <= Processor::BREAK)
				return (result == Processor::BREAK) ? true : false;
		}
		
		return true;
	}

	bool Composite::applyDescendantPreorder(UnaryProcessor<Composite>& processor)
	{
		return processor.start() && applyDescendantPreorderNostart_(processor) && processor.finish();
	}

	bool Composite::applyDescendantPreorderNostart_(UnaryProcessor<Composite>& processor)
	{
		if (isCollapsed() == true)
			return true;

		Processor::Result result;
		
		for (Composite *composite_ptr = first_child_;
				 composite_ptr != 0; composite_ptr = composite_ptr->next_)
		{
			result = processor(*composite_ptr);

			if (result <= Processor::BREAK)
				return (result == Processor::BREAK) ? true : false;

			if (composite_ptr->first_child_ != 0	&& composite_ptr->applyDescendantPreorderNostart_(processor) == false)
				return false;
		}
		
		return true;
	}

	bool Composite::applyDescendantPostorder(UnaryProcessor<Composite>& processor)
	{
		return processor.start() && applyDescendantPostorderNostart_(processor) && processor.finish();
	}
			
	bool Composite::applyDescendantPostorderNostart_(UnaryProcessor<Composite>& processor)
	{
		if (isCollapsed() == true)
			return true;

		Processor::Result result;
				
		for (Composite *composite_ptr = first_child_;
				 composite_ptr != 0; composite_ptr = composite_ptr->next_)
		{
			if (composite_ptr->first_child_ != 0 && composite_ptr->applyDescendantPostorderNostart_(processor) == false)
				return false;

			result = processor(*composite_ptr);
				
			if (result <= Processor::BREAK)
				return (result == Processor::BREAK) ? true : false;
		}
		
		return true;
	}
			
	bool Composite::applyLevel(UnaryProcessor<Composite>& processor, long level)
	{
		return processor.start() && applyLevelNostart_(processor, level) && processor.finish();
	}
					
	bool Composite::applyLevelNostart_(UnaryProcessor<Composite>& processor, long level)
	{
		if (level == 0)
		{
			Processor::Result result = processor(*this);

			if (result <= Processor::BREAK)
				return (result == Processor::BREAK) ? true : false;
		}
		else if (--level == 0)
		{
			return applyChildNostart_(processor);
		}
		else if (level > 0)
		{
			for (Composite *composite_ptr = first_child_;
					 composite_ptr != 0; composite_ptr = composite_ptr->next_)
			{
				if (composite_ptr->first_child_ != 0 && composite_ptr->applyLevelNostart_(processor, level) == false)
					return false;
			}
		}
		
		return true;
	}
					
	AggregateCompositeItem* Composite::find_(const Aggregate &aggregate) const
	{
		for (AggregateCompositeItem* aggregate_composite = first_; aggregate_composite != 0;
				 aggregate_composite = aggregate_composite->composite_next_)
		{
			if (&aggregate == aggregate_composite->aggregate_)
				return aggregate_composite;
		}

		return 0;
	}

	Size Composite::getHeight_(Size size, Size &max_height) const
	{
		if (++size > max_height)
			max_height = size;
		
		for (Composite *composite_ptr = first_child_;
				 composite_ptr != 0; composite_ptr = composite_ptr->next_)
		{
			if (composite_ptr->first_child_ != 0)
				composite_ptr->getHeight_(size, max_height);
		}
		
		return max_height;
	}
		
	Size Composite::countDescendants_() const
	{
		Size number_of_descendants = 1;
		
		for (Composite *composite_ptr = first_child_;
				 composite_ptr != 0; composite_ptr = composite_ptr->next_)
		{
			number_of_descendants += composite_ptr->countDescendants_();
		}
		
		return number_of_descendants;
	}

	Size Composite::count(const KernelPredicateType& predicate) const
	{
		Size hits = 0;

		SubcompositeIterator	sub_it = beginSubcomposite();
		for (; sub_it != endSubcomposite(); ++sub_it)
			if (predicate(*sub_it))
				hits++;
		
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
				cloned_ptr = (Composite *)composite_ptr->create(false);

				stack.appendChild(*cloned_ptr);
				
				cloned_ptr->properties_ = composite_ptr->properties_;
				
				if (composite_ptr->first_child_ != 0)
					clone_(*composite_ptr, *cloned_ptr, predicate);

			}	else {

				if (composite_ptr->first_child_ != 0)
				{
					clone_(*composite_ptr, stack, predicate);
				}
			}
		}
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
		Composite *composite_ptr =& composite;
		
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
			setCurrentPreorderForward_(*(position.current_), position, (bool)(position.empty_stack_ != 0));
			
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
			} else {
				position.current_ = position.current_->next_;
			}

			if (position.stack_ == position.empty_stack_)
				return (position.current_ = 0);
			
			if (position.current_ == 0)
			{
				position.current_ = position.stack_;
				position.stack_ = position.stack_->parent_;

				if (position.stack_	== position.empty_stack_)
					return (position.current_ = 0);

				if (position.current_->next_ == 0)
				{
						return getNextPreorderIteratorPosition_(position);

				} else {

					position.current_ = position.current_->next_;
				}
			}
		} else {
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

	Composite* Composite::getPreviousPreorderIteratorPosition_(CompositeIteratorPosition_ &position)
	{
		if (position.traversing_forward_ == true)
		{
			setCurrentPreorderBackward_(*(position.current_), position, (bool)(position.empty_stack_ != 0));
			
			return getPreviousPreorderIteratorPosition_(position);
		}

		if (position.continue_ == false)
		{
			if (position.current_->parent_ == position.empty_stack_
					|| position.current_->isCollapsed() == true)
			{
				return (position.current_ = 0);

			} else {

				position.current_ = position.current_->previous_;
			}
			
			if (position.stack_ == position.empty_stack_)
				return (position.current_ = 0);
			
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
		} else {
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
		(Composite& composite, CompositeIteratorPosition_ &position,
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
			position.stack_ =& composite;
			position.continue_ = false;

		} else {

			position.stack_ = composite.parent_;
			
			position.continue_ = (bool)(composite.first_child_ != 0);
		}

		position.current_ =& composite;
		position.traversing_forward_ = true;
		
		return& composite;
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
			position.stack_ =& composite;
		}
		else
		{
			position.stack_ = composite.parent_;
		}
		
		position.current_ =& composite;
		position.continue_ = false;
		position.traversing_forward_ = false;
		
		return& composite;
	}



	AggregateCompositeItem::AggregateCompositeItem()
		: PersistentObject(),
			aggregate_(0),
			composite_(0),
			aggregate_next_(0),
			composite_next_(0),
			aggregate_previous_(0),
			composite_previous_(0),
			handle_(Object::getNewHandle())
	{
	}

	AggregateCompositeItem::AggregateCompositeItem
		(Aggregate &aggregate,
		 Composite& composite,
		 AggregateCompositeItem *aggregate_next,
		 AggregateCompositeItem *composite_next)
		:	PersistentObject(),
			aggregate_(&aggregate),
			composite_(&composite),
			aggregate_next_(aggregate_next),
			composite_next_(composite_next),
			aggregate_previous_(0),
			composite_previous_(0),
			handle_(Object::getNewHandle())
	{
		if (aggregate_next_ != 0)
			aggregate_next_->aggregate_previous_ = this;
				
		if (aggregate_->first_ == 0)
			aggregate_->last_ = this;

		aggregate_->first_ = this;

		if (composite_next_ != 0)
			composite_next_->composite_previous_ = this;
				
		composite_->first_ = this;
	}

	AggregateCompositeItem::~AggregateCompositeItem()
	{
		destroy();
	}

  void AggregateCompositeItem::persistentWrite(PersistenceManager& pm, const char* name) const
  {
		pm.writeObjectHeader(this, name);
			pm.writeObjectPointer(aggregate_, "aggregate_");
			pm.writeObjectPointer(composite_, "composite_");
			pm.writeObjectPointer(aggregate_next_, "aggregate_next_");
			pm.writeObjectPointer(composite_next_, "composite_next_");
			pm.writeObjectPointer(aggregate_previous_, "aggregate_previous_");
			pm.writeObjectPointer(composite_previous_, "composite_previous_");
		pm.writeObjectTrailer(name);
	}

  void AggregateCompositeItem::persistentRead(PersistenceManager& pm)
  {
		pm.readObjectPointer(aggregate_, "aggregate_");
		pm.readObjectPointer(composite_, "composite_");
		pm.readObjectPointer(aggregate_next_, "aggregate_next_");
		pm.readObjectPointer(composite_next_, "composite_next_");
		pm.readObjectPointer(aggregate_previous_, "aggregate_previous_");
		pm.readObjectPointer(composite_previous_, "composite_previous_");
	}
 
	void AggregateCompositeItem::destroy()
	{
		if (aggregate_ != 0)
		{
			--(aggregate_->size_);

			if (aggregate_->first_ == this)
			{
				if (aggregate_->last_ == this)
				{
					aggregate_->first_ = aggregate_->last_ = 0;

				} else {

					aggregate_->first_ = aggregate_next_;
				}
			} else {
				if (aggregate_->last_ == this)
				{
					aggregate_->last_ = aggregate_previous_;
				}

				aggregate_previous_->aggregate_next_ = aggregate_next_;
			}
		}

		if (aggregate_next_ != 0)
		{
			aggregate_next_->aggregate_previous_ = aggregate_previous_;
		}

		if (composite_ != 0)
		{
			if (composite_->first_ == this)
			{
				composite_->first_ = composite_next_;
			}
			else
			{
				composite_previous_->composite_next_ = composite_next_;
			}
		}

		if (composite_next_ != 0)
		{
			composite_next_->composite_previous_ = composite_previous_;
		}

		clear();
	}

	void AggregateCompositeItem::dump(ostream &s, unsigned long depth) const
	{
		BALL_DUMP_STREAM_PREFIX(s);
		
		BALL_DUMP_DEPTH(s, depth);
		BALL_DUMP_CLASS_HEADER(s, AggregateCompositeItem, this);
		
		BALL_DUMP_DEPTH(s, depth);
		s << "  this: " << getHandle() << endl;
		
		BALL_DUMP_DEPTH(s, depth);
		s << "aggregate: " 
				 << ((aggregate_ == 0) 
			 ? INVALID_HANDLE 
			 : aggregate_->getHandle()) << endl;

		BALL_DUMP_DEPTH(s, depth);
		s << "composite: " 
				 << ((composite_ == 0) 
			 ? INVALID_HANDLE 
			 : composite_->getHandle()) << endl;
		
		BALL_DUMP_DEPTH(s, depth);
		s << "aggregate previous item: " 
				 << ((aggregate_previous_ == 0) 
			 ? INVALID_HANDLE 
			 : aggregate_previous_->getHandle()) << endl;
		
		BALL_DUMP_DEPTH(s, depth);
		s << "aggregate next item: " 
				 << ((aggregate_next_ == 0) 
			 ? INVALID_HANDLE 
			 : aggregate_next_->getHandle()) << endl;
		
		BALL_DUMP_DEPTH(s, depth);
		s << "composite previous item: " 
				 << ((composite_previous_ == 0) 
			 ? INVALID_HANDLE 
			 : composite_previous_->getHandle()) << endl;
		
		BALL_DUMP_DEPTH(s, depth);
		s << "composite next item: " 
				 << ((composite_next_ == 0) 
			 ? INVALID_HANDLE 
			 : composite_next_->getHandle()) << endl;
		
		BALL_DUMP_STREAM_SUFFIX(s);
	}



	Aggregate::Aggregate()
		:	PersistentObject(),
			size_(0),
			first_(0),
			last_(0)
	{
	}

	Aggregate::Aggregate(const Aggregate& aggregate, bool deep)
		:	PersistentObject(),
			size_(0),
			first_(0),
			last_(0)
	{
		aggregate.clone(*this, deep);
	}

	Aggregate::~Aggregate()
	{
		destroy();
	}

	void Aggregate::persistentWrite(PersistenceManager& pm, const char* name) const
  {
		pm.writeObjectHeader(this, name);
			pm.writePrimitive(size_, "size_");
			pm.writeObjectPointer(first_, "first_");
			pm.writeObjectPointer(last_, "last_");
		pm.writeObjectTrailer(name);
	}

  void Aggregate::persistentRead(PersistenceManager& pm)
  {
		pm.readPrimitive(size_, "size_");
		pm.readObjectPointer(first_, "first_");
		pm.readObjectPointer(last_, "last_");
	}
 
	void Aggregate::set(const Aggregate& aggregate, bool deep)
	{
		if (&aggregate == this)
			return;

		destroy();

		Aggregate* aggregate_ptr = (Aggregate *)aggregate.create(deep);

		size_ = aggregate_ptr->size_;
		first_ = aggregate_ptr->first_;
		last_ = aggregate_ptr->last_;

		for(AggregateCompositeItem* composite_aggregate = first_; composite_aggregate != 0;
				composite_aggregate = composite_aggregate->aggregate_next_)
		{
			composite_aggregate->aggregate_ = this;
		}
	 
		aggregate_ptr->size_ = 0;
		aggregate_ptr->first_ = aggregate_ptr->last_ = 0;

		delete aggregate_ptr;
	}

	void* Aggregate::clone(Aggregate& root, bool deep) const
	{
		if (&root == this)
			return 0;

		root.destroy();

		if (deep == false)
		{
			for(AggregateCompositeItem* composite_aggregate = last_; composite_aggregate != 0;
					composite_aggregate = composite_aggregate->aggregate_previous_)
			{
				root.insert(*(composite_aggregate->composite_));
			}
		}
		else
		{
			for(AggregateCompositeItem* composite_aggregate  = last_; composite_aggregate != 0;
					composite_aggregate = composite_aggregate->aggregate_previous_)
			{
				root.insert(*(Composite *)composite_aggregate->composite_->create(true));
			}
		}

		return &root;
	}

	void Aggregate::destroyComposites(UnaryPredicate<Composite> &predicate)
	{
		AggregateCompositeItem *next = 0;
		
		for(AggregateCompositeItem* composite_aggregate = first_; composite_aggregate != 0;)
		{
			next = composite_aggregate->aggregate_next_;

			if (predicate(*composite_aggregate->composite_) == true)
			{
				if (composite_aggregate->composite_->isAutoDeletable() == true)
				{
					delete composite_aggregate->composite_;
				}

				delete composite_aggregate;
			}

			 composite_aggregate = next;
		}
	}

	bool Aggregate::removeComposites(UnaryPredicate<Composite> &predicate)
	{
		AggregateCompositeItem *next = 0;
		
		for(AggregateCompositeItem* composite_aggregate = first_; composite_aggregate != 0;)
		{
			next = composite_aggregate->aggregate_next_;

			if (predicate(*composite_aggregate->composite_) == true)
			{
				delete composite_aggregate;
			}
			
			 composite_aggregate = next;
		}

		return true;
	}

	void Aggregate::insert(Composite& composite)
	{
		AggregateCompositeItem* composite_aggregate = composite.find_(*this);
		
		if (composite_aggregate != 0)
			return;
				
		++size_;
				
		newItem(*this, composite, first_, composite.first_);
	}

	void Aggregate::clear(bool deep)
	{
		AggregateCompositeItem *next = 0;
		
		if (deep == false)
		{
			for(AggregateCompositeItem* composite_aggregate = first_; composite_aggregate != 0;)
			{
				next = composite_aggregate->aggregate_next_;

				delete composite_aggregate;

				composite_aggregate = next;
			}
		} else {
			for(AggregateCompositeItem* composite_aggregate = first_; composite_aggregate != 0;)
			{
				next = composite_aggregate->aggregate_next_;

				if (composite_aggregate->composite_->isAutoDeletable() == true)
				{
					delete composite_aggregate->composite_;
				}

				delete composite_aggregate;

				composite_aggregate = next;
			}
		}

		size_ = 0;
	}

	void Aggregate::splice(Aggregate &aggregate, UnaryPredicate<Composite> &predicate)
	{
		if (&aggregate == this)
			return;

		AggregateCompositeItem *previous = 0;

		for(AggregateCompositeItem* composite_aggregate = aggregate.last_; composite_aggregate != 0;
				 composite_aggregate = previous)
		{
			previous = composite_aggregate->aggregate_previous_;
			
			if (composite_aggregate->composite_->find_(*this) == 0 
					&& predicate(*composite_aggregate->composite_) == true)
			{
				insert(*(composite_aggregate->composite_));

				delete composite_aggregate;
			}
		}
	}

	Aggregate& Aggregate::join
		(Aggregate &a, Aggregate &b,
		 Aggregate &result,UnaryPredicate<Composite> &predicate)
	{
		if (&a == &b)
			return (result = a);

		if (&a == &result)
			return a.join(b, predicate);

		if (&b == &result)
			return b.join(a, predicate);

		result = a;

		for(AggregateCompositeItem* composite_aggregate = b.last_; composite_aggregate != 0;
				composite_aggregate = composite_aggregate->aggregate_previous_)
		{
			if (predicate(*composite_aggregate->composite_) == true
					&& composite_aggregate->composite_->find_(a) == 0)
			{
				result.insert(*(composite_aggregate->composite_));
			}
		}

		return result;
	}

	Aggregate& Aggregate::intersect
		(Aggregate &a, Aggregate &b,
		 Aggregate &result, UnaryPredicate<Composite> &predicate)
	{
		if (&a == &b)
			return (result = a);

		if (&a == &result)
			return a.intersect(b, predicate);

		if (&b == &result)
			return b.intersect(a, predicate);

		result.destroy();

		for(AggregateCompositeItem* composite_aggregate = a.last_; composite_aggregate != 0;
				composite_aggregate = composite_aggregate->aggregate_previous_)
		{
			if (predicate(*composite_aggregate->composite_) == true
					&& composite_aggregate->composite_->find_(b) != 0)
			{
				result.insert(*(composite_aggregate->composite_));
			}
		}

		return result;
	}

	Aggregate& Aggregate::subtract
		(Aggregate &a, Aggregate &b,
		 Aggregate &result, UnaryPredicate<Composite> &predicate)
	{
		if (&a == &b)
		{
			result.destroy();

			return result;
		}

		if (&a == &result)
			return a.subtract(b, predicate);

		if (&b == &result)
			return b.subtract(a, predicate);

		result.destroy();

		for(AggregateCompositeItem* composite_aggregate = a.last_; composite_aggregate != 0;
				composite_aggregate = composite_aggregate->aggregate_previous_)
		{
			if (predicate(*composite_aggregate->composite_) == true
					&& composite_aggregate->composite_->find_(b) == 0)
			{
				result.insert(*(composite_aggregate->composite_));
			}
		}

		return result;
	}

	Aggregate& Aggregate::symsubtract
		(Aggregate &a, Aggregate &b,
		 Aggregate& result, UnaryPredicate<Composite> &predicate)
	{
		if (&a == &b)
		{
			result.destroy();

			return result;
		}

		if (&a == &result)
			return a.symsubtract(b, predicate);

		if (&b == &result)
			return b.symsubtract(a, predicate);

		result.destroy();

		AggregateCompositeItem* previous = 0;
		AggregateCompositeItem* composite_aggregate = a.last_;

		for(; composite_aggregate != 0; composite_aggregate = previous)
		{
			previous = composite_aggregate->aggregate_previous_;
			
			if (predicate(*composite_aggregate->composite_) == false)
				continue;

			if (composite_aggregate->composite_->find_(b) == 0)
			{
				result.insert(*(composite_aggregate->composite_));
			}
		}

		previous = 0;
		composite_aggregate = b.last_;

		for(;composite_aggregate != 0; composite_aggregate= previous)
		{
			previous = composite_aggregate->aggregate_previous_;
			
			if (predicate(*composite_aggregate->composite_) == false)
				continue;

			if (composite_aggregate->composite_->find_(a) == 0)
				result.insert(*(composite_aggregate->composite_));
		}

		return result;
	}

	Aggregate &Aggregate::join(const Aggregate &aggregate,UnaryPredicate<Composite> &predicate)
	{
		if (&aggregate == this)
			return *this;

		for(AggregateCompositeItem* composite_aggregate = aggregate.last_; composite_aggregate != 0;
				composite_aggregate = composite_aggregate->aggregate_previous_)
		{
			if (predicate(*composite_aggregate->composite_) == true
					&& composite_aggregate->composite_->find_(*this) == 0)
			{
				insert(*(composite_aggregate->composite_));
			}
		}
		
		return *this;
	}

	Aggregate& Aggregate::intersect(const Aggregate &aggregate, UnaryPredicate<Composite> &predicate)
	{
		if (&aggregate == this)
			return *this;

		AggregateCompositeItem *previous = 0;

		for(AggregateCompositeItem* composite_aggregate = last_; composite_aggregate != 0; composite_aggregate = previous)
		{
			previous = composite_aggregate->aggregate_previous_;
			
			if (predicate(*composite_aggregate->composite_) == true
					&& composite_aggregate->composite_->find_(aggregate) == 0)
			{
				remove(*(composite_aggregate->composite_));
			}
		}

		return *this;
	}

	Aggregate& Aggregate::subtract(const Aggregate &aggregate, UnaryPredicate<Composite> &predicate)
	{
		if (&aggregate == this)
		{
			destroy();

			return *this;
		}

		AggregateCompositeItem *previous = 0;

		for(AggregateCompositeItem* composite_aggregate = last_; composite_aggregate != 0; composite_aggregate = previous)
		{
			previous = composite_aggregate->aggregate_previous_;
			
			if (predicate(*composite_aggregate->composite_) == true
					&& composite_aggregate->composite_->find_(aggregate) != 0)
			{
				remove(*(composite_aggregate->composite_));
			}
		}

		return *this;
	}

	Aggregate& Aggregate::symsubtract(const Aggregate &aggregate, UnaryPredicate<Composite> &predicate)
		 
	{
		if (&aggregate == this)
		{
			destroy();

			return *this;
		}

		AggregateCompositeItem *previous = 0;

		for(AggregateCompositeItem* composite_aggregate = aggregate.last_; composite_aggregate != 0; composite_aggregate = previous)
		{
			previous = composite_aggregate->aggregate_previous_;
			
			if (predicate(*composite_aggregate->composite_) == false)
				continue;

			if (composite_aggregate->composite_->find_(*this) != 0)
			{
				remove(*(composite_aggregate->composite_));
			}
			else
			{
				insert(*(composite_aggregate->composite_));
			}
		}

		return *this;
	}

	bool Aggregate::isHomomorph(const Aggregate &aggregate) const
	{
		if (&aggregate == this)
			return true;

		AggregateCompositeItem* a = last_;

		AggregateCompositeItem* b = aggregate.last_;

		for(; a != 0 && b != 0; a = a->aggregate_previous_, b = b->aggregate_previous_)
		{
			if (a->composite_->isHomomorph(*(b->composite_)) == false)
				return false;
		}

		return true;
	}

	bool Aggregate::isValid() const
	{
		Size size = 0;
		AggregateCompositeItem *last = 0;
		AggregateCompositeItem*  composite_aggregate = first_;
		
		for(; composite_aggregate != 0; composite_aggregate = composite_aggregate->aggregate_next_)
		{
			if (composite_aggregate->composite_->isValid() == false)
			{
#				ifdef BALL_DEBUG      

				Log.level(LogStream::ERROR) << "INVALID: substructure is invalid" << endl;

#				endif      
				
				return false;
			}

			if (composite_aggregate->aggregate_ != this)
			{
#				ifdef BALL_DEBUG      

				Log.level(LogStream::ERROR) << "INVALID: aggregate of root " << composite_aggregate->getHandle()
						 << " should be " << getHandle()
						 << " but is " << composite_aggregate->aggregate_->getHandle() << endl;

#				endif
				
				return false;
			}

			last = composite_aggregate;

			++size;

			if (composite_aggregate->aggregate_next_ == 0)
				break;
		}
		
		if (composite_aggregate != last_)
		{
#			ifdef BALL_DEBUG      

			Log.level(LogStream::ERROR) << "INVALID: last root of " << getHandle()
					 << " should be " << last_->getHandle()
					 << " but is " << composite_aggregate->getHandle() << endl;

#			endif      
			
			return false;
		}

		if (size != size_)
		{
#			ifdef BALL_DEBUG      

			Log.level(LogStream::ERROR) << "INVALID: number of roots " << getHandle()
					 << " should be " << size_
					 << " but is " << size << endl;

#			endif      

			return false;
		}

		size = 0;
		
		for(composite_aggregate = last; composite_aggregate != 0; composite_aggregate = composite_aggregate->aggregate_previous_)
		{
			++size;

			if (composite_aggregate->aggregate_previous_ == 0)
				break;
		}
		
		if (composite_aggregate != first_)
		{
#			ifdef BALL_DEBUG      

			Log.level(LogStream::ERROR) << "INVALID: first root of " << getHandle()
					 << " sould be " << first_->getHandle()
					 << " but is " << composite_aggregate->getHandle() << endl;

#			endif      

			return false;
		}
		
		if (size != size_)
		{
#			ifdef BALL_DEBUG      

			Log.level(LogStream::ERROR) << "INVALID: number of roots of " << getHandle()
					 << " should be " << size_
					 << " but is " << size << endl;

#			endif      

			return false;
		}

		return true;
	}

	bool Aggregate::applyAggregateCompositeItem(UnaryProcessor<AggregateCompositeItem>& processor)
	{
		if (processor.start() == false)
			return false;

		Processor::Result result;

		for (AggregateCompositeItem* composite_aggregate = first_; composite_aggregate != 0;
				 composite_aggregate = composite_aggregate->aggregate_next_)
		{
			result = processor(*composite_aggregate);

			if (result <= Processor::BREAK)
				return (result == Processor::BREAK) ? true : false;
		}

		return processor.finish();
	}

	bool Aggregate::applyCompositeRoot(UnaryProcessor<Composite>& processor)
	{
		if (processor.start() == false)
			return false;

		Processor::Result result;

		for (AggregateCompositeItem* composite_aggregate = first_; composite_aggregate != 0;
				 composite_aggregate = composite_aggregate->aggregate_next_)
		{
			result = processor(*composite_aggregate->composite_);

			if (result <= Processor::BREAK)
				return (result == Processor::BREAK) ? true : false;
		}

		return processor.finish();
	}

	bool Aggregate::applyComposite(UnaryProcessor<Composite>& processor)
	{
		if (processor.start() == false)
			return false;

		for (AggregateCompositeItem* composite_aggregate = first_; composite_aggregate != 0;
				 composite_aggregate = composite_aggregate->aggregate_next_)
		{
			if (composite_aggregate->composite_->applyPreorderNostart_(processor) == false)
				return false;
		}

		return processor.finish();
	}

	void Aggregate::dump(ostream &s, unsigned long depth) const
	{
		BALL_DUMP_STREAM_PREFIX(s);
		
		Object::dump(s, depth);

		BALL_DUMP_DEPTH(s, depth);
		s << "  first root: " 
				 << ((first_ == 0) 
			 ? INVALID_HANDLE 
			 : first_->getHandle()) << endl;
		
		BALL_DUMP_DEPTH(s, depth);
		s << "  last root: " 
				 << ((last_ == 0) 
			 ? INVALID_HANDLE 
			 : last_->getHandle()) << endl;

		BALL_DUMP_DEPTH(s, depth);
		s << "size: " << size_ << endl;
		
		for(AggregateCompositeItem* aggregate_composite = first_; aggregate_composite!= 0;
				aggregate_composite= aggregate_composite->aggregate_next_)
		{
			aggregate_composite->dump(s,depth + 1);
		}

		BALL_DUMP_STREAM_SUFFIX(s);
	}

#	ifdef BALL_NO_INLINE_FUNCTIONS
#		include <BALL/CONCEPT/composite.iC>
#	endif

} // namespace BALL
