class ResidueRotamerSet
	{
%TypeHeaderCode
	#include <BALL/STRUCTURE/residueRotamerSet.h>
	using namespace BALL;
%End
		public:
		//typedef vector<Rotamer>::const_iterator	ConstIterator;
		// typedef vector<Rotamer>::const_iterator	const_iterator;
		// typedef vector<Rotamer>::iterator				Iterator;
		// typedef vector<Rotamer>::iterator				iterator;

		ResidueRotamerSet();
		ResidueRotamerSet(const ResidueRotamerSet& rotamer_set);
		ResidueRotamerSet(const Residue& residue, Size number_of_torsions);
		virtual ~ResidueRotamerSet();

		//Iterator begin();
	/*	{
			return rotamers_.begin();
		} */

		//Iterator end();
	/*	{
			return rotamers_.end();
		} */

		//ConstIterator begin() const;
		/* {
			return rotamers_.begin();
		} */

		//ConstIterator end() const;
		/*{
			return rotamers_.end();
		}*/

		//const ResidueRotamerSet& operator = (const ResidueRotamerSet& residue_rotamer_set);
		const String& getName() const; 
		bool isValid() const; 
		Size getNumberOfRotamers() const;
		Size getNumberOfTorsions() const;
		//void setNumberOfTorsions(Size number_of_torsions) throw(Exception::IndexOverflow);
		//const Rotamer& operator [] (Position index)	const throw(Exception::IndexOverflow);
		bool hasTorsionPhi() const;
		Angle getTorsionPhi() const;
		void setTorsionPhi(const Angle& phi);
		bool hasTorsionPsi() const;
		Angle getTorsionPsi() const;
		void setTorsionPsi(const Angle& psi);
		bool setTemplateResidue(const Residue& residue, Size number_of_torsions);
		bool setRotamer(Residue& residue, const Rotamer& rotamer);
		Rotamer getRotamer(const Residue& residue) const;
		const Rotamer& getRotamer(Position index) const;
		void setName(const String& name); 
		void addRotamer(const Rotamer& rotamer);
	//	void deleteRotamer(Iterator loc);
	//	void deleteRotamers(Iterator begin, Iterator end);
		void sort();

		/*
		struct RotamerProbabilityGreaterThan_ //: std::binary_function<Rotamer, Rotamer, bool>
		{
			bool operator () (const Rotamer& r1, const Rotamer& r2);
				return r1.P > r2.P;
		};
		*/

		//void addMovable_(std::vector<String>& movable, const Atom& a, const HashSet<String>& assigned_atoms); 
	//	void setTorsionAngle_(Residue& residue, const std::vector<String>& movable, float torsion); 

		/*
		bool valid_; 
		String name_;
		std::vector<Rotamer> rotamers_;
		std::vector<String> movable_atoms_chi1_;
		std::vector<String> movable_atoms_chi2_;
		std::vector<String> movable_atoms_chi3_;
		std::vector<String> movable_atoms_chi4_;

		Size number_of_torsions_;
		StringHashMap<Vector3> original_coordinates_;
		bool has_torsion_phi_;
		bool has_torsion_psi_;
		Angle phi_;
		Angle psi_;
*/
	};

