#include "geneticIndividual.h"

#include <BALL/FORMAT/HINFile.h>
#include <BALL/KERNEL/atom.h>
#include <BALL/KERNEL/molecule.h>
#include <BALL/KERNEL/system.h>
#include <BALL/KERNEL/selector.h>
#include <BALL/FORMAT/MOL2File.h>
#include <BALL/STRUCTURE/geometricTransformations.h>
#include <BALL/STRUCTURE/geometricProperties.h>
#include <BALL/MATHS/matrix44.h>
#include <BALL/DATATYPE/options.h>
#include <BALL/MATHS/quaternion.h>
#include <BALL/SYSTEM/path.h>

#include "doubleGene.h"
#include "quaternionGene.h"
#include "geneticPool.h"

#include <cmath>


#include "dockMapping.h"
#include "geneticPool.h"

#include <iostream> 
#include <string> 
#include <fstream>

using namespace std;



namespace BALL
{
  DockMapping::DockMapping():GenericMapping()
  {
  }
  
  DockMapping::DockMapping(const DockMapping& dm)
    :	GenericMapping(dm),
	ligand_(dm.ligand_),
	system_backup_a_(dm.system_backup_a_),
	system_backup_b_(dm.system_backup_b_),
	sys_(dm.sys_),
      	t_origin_(dm.t_origin_),
	t_extension_(dm.t_extension_),
	ligand_positions_(dm.ligand_positions_),
	redraw_(dm.redraw_)
  {
    if (dm.eg_ != 0) 
      eg_ = new EnergyGrid(*(dm.eg_));
    if (dm.rb_ !=0) 
      rb_ = new RotateBonds(*(dm.rb_));
  }
  
  
  DockMapping::DockMapping(System& sys_lig, 
			   const String& file,
			   Vector3 t_box_lower, 
			   Vector3 t_box_higer,
			   ForceField* ff)
    :ff_(ff)
  {
    setup(sys_lig,file,t_box_lower,t_box_higer,ff);
  }

  
  DockMapping& DockMapping::operator=(const DockMapping& dm)
  {
    if (this == &dm) 
      return *this;
    
    //!!!!!!!!!!!!!!!!!!!
    // if (dm.amber_ != 0) 
    //ff_ = new (*(dm.amber_));
    
    delete eg_;
    if (dm.eg_ != 0) 
      eg_ = new EnergyGrid(*(dm.eg_));
    
    delete rb_;
    if (dm.rb_ != 0) 
      rb_ = new RotateBonds(*(dm.rb_));
    
    ligand_ = dm.ligand_;
    system_backup_a_=dm.system_backup_a_;
    system_backup_b_=dm.system_backup_b_;
    sys_=dm.sys_;
    t_origin_=dm.t_origin_;
    t_extension_=dm.t_extension_;
    ligand_positions_=dm.ligand_positions_;  
    redraw_ = dm.redraw_;
    
    return *this;
  }
  
  
  
  
  
