// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//

class AminoAcidDescriptor
{
%TypeHeaderCode
	#include <BALL/STRUCTURE/peptideBuilder.h>
	using namespace BALL;
	using namespace BALL::Peptides;
%End
  public:

	 AminoAcidDescriptor();
	 
	 AminoAcidDescriptor(const String& , const Angle& =Angle(-47.,false),
					 const Angle& = Angle(-58.,false), const Angle& = Angle(180.,false));

	 virtual ~AminoAcidDescriptor();
	 
	 AminoAcidDescriptor(const AminoAcidDescriptor& );

	 void setAminoAcidType(const String& );
	 
	 void setPhi(const Angle& );
	 
	 void setPsi(const Angle& );
	 
	 void setOmega(const Angle& );
	 
	 const String& getType() const;
	 
	 const Angle& getPhi() const;
	 
	 const Angle& getPsi() const;
	 
	 const Angle& getOmega() const;
	   
  SIP_PYOBJECT __str__();
%MethodCode
	sipRes = PyString_FromString((String("{ ") + sipCpp->getType() + " " + 
			String(sipCpp->getPhi().toDegree()) + " " + 
			String(sipCpp->getPsi().toDegree()) + " " + 
			String(sipCpp->getOmega().toDegree()) + " }").c_str());
%End
};

class PeptideBuilder
{
%TypeHeaderCode
	#include <BALL/STRUCTURE/peptideBuilder.h>
	using namespace BALL;
	using namespace BALL::Peptides;
%End
  public:

	   PeptideBuilder();
	   
		 //PeptideBuilder(const vector<AminoAcidDescriptor>& sequence);

		 //PeptideBuilder(const String&, const Angle&, const Angle&, const Angle&);
	   
		 PeptideBuilder(const String&, const Angle& = Angle(-47., false),
									const Angle& = Angle(-58., false), const Angle& = Angle(180., false));
	   
	   PeptideBuilder(const PeptideBuilder&);

	   virtual ~PeptideBuilder();
	   
	   //void addAminoAcid(const String&, const Angle&, const Angle&, const Angle&);
	   
	   void addAminoAcid(const String&, const Angle& =Angle(-47.,false),
			     const Angle& =Angle(-58.,false), const Angle& =Angle(180.,false));
	   
	   void addAminoAcid(const AminoAcidDescriptor&);

	   void setChainName(const String&);
	  
	   const String& getChainName() const;

	   void setProteinName(const String&);
	  
	   const String& getProteinName() const;

	   Protein* construct();

		 void setFragmentDB(const FragmentDB*);

		 const FragmentDB* getFragmentDB() const;
};
