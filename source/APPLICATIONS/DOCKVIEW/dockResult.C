// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: dockResult.C,v 1.1.2.10 2005/06/15 14:46:29 haid Exp $
//

#include <BALL/FORMAT/INIFile.h>
#include <BALL/FORMAT/PDBFile.h>
//#include <BALL/VIEW/KERNEL/mainControl.h>

#include "dockResult.h"

namespace BALL
{
		// Default Constructor
		DockResult::DockResult()
			throw()
			: conformation_set_(0)
		{}
		
		// Constructor
		DockResult::DockResult(const String& docking_algorithm, ConformationSet* conformation_set,
														const Options& docking_options)
			throw()
		{
			docking_algorithm_ = docking_algorithm;
			conformation_set_ = conformation_set;
			docking_options_ = docking_options;
		}
		
		// Destructor
		DockResult::~DockResult()
			throw()
		{
			if (conformation_set_ != 0) delete conformation_set_;
		}
		
		// Assignment operator
		const DockResult& DockResult::operator =(const DockResult& dock_res)
			throw()
		{
			if (&dock_res != this)
			{
				docking_algorithm_ = dock_res.docking_algorithm_;
				conformation_set_ = dock_res.conformation_set_;
				docking_options_ = dock_res.docking_options_;
				scorings_ = dock_res.scorings_;
			}
			return *this;
		}
		
		
		void DockResult::setConformationSet(ConformationSet* conformation_set)
			throw()
		{
			conformation_set_ = conformation_set;
		}
		
		const String& DockResult::getDockingAlgorithm() const
			throw()
		{
			return docking_algorithm_;
		}
		
		const Options& DockResult::getDockingOptions() const
			throw()
		{
			return docking_options_;
		}
		
		const ConformationSet* DockResult::getConformationSet() const
			throw()
		{
			return conformation_set_;
		}
		
		ConformationSet* DockResult::getConformationSet()
			throw()
		{
			return conformation_set_;
		}
		
		// returns scores of scoring_ i
		const vector<float>& DockResult::getScores(int i) const
			throw()
		{
			return scorings_[i].scores_;
		}
		
		//returns name of scoring function of scoring_ i
		const String& DockResult::getScoringName(int i) const
			throw()
		{
			return scorings_[i].name_;
		}
		
		//returns options of scoring function of scoring_ i
		const Options& DockResult::getScoringOptions(int i) const
			throw()
		{
			return scorings_[i].options_;
		}
		
		// returns the number of scorings
		Size DockResult::numberOfScorings() const
			throw()
		{
			return scorings_.size();
		}
		
		// add new Scoring_ to vector scorings_
		void DockResult::addScoring(const String& name, const Options& options, const vector<float>& scores)
			throw()
		{
			scorings_.push_back(Scoring_(name, options, scores));
		}
		
