#include <stdio.h>
#include <BALL/NMR/assignShiftProcessor.h>
#include <BALL/NMR/NMRSpectrum.h>
#include <BALL/STRUCTURE/fragmentDB.h>
#include <BALL/STRUCTURE/residueChecker.h>
#include <BALL/FORMAT/HINFile.h>
#include <BALL/FORMAT/NMRStarFile.h>
#include <BALL/SYSTEM/path.h>

using namespace BALL;
using namespace std;

int main()
{
	HINFile f;
	f.open("data/AssignShiftProcessor_test.hin");
	Path path;

	System system;
	f >> system;
	FragmentDB frag_db;
	ResidueChecker rc(frag_db);
	system.apply(rc);
	

	NMRStarFile rs("data/bmr4789.str");
	cout << "Size: " << rs.getData()[0]->atomData.size() << endl;
	cout << "number of atoms: " << rs.getNumberOfAtoms() << endl;

	Log.info() << "System: #atoms = " << system.countAtoms() << endl;
	AssignShiftProcessor asp(rs.getData()[0]->atomData);
	system.apply(asp);

	int numberOfShiftAtoms = 0;
	AtomIterator atom_it = system.beginAtom();
	for (; +atom_it; ++atom_it)
	{
		if (atom_it->hasProperty(ShiftModule::PROPERTY__SHIFT))
		{
			numberOfShiftAtoms++;
		}
		else
		{
			Log.info() << atom_it->getFullName() << " " << atom_it->getProperty(ShiftModule::PROPERTY__SHIFT).getFloat() << endl;
		}
	}
 	cout << "numberOfShiftAtoms " << numberOfShiftAtoms << endl;

	NMRSpectrum spectrum;
	spectrum.setSystem(&system);
	spectrum.setDensity(32768);
	spectrum.createSpectrum();
	Log.info() << " Number of peaks in spectrum: " << spectrum.getPeakList().size() << endl;
	spectrum.plotSpectrum("parv_synth.dat");
	spectrum.writePeaks("parv_synth.peaks");
}
