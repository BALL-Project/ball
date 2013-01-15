// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//

#include <BALL/FORMAT/CIFFile.h>
#include <BALL/KERNEL/system.h>
#include <BALL/KERNEL/PTE.h>

#include <utility>

// defined in the lexer (CIFParserLexer.l)
extern void CIFParser_initBuffer(const char* buf);
extern void CIFParser_delBuffer();
extern void CIFParser_destroy();
extern int  CIFParserparse();
extern void CIFParserLexer_reset();

//extern int CIFParserdebug;

namespace BALL
{
	CIFFile::CIFFile()
		: File(),
			molecule_(0),
			current_datablock_(),
			current_saveframe_(),
			current_item_(),
			datablocks_hash_(),
			datablocks_()
	{
	}

	CIFFile::CIFFile(const String& name, File::OpenMode open_mode)
		: File(),
			molecule_(0),
			current_datablock_(),
			current_saveframe_(),
			current_item_(),
			datablocks_hash_(),
			datablocks_()
	{
		open(name, open_mode);
		// To Think:
		//	(if we just enable it here, we run into problems with derived classes...)
		//	read();
	}

	CIFFile::~CIFFile()
	{
		CIFParser_destroy();
	}

	bool CIFFile::write()
	{
		if (!isOpen() || getOpenMode() != std::ios::out)
		{
			throw(File::CannotWrite(__FILE__, __LINE__, name_));
		}	
		
		vector<Datablock>::iterator si;
		for (si = datablocks_.begin(); si != datablocks_.end(); si++)
		{
			*si >> getFileStream();
		}

		return true;
	}
	
	bool CIFFile::read()
	{
		if (!isValid())
		{
//			Log.error() << "Trying to read from invalid CIFFile '" << getName() << "'" << std::endl;
			return false;
		}
	
		try {
			CIFParserLexer_reset();
			state.current_parser = this;
			//CIFParserdebug = 1;
			CIFParserparse();
		}
		catch (Exception::ParseError& e)
		{
			Log.error() << "CIFFile: Cannot read " << getName() << std::endl;
		}
	
		return true;
	}
	
	void CIFFile::initRead_()
	{
	}

	void CIFFile::insertDatablock(const Datablock& datablock)
	{
		if ( !datablocks_hash_.has(datablock.name) )
		{
			datablocks_.push_back(datablock);
			datablocks_hash_.insert(datablock.name, datablocks_.size()-1);
			current_datablock_ = datablock.name;
		}
		else
		{
			Log.info()<<__FILE__<< __LINE__ << ": Key " << datablock.name << " already exists!" << std::endl;
		}
	}
	

	void CIFFile::setMolecule(Molecule* molecule)
	{
		molecule_ = molecule;
	}

	void CIFFile::clearParameters()
	{
		molecule_ = 0;
		datablocks_.clear();
		datablocks_hash_.clear();
		current_datablock_ = "";
		current_saveframe_ = "";
		current_item_ =  "";
	}

	///////////////////////// Item  ///////////////
	CIFFile::Item::Item() 
		: is_loop(false),
			keys(),
			values(),
			entry()
	{
	}
	
	CIFFile::Item::Item(const Item& item) 
		: is_loop(item.is_loop),
			keys(item.keys),
			values(item.values),
			entry(item.entry)
	{
	}

	 CIFFile::Item::~Item()
	{

	}

	void CIFFile::Item::clear()
	{
		is_loop = false;
		//data.clear();
		keys.clear();
		values.clear();
		//entry.clear();
		entry.first  = "";
		entry.second = "";
	}
	
	void CIFFile::Item::startLoop()
	{
		clear();
		is_loop = true;
	}
	
	void CIFFile::Item::addPair(String key, String value)
	{
		is_loop = false;
		entry.first = key;
		entry.second = value;
	}
	
	void CIFFile::Item::addTag(String tag)
	{
		keys.push_back(tag);
	}

	void CIFFile::Item::addValue(String value)
	{
		Size num_of_lines = values.size();
		if (num_of_lines == 0 || values[num_of_lines-1].size() == keys.size())
		{
			std::vector<String> tmp;
			values.push_back(tmp);
			num_of_lines++;
		}
		values[num_of_lines-1].push_back(value);
	}

	Index CIFFile::Item::getKeyIndex(String key) 
	{
		if (is_loop)
		{
			for (Position i=0; i<keys.size(); i++)
			{
				if (keys[i]==key)
					return i;
			}
		}
		return -1;
	}

	
	const CIFFile::Datablock& CIFFile::getDatablock(const String& name) const
	{
		return datablocks_[datablocks_hash_[name]];
	}
	