		// store dock result in a file
		bool DockResult::writeDockResult(const String& filename)
			throw()
		{
			// first: store information about algorithm/ scoring function in temporary INIFile
		 	String INI_temp;
			File::createTemporaryFilename(INI_temp);
			INIFile INI_out(INI_temp);
			INI_out.appendSection("ALGORITHM");
			INI_out.insertValue("ALGORITHM", "name", docking_algorithm_);
			INI_out.appendSection("ALGORITHM_OPTIONS");
			Options::ConstIterator it = docking_options_.begin();
			for (; +it; ++it)
			{
				INI_out.insertValue("ALGORITHM_OPTIONS", it->first, it->second);
			}
			for (unsigned int i = 0; i < scorings_.size(); i++)
			{
				String section = String("SCORING_NAME_") + String(i);
				INI_out.appendSection(section);
				INI_out.insertValue(section, "name", scorings_[i].name_);
				
				section = String("SCORING_OPTIONS_") + String(i);
				INI_out.appendSection(section);
				it = scorings_[i].options_.begin();
				for (; +it; ++it)
				{
					INI_out.insertValue(section, it->first, it->second);
				}
				
				section = String("SCORES_") + String(i);
				INI_out.appendSection(section);
				for (unsigned int j = 0; j < scorings_[i].scores_.size(); j++)
				{
					INI_out.insertValue(section, String(j), scorings_[i].scores_[j]);
				}
			}
			/// second: store docked system in a temporary PDBFile
			String PDB_temp;
			File::createTemporaryFilename(PDB_temp);
			PDBFile PDB_out(PDB_temp, std::ios::out);
			PDB_out << conformation_set_->getSystem();
			
			/// third: store trajectories in a temporary DCDFile
			String DCD_temp;
			File::createTemporaryFilename(DCD_temp);
			conformation_set_->writeDCDFile(DCD_temp);
			
			// before putting these 3 files into one, we need to know
			// how many lines the INIFile and PDBFile have
			PDB_out.reopen(std::ios::in);
			int line_nr_PDB = 0;
			while(PDB_out.LineBasedFile::readLine())
			{
			 	line_nr_PDB++;
			}
			// write the number of lines in the first 2 lines of the result file
			File result(filename, std::ios::out | std::ios::binary);
			result << INI_out.getNumberOfLines() << std::endl;
			result << line_nr_PDB << std::endl;
			
			/// write INIFile in File result
			INIFile::LineIterator lit = INI_out.getLine(0);
			for (; +lit; ++lit)
			{
				result << *lit << std::endl;
			}
			/// write PDBFile in File result
			PDB_out.reopen(std::ios::in);
			while(PDB_out.LineBasedFile::readLine())
			{
			 	result << PDB_out.getLine() << std::endl;
			}
			/// write DCDFile in File result
			std::ifstream DCD_file(DCD_temp.c_str(), std::ios::in | std::ios::binary);
			char c;
			while(DCD_file.good())
			{
				DCD_file.get(c);
				result << c;
			}
			DCD_file.close();
			
			// close result file; remove the 3 temporary files
			result.close();
			File::remove(INI_temp);
			File::remove(PDB_temp);
			File::remove(DCD_temp);
			
			return true;
		}
		
