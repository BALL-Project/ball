// $Id: resourceFile.C,v 1.11 2000/09/25 19:13:09 oliver Exp $

#include <BALL/FORMAT/resourceFile.h>

#include <stack>

using std::istream;
using std::ostream;
using std::endl;
using std::ios;
using std::streampos;
using std::stack;

namespace BALL 
{

	char ResourceFile::ENTRY_BEGIN = '<';
	char ResourceFile::ENTRY_END = '>';
	char ResourceFile::SEPARATOR = '/';

	ResourceEntry::ResourceEntry()
		:	AutoDeletable(),
			key_(""),
			value_(""),
			parent_(0),
			child_(0),
			number_children_(0)
	{
	}

	ResourceEntry::ResourceEntry(const ResourceEntry& entry, bool deep)
		:	AutoDeletable(),
			key_(entry.key_),
			value_(entry.value_),
			parent_(0),
			child_(0),
			number_children_(0)
	{
		if (deep == true)
		{
			ResourceEntry* cloned = entry.clone_(0);

			mergeChildrenOf(*cloned, true);

			delete cloned;
		}
	}

	ResourceEntry::ResourceEntry(const String& key, const String& value, ResourceEntry* parent)
		:	AutoDeletable(),
			key_(key),
			value_(value),
			parent_(parent),
			child_(0),
			number_children_(0)
	{
	}

	ResourceEntry::~ResourceEntry()
	{
		clear();
	}

	ResourceEntry* ResourceEntry::clone_(ResourceEntry* parent) const
	{
		ResourceEntry* cloned = newEntry(key_, value_, parent);

		cloned->number_children_ = number_children_;

		if (number_children_ != 0)
		{
			register ResourceEntry** cloned_child = cloned->child_ = newEntryArray(number_children_);

			for (register Index index = 0; index < (Index)number_children_; ++index)
			{
				*cloned_child++ = child_[index]->clone_(cloned);
			}
		}

		return cloned;
	}

	ResourceEntry& ResourceEntry::getRoot()
	{
		register ResourceEntry* entry = this;
		
		for (; entry->parent_ != 0; entry = entry->parent_);

		return* entry;
	}

	ResourceEntry* ResourceEntry::getEntry(const String& key_path)
	{
		char old = 0;
		const char* key = ((String &)key_path).c_str();
		register ResourceEntry* entry = this;

		if (*key == ResourceFile::SEPARATOR)
		{
			++key;
		}

		for (char* sep = const_cast<char*>(strchr(key, ResourceFile::SEPARATOR));
				 sep != 0; sep = const_cast<char*>(strchr(key, ResourceFile::SEPARATOR)))
		{
			old = *sep;
			*sep = 0;
			
			entry = entry->findChild(key);
			
			if (entry == 0)
			{
				return 0;
			}
		
			*sep = old;
			key = sep + 1;
		}
		
		if (entry != 0) 
		{
			return entry->findChild(key);
		} 
		else 
		{
			return 0;
		}
	}

	String ResourceEntry::getPath() const
	{
		String path(key_);

		for (register const ResourceEntry* parent = parent_;
				 parent != 0; parent = parent->parent_)
		{
			path = parent->getKey() + ResourceFile::SEPARATOR + path;
		}
		
		path = ResourceFile::SEPARATOR + path;

		return path;
	}

	Size ResourceEntry::countDescendants() const
	{
		Size size = number_children_;
		
		for (register Index index = 0; index < (Index)number_children_; ++index)
		{
			if (child_[index]->number_children_ > 0)
			{
				size += child_[index]->countDescendants();
			}
		}
		
		return size;
	}

	Size ResourceEntry::getDepth() const
	{
		Size depth = 0;

		for (register const ResourceEntry* entry = parent_;
				 entry != 0; entry = entry->parent_)
		{
			++depth;
		}
		
		return depth;
	}
		
