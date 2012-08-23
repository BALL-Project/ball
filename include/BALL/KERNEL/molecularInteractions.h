#ifndef BALL_KERNEL_MOLECULARINTERACTIONS_H
#define BALL_KERNEL_MOLECULARINTERACTIONS_H

#include <BALL/KERNEL/atom.h>
#include <BALL/DATATYPE/string.h>
#include <map>


namespace BALL
{
	class BALL_EXPORT MolecularInteractions
	{
		public:

			static bool SAVE_ONLY_SUMS;

			MolecularInteractions();

			void clear();

			void setThreshold(double threshold);

			void addInteraction(const Atom* atom, String interaction_type, double energy);

			void addInteraction(String interaction_type, double energy);

			bool hasInteraction(const Atom* atom, String interaction_type) const;

			double getInteractionEnergy(const Atom* atom, String interaction_type) const;

			double getInteractionEnergy(const Atom* atom, const list<String>& interaction_types) const;

			const std::map<const Atom*,double>* getInteractions(String interaction_type) const;

			void getInteractions(const std::list<String>& interaction_types, std::map<const Atom*,double>& interactions) const;

			double getInteractionEnergy(String interaction_type) const;

			double getInteractionEnergy(const std::list<String>& interaction_types) const;

			double getInteractionEnergy() const;


		private:

			double threshold_;

			/** Contains one map for each type of interaction (vdW,electrostatic, etc.) */
			std::map<String, std::map<const Atom*,double> > interactions_map_;

			/** Contains the total energy for each type of interaction */
			std::map<String,double> interaction_energies_;

			/** The total interaction energy, i.e. sum over all components (vdW,electrostatic, etc.) */
			double total_energy_;

	};
}

#endif // BALL_KERNEL_MOLECULARINTERACTIONS_H
