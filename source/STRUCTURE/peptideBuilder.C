/// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//

#include <BALL/STRUCTURE/peptideBuilder.h>

#include <BALL/MATHS/analyticalGeometry.h>
#include <BALL/STRUCTURE/geometricTransformations.h>
#include <BALL/KERNEL/PTE.h>
#include <cmath>
#include <BALL/STRUCTURE/fragmentDB.h>

namespace BALL
{
	namespace Peptides
	{

		// Default bond lengths for divers bonds in the peptide backbone.
		const float BOND_LENGTH_N_C  = 1.335;
		const float BOND_LENGTH_C_CA = 1.520;
		const float BOND_LENGTH_N_CA = 1.450;
		const float BOND_LENGTH_C_O  = 1.240;
		const float BOND_LENGTH_N_H  = 1.020;

		AminoAcidDescriptor::AminoAcidDescriptor()
			:type_(),
			 phi_(Angle(-47.,false)),
			 psi_(Angle(-58.,false)),
			 omega_(Angle(Constants::PI,true))
		{
		}

		AminoAcidDescriptor::AminoAcidDescriptor
			(const String& type, const Angle& phi,
			 const Angle& psi, const Angle& omega)
			: phi_(phi),
				psi_(psi),
				omega_(omega)
		{
			if (type.size() == 1)
			{
				type_ = ThreeLetterCode(type.toChar());
			}
			else
			{
				type_ = type;
			}
		}


		// inline functions (alle accessors), dtors ????
		AminoAcidDescriptor::~AminoAcidDescriptor()
		{
		}


		AminoAcidDescriptor::AminoAcidDescriptor(const AminoAcidDescriptor& aad)
			: type_(aad.getType()),
				phi_(aad.getPhi()),
				psi_(aad.getPsi()),
				omega_(aad.getOmega())
		{
		}


		void AminoAcidDescriptor::setAminoAcidType(const String& type)
		{
			if (type.size() == 1)
			{
				type_ = ThreeLetterCode(type.toChar());
			}
			else
			{
				type_ = type;
			}
		}

		void AminoAcidDescriptor::setPhi(const Angle& phi)
		{
			phi_ = phi;
		}

		void AminoAcidDescriptor::setPsi(const Angle& psi)
		{
			psi_ = psi;
		}

		void AminoAcidDescriptor::setOmega(const Angle& omega)
		{
			omega_ = omega;
		}

		const String& AminoAcidDescriptor::getType() const
		{
			return type_;
		}

		const Angle& AminoAcidDescriptor::getPhi() const
		{
			return phi_;
		}

		const Angle& AminoAcidDescriptor::getPsi() const
		{
			return psi_;
		}

		const Angle& AminoAcidDescriptor::getOmega() const
		{
			return omega_;
		}

		PeptideBuilder::PeptideBuilder()
			: sequence_(),
				chainname_("Chain"),
				proteinname_("Protein"),
				is_proline_(false),
				fragment_db_(0)
		{
		}

		PeptideBuilder::PeptideBuilder(const std::vector<AminoAcidDescriptor>& sequence)
			: sequence_(sequence),
				chainname_("Chain"),
				proteinname_("Protein"),
				is_proline_(false),
				fragment_db_(0)
		{
		}

		PeptideBuilder::PeptideBuilder(const String& sequence, const Angle& phi, const Angle& psi, const Angle& omega)
			: sequence_(),
				chainname_("_"),
				proteinname_(sequence),
				is_proline_(false),
				fragment_db_(0)
		{
			// Iterate over the sequence (one-letter code) and construct
			// add descriptors for the aa with the given geometry.
			for (Position i = 0; i < sequence.length(); ++i)
			{
				addAminoAcid(sequence[i], phi, psi, omega);
			}
		}

		PeptideBuilder::PeptideBuilder(const PeptideBuilder& pc)
			:sequence_(pc.sequence_),
			 chainname_(pc.chainname_),
			 proteinname_(pc.proteinname_),
			 is_proline_(false),
			 fragment_db_(0)
		{
		}

		PeptideBuilder::~PeptideBuilder()
		{
		}


		void PeptideBuilder::addAminoAcid
			(const String& type, const Angle& phi, const Angle& psi, const Angle& omega)
		{
			AminoAcidDescriptor aa(type, phi, psi, omega);
			sequence_.push_back(aa);
		}

		void PeptideBuilder::addAminoAcid(const AminoAcidDescriptor& aad)
		{
			sequence_.push_back(aad);
		}

		void PeptideBuilder::setChainName(const String& name)
		{
			chainname_ = name;
		}

