#ifndef COMBIASSEMBLER_H
#define COMBIASSEMBLER_H

#include <BALL/STRUCTURE/LIGAND3DGEN/ligand3DBase.h>
#include <BALL/STRUCTURE/LIGAND3DGEN/ioModule.h>
#include <BALL/STRUCTURE/LIGAND3DGEN/moleculeConnector.h>
#include <BALL/STRUCTURE/clashResolver.h>
#include <BALL/FORMAT/SDFile.h>

namespace BALL 
{
/**
 * @brief The CombiAssembler class - creates valid 3D coordinates for all
 * possible combinations of a combi-lib
 */
class CombiAssembler
{
public:
	/**
	 * @brief CombiAssembler - generates 3D coordinates in a manner that is
	 * optimized for speed. Before application it is necessary to set the
	 * central scaffold with setScaffold()
	 *
	 * @param site_db - collection of site templates to be able to connect the
	 * individual fragments
	 * @param clib - input combilib which needs to be initialized with 3D
	 * coordinates for the contained R-fragments
	 */
	CombiAssembler(SiteFragmentDB& site_db, BALL::CombiLibMap* clib);
	
	~CombiAssembler();
	
	/**
	 * @brief setScaffold - setter for the central scaffold for all combinations
	 * @param scaffold
	 */
	void setScaffold(BALL::RFragment& scaffold);

	/**
	 * @brief setCombiLib - setter for the internal combilib
	 * @param clib
	 */
	void setCombiLib(BALL::CombiLibMap& clib);
	
	/**
	 * @brief writeCombinations - calculates and writes the combinations with
	 * their 3D coordinates directly to the given SDFile handle
	 * @param handle
	 */
	void writeCombinations(BALL::SDFile& handle);
	
	
private:
	
	bool _connectClashFree(BALL::Atom &at1, BALL::Atom &at2, BALL::ConnectList& connections);
	void _checkAndConnect(BALL::RAtom& acceptor, BALL::RFragment& donor);
	void _addSet(BALL::RFragment& mol);
	
	BALL::RFragment*          _work_mol;
	BALL::CombiLibMap*        _r_groups;
	std::list< BALL::RAtom* > _r_atms;
	
	MoleculeConnector  _connector;
	ConnectionResolver _cresolv;

	std::list< BALL::RFragment* > _current_combination;
	void _combineRecur(BALL::SDFile& handle);
	
	void newSetForCurrentCombination();
};

}// End Namespace BALL
#endif // COMBIASSEMBLER_H
