// $Id: MOL2File.h,v 1.1 1999/08/26 07:53:20 oliver Exp $

#ifndef BALL_FORMAT_MOL2FILE_H
#define BALL_FORMAT_MOL2FILE_H

#ifndef BALL_COMMON_H
# include <BALL/common.h>
#endif

#ifndef BALL_SYSTEM_FILE_H
#	include <BALL/SYSTEM/file.h>
#endif

#ifndef BALL_KERNEL_SYSTEM_H
# include <BALL/KERNEL/system.h>
#endif

namespace BALL 
{

	/**	HyperChem file class.
			This class enables BALL to read and write SYBYL MOL2 files (Tripos).
			\\
			{\bf Definition:} \URL{BALL/FORMAT/MOL2File.h}
			\\
	*/
	class MOL2File
		: public File
	{
		public:

		/**	@name	Constants	
		*/
		//@{
			
		/**	Tripos Record Type Identifier (RTI)
		*/
		static const String TRIPOS;
		//@}
		
		/**	@name	Constructors and Destructors
		*/
		//@{

		/**	Default constructor
		*/
		MOL2File();

		/// Filename constructor
		MOL2File(const String& filename);

		/// Destructor
		virtual ~MOL2File();
		
		//@}

		/**	@name Reading and Writing of Kernel Datastructures
		*/
		//@{
		
		/**	Write a system to the HIN file
		*/
		virtual void write(const System&	system);
		
		/**	Read a system from the HIN file
		*/
		virtual void read(System&	system);

		/**
		*/
		virtual MOL2File& operator >> (System& system)
		{
			read(system);
			
			return *this;
		}
		
		/**
		*/
		virtual MOL2File& operator << (const System& system)
		{
			write(system);
			
			return *this;
		}
		
		//@}

		protected:
		
		virtual String readAtomSection_();

		virtual String readBondSection_();

		virtual String readMoleculeSection_();

		virtual String readSetSection_();

		virtual String readSubstructureSection_();

		struct AtomStruct
		{
			String	name;
			Vector3	position;
			String	type;
			int			substructure;
			String	substructure_name;
			float		charge;
		};

		struct BondStruct
		{
			int			atom1;
			int			atom2;
			String	type;
		};

		struct MoleculeStruct
		{
			String			name;
			int					number_of_atoms;
			int					number_of_bonds;
			int					number_of_substructures;
			int					number_of_features;
			int					number_of_sets;
			String			type;
			String			charge_type;
			String			comment;
		};

		struct SetStruct
		{
			String			name;
			String			type;
			String			subtype;
			String			comment;
			int					number_of_members;
			vector<int>	members;
		};
		
		struct SubstructureStruct
		{
			String			name;
			int					root_atom;
			String			substructure_type;
			int					dictionary_type;
			String			chain;
			String			sub_type;
			int					inter_bonds;
			String			comment;
		};

			
		vector<AtomStruct>					atoms_;
		vector<BondStruct>					bonds_;
		vector<SetStruct>						sets_;
		vector<MoleculeStruct>			molecules_;
		vector<SubstructureStruct>	substructures_;

		Size	number_of_lines_;
		static const Size MAX_LENGTH_;
		static char buffer_[4096];
	
	};

} // namespace BALL

#endif // BALL_FORMAT_MOL2FILE_H
