// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: dockResults.C,v 1.1.2.3 2005/03/31 08:41:32 haid Exp $
//

#include "dockResults.h"

namespace BALL
{
	namespace VIEW
	{
		// Default Constructor
		DockResults::DockResults()
			throw()
		{}
		
		// Constructor
		DockResults::DockResults(String& docking_algorithm, ConformationSet& conformation_set,
															Options& docking_options, vector<Scoring_>& scorings, String DCD_file)
			throw()
		{
			docking_algorithm_ = docking_algorithm;
			conformation_set_ = conformation_set;
			docking_options_ = docking_options;
			scorings_ = scorings;
			DCD_file_ = DCD_file;
		}
		
		// Destructor
		DockResults::~DockResults()
			throw()
		{}
		
		/** Assignment operator
		*/
		const DockResults& DockResults::operator =(const DockResults& dock_res)
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
		
		const String& DockResults::getDockingAlgorithm() const
			throw()
		{return docking_algorithm_;}
					
		const ConformationSet& DockResults::getConformationSet() const
			throw()
		{return conformation_set_;}
					
		const Options& DockResults::getDockingOptions() const
			throw()
		{return docking_options_;}
			
		const vector<DockResults::Scoring_>& DockResults::getScorings() const
			throw()
		{return scorings_;}
			
		const String& DockResults::getDCDFile() const
			throw()
		{return DCD_file_;}
		
		// add a Scoring_ to vector scorings_
		void DockResults::addScoring(Scoring_ scoring)
		{
			scorings_.push_back(scoring);
		}
		
		
		/** Implementation of the nested class Scoring_ **/
	
		/** Scoring_ class
				Default Constructor
		*/
		DockResults::Scoring_::Scoring_() throw()
		{}
		
		/** Scoring_ class
				Constructor
		*/
		DockResults::Scoring_::Scoring_(String& name, Options& options, vector<float>& scores) throw()
		{
			name_ = name;
			options_ = options;
			scores_ = scores;
		}
		
		/** Scoring_ class
				Destructor
		*/
		DockResults::Scoring_::~Scoring_() throw()
		{}
			
		/** Scoring_ class 
				Assignment operator
		*/
		const DockResults::Scoring_& DockResults::Scoring_::operator =(const Scoring_& scoring)
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
