// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: CIFFile.C,v 1.2 2007/08/22 10:08:20 anne Exp $
//

#include <BALL/FORMAT/CIFFile.h>
#include <BALL/KERNEL/system.h>
#include <BALL/KERNEL/PTE.h>

// defined in the lexer (CIFParserLexer.l)
extern void CIFParser_initBuffer(const char* buf);
extern void CIFParser_delBuffer();
extern int  CIFParserparse();

namespace BALL
{
	CIFFile::CIFFile()
		: File(),
			molecule_(0),
			datablocks_hash_(),
			datablocks_()
	{
	}

	CIFFile::CIFFile(const CIFFile& file)
		throw(Exception::FileNotFound)
		:	File(),
			molecule_(file.molecule_),
			current_datablock_(file.current_datablock_),
			datablocks_hash_(file.datablocks_hash_),
			datablocks_(file.datablocks_)
	{
		if (file.getName() != "")
		{
			try
			{	
				open(file.getName());
			}
			catch (Exception::FileNotFound)
			{
			}
		}
	}
	
	CIFFile::CIFFile(const String& name, File::OpenMode open_mode)
		throw(Exception::FileNotFound)
		: File(),
			molecule_(0)
	{
		open(name, open_mode);
		// TO THink
	//	read();
	}

	CIFFile::~CIFFile()
	{
	}

	const CIFFile& CIFFile::operator = (const CIFFile& rhs)
		throw(Exception::FileNotFound)
	{
		File::operator = (rhs);

		molecule_ = rhs.molecule_;
		current_datablock_ = rhs.current_datablock_;
		datablocks_ = rhs.datablocks_;
		datablocks_hash_ = rhs.datablocks_hash_;

		return *this;
	}

	bool CIFFile::write()
		throw(File::CannotWrite)
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
		throw(Exception::ParseError)
	{
		if (!isValid())
		{
//			Log.error() << "Trying to read from invalid CIFFile '" << getName() << "'" << std::endl;
			return false;
		}
	
		try {
			state.current_parser = this;
			CIFParserparse();
		}
		catch (Exception::ParseError& e)
		{
			Log.error() << "Cannot read " << getName() << std::endl;
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
			datablocks_hash_.insert(datablock.name, &(datablocks_[datablocks_.size()-1]));
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
		datablocks_.clear();
		datablocks_hash_.clear();
		current_datablock_ = "";
		current_saveframe_ = "";
		current_item_ =  "";
	}

	///////////////////////// Item  ///////////////
	void CIFFile::Item::clear()
	{
		is_loop = false;
		//data.clear();
		keys.clear();
		values.clear();
		std::pair<String, String> tmp;
		entry = tmp;
	}
	
	void CIFFile::Item::startLoop()
	{
		clear();
		is_loop = true;
	}
	
	void CIFFile::Item::addPair(String key, String value)
	{
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

	void CIFFile::Item::operator >> (ostream& os) const
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
		return;
	}
		
	///////////////////////// SaveFrame  ///////////////

	void CIFFile::SaveFrame::operator >> (ostream& os) const
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
		return;
	}
	
	void CIFFile::SaveFrame::clear()
	{
		framename = "";
		items.clear();
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
			Item* last_item = &(items[items.size()-1]);
			pair_items.insert((last_item->entry).first, last_item);
		}
	}
	
	///////////////////////// Datacontent  ///////////////

	void CIFFile::Datacontent::operator >> (ostream& os) const
	{	
		if (is_saveframe)
		{
			saveframe >> os;
		}
		else
		{
			dataitem >> os;
		}
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
	
	const CIFFile::Datablock* CIFFile::getDatablock(const String& name)
	{
		return datablocks_hash_[name];
	}

	void CIFFile::Datablock::operator >> (ostream& os) const
	{
		String result = "data_" + name + "\n";	
		os << result;
		std::vector<Datacontent>::const_iterator dci;
		for (dci=data.begin(); dci != data.end(); dci++)
		{
			*dci >> os;
		}
		return;
	}
	
	void CIFFile::Datablock::clear()
	{
		name = "";
		data.clear();
		saveframe_names.clear();
		saveframe_categories.clear();
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
	
	void CIFFile::Datablock::insertDatacontent(Datacontent content)
	{
		data.push_back(content);
		if (content.is_saveframe)
		{
			SaveFrame* sf = &(data[data.size()-1].saveframe);
			saveframe_names.insert(sf->framename, sf);
			if (saveframe_categories.has(sf->category))
			{
				saveframe_categories[sf->category].push_back(sf);
			}
			else
			{
				std::vector<SaveFrame*> tmp;
				tmp.push_back(sf);
				saveframe_categories.insert(sf->category,tmp);
			}
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

	void  CIFFile::Datablock::insertDatacontent(const Item item) 
	{
		Datacontent dc(item);
		data.push_back(dc);
	}


	struct CIFFile::State CIFFile::state;
}