	CIFFile::Datablock& CIFFile::getDatablock(const String& name) 
	{
		return datablocks_[datablocks_hash_[name]];
	}

	std::ostream& CIFFile::Item::operator >> (std::ostream& os) const
	{
		if (is_loop)
		{
			String tmp;
			tmp = "\n\tloop_\n";
			// all keys
			std::vector<String>::const_iterator ki;
			for (ki = keys.begin(); ki !=keys.end(); ki++)
			{
				tmp += "\t\t_"+*ki+"\n";
			}
			tmp += "\n"; 
			// all values
			for (Size i=0; i < values.size(); i++)
			{
				tmp += "\t\t";
				for (Size j=0; j < values[i].size(); j++)
				{
					tmp += values[i][j]+"\t";
				}
				tmp += "\n";
			}
			tmp += "\n\tstop_\n\n";
			os << tmp;
		}
		else
		{
			os << "\t" << entry.first << "\t\t\t\t" ;
			
			if (entry.second[0]==';')
				os << "\n" <<  entry.second << std::endl;
			else	os << entry.second << std::endl;
		}
		return os;
	}
		
	///////////////////////// SaveFrame  ///////////////

	std::ostream& CIFFile::SaveFrame::operator >> (std::ostream& os) const
	{	
		String tmp = "\nsave_" + framename + "\n";
		os << tmp;
		std::vector<Item>::const_iterator ici;
		for (ici = items.begin(); ici != items.end(); ici++)
		{
			*ici >> os;
		}
		tmp = "\nsave_ \n\n";
		os << tmp;
		return os;
	}
	
	void CIFFile::SaveFrame::clear()
	{
		framename = "";
		category="";
		items.clear();
		pair_items.clear();
	}
	
	void CIFFile::SaveFrame::start(String name)
	{
		clear();
		framename = name;
	}
	
	void CIFFile::SaveFrame::addDataItem(Item item)
	{
		items.push_back(item);
		if (!item.is_loop)
		{
			pair_items.insert(item.entry.first, items.size()-1);
		}
	}
	
	const CIFFile::Item&  CIFFile::SaveFrame::getDataItem(const String& item_name) const 
	{
		if ( pair_items.has(item_name) )
		{
			return items[pair_items[item_name]];
		}
		else
		{
			Log.error() << "Warning non-existing DataItem requested, returning a dummy..." << std::endl;
			return dummy_data_item_; 	
		}

	}

	CIFFile::Item&  CIFFile::SaveFrame::getDataItem(const String& item_name) 
	{
		if ( pair_items.has(item_name) )
		{
			return items[pair_items[item_name]];
		}
		else
		{
			Log.error() << "Warning non-existing DataItem requested, returning a dummy..." << std::endl;
			return dummy_data_item_; 	
		}
	}	
	
	String&  CIFFile::SaveFrame::getDataItemValue(const String& item_name) 
	{
		if ( pair_items.has(item_name) )
		{
			if (!items[pair_items[item_name]].is_loop)
				return items[pair_items[item_name]].entry.second;
		}
		Log.error() << "Warning non-existing DataItem requested, returning a empty String..." << std::endl;
		return dummy_data_item_.entry.second; 	
	}
	
	const String&  CIFFile::SaveFrame::getDataItemValue(const String& item_name) const
	{
		if ( pair_items.has(item_name) )
		{
			if (!items[pair_items[item_name]].is_loop)
				return items[pair_items[item_name]].entry.second;
		}
		Log.error() << "Warning non-existing DataItem requested, returning a empty String..." << std::endl;
		return dummy_data_item_.entry.second; 	
	}


	
	bool CIFFile::SaveFrame::hasItem (const String& item_name) const 
	{
		return pair_items.has(item_name);
	}

	String CIFFile::SaveFrame::getItemValue(const String& name) const 
	{
		if (pair_items.has(name) &&  (!items[pair_items[name]].is_loop))
			return items[pair_items[name]].entry.second;
		else
			return "";
	}

	
	///////////////////////// Datacontent  ///////////////

	std::ostream& CIFFile::Datacontent::operator >> (std::ostream& os) const
	{	
		if (is_saveframe)
		{
			saveframe >> os;
		}
		else
		{
			dataitem >> os;
		}

		return os;
	}
	
	CIFFile::Datacontent::Datacontent()
			: is_saveframe(),
				saveframe(),
				dataitem()
	{
	}

