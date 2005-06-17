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

namespace BALL
{
		/** This class contains the result of an docking calculation, 
		 *  like what algorithm was used, with which options,
		 * 	the conformation set which was produced by the algorithm
		 *	and the scores of all scoring functions that where used for the ranking / reranking
		 */
		class DockResult
		{
			public:
			
				friend std::ostream& operator <<(ostream& out, const DockResult& dock_res)
					throw();
				
				/** Default Constructor
				*/
				DockResult()
					throw();
					
				/** Constructor
				*/
				DockResult(const String& docking_algorithm, ConformationSet* conformation_set,
										const Options& docking_options)
					throw();
				
				/** Copy constructor.
					*/
				DockResult(const DockResult& dock_res)
					throw();
					
				/** Destructor
				*/
				virtual ~DockResult()
					throw();
					
				/** Assignment operator
				*/
				const DockResult& operator =(const DockResult& dock_res)
					throw();
					
				/**
				*/
				void setConformationSet(ConformationSet* conformation_set)
					throw();
					
				const String& getDockingAlgorithm() const
					throw();
					
				const Options& getDockingOptions() const
					throw();
				
				const ConformationSet* getConformationSet() const
					throw();
					
				ConformationSet* getConformationSet()
					throw();
				
				/** returns scores of scoring_ i
				*/
				const vector<float>& getScores(int i) const
					throw();
					
				/** returns name of scoring function of scoring_ i
				*/
				const String& getScoringName(int i) const
					throw();
					
				/** returns options of scoring function of scoring_ i
				*/
				const Options& getScoringOptions(int i) const
					throw();
					
				/** returns the number of scorings
				*/
				Size numberOfScorings() const
					throw();
				
				/** add new Scoring_ to vector scorings_
				*/
				void addScoring(const String& name, const Options& options, const vector<float>& scores)
					throw();
				
				/* store dock result in a file
				*/
				bool writeDockResult(const String& filename)
					throw();
					
				bool readDockResult(const String& filename)
					throw();
					
				/** delete i-th Scoring_ of vector scorings_
				*/
				void deleteScoring(int i)
					throw();
						
			protected:
				
				/**
				 *	nested class Scoring_ 
				 *	This class contains information about the scoring: 
				 * 	name of the scoring function, 
				 * 	options of the function and 
				 * 	scores
      	 */
      	class Scoring_
				{
					public:
					
						/** Default Constructor
						*/
						Scoring_() throw();
						
						/** Constructor
						*/
						Scoring_(const String& name, const Options& options, const vector<float>& scores) throw();
						
						/** Destructor
						*/
						~Scoring_() throw();
						
						/** Assignment operator
						*/
						const Scoring_& operator =(const Scoring_& scoring)
							throw();
						
						String name_;
						Options options_;
						/** in this vector score i belongs to the conformation with snapshot number i
						*/
						vector<float> scores_;
				};
				
				/** name of docking algorithm
				*/
				String docking_algorithm_;
				/** options of the docking algorithm
				*/
				Options docking_options_;
				/** conformation set which was produced by the docking algorithm
				*/
				ConformationSet* conformation_set_;
				/** vector contains name, options and scores of each scoring function
				 *	the scores of each scoring are sorted by snapshot number
				 */
				vector<Scoring_> scorings_;
		};
		
		std::ostream& operator <<(ostream& out, const DockResult& dock_res)
			throw();
}
#endif