		const String& PeptideBuilder::getChainName() const
		{
			return chainname_;
		}

		void PeptideBuilder::setProteinName(const String& name)
		{
			proteinname_ = name;
		}

		const String& PeptideBuilder::getProteinName() const
		{
			return proteinname_;
		}

		Protein* PeptideBuilder::construct()
		{
			if (fragment_db_ == 0)
			{
				Log.warn() << "PeptideBuilder::construct(): no FragmengDB given!" << std::endl;
				return 0;
			}
			if (sequence_.empty())
			{

				Log.warn() << "PeptideBuilder::construct(): no amino acid sequence specified." << std::endl;
				return 0;
			}
			int id = 1;
			Protein *protein = new Protein(proteinname_);
			Chain *chain = new Chain(chainname_);

			// create the first residue
			Residue* residue = createResidue_(sequence_[0].getType(), id);
			chain->insert(*residue);
			Residue* residueold = residue;
			std::vector<AminoAcidDescriptor>::iterator i = sequence_.begin();
			++id;

			// consistency check for empty sequences and sequences of length < 2!!	
			// loop for the remaining residues ;
			for (++i; i != sequence_.end(); ++i)
			{
				// We have to take care of two special cases:
				// 		- the residue we are looking at is proline
				// 		- the last residue was proline
				String type = (i-1)->getType();
				type.toUpper();

				// special case: residue is a proline
				type = i->getType();
				type.toUpper();
				is_proline_ = type == "PRO";

				Residue* residue2 = createResidue_(i->getType(), id);

				insert_(*residue2, *residueold);
				chain->insert(*residue2);

				// set the torsion angle 
				transform_(i->getPhi(), (i-1)->getPsi(),*residueold, *residue2);
				peptide_(*residueold,*residue2);

				// set the peptide bond angle omega
				setOmega_(*residueold, *residue2, i->getOmega());

				residueold=residue2;
				++id;
			}

			protein->insert(*chain);

			// read the names for a unique nomenclature 
			protein->apply(fragment_db_->normalize_names);

			// add missing bonds and atoms (including side chains!)
			ReconstructFragmentProcessor rfp(*fragment_db_);
			protein->apply(rfp);
			protein->apply(fragment_db_->build_bonds);

			return protein;
		}

		Residue* PeptideBuilder::createResidue_(const String& type, const int id)
		{
			Residue* res = new Residue(type, String(id));
			PDBAtom* nitrogen = new PDBAtom(PTE[Element::N], "N");
			PDBAtom* carbona = new PDBAtom(PTE[Element::C], "CA");
			PDBAtom* carbon = new PDBAtom(PTE[Element::C], "C");

			// put CA into the starting position 
			carbona->setPosition(Vector3( 0.00, 0.00, 0.0));

			// insert N and C 
			nitrogen->setPosition(Vector3(-1 * BOND_LENGTH_N_CA, 0.00,  0.00));
			carbon->setPosition
				(Vector3(BOND_LENGTH_C_CA * (cos(Constants::PI * 71./180.)),
								 0.00,
								 BOND_LENGTH_C_CA * (sin(Constants::PI * 71. / 180.))));

			// insert atomes into residue/chain/protein
			res->insert(*nitrogen);
			res->insert(*carbona);
			res->insert(*carbon);

			// flag as an amino acid  
			res->setProperty(Residue::PROPERTY__AMINO_ACID);

			// create the bonds
			nitrogen->createBond(*carbona)->setOrder(Bond::ORDER__SINGLE);
			carbona->createBond(*carbon)->setOrder(Bond::ORDER__SINGLE);

			return res;
		}