	CIFFile::Datacontent::Datacontent(SaveFrame new_saveframe)
	{
		is_saveframe = true;
		saveframe = new_saveframe;
	}

	CIFFile::Datacontent::Datacontent(Item new_item)
	{
		is_saveframe = false;
		dataitem = new_item;
	}

	

		///////////////////////// Datablock  ///////////////
	
	
	std::ostream& CIFFile::Datablock::operator >> (std::ostream& os) const
	{
		String result = "data_" + name + "\n";	
		os << result;
		std::vector<Datacontent>::const_iterator dci;
		for (dci=data.begin(); dci != data.end(); dci++)
		{
			*dci >> os;
		}
		return os;
	}
	
	void CIFFile::Datablock::clear()
	{
		name = "";
		data.clear();
		saveframe_names.clear();
		saveframe_categories.clear();
		item_names.clear();
		dummy_data_item_.clear();
		dummy_saveframe_.clear();
		dummy_saveframes_.clear();
		dummy_indices_.clear();
	}
	
	void CIFFile::Datablock::start(String blockname)
	{
		clear();
		name = blockname;
	}

	void CIFFile::Datablock::setName(String blockname)
	{
		name = blockname;
	}
	
	void CIFFile::Datablock::insertDatacontent(const Datacontent& content)
	{
		/*std::cout << "--------------------------------"<< std::endl;
		content >> std::cout;
		std::cout << std::endl;
		std::cout << "--------------------------------"<< std::endl;
		std::cout << "all: " << data.size() << " names: "  << saveframe_names.size() << " cate:" << saveframe_categories.size() <<  " " << " " <<  saveframe_names.has(content.saveframe.framename) << " " << saveframe_categories.count(content.saveframe.category) << std::endl;
		std::cout << "--------------------------------"<< std::endl; */

		data.push_back(content);
		
		if (content.is_saveframe)
		{
			if (!saveframe_names.has(content.saveframe.framename))
			{
				saveframe_names.insert(content.saveframe.framename, data.size()-1);
		//	saveframe_categories.insert(content.saveframe.category,data.size()-1);
			}
			saveframe_categories.insert(std::pair<String, Index>( content.saveframe.category, data.size()-1));
			//std::cout << "insertDatacontent(): category " << content.saveframe.category << " has now " << saveframe_categories.count(content.saveframe.category) << " entries"  <<  std::endl;
			/*if (saveframe_categories.count(content.saveframe.category) == 0)
			{
				std::vector<Index> tmp;
				tmp.push_back(data.size()-1);
				std::cout << ">>>>>> " << tmp.size() << std::endl;
				//saveframe_categories[content.saveframe.category].push_back(data.size()-1);
				saveframe_categories.insert(content.saveframe.category,tmp);
				//saveframe_categories[content.saveframe.category] = tmp;

//				std::cout << ">>>>>> now: " << saveframe_categories[content.saveframe.category].size() << std::endl;
			}
			else
			{
				std::cout << " doubled category: add Index " <<  data.size()-1 << " belonging to " << content.saveframe.category << std::endl;
				std::cout <<  saveframe_categories[content.saveframe.category].size() << std::endl;
				std::vector<Index> tmp = saveframe_categories[content.saveframe.category];
				printf("dazwischen");
				tmp.push_back(data.size()-1);
				std::cout << "before überschreiben" << std::endl;
				//saveframe_categories.insert(content.saveframe.category,tmp);
				saveframe_categories[content.saveframe.category] = tmp;
				//(saveframe_categories[content.saveframe.category]).push_back(data.size()-1);
			}
			*/
		}

	}
	
	void CIFFile::Datablock::insertDatacontent(const SaveFrame& saveframe)
	{
		Datacontent dc(saveframe);
		insertDatacontent(dc);
			/*
		data.push_back(dc);
		if (dc.is_saveframe)
		{
			saveframes.insert(dc.saveframe.framename, &(data[data.size()-1].saveframe));
		}*/
	}

	void  CIFFile::Datablock::insertDatacontent(const Item& item) 
	{
		Datacontent dc(item);
		data.push_back(dc);
	}
	
	const CIFFile::Item& CIFFile::Datablock::getDataItem(const String& item_name) const
	{
		if ( item_names.has(item_name) )
		{
			return data[item_names[item_name]].dataitem;
		}
		else
		{
			Log.error() << "Warning non-existing DataItem requested, returning a dummy..." << std::endl;
			return dummy_data_item_; 	
		}
	}
	