	ResourceEntry* ResourceEntry::insertChild
		(const String& key, const String& value, bool replace_value)
	{
		ResourceEntry* entry = findChild(key);
		
		if (entry != 0)
		{
			if (replace_value == true)
			{
				entry->value_ = value;
			}

			return entry;
		}
		
		if (number_children_ == 0)
		{
			child_ = newEntryArray(1);
			
			entry = child_[0] = newEntry(key, value, this);
		} 
		else 
		{
			ResourceEntry** new_child = newEntryArray(number_children_ + 1);
			
			entry = newEntry(key, value, this);
			
			for (register Index new_index = 0, old_index = 0;
					 new_index < (Index)number_children_; ++new_index, ++old_index)
			{
				if (key < child_[old_index]->key_)
				{
					new_child[new_index] = entry;

					++number_children_;

					for (++new_index; new_index < (Index)number_children_; ++new_index, ++old_index)
					{
						new_child[new_index] = child_[old_index];
					}
		
					deleteEntryArray(child_);
					child_ = new_child;
		
					return entry;
				} 
				else 
				{
					new_child[new_index] = child_[old_index];
				}
			}

			new_child[number_children_] = entry;
			deleteEntryArray(child_);
			child_ = new_child;
		}
				
		++number_children_;
				
		return entry;
	}

	ResourceEntry* ResourceEntry::insertChild(ResourceEntry& entry, bool replace_value)
	{
		if (&entry == this || entry.isAncestorOf(*this) == true)
		{
			return 0;
		}

		ResourceEntry* entry_ptr = 0;
		String& key = entry.key_;

		if (entry.parent_ != 0)
		{
			entry.parent_->removeChild(key, &entry_ptr);
		}
		
		entry_ptr = findChild(key);
		
		if (entry_ptr != 0)
		{
			entry_ptr->clear();

			if (replace_value == true)
			{
				entry_ptr->value_ = entry.value_;
			}
			entry_ptr->child_ = entry.child_;
			entry_ptr->number_children_ = entry.number_children_;

			deleteEntry(&entry);
			
			return entry_ptr;
		}

		entry.parent_ = this;
		
		if (number_children_ == 0)
		{
			child_ = newEntryArray(1);
			
			entry_ptr = child_[0] = &entry;
		}
		else
		{
			ResourceEntry **new_child = newEntryArray(number_children_ + 1);
			
			entry_ptr = &entry;
			
			for (register Index new_index = 0, old_index = 0;
					 new_index < (Index)number_children_;	++new_index, ++old_index)
			{
				if (key < child_[old_index]->key_)
				{
					new_child[new_index] = entry_ptr;

					++number_children_;

					for (++new_index; new_index < (Index)number_children_; ++new_index, ++old_index)
					{
						new_child[new_index] = child_[old_index];
						}
		
					deleteEntryArray(child_);
					child_ = new_child;
		
					return entry_ptr;
				}
				else
				{
					new_child[new_index] = child_[old_index];
				}
			}

			new_child[number_children_] = entry_ptr;
			deleteEntryArray(child_);
			child_ = new_child;
		}
				
		++number_children_;
				
		return entry_ptr;
	}
		
	ResourceEntry* ResourceEntry::insert(const String& key_path, const String& name)
	{
		char oldc = 0;
		const char *key = ((String &)key_path).c_str();
		ResourceEntry *child_entry = 0;
		register ResourceEntry *entry = this;
		
		if (*key == ResourceFile::SEPARATOR)
		{
			++key;
		}

		for (char* sep = const_cast<char*>(strchr(key, ResourceFile::SEPARATOR));
				 sep != 0; sep = const_cast<char*>(strchr(key, ResourceFile::SEPARATOR)))
		{
			oldc = *sep;
			*sep = 0;

			child_entry = entry->findChild(key);
		
			if (child_entry == 0)
			{
				child_entry = entry->insertChild(key, "");
			}

			entry = child_entry;
		
			*sep = oldc;
			key = sep + 1;
		}
				
		return entry->insertChild(key, name.c_str());
	}

