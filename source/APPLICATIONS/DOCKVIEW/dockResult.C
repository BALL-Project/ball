// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: dockResult.C,v 1.1.2.5 2005/04/07 17:02:06 leonhardt Exp $
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
		DockResult::DockResult(const QString& docking_algorithm, const ConformationSet& conformation_set,
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
		{}
		
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
		
		void DockResult::setConformationSet(const ConformationSet& conformation_set)
			throw()
		{
			conformation_set_ = conformation_set;
		}
		
		const QString& DockResult::getDockingAlgorithm() const
			throw()
		{
			return docking_algorithm_;
		}
		
		const Options& DockResult::getDockingOptions() const
			throw()
		{
			return docking_options_;
		}
		
		const ConformationSet& DockResult::getConformationSet() const
			throw()
		{
			return conformation_set_;
		}
		
		ConformationSet& DockResult::getConformationSet()
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
		const QString& DockResult::getScoringName(int i) const
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
		void DockResult::addScoring(const QString& name, const Options& options, const vector<float>& scores)
			throw()
		{
			scorings_.push_back(Scoring_(name, options, scores));
		}
		
		// delete i-th Scoring_ of vector scorings_
		void DockResult::deleteScoring(int i)
			throw()
		{
			/////////////////////////////////////// TODO ////////////////////////////////////// 
			//scorings_.erase(i);	
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
		DockResult::Scoring_::Scoring_(const QString& name, const Options& options, const vector<float>& scores) throw()
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
	
	} // end of namespace VIEW
} // end of namespace BALL