		void PeptideBuilder::insert_(Residue& resnew, Residue& resold)
		{
			PDBAtom* pcarbon     = getAtomByName_(resold, "C");
			PDBAtom* pcarbona    = getAtomByName_(resold, "CA");
			PDBAtom* pnitrogen   = getAtomByName_(resold, "N");
			PDBAtom* pnitrogen_n = getAtomByName_(resnew, "N");
			PDBAtom* pcarbona_n  = getAtomByName_(resnew, "CA");
			PDBAtom* pcarbon_n   = getAtomByName_(resnew, "C");

			Vector3 rot_axis;    // axis  for the torsion angle

			// set C-N-bond 
			pcarbon->createBond(*pnitrogen_n)->setOrder(Bond::ORDER__SINGLE);;


			//  --------------  move N into  0|0|0   
			TranslationProcessor translation(Vector3(BOND_LENGTH_N_CA, 0., 0.));
			resnew.apply(translation);

			// --------------- calculate the coordinates for N
			Vector3 CA_C_axis = (pcarbon->getPosition() - pcarbona->getPosition()).normalize();
			Vector3 N_CA_axis = (pcarbona->getPosition() - pnitrogen->getPosition()).normalize();
			Vector3 CA_N_axis = ((float)-1.) * N_CA_axis;
			Vector3 newpos = (CA_N_axis - (CA_C_axis * CA_N_axis) * CA_C_axis).normalize();
			Vector3 normaxis = (CA_C_axis % newpos).normalize();

			Matrix4x4 mat;
			mat.setRotation(Angle(-1. * 30.* Constants::PI/180.), normaxis);

			newpos = mat * newpos;               // rotation
			newpos = newpos *  BOND_LENGTH_N_C;  // scaling of the bond length
			newpos = newpos + pcarbon->getPosition();

			// positioning of the nitrogen
			translation.setTranslation(newpos);
			resnew.apply(translation);


			 //---------------set the angle in NN to 120 degrees
			Vector3 NN_C_axis   = pcarbon->getPosition() - pnitrogen_n->getPosition();
			Vector3 NN_NCA_axis = pcarbona_n->getPosition() - pnitrogen_n->getPosition();
			CA_C_axis = pcarbon->getPosition() - pcarbona->getPosition();

			// current angle for C
			Angle current(acos(NN_C_axis * NN_NCA_axis /
										 (NN_C_axis.getLength() * NN_NCA_axis.getLength() )));

			Matrix4x4 transmat;
			Vector3 rotaxis;

			if (current < (float)(Constants::PI - 1e-2))
			{
				current = Angle(120.*Constants::PI/180.) - current;
				rotaxis = (NN_C_axis%NN_NCA_axis).normalize();
			}
			else
			{
				if (current > (float)(Constants::PI+1e-2))
				{
				  // is this line really correct???
					current = Angle(-120. * 120. * Constants::PI / 180.) - current;
					rotaxis = (NN_C_axis % NN_NCA_axis).normalize();
				}
				else
				{
					current = Angle(-120. * Constants::PI / 180.) - current;
					rotaxis = (NN_C_axis%CA_C_axis).normalize();
				}
			}

			transmat.rotate(current, rotaxis);

			TransformationProcessor transformation(transmat);

			// translate to 0|0|0 (needed for the rotation)
			Vector3 toOrigin = pnitrogen_n->getPosition();
			translation.setTranslation(((float)(-1.))*toOrigin);
			resnew.apply(translation);
			// rotate in 0|0|0
			resnew.apply(transformation);
			// translate back to the correct position
			translation.setTranslation(toOrigin);
			resnew.apply(translation);

			//---------------- set torsion angle to 0
			Vector3 v_pca  = pcarbona->getPosition();
			Vector3 v_pc   = pcarbon->getPosition();
			Vector3 v_pn_n = pnitrogen_n->getPosition();
			Vector3 v_pca_n = pcarbona_n->getPosition();
			Vector3 v_pc_n  = pcarbon_n->getPosition();
			Vector3 v_pn   =pnitrogen->getPosition();
			current = getTorsionAngle( v_pca.x,   v_pca.y,   v_pca.z,
					 v_pc.x,    v_pc.y,    v_pc.z,
					 v_pn_n.x,  v_pn_n.y,  v_pn_n.z,
					 v_pca_n.x, v_pca_n.y, v_pca_n.z );


			rot_axis =  (pnitrogen_n->getPosition() - pcarbon->getPosition()).normalize();
			transmat.getZero();/// is this necessary???
			transmat.rotate(Angle(-1. * current), rot_axis);
			transformation.setTransformation(transmat);

			// translate to 0|0|0
			toOrigin = pnitrogen_n->getPosition();
			translation.setTranslation(((float)(-1.))*toOrigin);
					resnew.apply(translation);
			// set torsion angles
			 resnew.apply(transformation);

			// translate back
			translation.setTranslation(toOrigin);
			resnew.apply(translation);

			// ---------------------set the peptide bond angle omega  to 180 degrees
			v_pca  = pcarbona->getPosition();
			v_pc   = pcarbon->getPosition();
			v_pn_n = pnitrogen_n->getPosition();
			v_pca_n = pcarbona_n->getPosition();
			v_pc_n  = pcarbon_n->getPosition();
			current = getTorsionAngle( v_pca.x,   v_pca.y,   v_pca.z,
					 v_pc.x,    v_pc.y,    v_pc.z,
					 v_pn_n.x,  v_pn_n.y,  v_pn_n.z,
					 v_pca_n.x, v_pca_n.y, v_pca_n.z );

			transmat.getZero();
			transmat.rotate( Angle(Constants::PI , true),
					 (pnitrogen_n->getPosition() - pcarbon->getPosition()).normalize());
			transformation.setTransformation(transmat);
			// translate to 0|0|0 (needed for the rotation)
			toOrigin = pnitrogen_n->getPosition();
			translation.setTranslation(((float)(-1.))*toOrigin);
			resnew.apply(translation);
			//rotate in 0|0|0  about 180 degrees
			resnew.apply(transformation);
			//translate back
			translation.setTranslation(toOrigin);
			 resnew.apply(translation);

			//------------- angle psi
			v_pca  = pcarbona->getPosition();
			v_pc   = pcarbon->getPosition();
			v_pn_n = pnitrogen_n->getPosition();
			v_pca_n = pcarbona_n->getPosition();
			v_pc_n  = pcarbon_n->getPosition();
			current = getTorsionAngle( v_pc.x,    v_pc.y,    v_pc.z,
					 v_pn_n.x,  v_pn_n.y,  v_pn_n.z,
					 v_pca_n.x, v_pca_n.y, v_pca_n.z,
					 v_pc_n.x,  v_pc_n.y,  v_pc_n.z  );

			rot_axis = (pcarbona_n->getPosition()-pnitrogen_n->getPosition()).normalize();

			transmat.rotate(Angle(-1. * current), rot_axis);
			transformation.setTransformation(transmat);

			//translate to 0|0|0 (needed for the rotation)
			toOrigin = pcarbona_n->getPosition();
			translation.setTranslation(((float)(-1.))*toOrigin);
			 resnew.apply(translation);

			resnew.apply(transformation);
			// translate back to the correct position
			translation.setTranslation(toOrigin);
			resnew.apply(translation);

			return;
		}

