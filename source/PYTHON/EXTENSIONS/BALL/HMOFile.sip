// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//

class HMOFile 
	: public LineBasedFile
{
%TypeHeaderCode
  #include <BALL/FORMAT/HMOFile.h>
	using namespace BALL;
	using namespace BALL::Exception;
	typedef HMOFile::HMOCharge HMOFile_HMOCharge;
%End

	public:
		class HMOCharge
		{
			public:
				Vector3 position;
				float   value;

//				bool operator == (const HMOCharge&) const;
		};

		enum ElementType
		{
			L2   = 60,          // line element with two nodes
			L3   = 63,          // line element with three nodes
			T3   = 103,         // triangular element with three nodes
			T6   = 106,         // triangular element with six nodes
			Q4   = 104,         // quadrilateral element with four nodes
			Q8   = 108,         // quadrilateral element with eight nodes
			TH4  = 204,         // tetrahedral element with four nodes
			TH10 = 210,         // tetrahedral element with ten nodes
			P6   = 206,         // pentahedral element with six nodes
			P15  = 215,         // pentahedral element with fifteen nodes
			H8   = 208,         // hexaedral element with eight nodes
			H20  = 220          // hexaedral element with twenty nodes
		};

		HMOFile();
		HMOFile(const String& filename, OpenMode open_mode = std::ios::in);
		virtual ~HMOFile();

		virtual void clear();
		bool operator == (const HMOFile& file);
		bool hasCharges() const;
		vector<HMOFile_HMOCharge>& getCharges();
		//vector<HMOCharge> const& getCharges() const;
		bool hasComments() const;
		vector<String>& getComments();
		//vector<String> const& getComments();

		virtual bool open(const String& name, OpenMode open_mode = std::ios::in);
		virtual bool read(Surface& surface) throw(FileNotFound, ParseError);

		virtual bool write(const Surface& surface);
		virtual bool write(const Surface& surface, const AtomContainer& ac);

	private:
		HMOFile(const HMOFile&);
};
