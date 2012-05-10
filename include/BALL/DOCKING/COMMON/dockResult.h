// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: dockResult.h,v 1.2.16.2 2007/08/07 16:16:15 bertsch Exp $
//

#ifndef BALL_DOCKING_COMMON_DOCKRESULT_H
#define BALL_DOCKING_COMMON_DOCKRESULT_H

#ifndef BALL_DATATYPE_OPTIONS_H
# include <BALL/DATATYPE/options.h>
#endif

#ifndef BALL_DOCKING_COMMON_CONFORMATIONSET_H
# include <BALL/DOCKING/COMMON/conformationSet.h>
#endif

namespace BALL
{
		/** Result of a (re)docking calculation.
		    the used algorithm and its options,
			the conformation set which was produced by the algorithm
			and the scores, names and options of all scoring functions that were used for the ranking / reranking.
		    On the one hand it allows to recieve the scores of a certain scoring,
		    on the other hand it can return scores sorted by another scoring.
		    This offers a simple way to compare the different scores with each other
				\ingroup Docking
		 */
		class BALL_EXPORT DockResult
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
				const DockResult& operator = (const DockResult& dock_res)
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

				/*  Sets scoring flag by which all scores are sorted displayed
						a negative index corresponds to sorting by snapshot index
				 */
				 void sortBy(Index scoring_index)
					throw(Exception::IndexOverflow);

				/*  Get the number of the scoring by which all scores are sorted displayed
				 */
				Index isSortedBy() const
					throw();

				/** Returns score i of the scoring j in respect of the current sorting
				 *  indicated by the flag \link DockResult::sorted_by_ sorted_by_ \endlink.
				 */
				float operator()(Position i, Position j)
					throw(Exception::IndexOverflow);

				/** Returns the scores of \link DockResult::Scoring_ scoring \endlink i.
				*/
				const vector < ConformationSet::Conformation > getScores(Position i) const
					throw(Exception::IndexOverflow);

				/** Returns the name of scoring function of \link DockResult::Scoring_ scoring \endlink i.
				*/
				const String& getScoringName(Position i) const
					throw(Exception::IndexOverflow);

				/** Returns the scoring function options of \link DockResult::Scoring_ scoring \endlink i.
				*/
				const Options& getScoringOptions(Position i) const
					throw(Exception::IndexOverflow);

				/** Returns the number of scorings.
				*/
				Size numberOfScorings() const
					throw();

				/** Add  a new Scoring_ to vector \link DockResult::scorings_ scorings_ \endlink.
				 *  @param			name name of the scoring function
				 *  @param			options options of the scoring function
				 *  @param			scores scores calculated by the scoring function
				*/
				void addScoring(const String& name, const Options& options, vector < ConformationSet::Conformation > scores)
					throw();

				/** Deletes Scoring_ i of vector \link DockResult::scorings_ scorings_ \endlink.
				*/
				void deleteScoring(Position i)
					throw(Exception::IndexOverflow);

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
				friend std::ostream& operator <<(std::ostream& out, const DockResult& dock_res)
					throw();

				/** Operator to read dock result into an istream.
					* Calls \link DockResult::readDockResult readDockResult(std::istream& file) \endlink.
				*/
				friend std::istream& operator >>(std::istream& in, DockResult& dock_res)
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
						Scoring_(const String& name, const Options& options, const vector<float>& scores, const vector<Index>& snapshot_order) throw();

						/** Destructor
						*/
						~Scoring_() throw();

						/** Assignment operator
						*/
						const Scoring_& operator = (const Scoring_& scoring)
							throw();

						/** Name of scoring function
						*/
						String name_;
						/** options of scoring function
						*/
						Options options_;
						/** Vector of scores
							* The score at position i belongs to the snapshot i
						*/
						vector<float> scores_;
						/** Vector of snapshot indices
							* The indices are sorted by their scores
						 */
						vector<Index> snapshot_order_;
				};

				/**
				* Nested class in DockResult.
				* This class is needed for the sorting of the vector of Conformations
				* in \link DockResult::addScoring addScoring(const String& name, const Options& options, const vector < ConformationSet::Conformation > & scores)\endlink.
				* The vector should be sorted by the score values.
				*/
				class Compare_
				{
					public:

						/** Default constructor
						*/
						Compare_() throw();

						/** Destructor
						*/
						~Compare_() throw();

						/** Operator ()
						*/
						//bool operator() (const std::pair<Index, float>& a, const std::pair<Index, float>& b) const
						bool operator() (const ConformationSet::Conformation& a, const ConformationSet::Conformation& b) const
							throw();
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
				 *	The scores of each scoring are sorted.
				 */
				vector<Scoring_> scorings_;
				/** Flag that indicated by which scoring all scorings are sorted
						-1 corresponds to a sorting by snapshot index
						It is needed for the operator(i, j)
				 */
				Index sorted_by_;
		};

		std::ostream& operator <<(std::ostream& out, const DockResult& dock_res)
			throw();

		std::istream& operator >>(std::istream& in, DockResult& dock_res)
			throw();
}
#endif