  void DockMapping::setup( System& sys_lig, 
			   const String& file,
			   Vector3 t_box_lower, 
			   Vector3 t_box_higher,
			   ForceField* ff)
  {
    
    ff_ = ff;
    
    redraw_ = false;
    
    ligand_ = new Molecule(*(sys_lig.getMolecule(0)));
    draw_ligand_ = new Molecule(*(sys_lig.getMolecule(0)));
    
    original_ligand_ = sys_lig.getMolecule(0);
    
    system_backup_b_ = sys_lig;
    
    Log.error() << "before create energy grid" << std::endl;
    /** create energy grid 
     */

    
    /** create rotable bonds class with ligand
     */
    rb_ = new RotateBonds(*ligand_);
    
    /** insert ligand to calculate conformation energy
     */
    sys_.insert(*ligand_);
    
    /** setup amber to calculate ligand
     */
    sys_.select();
    ff_->setup(sys_);
    sys_.deselect();
    ligand_->select();
    


    eg_ = new EnergyGrid(file,system_backup_a_, *ligand_);    	
    Log.error() << "after create energy grid" << std::endl;
    
    
    draw_system_ = system_backup_a_;
    
    draw_system_.insert(*draw_ligand_);

    //String docking_name = system_ba.getName() + "_" + S2.getName();
    draw_system_.setName(system_backup_a_.getName() + "_" + system_backup_b_.getName() );

    bool bound;
    
    eg_->getGridInfo(t_origin_,t_extension_,bound);
    
    Vector3 t_box_orig = t_box_lower;
    Vector3 t_box_ext = t_box_higher - t_box_lower;
    
    /** if box for translation is defined
     */
    if (t_box_ext.getLength() != 0 && bound)
      {
	Vector3 v1 = t_box_orig - t_origin_;
	Vector3 v2 = t_origin_ + t_extension_ - t_box_higher;
	
	if (v1.x < 0 || v1.y < 0 || v1.z < 0 ||
	    v2.x < 0 || v2.y < 0 || v2.z < 0)
	  {
	    cerr << "translationbox not in bounded grid" << endl;
	    exit(1);
	  }
	
	t_origin_ = t_box_orig;
	t_extension_ = t_box_ext;
      }
    else if(t_box_ext.getLength() == 0 && !bound)
      {
	BoundingBoxProcessor bbp;
	ligand_->apply(bbp);
	Vector3 lower = bbp.getLower();
	Vector3 higher = bbp.getUpper();
	
	higher -= lower;
	
	t_origin_ -= higher*0.5;
	t_extension_ += higher;
      }

    /** create rotable bonds class with ligand
     */
    rb_ = new RotateBonds(*ligand_);
    
//     /** insert ligand to calculate conformation energy
//      */
//     sys_.insert(*ligand_);
    
//     /** setup amber to calculate ligand
//      */
//     sys_.select();
//     ff_->setup(sys_);
//     sys_.deselect();
//     ligand_->select();
    
    /** save ligand positions to restore after calculation 
     */
    for (uint pos = 0; pos < ligand_->countAtoms(); pos++)
      ligand_positions_.push_back( (ligand_->getAtom(pos))->getPosition() );
  }
  
  
  DockMapping::~DockMapping()
  {
    delete eg_;
  }
  
  
  double DockMapping::calculate(GeneticIndividual* gi, uint )
  {
    move(gi);
    
    /** get conformation energy
     */
    
    ff_->updateEnergy();
        
    double energy = ff_->getEnergy()+ eg_->getEnergy(ligand_);


    //cout << energy << endl;
  
    restore();
    
    return energy;
  }
  
  
  
  bool DockMapping::verify(GeneticIndividual* gi1, GeneticIndividual* gi2)
  {  
    for (uint x = 0; x < gi1->numberOfGenes()-2;x++)
      {
	vector<double> v1 = dynamic_cast<DoubleGene*>(gi1->getGene(x))->getValues();
	vector<double> v2 = dynamic_cast<DoubleGene*>(gi2->getGene(x))->getValues();
	
	for ( uint y = 0; y < v1.size(); y++)
	  if (!(v1[y]-v2[y]<0.2&&v1[y]-v2[y]>-0.2||v1[y]-v2[y]<-0.8&&v1[y]-v2[y]>0.8))
	    return true;
      }
    
    vector<double> v1 = dynamic_cast<DoubleGene*>(gi1->getGene(gi1->numberOfGenes()-1))->getValues();
    vector<double> v2 = dynamic_cast<DoubleGene*>(gi2->getGene(gi2->numberOfGenes()-1))->getValues();
    
    /** compare all values
     *  if every translation is within 0.5 angstrom and every rotation is within 5 degree
     *  return false 
     */
    double x_trans1 = t_origin_.x + v1[0]*t_extension_.x;
    double y_trans1 = t_origin_.y + v1[1]*t_extension_.y;
    double z_trans1 = t_origin_.z + v1[2]*t_extension_.z;
    
    double x_trans2 = t_origin_.x + v2[0]*t_extension_.x;
    double y_trans2 = t_origin_.y + v2[1]*t_extension_.y;
    double z_trans2 = t_origin_.z + v2[2]*t_extension_.z;
    
    if (!(x_trans1 - x_trans2 < 1.5 && x_trans1 - x_trans2 > -1.5))
    return true;
    
    if (!(y_trans1 - y_trans2 < 1.5 && y_trans1 - y_trans2 > -1.5))
      return true;
    
    if (!(z_trans1 - z_trans2 < 1.5 && z_trans1 - z_trans2 > -1.5))
      return true;
    
    Quaternion q1 = dynamic_cast<QuaternionGene*>(gi1->getGene(gi1->numberOfGenes()-2))->getValue();
    Quaternion q2 = dynamic_cast<QuaternionGene*>(gi2->getGene(gi2->numberOfGenes()-2))->getValue();
    
    double q = q1.i * q2.i + q1.j * q2.j + q1.k * q2.k + q1.angle * q2.angle;
    
    if (q <0.8 && q > -0.80)
      return true;
    
    return false;
  }
  
  
  