		void PeptideBuilder::transform_
			(const Angle& phi, const Angle& psi, Residue& resold, Residue& resnew)
		{
			Matrix4x4 psimat;   // rotation matrix
			Matrix4x4 phimat;   //    "
			Vector3   psiaxis;  // rotation axis for the torsion angles
			Vector3   phiaxis;
			TranslationProcessor translation;


			PDBAtom* pcarbon     = getAtomByName_(resold, "C");
			PDBAtom* pcarbona    = getAtomByName_(resold, "CA");
			PDBAtom* pnitrogen_n = getAtomByName_(resnew, "N");
			PDBAtom* pcarbona_n  = getAtomByName_(resnew, "CA");
			PDBAtom* pcarbon_n   = getAtomByName_(resnew, "C");
			PDBAtom* pnitrogen   = getAtomByName_(resold, "N");
			Angle currentpsi;
			Angle currentphi;

			Vector3 v_pn     = pnitrogen->getPosition();
			Vector3 v_pca    = pcarbona->getPosition();
			Vector3 v_pc     = pcarbon->getPosition();
			Vector3 v_pn_n   = pnitrogen_n->getPosition();
			Vector3 v_pca_n  = pcarbona_n->getPosition();
			Vector3 v_pc_n   = pcarbon_n->getPosition();
			currentpsi = getTorsionAngle( v_pn.x,    v_pn.y,   v_pn.z,
							v_pca.x,   v_pca.y,  v_pca.z,
							v_pc.x,  v_pc.y, v_pc.z,
							v_pn_n.x,  v_pn_n.y,  v_pn_n.z);

			psiaxis = ( pcarbon->getPosition() - pcarbona->getPosition()).normalize();
			psimat.rotate((float)-1. * currentpsi, psiaxis);
			TransformationProcessor psitrans(psimat);

			//translate  to 0|0|0 (needed for the rotation)
			Vector3 toOrigin = pcarbon->getPosition();
			translation.setTranslation(((float)(-1.))*toOrigin);
			resnew.apply(translation);

			//rotate
			resnew.apply(psitrans);

			psimat.rotate(psi, psiaxis);
			psitrans.setTransformation(psimat);
			resnew.apply(psitrans);

			//translate back
			translation.setTranslation(toOrigin);
			resnew.apply(translation);

			v_pn     = pnitrogen->getPosition();
			v_pca    = pcarbona->getPosition();
			v_pc     = pcarbon->getPosition();
			v_pn_n   = pnitrogen_n->getPosition();
			v_pca_n  = pcarbona_n->getPosition();
			v_pc_n   = pcarbon_n->getPosition();

			currentphi = getTorsionAngle( v_pc.x,    v_pc.y,    v_pc.z,
					 v_pn_n.x,  v_pn_n.y,  v_pn_n.z,
					 v_pca_n.x, v_pca_n.y, v_pca_n.z,
					 v_pc_n.x,  v_pc_n.y,  v_pc_n.z);


			phiaxis = (pcarbona_n->getPosition() -  pnitrogen_n->getPosition()).normalize();
			phimat.rotate(phi, phiaxis);
			TransformationProcessor phitrans(phimat);

			// translate to 0|0|0 (needed for the rotation)
			toOrigin =  pcarbona_n->getPosition();
			translation.setTranslation(((float)(-1.))*toOrigin);
			resnew.apply(translation);

			// rotate 
			resnew.apply(phitrans);

			// translate back to the correct position
			translation.setTranslation(toOrigin);
			resnew.apply(translation);

			return;
		}