	bool ResourceEntry::mergeChildrenOf(ResourceEntry& resource_entry, bool replace_value)
	{
		if (&resource_entry == this || resource_entry.isAncestorOf(*this) == true)
		{
			return false;
		}

		while(resource_entry.number_children_ != 0)
		{
			insertChild(*(resource_entry.child_[0]), replace_value);
		}

		return true;
	}

	bool ResourceEntry::removeChild(const String& key, ResourceEntry** removed)
	{
		Index found = 0;
		
		if (findGreaterOrEqual_(key, found) == true)
		{
			if (removed == 0)
			{
				deleteEntry(child_[found]);
				child_[found] = 0;
			} 
			else 
			{
				*removed = child_[found];
			}
			
			for (register Index index = found + 1; index < (Index)number_children_; ++index)
			{
				child_[index - 1] = child_[index];
			}
			
			if (--number_children_ == 0)
			{
				deleteEntryArray(child_);
				
				child_ = 0;
			}
			
			return true;
		}
		else
		{
			return false;
		}
	}

	bool ResourceEntry::removeKey(const String& key_path)
	{
		char oldc = 0;
		const char *key = ((String &)key_path).c_str();
		register ResourceEntry *entry = this;

		if (*key == ResourceFile::SEPARATOR)
		{
			++key;
		}

		for (char *sep = const_cast<char*>(strchr(key, ResourceFile::SEPARATOR));
				 sep != 0; sep = const_cast<char*>(strchr(key, ResourceFile::SEPARATOR)))
		{
			oldc = *sep;
			*sep = 0;
			
			entry = entry->findChild(key);
			
			if (entry == 0)
			{
				return false;
			}
			
			*sep = oldc;
			key = sep + 1;
		}
		
		return (bool)entry->removeChild(key);
	}

	void ResourceEntry::clear()
	{
		for (register Index index = 0; index < (Index)number_children_; ++index)
		{
			deleteEntry(child_[index]);
		}
		
		deleteEntryArray(child_);

		child_ = 0;
		number_children_ = 0;
	}


	ResourceEntry* ResourceEntry::findDescendant(const String& key)
	{
		register ResourceEntry *entry = 0;
		
		for (register Index index = 0; index < (Index)number_children_; ++index)
		{
			entry = child_[index];
			
			if (entry->key_ == key)
			{
				return entry;
			}
			
			if (entry->number_children_ > 0)
			{
				entry = entry->findDescendant(key);
				
				if (entry != 0)
				{
					return entry;
				}
			}
		}
		
		return 0;
	}

	bool ResourceEntry::isAncestorOf(const ResourceEntry& resource_entry) const
	{
		for (register const ResourceEntry* entry = resource_entry.parent_; entry != 0; entry = entry->parent_)
		{
			if (entry == this)
			{
				return true;
			}
		}
		
		return false;
	}

	bool ResourceEntry::isValid() const
	{
		if (number_children_ == 0)
		{
			if (child_ != 0)
			{
				return false;
			}
		}
		else
		{
			if (child_ == 0)
			{
				return false;
			}

			for (register Index index = 0; index < (Index)number_children_; ++index)
			{
				if (child_[index]->isValid() == false)
				{
					return false;
				}
			}
		}
		
		return true;
	}

	void ResourceEntry::dump(ostream& s, Size depth) const
	{
		BALL_DUMP_STREAM_PREFIX(s);

		BALL_DUMP_DEPTH(s, depth);
		BALL_DUMP_CLASS_HEADER(s, ResourceEntry, this);

		BALL_DUMP_DEPTH(s, depth);
		s << "  key: " << key_ << endl;;

		BALL_DUMP_DEPTH(s, depth);
		s << "  parent: " << (void *)parent_ << endl;
		
		BALL_DUMP_DEPTH(s, depth);
		s << "  value: " << value_ << endl;
		
		BALL_DUMP_DEPTH(s, depth);
		s << "  size: " << number_children_ << endl;
		
		for (register Index index = 0; index < (Index)number_children_; ++index)
		{
			child_[index]->dump(s, depth + 1);
		}

		BALL_DUMP_STREAM_SUFFIX(s);
	}
	
