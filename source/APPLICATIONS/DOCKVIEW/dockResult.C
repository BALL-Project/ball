// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: dockResult.C,v 1.1.2.1 2005/04/01 14:30:22 haid Exp $
//

#include "dockResult.h"

namespace BALL
{
	namespace VIEW
	{
		// Default Constructor
		DockResult::DockResult()
			throw()
		{}
		
		// Constructor
		DockResult::DockResult(QString docking_algorithm, ConformationSet& conformation_set,
															Options& docking_options, String DCD_file)
			throw()
		{
			docking_algorithm_ = docking_algorithm;
			conformation_set_ = conformation_set;
			docking_options_ = docking_options;
			DCD_file_ = DCD_file;
		}
		
		// Destructor
		DockResult::~DockResult()
			throw()
		{}
		
		/** Assignment operator
		*/
		const DockResult& DockResult::operator =(const DockResult& dock_res)
		{
			if (&dock_res != this)
			{
				docking_algorithm_ = dock_res.docking_algorithm_;
				conformation_set_ = dock_res.conformation_set_;
				docking_options_ = dock_res.docking_options_;
				scorings_ = dock_res.scorings_;
				DCD_file_ = dock_res.DCD_file_;
			}
			return *this;
		}
		
		const QString& DockResult::getDockingAlgorithm() const
			throw()
		{return docking_algorithm_;}
					
		const ConformationSet& DockResult::getConformationSet() const
			throw()
		{return conformation_set_;}
		
		/*ConformationSet& DockResult::getConformationSet()
			throw()
		{return conformation_set_;}*/
					
		const Options& DockResult::getDockingOptions() const
			throw()
		{return docking_options_;}
		
		// returns scores of scoring_ i
		const vector<float>& DockResult::getScores(int i) const
			throw()
		{return scoring_[i].scores_;}
					
		const QString& DockResult::getScoringName(int i) const
			throw()
		{return scoring_[i].name_;}
					
		const Options& DockResult::getScoringOptions(int i) const
			throw()
		{return scoring_[i].options_}
			
		const String& DockResult::getDCDFile() const
			throw()
		{return DCD_file_;}
		
		// add a Scoring_ to vector scorings_
		void DockResult::addScoring(QString name, Options options, vector<float> scores)
		{
			Scoring_ scoring = Scoring_(name, options, scores);
			scorings_.push_back(scoring);
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
		DockResult::Scoring_::Scoring_(QString name, Options& options, vector<float>& scores) throw()
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
		{
			if (&scoring != this)
			{
				name_ = scoring.name_;
				options_ = scoring.options_;
				scores_ = scoring.scores_;
			}
			return *this;
		}
	}
}