  bool DockMapping::verify(GeneticIndividual* gi)
  {
    /** test entire pool
     */
    for (int x = 0; x < gp_->size(); x++)
      {
	GeneticIndividual* gi2 = (*gp_)[x];
	
	if (gi == gi2)
	  continue;
	
	if (!verify(gi,gi2))
	  return false;
      }
    return true;
  }
  
  
  
  void DockMapping::move(GeneticIndividual* gi)
  {
    
    rb_->rotate(dynamic_cast<DoubleGene*>(gi->getGene(0))->getValues());
 
    GeometricCenterProcessor gcp;
    
    ligand_->apply(gcp);
    
    Vector3 center = gcp.getCenter();
  
    TranslationProcessor center_translation(center.negate());
   
    ligand_->apply(center_translation);

    Matrix4x4 r_matrixq;
    
    dynamic_cast<QuaternionGene*>(gi->getGene(gi->numberOfGenes()-2))->getValue().getRotationMatrix(r_matrixq);
    
    TransformationProcessor rotationq(r_matrixq);  
    
    /** execute transformation
     //    */
    ligand_->apply(rotationq);
    
    vector<double> v = dynamic_cast<DoubleGene*>(gi->getGene(gi->numberOfGenes()-1))->getValues();

    TranslationProcessor translation(Vector3(t_origin_.x + v[0]*t_extension_.x,
					     t_origin_.y + v[1]*t_extension_.y,
					     t_origin_.z + v[2]*t_extension_.z));
    
    ligand_->apply(translation);
  
  }
  
  
  void DockMapping::restore()
  {
    /** restore ligand
     */
    for (uint pos = 0; pos < ligand_positions_.size(); pos++)
      (ligand_->getAtom(pos))->setPosition(ligand_positions_[pos]);
  }

  bool DockMapping::redraw()
  {
    if (redraw_)
      {
	redraw_ = false;
	return true;
      }
    
    return false;
  }
  

  void DockMapping::update()
  {
    move((*gp_)[0]);
    
    for (uint y = 0; y < ligand_->countAtoms(); y++)
      draw_ligand_->getAtom(y)->setPosition(ligand_->getAtom(y)->getPosition());
    redraw_ = true;

    restore();
  }

  void DockMapping::initIndivid(GeneticIndividual* gi)
  {
    gi->insert(new DoubleGene(rb_->numberBonds()));
    
    gi->insert(new QuaternionGene);
    
    gi->insert(new DoubleGene(3));
  }
  
  GenericMapping* DockMapping::clone()
  {
    return new DockMapping(*this);
  }
  
  
  ConformationSet DockMapping::getConformationSet(Index total_number)
  {
    
    //System S = system_backup_a_;
    //System S2 = system_backup_b_;
    
    //S.splice(S2);
    //String docking_name = system_ba.getName() + "_" + S2.getName();
    
    //S.setName(docking_name);
  	//ConformationSet rc(S);
    ConformationSet rc(draw_system_);

    System sys_a = system_backup_a_;
    System sys_b = system_backup_b_;
    
    sys_a.splice(sys_b);

    total_number = 1;

    for (int x = 0; x < total_number && x < gp_->size(); x++)
      {
	System sys_a = system_backup_a_;
	System sys_b = system_backup_b_;

	/** setup amber to calculate ligand
	 */
	sys_.select();
	ff_->setup(sys_);
	sys_.deselect();
	ligand_->select();
	
	move((*gp_)[x]);
	
	for (uint y = 0; y < ligand_->countAtoms(); y++)
	  sys_b.getMolecule(0)->getAtom(y)->setPosition(ligand_->getAtom(y)->getPosition());

	restore();
	
	sys_a.splice(sys_b);
	
	rc.add(calculate((*gp_)[x],0), sys_a);

	double d = calculate((*gp_)[x],0);
	
	move((*gp_)[x]);

	restore();

      }
    
    return rc;
  }

  const System& DockMapping::getIntermediateResult(bool& b)
  {
    //b = redraw_;

    if (redraw_) redraw_ = false;

    return draw_system_; 
  }

}
