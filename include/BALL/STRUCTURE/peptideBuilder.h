// $Id: peptideBuilder.h,v 1.1 2003/04/16 10:58:49 oliver Exp $
// ???? Editor header

#ifndef BALL_COMMON_H
#	include <BALL/common.h>
#endif

#ifndef BALL_STRUCTURE_PEPTIDES_H
#	include <BALL/STRUCTURE/peptides.h>
#endif

#ifndef BALL_KERNEL_PROTEIN_H
#	include <BALL/KERNEL/protein.h>
#endif


namespace BALL
{
	namespace Peptides
	{
		/** This class represents one amino acid in the sequence.
		*/
		class AminoAcidDescriptor
		{
			public:
	   
	   /** Default constructor.
	    */
	   AminoAcidDescriptor();
	   
	   /** Detailed constructor. Here, type is considered to be
	    *  either a three - letter or a one letter string for the
	    *  amino acid type, phi is the torsion angle phi, psi is
	    *  the psi torsion angle and omega is the angle of the peptide
	    *  bond.
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
       
      /**
      */
       class PeptideBuilder
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
	   
	 protected:
	   std::vector<AminoAcidDescriptor> sequence_;
	   String chainname_;
	   String proteinname_;
	   
	   /** some helper functions for the construction of the residue; for internal use only
	    * omega is not yet implemented
	    */
	   Residue* createResidue_(const String& type);
	   void	insert_(Residue& resnew, Residue& resold);
	   void	transform_(const Angle& phi, const Angle& psi, const Angle& omega, Residue& resold, Residue& resnew);
	   void peptide_(Residue& resold, Residue& resnew);
	   PDBAtom* getAtomByName_(Residue& res, const String& name);
	 };
       
     } // namespace Peptides
} // namespace BALL