		//
		bool DockResult::readDockResult(const String& filename)
			throw()
		{
			// open result file for reading
			// read first two lines with line numbers of INIFile and PDBFile
			File file(filename, std::ios::in | std::ios::binary);
			unsigned int INI_lines, PDB_lines;
			file >> INI_lines;
			file >> PDB_lines;
			// first: read INI part from result file in INIFile
			INIFile INI_in;
			char buffer[2000];
			for (Position p = 0; p <= INI_lines; p++)
			{
				if (!file.getline(&(buffer[0]), 2000))
				{
					Log.error() << "Error while reading Dock Result file, could not read INI part" << std::endl;
					return false;
				}
    	
				if (!INI_in.appendLine(buffer)) 
				{
					Log.error() << "Error while reading Dock Result file, could not read INI part" << std::endl;
					return false;
				}
			}
			// read INIFile, fill DockResult
			// read algorithm name
			if (!INI_in.hasEntry("ALGORITHM", "name")) return false;
			docking_algorithm_ = INI_in.getValue("ALGORITHM", "name");
			// read algorithm options
			if (!INI_in.hasSection("ALGORITHM_OPTIONS")) return false;
			INIFile::LineIterator it = INI_in.getSectionFirstLine("ALGORITHM_OPTIONS");
			it.getSectionNextLine();
			for (; +it; it.getSectionNextLine())
			{
				String line(*it);
				docking_options_.set(line.before("="), line.after("="));
			}
			
			// there can be several stored scorings
			// assumption: there are not more than 9999999!!!
			for (Position p = 0; p < 9999999; p++)
			{
				// read scoring name
				if (!INI_in.hasEntry("SCORING_NAME_" + String(p), "name")) break;
        String name = INI_in.getValue("SCORING_NAME_" + String(p), "name");
				// read scoring options
        Options options;
				it = INI_in.getSectionFirstLine("SCORING_OPTIONS_" + String(p));
				it.getSectionNextLine();
				for (; +it; it.getSectionNextLine())
				{
				 	String line(*it);
					options.set(line.before("="), line.after("="));
				}
				// read scores
				vector<float> scores;
				it = INI_in.getSectionFirstLine("SCORES_" + String(p)); 
				it.getSectionNextLine();
				for (; +it; it.getSectionNextLine())
				{
					String line(*it);
				 	scores.push_back(((line.after("=")).toString()).toFloat());
				}
				// add new Scoring_ to vector scorings_
        addScoring(name, options, scores);
			}
			
			// second: read PDB part from result file in a temporary PDBFile
			String PDB_temp;
			File::createTemporaryFilename(PDB_temp);
			PDBFile PDB_in(PDB_temp, std::ios::out);
			for (Position p = 0; p < PDB_lines; p++)
			{
				if (!file.getline(&(buffer[0]), 2000))
				{
					Log.error() << "Error while reading Dock Result file, could not read PDB part" << std::endl;
					return false;
				}
				PDB_in << buffer << std::endl;
			}
			
			// read PDBFile, fill system
			PDB_in.reopen(std::ios::in);
			System s;
			PDB_in >> s;
			// create new ConformationSet and set the docked system
			conformation_set_ = new ConformationSet(s);
			// set scoring of ConformationSet
			vector<ConformationSet::Conformation> conformations;
			ConformationSet::Conformation conf;
			vector<float> last_scores = scorings_[scorings_.size()-1].scores_;
			for (unsigned int i = 0; i < last_scores.size(); i++)
			{
			 	conf.first = i;
				conf.second = last_scores[i];
				conformations.push_back(conf);
			}
			conformation_set_->setScoring(conformations);
			
			// third: read DCD part from result file in temporary DCDFile
			String DCD_temp;
			File::createTemporaryFilename(DCD_temp);
			std::ofstream DCD_in(DCD_temp.c_str(), std::ios::out | std::ios::binary);
			char c;
			while(file.good())
			{
				file.get(c);
				DCD_in << c;
			}
			DCD_in.close();
			// read the snapshots from DCDFile 
			if (!conformation_set_->readDCDFile(DCD_temp))
			{
				Log.error() << "Error while reading Dock Result file, could not read DCD part" << std::endl;
				return false;
			};
			
			// close result file and remove temporary files
			file.close();
			File::remove(PDB_temp);
			File::remove(DCD_temp);
			
			Log.info() << "---------------------------------------------------------------" << std::endl;
			Log.info() << "algorithm name: " << docking_algorithm_ << std::endl;
			Options::Iterator o_it = docking_options_.begin();
			for (; +o_it; ++o_it)
			{
				Log.info() << "algorithm options " << o_it->first << " : " << o_it->second << std::endl;
			}
			for (unsigned int i = 0; i < scorings_.size(); i++)
			{
			 	Log.info() << "scoring " << i << ":" << std::endl;
				Log.info() << "name: " << scorings_[i].name_ << std::endl;
				o_it = scorings_[i].options_.begin();
				for (; +o_it; ++o_it)
				{
					Log.info() << "scoring options " << o_it->first << " : " << o_it->second << std::endl;
				}
				for (unsigned int j = 0; j < scorings_[i].scores_.size(); j++)
				{
				 	Log.info() << "score " << j << ": " << scorings_[i].scores_[j] << std::endl;
				}
			}
			Log.info() << "---------------------------------------------------------------" << std::endl;
			
			return true;
		}
		
		/// delete i-th Scoring_ of vector scorings_
		void DockResult::deleteScoring(int i)
			throw()
		{
			vector<Scoring_>::iterator scoring_it;
			scorings_.erase(scorings_.begin()+i);	
		}
		
		
		/** Implementation of the nested class Scoring_ **/
	
		/** Scoring_ class
				Default Constructor
		*/
		DockResult::Scoring_::Scoring_() throw()
		{}
		
		/** Scoring_ class
				Constructor
		*/
		DockResult::Scoring_::Scoring_(const String& name, const Options& options, const vector<float>& scores) throw()
		{
			name_ = name;
			options_ = options;
			scores_ = scores;
		}
		
		/** Scoring_ class
				Destructor
		*/
		DockResult::Scoring_::~Scoring_() throw()
		{}
			
		/** Scoring_ class 
				Assignment operator
		*/
		const DockResult::Scoring_& DockResult::Scoring_::operator =(const Scoring_& scoring)
			throw()
		{
			if (&scoring != this)
			{
				name_ = scoring.name_;
				options_ = scoring.options_;
				scores_ = scoring.scores_;
			}
			return *this;
		}

		
	std::ostream& operator <<(ostream& out, const DockResult& dock_res)
		throw()
	{
		return out;
	}
	
} // end of namespace BALL
