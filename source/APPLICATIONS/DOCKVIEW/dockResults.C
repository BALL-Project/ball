// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: dockResults.C,v 1.1.2.2 2005/03/29 11:53:00 haid Exp $
//

#include "dockResults.h"

namespace BALL
{
	namespace VIEW
	{
		//Constructor
		DockResults::DockResults()
			throw()
		{}

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
		
		/// Destructor
		DockResults::~DockResults()
			throw()
		{}
		
		const String& DockResults::getDockingAlgorithm() const
			throw()
		{return docking_algorithm_;}
					
		const ConformationSet& DockResults::getConformationSet() const
			throw()
		{return conformation_set_;}
					
		const Options& DockResults::getDockingOptions() const
			throw()
		{return docking_options_;}
			
		const vector<Scoring_>& DockResults::getScorings() const
			throw()
		{return scorings_;}
			
		const String& DockResults::getDCDFile() const
			throw()
		{return DCD_file_;}
			
		void DockResults::addScoring(Scoring_ scoring)
		{
			scorings_.push_back(scoring);
		}
	}
}
