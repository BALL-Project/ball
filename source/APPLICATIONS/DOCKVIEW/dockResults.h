//   // -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//

#ifndef DOCKRESULTS_H
#define DOCKRESULTS_H

#ifndef BALL_DATATYPE_OPTIONS_H
# include <BALL/DATATYPE/options.h>
#endif

#ifndef BALL_STRUCTURE_DOCKING_CONFORMATIONSET_H
# include <BALL/STRUCTURE/DOCKING/conformationSet.h>
#endif

namespace BALL
{

		class DockResults
		{
			public:
				
				/**
				nested class Scoring_ 
				This class contains information about the scoring: name of the scoring function, options of the function and scores
      	*/
      	class Scoring_
				{
					public:
						Scoring_() throw()
						{}
						
						Scoring_(String& name, Options& options, vector<float>& scores) throw()
						{
							name_ = name;
							options_ = options;
							scores_ = scores;
						}
						
						~Scoring_() throw()
						{}
						
						/**  Assignment operator
						*/
						const Scoring_& operator =(const Scoring_& scoring)
						{
							if (&scoring != this)
							{
								name_ = scoring.name_;
								options_ = scoring.options_;
								scores_ = scoring.scores_;
							}
							return *this;
						}
						
						String name_;
						Options options_;
						vector<float> scores_;
				};
			
				//Constructor
				DockResults()
					throw();
				
				DockResults(String& docking_algorithm, ConformationSet& conformation_set,
										Options& docking_options, vector<Scoring_>& scorings, String DCD_file)
					throw();
				
				/// Destructor
				virtual ~DockResults()
					throw();
					
				const String& getDockingAlgorithm() const
					throw();
					
				const ConformationSet& getConformationSet() const
					throw();
					
				const Options& getDockingOptions() const
					throw();
					
				const vector<Scoring_>& getScorings() const
					throw();
					
				const String& getDCDFile() const
					throw();
				
				// add a Scoring_ to vector scorings_
				void addScoring(Scoring_ scoring);
				
			private:
				
				// docking algorithm
				String docking_algorithm_;
				//
				ConformationSet conformation_set_;
				// options of the docking algorithm
				Options docking_options_;
				//
				vector<Scoring_> scorings_;
				// name of DCDFile
				String DCD_file_;
		};
		
}
#endif
