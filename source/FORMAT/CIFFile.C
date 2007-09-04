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
			datablocks_()
	{
	}

	CIFFile::CIFFile(const CIFFile& file)
		throw(Exception::FileNotFound)
		:	File(),
			molecule_(file.molecule_),
			current_datablock_(file.current_datablock_),
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

		return *this;
	}

	bool CIFFile::write()
		throw(File::CannotWrite)
	{
		if (!isOpen() || getOpenMode() != std::ios::out)
		{
			throw(File::CannotWrite(__FILE__, __LINE__, name_));
		}	
		
		StringHashMap<Datablock>::ConstIterator si;
		for (si = datablocks_.begin(); si != datablocks_.end(); si++)
		{
			si->second >> getFileStream();
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
		if ( !datablocks_.has(datablock.name) )
		{
			datablocks_.insert(datablock.name, datablock);
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
			tmp = "\tloop_\n";
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
			tmp += "\tstop_\n";
			os << tmp;
		}
		else
		{
			char line[255]; 
			sprintf(line, " %33s \t%33s\n", entry.first.c_str(), entry.second.c_str());
			os << line; 
		}
		return;
	}
		
	///////////////////////// SaveFrame  ///////////////

	void CIFFile::SaveFrame::operator >> (ostream& os) const
	{	
		String tmp = "save_" + framename + "\n";
		os << tmp;
		std::vector<Item>::const_iterator ici;
		for (ici = items.begin(); ici != items.end(); ici++)
		{
			*ici >> os;
		}
		tmp = "save_ \n";
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
	}
	
	///////////////////////// Datacontent  ///////////////

	void CIFFile::Datacontent::operator >> (ostream& os) const
	{	
		if (is_saveframe)
		{
			sframe >> os;
		}
		else
		{
			dataitem >> os;
		}
	}
	
	CIFFile::Datacontent::Datacontent()
			: is_saveframe(),
				sframe(),
				dataitem()
	{
	}

	CIFFile::Datacontent::Datacontent(SaveFrame new_saveframe)
	{
		is_saveframe = true;
		sframe = new_saveframe;
	}

	CIFFile::Datacontent::Datacontent(Item new_item)
	{
		is_saveframe = false;
		dataitem = new_item;
	}

	

		///////////////////////// Datablock  ///////////////
	
	const CIFFile::Datablock CIFFile::getDatablock(const String& name)
	{
		return datablocks_[name];
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
	}
	
	void CIFFile::Datablock::insertDatacontent(const SaveFrame& saveframe)
	{
		Datacontent dc(saveframe);
		//(datablocks_[current_datablock_]).
		data.push_back(dc);
	}

	void  CIFFile::Datablock::insertDatacontent(const Item item) 
	{
		Datacontent dc(item);
		//(datablocks_[current_datablock_]).
		data.push_back(dc);
	}


	struct CIFFile::State CIFFile::state;
}
