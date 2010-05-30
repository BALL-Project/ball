// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//

#ifndef BALL_STRUCTURE_PEPTIDEBUILDER_H
#define BALL_STRUCTURE_PEPTIDEBUILDER_H

#ifndef BALL_STRUCTURE_PEPTIDES_H
#	include <BALL/STRUCTURE/peptides.h>
#endif

namespace BALL
{
	class FragmentDB;

	namespace Peptides
	{
		/** This class represents one amino acid in the sequence.
				\ingroup StructurePeptideBuilder
		*/
		class BALL_EXPORT AminoAcidDescriptor
		{
			public:
	   
	   /** Default constructor.
	    */
	   AminoAcidDescriptor();
	   
	   /** Detailed constructor. Here, type is considered to be
	    *  either a three - letter or a one letter string for the
	    *  amino acid type, phi is the torsion angle phi, psi is
	    *  the psi torsion angle and omega is the angle of the peptide
	    *  bond. By default, a standard alpha-helical geometry is constructed.
	    */
	   AminoAcidDescriptor(const String& type, const Angle& phi=Angle(-47.,false),
			       const Angle& psi=Angle(-58.,false), const Angle& omega=Angle(180.,false));
	   /// Default angles: sheet ????
	   
	   /** Destructor.
	    */
	   virtual ~AminoAcidDescriptor();
	   
	   /** copy constructor
	    */
	   AminoAcidDescriptor(const AminoAcidDescriptor& aad);

	   /** Set the type of the amino acid. type is considered to be a
	    *  one - or three letter code.
	    */
	   void setAminoAcidType(const String& type);
	   
	   /** Set the torsion angle phi.
	    */
	   void setPhi(const Angle& phi);
	   
	   /** Set the torsion angle psi.
	    */
	   void setPsi(const Angle& psi);
	   
	   /** Set the peptide bond angle omega.
	    */
	   void setOmega(const Angle& omega);
	   
	   /** Returns the type of this amino acid.
	    */
	   const String& getType() const;
	   
	   /** Returns the torsion angle phi.
	    */
	   const Angle& getPhi() const;
	   
	   /** Returns the torsion angle psi.
	    */
	   const Angle& getPsi() const;
	   
	   /** Returns the angle of the peptide bond omega.
	    */
	   const Angle& getOmega() const;
	   
	 protected:
	   
	   String type_;
	   Angle phi_;
	   Angle psi_;
	   Angle omega_;
	 };
       
   /** Build a Peptide from a sequence and the corresponing angles.
			 Dont forget to call setFragmentDB() before using this class.
			\ingroup StructurePeptideBuilder
   */
   class BALL_EXPORT PeptideBuilder
	 {
		 public:

	   /** Default constructor.
	    */
	   PeptideBuilder();
	   
	   /** Detailed constructor. This constructor takes a vector of
	    * 	AminoAcidDescriptors and prepares everything for the construction
	    * 	process.
	    */
	   PeptideBuilder(const std::vector<AminoAcidDescriptor>& sequence);

		 /** Construct a peptide from a one-letter code sequence.
		 */
		 PeptideBuilder(const String& sequence, const Angle& phi = Angle(-47., false),
										const Angle& psi = Angle(-58., false), const Angle& omega = Angle(180., false));
	   
	   /** copy constructor 
	    */
	   PeptideBuilder(const PeptideBuilder& pc);

	   /** Destructor.
	    */
	   virtual ~PeptideBuilder();
	   
	   /** Adds another amino acid to the sequence.
	    */
	   void addAminoAcid(const String& type, const Angle& phi=Angle(-47.,false),
			     const Angle& psi=Angle(-58.,false), const Angle& omega=Angle(180.,false));
	   // Sheet angles ???
	   
	   /** Adds another amino acid to the sequence.
	    */   
	   void addAminoAcid(const AminoAcidDescriptor& aad);

	   /** Sets the name of the chain.
	    */  
	   void setChainName(const String& name);
	  
	   /** Returns the name of the chain.
	    */  
	   const String& getChainName() const;

	   /** Sets the name of the protein.
	    */  
	   void setProteinName(const String& name);
	  
	   /** Returns the name of the protein.
	    */  
	   const String& getProteinName() const;

	   /** Constructs the peptide and returns it.
	    */
	   Protein* construct();

		 ///
		 void setFragmentDB(const FragmentDB* db)
			 ;

		 ///
		 const FragmentDB* getFragmentDB() const
			 ;
	   
	 protected:
	   std::vector<AminoAcidDescriptor> sequence_;
	   String chainname_;
	   String proteinname_;
	   
		 /// The proline flag
		 bool is_proline_;
		 FragmentDB* fragment_db_;

	   /** some helper functions for the construction of the residue; for internal use only
	    * omega is not yet implemented
	    */
	   Residue* createResidue_(const String& type, const int id);
	   void	insert_(Residue& resnew, Residue& resold);
	   void	transform_(const Angle& phi, const Angle& psi, Residue& resold, Residue& resnew);
	   void peptide_(Residue& resold, Residue& resnew);
		 void setOmega_(Residue& resold, Residue& residue, const Angle& omega);
	   PDBAtom* getAtomByName_(Residue& res, const String& name);
	 };
       
  } // namespace Peptides

} // namespace BALL

#endif // BALL_STRUCTURE_PEPTIDEBUILDER_H