		void PeptideBuilder::peptide_(Residue& resold, Residue& resnew)
		{
			PDBAtom* poxygen = new PDBAtom(PTE[Element::O], "O");
			// dynamic_cast<PDBAtom*>(resold->getAtom("C"));   ?????
			PDBAtom* pcarbon     = getAtomByName_(resold, "C");
			PDBAtom* pcarbona    = getAtomByName_(resold, "CA");
			PDBAtom* pnitrogen_n = getAtomByName_(resnew, "N");

			Vector3 CA_C_axis =(pcarbon->getPosition() - pcarbona->getPosition()).normalize();
			Vector3 C_NN_axis =(pnitrogen_n->getPosition() - pcarbon->getPosition()).normalize();

			Matrix4x4 mat;
			mat.setRotation(Angle(Constants::PI), CA_C_axis);
			Vector3  newpos = mat * C_NN_axis;

			// newpos = mat * newpos; 
			newpos = newpos.normalize() * BOND_LENGTH_C_O + pcarbon->getPosition();

			//newpos= pcarbon->getPosition();
			poxygen->setPosition(newpos);

			//set C-O-bond
			(pcarbon->createBond(*poxygen))->setOrder(Bond::ORDER__DOUBLE);
			resold.insert(*poxygen);

			//----------set hydrogen. We can't do this for proline,
			//					since in this case, this hydrogen doesn't exist
			if (!is_proline_)
			{
				PDBAtom* phydrogen = new PDBAtom(PTE[Element::H], "H");
				pcarbon     = getAtomByName_(resold, "C");
				pnitrogen_n = getAtomByName_(resnew, "N");
				poxygen     = getAtomByName_(resold, "O");

				newpos =  (pcarbon->getPosition() - poxygen->getPosition()).normalize();

				newpos = newpos * BOND_LENGTH_N_H + pnitrogen_n->getPosition();
				phydrogen->setPosition(newpos);
				phydrogen->createBond(*pnitrogen_n)->setOrder(Bond::ORDER__SINGLE);;
				resnew.insert(*phydrogen);
			}
			return;
		}

		void PeptideBuilder::setOmega_(Residue& resold, Residue& resnew, const Angle& omega)
		{
			PDBAtom* pcarbon     = getAtomByName_(resold, "C");
			PDBAtom* pnitrogen_n = getAtomByName_(resnew, "N");

			Vector3 C_NN_axis =(pnitrogen_n->getPosition() - pcarbon->getPosition()).normalize();

			// At this point, omega has been fixed to 180 degrees
			Matrix4x4 mat;
			mat.setRotation(Angle(omega-Angle(Constants::PI)), C_NN_axis);
			TransformationProcessor omegatrans(mat);

			// translate to 0|0|0 (needed for the rotation)
			Vector3 toOrigin =  pnitrogen_n->getPosition();
			TranslationProcessor translation;
			translation.setTranslation(((float)(-1.))*toOrigin);
			resnew.apply(translation);

			// rotate 
			resnew.apply(omegatrans);

			// translate back to the correct position
			translation.setTranslation(toOrigin);
			resnew.apply(translation);

			return;
		}

		PDBAtom* PeptideBuilder::getAtomByName_(Residue& res, const String& name)
		{
			PDBAtomIterator ai;
			for (ai = res.beginPDBAtom(); +ai; ++ai)
			{
				if (ai->getName() == name)
				{
					return &(*ai);
				}
			}

			return 0;
		}


		void PeptideBuilder::setFragmentDB(const FragmentDB* db)
		{
			fragment_db_ = (FragmentDB*) db;
		}

		const FragmentDB* PeptideBuilder::getFragmentDB() const
		{
			return fragment_db_;
		}


	} // namespace Peptides
} // namespace BALL