	bool ResourceEntry::apply(UnaryProcessor<ResourceEntry>& processor)
	{		
		return processor.start() && applyNostart_(processor) && processor.finish();
	}
	
	bool ResourceEntry::applyNostart_(UnaryProcessor<ResourceEntry>& processor)
	{
		register Processor::Result result = Processor::ABORT;
		register ResourceEntry *entry = 0;
		
		for (register Index index = 0; index < (Index)number_children_; ++index)
		{
			entry = child_[index];
			result = processor(*entry);
			
			if (result <= Processor::BREAK)
			{
				return (result == Processor::BREAK) ? true : false;
			}
			
			if (entry->number_children_ > 0 && entry->applyNostart_(processor) == false)
			{
				return false;
			}
		}
		
		return true;
	}

	bool ResourceEntry::applyChildren(UnaryProcessor<ResourceEntry>& processor)
	{
		if (processor.start() == false)
		{
			return false;
		}

		register Processor::Result result = Processor::ABORT;
		
		for (register Index index = 0; index < (Index)number_children_; ++index)
		{
			result = processor(*child_[index]);
			
			if (result <= Processor::BREAK)
			{
				return (result == Processor::BREAK) ? true : false;
			}
		}
		
		return processor.finish();
	}

	bool  ResourceEntry::findGreaterOrEqual_(const String& key, Index& found) const
	{
		register Index lower_index = 0L;
		register Index upper_index = (Index)number_children_ - 1;
		register Index median_index;
		
		while (upper_index > lower_index)
		{
			median_index = (lower_index + upper_index) >> 1;
			
			if (child_[median_index]->key_ < key)
			{
				lower_index = median_index + 1;
			} 
			else 
			{
				upper_index = median_index;
			}
		}
		
		found = lower_index;

		return (bool)((lower_index == upper_index) && (child_[lower_index]->key_ == key));
	}      



	ResourceFile::ResourceFile()
		:	File(),
			root_()
	{
	}

	ResourceFile::ResourceFile(const String& name)
		:	File(),
			root_()
	{
		open(name);
	}

	ResourceFile::ResourceFile(const ResourceFile& resource_file)
		:	File(resource_file),
			root_()
	{
		if (good())
		{
			*this >> *this;
			if (eof())
			{
				clear();
			}
		}
	}

	ResourceFile::~ResourceFile()
	{
		close();
	}

	bool ResourceFile::open(const String& name)
	{
		if (File::open(name.c_str(), File::IN) && good())
		{
			*this >> *this;
			
			if (eof())
			{
				clear();
			}

		} 
		else 
		{
			clear(rdstate() | ios::failbit);
		}

		return (bool)good();
	}

	void ResourceFile::saveAs(const Entry& entry, const String& name)
	{
		File file(name.c_str(), File::OpenMode(File::OUT | File::TRUNC));
		
		Size depth = 0;
		
		if (entry.isEmpty() == false)
		{
			save_(file, &entry, depth);
		}

		file.close();
	}

	void ResourceFile::destroy()
	{
		root_.destroy();
	}

	void ResourceFile::dump(ostream& s, Size depth) const
	{
		BALL_DUMP_STREAM_PREFIX(s)

		BALL_DUMP_DEPTH(s, depth);
		s << "  tree: " << endl;
		root_.dump(s, depth + 1);

		BALL_DUMP_STREAM_SUFFIX(s)
	}

