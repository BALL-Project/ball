// ----------------------------------------------------
// $Maintainer: Marc Röttig $
// $Authors: Michael Betz, Marc Röttig $
// ----------------------------------------------------

#ifndef BALL_STRUCTURE_RGROUP_ASSEMBLER_H
#define BALL_STRUCTURE_RGROUP_ASSEMBLER_H

#include <map>
#include <vector>
#include <BALL/KERNEL/forEach.h>
#include <BALL/STRUCTURE/smilesParser.h>
#include <BALL/KERNEL/molecule.h>
#include <BALL/KERNEL/system.h>
#include <BALL/DATATYPE/string.h>
#include <BALL/KERNEL/atom.h>


namespace BALL
{
		class BALL_EXPORT RGroupAssembler
		{
			public:
				RGroupAssembler(String& scaffold, std::map<String, std::vector<String> > & rgroups);
				RGroupAssembler(std::vector<String>& scaffolds, std::map<String, std::vector<String> > & rgroups);
				~RGroupAssembler();

				/** Generate the next combination of scaffold + R-group and return this molecule. If there are no more combination to be generated, null is returned. */
				Molecule* generateNextMolecule();

			private:
				void initCounter_();
				bool incrementCounter_();
				void appendMoiety_(AtomContainer* molecule, AtomContainer* moiety, String pm_atom);
				bool isPlacemark_(const String& s);

				std::vector<System*> scaffolds_;
				std::map<String, std::vector<System*> > moieties_;

				unsigned int scaffold_counter_;
				std::map<String, unsigned int> moiety_counter_;
				std::vector<std::map<String, unsigned int> > maxcounter_;

				bool not_finished_;
		};
}

#endif /* BALL_STRUCTURE_RGROUP_ASSEMBLER_H */
