//   // -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//

#ifndef DOCKRESULT_H
#define DOCKRESULT_H

#ifndef BALL_DATATYPE_OPTIONS_H
# include <BALL/DATATYPE/options.h>
#endif

#ifndef BALL_STRUCTURE_DOCKING_CONFORMATIONSET_H
# include <BALL/STRUCTURE/DOCKING/conformationSet.h>
#endif

#include <qstring.h>

namespace BALL
{

		class DockResult
		{
			public:
			
				// Default Constructor
				DockResult()
					throw();
					
				// Constructor
				DockResult(const QString& docking_algorithm, const ConformationSet& conformation_set,
										const Options& docking_options, const String& DCD_file)
					throw();
				
				// Destructor
				virtual ~DockResult()
					throw();
					
				/** Assignment operator
				*/
				const DockResult& operator =(const DockResult& dock_res)
					throw();
				
				void setConformationSet(const ConformationSet& conformation_set)
					throw();
					
				const QString& getDockingAlgorithm() const
					throw();
					
				const ConformationSet& getConformationSet() const
					throw();
					
				ConformationSet& getConformationSet()
					throw();
					
				const Options& getDockingOptions() const
					throw();
				
				// returns scores of scoring_ i
				const vector<float>& getScores(int i) const
					throw();
					
				const QString& getScoringName(int i) const
					throw();
					
				const Options& getScoringOptions(int i) const
					throw();
					
				const String& getDCDFile() const
					throw();
					
				//
				Size numberOfScorings() const
					throw();
				
				// add new Scoring_ to vector scorings_
				void addScoring(const QString& name, const Options& options, const vector<float>& scores)
					throw();
				
			protected:
				
				/**
				nested class Scoring_ 
				This class contains information about the scoring: name of the scoring function, options of the function and scores
      	*/
      	class Scoring_
				{
					public:
					
						//Default Constructor
						Scoring_() throw();
						
						//Constructor
						Scoring_(const QString& name, const Options& options, const vector<float>& scores) throw();
						
						//Destructor
						~Scoring_() throw();
						
						/**  Assignment operator
						*/
						const Scoring_& operator =(const Scoring_& scoring)
							throw();
						
						QString name_;
						Options options_;
						vector<float> scores_;
				};
				
			private:
				
				// docking algorithm
				QString docking_algorithm_;
				//
				ConformationSet conformation_set_;
				// options of the docking algorithm
				Options docking_options_;
				// vector contains name, options and scores of each scoring function
				vector<Scoring_> scorings_;
				// name of DCDFile where trajectories are stored
				String DCD_file_;
		};
		
}
#endif
