// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: dockResult.C,v 1.3.16.2 2007/08/07 16:16:17 bertsch Exp $
//

#include <BALL/FORMAT/INIFile.h>
#include <BALL/FORMAT/PDBFile.h>
#include <BALL/DOCKING/COMMON/dockResult.h>

//#define BALL_VIEW_DEBUG
namespace BALL
{
		// Default Constructor
		DockResult::DockResult()
			throw()
			: conformation_set_(0),
				sorted_by_(0)
		{}

		// Constructor
		DockResult::DockResult(const String& docking_algorithm, ConformationSet* conformation_set, const Options& docking_options)
			throw()
			: sorted_by_(0)
		{
			docking_algorithm_ = docking_algorithm;
			conformation_set_ = conformation_set;
			docking_options_ = docking_options;
		}

		// Copy constructor.
		DockResult::DockResult(const DockResult& dock_res)
			throw()
			: docking_algorithm_(dock_res.docking_algorithm_),
				docking_options_(dock_res.docking_options_),
				conformation_set_(dock_res.conformation_set_),
				scorings_(dock_res.scorings_),
				sorted_by_(dock_res.sorted_by_)
		{}

		// Destructor
		DockResult::~DockResult()
			throw()
		{
			if (conformation_set_)
			{
				delete conformation_set_;
			}
		}

