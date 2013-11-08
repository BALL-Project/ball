// ----------------------------------------------------
// $Maintainer: Marcel Schumann $
// $Authors: Marcel Schumann $
// ----------------------------------------------------

#ifndef BALL_SCORING_FUNCTIONS_RESCORING3D_H
#define BALL_SCORING_FUNCTIONS_RESCORING3D_H

#include <BALL/SCORING/FUNCTIONS/rescoring.h>
#include <BALL/DATATYPE/regularData3D.h>


namespace BALL
{
    class BALL_EXPORT Rescoring3D : public Rescoring
    {
        public:

            Rescoring3D(AtomContainer& receptor, AtomContainer& reference_ligand, Options& options, String free_energy_label, ScoringFunction* sf);

            std::list<std::pair<String, RegularData3D*> > generateRegularData3DGrids();


        protected:

            void setup_();

            void generateScoreContributions_(Molecule* mol, std::vector<std::vector<double> >* matrix, std::vector<double>* v);

            /** Obtain score-contributions for a single atom. Overloaded by class Rescoring4D */
            virtual void generateAtomScoreContributions_(const Atom* atom, std::vector<double>& contributions);

            Size sizeX_;
            Size sizeY_;
            Size sizeZ_;
            double resolution_;
            Vector3 origin_;

            /** the names for the RegularData3D-grids to be created by generateRegularData3DGrids() */
            std::vector<String> reg3D_grid_names_;

    };
}
#endif