	CIFFile::Item& CIFFile::Datablock::getDataItem(const String& item_name) 
	{
		if ( item_names.has(item_name) )
		{
			return data[item_names[item_name]].dataitem;
		}
		else
		{		
			Log.error() << "Warning non-existing DataItem requested, returning a dummy..." << std::endl;
			return dummy_data_item_; 	
		}

	}
	
	std::vector<CIFFile::SaveFrame>  CIFFile::Datablock::getSaveframesByCategory(const String& name) const
	{
		std::vector<SaveFrame> tmp; 
		if (saveframe_categories.count(name)>0)
		{
			std::pair<std::multimap<String, Index>::const_iterator, 
								std::multimap<String, Index>::const_iterator> range = saveframe_categories.equal_range(name);

			for (std::multimap<String, Index>::const_iterator it = range.first; it != range.second; it++)
			{
				tmp.push_back(data[it->second].saveframe);
			}

			return tmp;
		}
		else
		{		
			Log.error() << "Warning non-existing Saveframe requested, returning a dummy..." << std::endl;
			return dummy_saveframes_; 	
		}
	}

	const std::vector<Index>  CIFFile::Datablock::getSaveframeIndicesByCategory(const String& name) const
	{
		if (saveframe_categories.count(name)>0)
		{
			std::vector<Index> tmp;
			std::pair<std::multimap<String, Index>::const_iterator, 
								std::multimap<String, Index>::const_iterator> range = saveframe_categories.equal_range(name);

			for (std::multimap<String, Index>::const_iterator it = range.first; it != range.second; it++)
			{
				tmp.push_back(it->second);
			}

	
			return tmp;
		}
		else
		{		
			Log.error() << "Warning non-existing Saveframe requested, returning a dummy..." << std::endl;
			return dummy_indices_; 	
		}
	}

	std::vector<Index>  CIFFile::Datablock::getSaveframeIndicesByCategory(const String& name)
	{
		if (saveframe_categories.count(name)>0)
		{
			std::vector<Index> tmp;
			std::pair<std::multimap<String, Index>::iterator, 
								std::multimap<String, Index>::iterator> range = saveframe_categories.equal_range(name);

			for (std::multimap<String, Index>::iterator it = range.first; it != range.second; it++)
			{
				tmp.push_back(it->second);
			}

	
			return tmp;
		}
		else
		{		
			Log.error() << "Warning non-existing Saveframe requested, returning a dummy..." << std::endl;
			return dummy_indices_; 	
		}
	}

	const CIFFile::SaveFrame& CIFFile::Datablock::getSaveframeByName(const String& name) const 
	{
		if (saveframe_names.has(name))
		{
			return data[saveframe_names[name]].saveframe; 
		}
		else
		{		
			Log.error() << "Warning non-existing Saveframe requested, returning a dummy..." << std::endl;
			return dummy_saveframe_; 	
		}

	}
	
	CIFFile::SaveFrame& CIFFile::Datablock::getSaveframeByName(const String& name) 
	{
		if (saveframe_names.has(name))
		{
			return data[saveframe_names[name]].saveframe; 
		}
		else
		{		
			Log.error() << "Warning non-existing Saveframe requested, returning a dummy..." << std::endl;
			return dummy_saveframe_; 	
		}
	}
	
	const CIFFile::SaveFrame& CIFFile::Datablock::getSaveframeByIndex(const Index index) const 
	{
		if (index < (Index)data.size())
		{
			return data[index].saveframe;
		}
		else
		{
			Log.error() << "Warning non-existing Saveframe requested, returning a dummy..." << std::endl;
			return dummy_saveframe_;
		}
	}

	CIFFile::SaveFrame& CIFFile::Datablock::getSaveframeByIndex(const Index index) 
	{
		if (index < (Index)data.size())
		{
			return data[index].saveframe;
		}
		else
		{
			Log.error() << "Warning non-existing Saveframe requested, returning a dummy..." << std::endl;
			return dummy_saveframe_;
		}

	}

	bool CIFFile::Datablock::hasSaveframeName(const String& name) const 
	{
		if (saveframe_names.has(name))	return true;
		else  return false;
				
	}

	bool CIFFile::Datablock::hasSaveframeCategory(const String& name) const 
	{
		//std::cout << " hasSaveframeCategory(): looking for " << name  
		//					<< " " << saveframe_categories.count(name) << std::endl;
		if (saveframe_categories.count(name)>0)	return true;
		else  return false;
	}
	
	bool CIFFile::Datablock::hasItem(const String& name) const 
	{
		if (item_names.has(name))	return true;
		else  return false;
	}
	
	struct CIFFile::State CIFFile::state;
}