		// Assignment operator
		const DockResult& DockResult::operator = (const DockResult& dock_res)
			throw()
		{
			if (&dock_res != this)
			{
				docking_algorithm_ = dock_res.docking_algorithm_;
				if (conformation_set_)
				{
					delete conformation_set_;
				}
				conformation_set_ = dock_res.conformation_set_;
				docking_options_ = dock_res.docking_options_;
				scorings_ = dock_res.scorings_;
				sorted_by_ = dock_res.sorted_by_;
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

		void DockResult::sortBy(Index scoring_index)
			throw(Exception::IndexOverflow)
		{
			if (scoring_index >= (Index)scorings_.size())
			{
				throw Exception::IndexOverflow(__FILE__, __LINE__, scoring_index, (Size)(scorings_.size()-1));
			}

			sorted_by_ = scoring_index;
		}

		Index DockResult::isSortedBy() const
			throw()
		{
			return sorted_by_;
		}

		// i -> score row, j -> scoring column
		float DockResult::operator()(Position i, Position j)
			throw(Exception::IndexOverflow)
		{
			// check if i and j are valid
			if (j >= scorings_.size())
			{
				throw Exception::IndexOverflow(__FILE__, __LINE__, (Index)j, (Size)(scorings_.size()-1));
			}
			if (i >= scorings_[j].scores_.size())
			{
				throw Exception::IndexOverflow(__FILE__, __LINE__, (Index)i, (Size)(scorings_[j].scores_.size()-1));
			}

			if (sorted_by_ < 0)
			{
				return scorings_[j].scores_[i];
			}
			else
			{
				// get to know the snaphot index for the score
				Index snapshot_index = scorings_[sorted_by_].snapshot_order_[i];
				// get score of scoring j that belongs to the snapshot index
				return scorings_[j].scores_[snapshot_index];
			}
		}

		// returns sorted scores of scoring_ i
		const vector < ConformationSet::Conformation > DockResult::getScores(Position i) const
			throw(Exception::IndexOverflow)
		{
			if (i >= scorings_.size())
			{
				throw Exception::IndexOverflow(__FILE__, __LINE__, (Index)i, (Size)(scorings_.size()-1));
			}

			vector < ConformationSet::Conformation > conformations;
			for (Position j = 0; j < scorings_[i].snapshot_order_.size(); j++)
			{
				Index ss_index = scorings_[i].snapshot_order_[j];
				ConformationSet::Conformation c(ss_index, scorings_[i].scores_[ss_index]);
				conformations.push_back(c);
			}
			return conformations;
		}

		//returns name of scoring function of scoring_ i
		const String& DockResult::getScoringName(Position i) const
			throw(Exception::IndexOverflow)
		{
			if (i >= scorings_.size())
			{
				throw Exception::IndexOverflow(__FILE__, __LINE__, (Index)i, (Size)(scorings_.size()-1));
			}

			return scorings_[i].name_;
		}

		//returns options of scoring function of scoring_ i
		const Options& DockResult::getScoringOptions(Position i) const
			throw(Exception::IndexOverflow)
		{
			if (i >= scorings_.size())
			{
				throw Exception::IndexOverflow(__FILE__, __LINE__, (Index)i, (Size)(scorings_.size()-1));
			}

			return scorings_[i].options_;
		}

		// returns the number of scorings
		Size DockResult::numberOfScorings() const
			throw()
		{
			return scorings_.size();
		}

		// add new Scoring_ to vector scorings_
		// the score vector has to be sorted
		void DockResult::addScoring(const String& name, const Options& options, vector < ConformationSet::Conformation > scores)
			throw()
		{
			Compare_ compare_func = Compare_();
			sort(scores.begin(), scores.end(), compare_func);
			// split up vector of Conformations
			vector<Index> snapshot_order(scores.size());
			vector<float> score_values(scores.size());
			for (Position i = 0; i < scores.size(); i++)
			{
				snapshot_order[i] = scores[i].first;
				score_values[scores[i].first] = scores[i].second;
			}
			scorings_.push_back(Scoring_(name, options, score_values, snapshot_order));
		}

		/// delete i-th Scoring_ of vector scorings_
		void DockResult::deleteScoring(Position i)
			throw(Exception::IndexOverflow)
		{
			if (i >= scorings_.size())
			{
				throw Exception::IndexOverflow(__FILE__, __LINE__, (Index)i, (Size)(scorings_.size()-1));
			}

			vector<Scoring_>::iterator scoring_it;
			scorings_.erase(scorings_.begin()+i);
		}

		// store dock result in a file
		bool DockResult::writeDockResult(const String& filename)
			throw()
		{
			File result(filename, std::ios::out | std::ios::binary);
			bool successful = writeDockResult(result);
			result.close();
			return successful;
		}

		// store dock result in a file
		bool DockResult::writeDockResult(std::ostream& result) const
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
			for (; it != docking_options_.end(); ++it)
			{
				INI_out.insertValue("ALGORITHM_OPTIONS", it->first, it->second);
			}
			for (Position i = 0; i < scorings_.size(); i++)
			{
				String section = String("SCORING_NAME_") + String(i);
				INI_out.appendSection(section);
				INI_out.insertValue(section, "name", scorings_[i].name_);

				section = String("SCORING_OPTIONS_") + String(i);
				INI_out.appendSection(section);
				it = scorings_[i].options_.begin();
				for (; it != scorings_[i].options_.end(); ++it)
				{
					INI_out.insertValue(section, it->first, it->second);
				}

				section = String("SCORES_") + String(i);
				INI_out.appendSection(section);
				for (Position j = 0; j < scorings_[i].scores_.size(); j++)
				{
					INI_out.insertValue(section, String(j), String(scorings_[i].scores_[j]));
				}

				section = String("SNAPSHOT_ORDER_") + String(i);
				INI_out.appendSection(section);
				for (Position j = 0; j < scorings_[i].snapshot_order_.size(); j++)
				{
					INI_out.insertValue(section, String(j), String(scorings_[i].snapshot_order_[j]));
				}
			}
			// second: store docked system in a temporary PDBFile
			String PDB_temp;
			File::createTemporaryFilename(PDB_temp);
			PDBFile PDB_out(PDB_temp, std::ios::out);
			PDB_out << conformation_set_->getSystem();

			// third: store trajectories in a temporary DCDFile
			String DCD_temp;
			File::createTemporaryFilename(DCD_temp);
			conformation_set_->writeDCDFile(DCD_temp);

			// before putting these 3 files into one, we need to know
			// how many lines the INIFile and PDBFile have
			PDB_out.reopen(std::ios::in);
			Size line_nr_PDB = 0;
			while (PDB_out.LineBasedFile::readLine())
			{
				line_nr_PDB++;
			}
			// write the number of lines in the first 2 lines of the result file
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
			while (PDB_out.LineBasedFile::readLine())
			{
				result << PDB_out.getLine() << std::endl;
			}
			/// write DCDFile in File result
			std::ifstream DCD_file(DCD_temp.c_str(), std::ios::in | std::ios::binary);
			char c;
			while (DCD_file.good())
			{
				DCD_file.get(c);
				result << c;
			}
			DCD_file.close();

			// remove the 3 temporary files
			File::remove(INI_temp);
			File::remove(PDB_temp);
			File::remove(DCD_temp);

			return true;
		}

		// read dock result from file
		bool DockResult::readDockResult(const String& filename)
			throw()
		{
			File file(filename, std::ios::in | std::ios::binary);
			bool successful = readDockResult(file);
			file.close();
			return successful;
		}

		// read dock result from file
		bool DockResult::readDockResult(std::istream& file)
			throw()
		{
			// read first two lines with line numbers of INIFile and PDBFile
			Size INI_lines, PDB_lines;
			file >> INI_lines;
			file >> PDB_lines;
			// first: read INI part from result file in INIFile
			INIFile INI_in;
			char buffer[2000];
			for (Position p = 0; p <= INI_lines; p++)
			{
				if (!file.getline(&(buffer[0]), 2000))
				{
					Log.error() << "Error while reading Dock Result file, could not read INI part! " << __FILE__ << " " << __LINE__ << std::endl;
					return false;
				}

				if (!INI_in.appendLine(buffer))
				{
					Log.error() << "Error while reading Dock Result file, could not read INI part! "  << __FILE__ << " " << __LINE__<< std::endl;
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
					try
						{
							scores.push_back(((line.after("=")).toString()).toFloat());
						}
					catch (Exception::InvalidFormat)
						{
							Log.error() << "Conversion from String to float failed: invalid format! " << __FILE__ << " " << __LINE__ << std::endl;
							return false;
						}
				}
				// read snapshot order
				vector<Index> snapshot_order;
				it = INI_in.getSectionFirstLine("SNAPSHOT_ORDER_" + String(p));
				it.getSectionNextLine();
				for (; +it; it.getSectionNextLine())
				{
					String line(*it);
					try
						{
							snapshot_order.push_back(((line.after("=")).toString()).toInt());
						}
					catch (Exception::InvalidFormat)
						{
							Log.error() << "Conversion from String to int failed: invalid format! " << __FILE__ << " " << __LINE__ << std::endl;
							return false;
						}
				}
				// add new Scoring_ to vector scorings_
				scorings_.push_back(Scoring_(name, options, scores, snapshot_order));
			}

			// second: read PDB part from result file in a temporary PDBFile
			String PDB_temp;
			File::createTemporaryFilename(PDB_temp);
			PDBFile PDB_in(PDB_temp, std::ios::out);
			for (Position p = 0; p < PDB_lines; p++)
			{
				if (!file.getline(&(buffer[0]), 2000))
				{
					Log.error() << "Error while reading Dock Result file, could not read PDB part! " << __FILE__ << " " << __LINE__ << std::endl;
					return false;
				}
				PDB_in << buffer << std::endl;
			}

			// read PDBFile, fill system
			PDB_in.reopen(std::ios::in);
			System s;
			PDB_in >> s;
			// create new ConformationSet and set the docked system
			if (conformation_set_)
			{
				delete conformation_set_;
			}
			conformation_set_ = new ConformationSet(s);
			// set scoring of ConformationSet
			vector < ConformationSet::Conformation > conformations;
			ConformationSet::Conformation conf;
			vector<float> last_scores = scorings_[scorings_.size()-1].scores_;
			for (Position i = 0; i < last_scores.size(); i++)
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
			while (file.good())
			{
				file.get(c);
				DCD_in << c;
			}
			DCD_in.close();
			// read the snapshots from DCDFile
			if (!conformation_set_->readDCDFile(DCD_temp))
			{
				Log.error() << "Error while reading Dock Result file, could not read DCD part! " << __FILE__ << " " << __LINE__ << std::endl;
				return false;
			};

			// remove temporary files
			File::remove(PDB_temp);
			File::remove(DCD_temp);

			return true;
		}

		/** Implementation of the nested class Scoring_ **/

		/** Scoring_ class
			* Default Constructor
			*/
		DockResult::Scoring_::Scoring_() throw()
		{}

		/** Scoring_ class
			* Constructor
			*/
		DockResult::Scoring_::Scoring_(const String& name, const Options& options, const vector<float>& scores, const vector<Index>& snapshot_order) throw()
		{
			name_ = name;
			options_ = options;
			scores_ = scores;
			snapshot_order_ = snapshot_order;
		}

		/** Scoring_ class
			* Copy constructor.
			*/
		DockResult::Scoring_::Scoring_(const Scoring_& scoring)
			throw()
			: name_(scoring.name_),
				options_(scoring.options_),
				scores_(scoring.scores_),
				snapshot_order_(scoring.snapshot_order_)
		{}

		/** Scoring_ class
				Destructor
		*/
		DockResult::Scoring_::~Scoring_() throw()
		{}

		/** Scoring_ class
				Assignment operator
		*/
		const DockResult::Scoring_& DockResult::Scoring_::operator = (const Scoring_& scoring)
			throw()
		{
			if (&scoring != this)
			{
				name_ = scoring.name_;
				options_ = scoring.options_;
				scores_ = scoring.scores_;
				snapshot_order_ = scoring.snapshot_order_;
			}
			return *this;
		}

		/*implementation of nested class Compare_
		*/
		// default constructor
		DockResult::Compare_::Compare_() throw()
		{}

		// destructor
		DockResult::Compare_::~Compare_() throw()
		{}

		// operator ()
		bool DockResult::Compare_::operator() (const ConformationSet::Conformation& a, const ConformationSet::Conformation& b) const
		//bool DockResult::Compare_::operator() (const std::pair<Index, float>& a, const std::pair<Index, float>& b) const
			throw()
		{ return a.second < b.second; }

		std::ostream& operator <<(std::ostream& out, const DockResult& dock_res)
			throw()
		{
			if (!dock_res.writeDockResult(out))
			{
				Log.error() << "Could not write dock result! " << __FILE__ << " " << __LINE__ << std::endl;
			}
			return out;
		}

		std::istream& operator >>(std::istream& in, DockResult& dock_res)
			throw()
		{
			if (!dock_res.readDockResult(in))
			{
				Log.error() << "Could not read dock result file! " << __FILE__ << " " << __LINE__ << std::endl;
			}
			return in;
		}

} // end of namespace BALL