	istream& operator >> (istream& s, ResourceFile& resource_file)
	{
		resource_file.destroy();
		
		char c = 0;
		ResourceFile::Entry* entry = &(resource_file.root_);

		stack<String>	tag_stack;
		String tmp;
		String tag;
		String last_string;
		String last_node_tag;
		String key;
		String value;
		bool value_set = false;
		

		while (s.good())
		{
			s.get(c);

			if (c == ResourceFile::ENTRY_BEGIN)
			{
				// remember the last string
				last_string = tmp;
				last_string.trim();

				tmp = "";
				resource_file.skipWhitespaces_();

				s.get(c);
				while (!String::isSpace(c) && c != ResourceFile::ENTRY_END && s.good())
				{	
					tmp += c;
					s.get(c);
				}
				if (!s.good())
				{
					Log.error() << "ResourceFile:: error reading tag " << tmp << endl;
					return s;
				}
				tag = tmp;

				if (tag.size() == 0)
				{
					Log.error() << "ResourceFile: empty tag. " << endl;
					return s;
				}
				
				if (tag[0] == '/')
				{
					// closing tag -- check the stack
					if (tag_stack.size() == 0)
					{
						Log.error() << "ResourceFile: no opening tag for " << tag << endl;
						return s;
					}
					tmp = tag_stack.top();
					tag_stack.pop();
					if ("/" + tmp != tag)
					{
						Log.error() << "ResourceFile: tag " << tmp << " closed with " << tag << endl;
						return s;
					}
					
					if (tag == "/node")
					{
						if (last_node_tag == "node")
						{
							// create new entry	and stay on this level
							if (!value_set)
							{
								key = last_string;
								value = "";
							}
							entry = entry->insertChild(key, value);
							entry = entry->getParent();
							value_set = false;
						}
						if (last_node_tag == "/node")
						{
							entry = entry->getParent();
						}
						last_node_tag = "/node";
					}
					else if (tag == "/value")
					{
						value = last_string;
						value_set = true;
					} else
					{
						Log.error() << "ResourceFile: unknown tag " << tag << endl;
					}
				} 
				else 
				{
					if (tag == "node")
					{
						if (last_node_tag == "node")
						{
							// create new entry				
							if (!value_set)
							{
								key = last_string;
								value = "";
							}
							entry = entry->insertChild(key, value);
							value_set = false;
						}
						last_node_tag = "node";
					}
					else if (tag == "value")
					{
						key = last_string;
					} 
					else
					{
						Log.error() << "ResourceFile: unknown tag " << tag << endl;
					}
					// opening tag - push it on the stack
					tag_stack.push(tag);
				}
			
				while (c != ResourceFile::ENTRY_END && s.good())
				{
					s.get(c);
				}
				tmp = "";
			}
			else if (c == ResourceFile::ENTRY_END) 
			{
				Log.error() << "ResourceFile: read '>' without opening '<'" << endl;
			}
			else 
			{
				tmp += c;
			}
		}
		
		return s;
	}

	void ResourceFile::save_(File& file, register const Entry* entry, Size& depth)
	{
		Size l = 0;
		const Entry* child_entry = 0;
		
		++depth;
		
		for (register Index index = 0; index < (Index)entry->countChildren(); ++index)
		{
			child_entry = entry->getChild(index);
			
			for (l = 1; l < depth; l++)
			{
				file << "  ";
			}

			file	<< "<node>" << child_entry->getKey();
			if (child_entry->getValue() != "")
			{
				file << "<value>" << child_entry->getValue() << "</value>";
			}
			
			if (child_entry->countChildren() > 0)
			{
				file << endl;
				
				save_(file, child_entry, depth);
				
				for (l = 1; l < depth; l++)
				{
					file << "  ";
				}
			}
			
			file << "</node>" << endl;
		}

		--depth;
	}

#	ifdef BALL_NO_INLINE_FUNCTIONS
#		include <BALL/FORMAT/resourceFile.iC>
#	endif

} // namespace BALL
