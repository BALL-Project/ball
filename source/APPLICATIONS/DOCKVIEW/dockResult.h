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
		/** This class contains the result of a (re)docking calculation: 
		 *  the used algorithm and its options,
		 * 	the conformation set which was produced by the algorithm
		 *	and the scores, names and options of all scoring functions that were used for the ranking / reranking.
		 */
		class DockResult
		{
			public:
					
				/**	@name	Constructors and Destructors
				 */	
				//@{
					
				/** Default Constructor
				*/
				DockResult()
					throw();
					
				/** Constructor
				*/
				DockResult(const String& docking_algorithm, ConformationSet* conformation_set,
										const Options& docking_options)
					throw();
				
				/** Copy constructor
					*/
				DockResult(const DockResult& dock_res)
					throw();
					
				/** Destructor
				*/
				virtual ~DockResult()
					throw();
					
				//@}
				
				/**	@name	Assignment
				 */	
				//@{
				
				/** Assignment operator
				*/
				const DockResult& operator =(const DockResult& dock_res)
					throw();
					
				//@}
				
				/**	@name	Accessors: inspectors and mutators
				 */	
				//@{
				
				/** Sets the conformation set.
				*/
				void setConformationSet(ConformationSet* conformation_set)
					throw();
				
				/** Get the docking algorithm.
				*/
				const String& getDockingAlgorithm() const
					throw();
					
				/** Get the options of the docking algorithm.
				*/
				const Options& getDockingOptions() const
					throw();
				
				/** Get the conformation set.
				*/
				const ConformationSet* getConformationSet() const
					throw();
				
				/** Get the conformation set.
				*/
				ConformationSet* getConformationSet()
					throw();
				
				/** Returns the scores of \link DockResult::scorings_ scorings_ \endlink i.
				*/
				const vector<float>& getScores(Position i) const
					throw();
					
				/** Returns the name of scoring function of \link DockResult::scorings_ scorings_ \endlink i.
				*/
				const String& getScoringName(Position i) const
					throw();
					
				/** Returns the scoring function options of \link DockResult::scorings_ scorings_ \endlink i.
				*/
				const Options& getScoringOptions(Position i) const
					throw();
					
				/** Returns the number of scorings.
				*/
				Size numberOfScorings() const
					throw();
					
				/** Add  a new Scoring_ to vector \link DockResult::scorings_ scorings_ \endlink.
				 *  @param			name name of the scoring function
				 *  @param			options options of the scoring function
				 *  @param			scores scores calculated by the scoring function
				*/
				void addScoring(const String& name, const Options& options, const vector<float>& scores)
					throw();
					
				/** Deletes Scoring_ i of vector \link DockResult::scorings_ scorings_ \endlink.
				*/
				void deleteScoring(Position i)
					throw();
				
				//@}	
					
				/**	@name	Reading and writing
				 */	
				//@{
					
				/** Stores dock result in a file.
					* Calls \link DockResult::writeDockResult writeDockResult(std::ostream& file) \endlink.
					* @param 	  filename name of the file 
				  * @return 	true if writing was successful.
					* @return 	false otherwise
				*/
				bool writeDockResult(const String& filename)
					throw();
				
				/** Stores dock result in an ostream.
					* @param 	  file ostream 
				  * @return 	true if writing was successful.
					* @return 	false otherwise
				*/
				bool writeDockResult(std::ostream& file) const
					throw();
					
				/** Reads dock result from a file.
					* Calls \link DockResult::readDockResult readDockResult(std::istream& file) \endlink.
					* @param 	  filename name of the file 
				  * @return 	true if reading was successful.
					* @return 	false otherwise
				*/
				bool readDockResult(const String& filename)
					throw();
					
				/** Read dock result from an istream.
					* @param 	  file istream 
				  * @return 	true if reading was successful.
					* @return 	false otherwise
				*/
				bool readDockResult(std::istream& file)
					throw();
				
				/** Operator to write dock result into an ostream.
					* Calls \link DockResult::writeDockResult writeDockResult(std::ostream& file)\endlink.
				*/
				friend ostream& operator <<(ostream& out, const DockResult& dock_res)
					throw();
				
				/** Operator to read dock result into an istream.
					* Calls \link DockResult::readDockResult readDockResult(std::istream& file) \endlink.
				*/
				friend istream& operator >>(istream& in, DockResult& dock_res)
					throw();
				
				//@}
				
			protected:
				
				/** Nested class in DockResult.
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
						
						/** Copy constructor
						*/
						Scoring_(const Scoring_& scoring)
							throw();
						
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
						
						/** name of scoring function
						*/
						String name_;
						/** options of scoring function
						*/
						Options options_;
						/** Vector of scores.
							* In this vector score i belongs to the conformation with snapshot number i.
						*/
						vector<float> scores_;
				};
				
				/** Name of docking algorithm
				*/
				String docking_algorithm_;
				/** Options of the docking algorithm
				*/
				Options docking_options_;
				/** Conformation set which was produced by the docking algorithm.
				*/
				ConformationSet* conformation_set_;
				/** Vector contains name, options and scores of each scoring function.
				 *	The scores of each scoring are sorted by snapshot number.
				 */
				vector<Scoring_> scorings_;
		};
		
		std::ostream& operator <<(std::ostream& out, const DockResult& dock_res)
			throw();
			
		std::istream& operator >>(std::istream& in, DockResult& dock_res)
			throw();
}
#endif
