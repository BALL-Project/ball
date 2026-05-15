// ----------------------------------------------------
// $Maintainer: Marcel Schumann $
// $Authors: Jan Fuhrmann, Marcel Schumann $
// ----------------------------------------------------

#ifndef BALL_DOCKING_GENETICDOCK_GENETICINDIVIDUAL_H
#define BALL_DOCKING_GENETICDOCK_GENETICINDIVIDUAL_H

#include <vector>

#include <BALL/DOCKING/GENETICDOCK/genes.h>

namespace BALL
{
	// NOTE: BALL_DEPRECATED was removed (Phase 05.1-09, BACKLOG Task B4).
	// The class is still load-bearing internally — GeneticAlgorithm holds
	// std::vector<std::vector<GeneticIndividual>> and a GeneticIndividual
	// template_individual_ by value, and EvolutionaryDocking depends on the
	// same. MSVC's STL templates instantiate the deprecated members from
	// inside the containers, which emitted 5 spurious C4996 warnings. Since
	// there is no replacement class and the methods are not actually going
	// away, the deprecation was incorrect. Re-add BALL_DEPRECATED only when
	// a replacement lands and internal callers migrate off.
	class BALL_EXPORT GeneticIndividual
	{
		public:
			/** standard constructor
			 */
			GeneticIndividual();

			/** copy constructor
			 */
			GeneticIndividual(const GeneticIndividual& gi);

			/** assigment operator
			 */
			GeneticIndividual& operator= (const GeneticIndividual& gi);

			/** destructor
			 */
			~GeneticIndividual();

			/** smaller operator regarding fitness score
			 */
			bool operator<(const GeneticIndividual& gi) const;

			/** assigns fitness score
			 */
			void setFitnessValue(const double& d);

			/** get fitness score
			 */
			double getFitnessValue();

			/**  assigns value to altered_
			 */
			void setAltered(bool b);

			/** true if individual has changed since the last computation of the fitness score
			 */
			bool isAltered();

			/** counts number of genes
			 */
			Size numberOfGenes();

			/** get i-th gene
			 */
			GenericGene* getGene(const Size& i);

			/** push back gene gg
			 */
			void insert(GenericGene* gg);

			/** mate two individuals
			 */
			void mate(GeneticIndividual& parent, GeneticIndividual& offspring);

			/** true if indiviual has been mutated
			 */
			bool isMutated();

			/** assigns value to mutated_
			 */
			void setMutated(bool);

			/** randomize all genes
			 */
			void randomize();

		private:

			/** fitness score
			 */
			double fitness_value_;

			/** true if individual has changed since the last computation of the fitness score
			 */
			bool altered_;

			/** true if indiviual has been mutated
			 */
			bool mutated_;

			/** container for genes
			 */
			std::vector<GenericGene*> genes_;
	};
}

#endif /* BALL_DOCKING_GENETICDOCK_GENETICINDIVIDUAL_H */
