// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: smartsParser.h,v 1.6 2006/01/25 14:41:46 bertsch Exp $
//

#ifndef BALL_STRUCTURE_SMARTES_PARSER_H
#define BALL_STRUCTURE_SMARTES_PARSER_H

#ifndef BALL_COMMON_H
#	include <BALL/common.h>
#endif

#ifndef BALL_KERNEL_ATOM_H
#	include <BALL/KERNEL/atom.h>
#endif

#ifndef BALL_KERNEL_BOND_H
#	include <BALL/KERNEL/bond.h>
#endif

#ifndef BALL_KERNEL_SYSTEM_H
#	include <BALL/KERNEL/system.h>
#endif

#ifndef BALL_KERNEL_EXPRESSION_H
#	include <BALL/KERNEL/expression.h>
#endif

#ifndef BALL_CONCEPT_PROPERTY_H
#	include <BALL/CONCEPT/property.h>
#endif

#include <utility>
#include <vector>

namespace BALL 
{

	/** @name	\brief SMARTS Parser.
	*/
	class SmartsParser
	{
		public:

		enum ZEIsomerType
		{
			ANY_ZE = 1,
			NONE,
			Z,
			E
		};

		// chiral class definitions CW = clock wise, CCW = counter clock wise
		enum ChiralClass
		{
			CHIRAL_CLASS_UNSPECIFIED = 1,
	    NONCHIRAL,
			NONCHIRAL_OR_UNSPECIFIED,
			CW_DEFAULT, // TH
			CW_DEFAULT_OR_UNSPECIFIED,
			CCW_DEFAULT, // TH
			CCW_DEFAULT_OR_UNSPECIFIED,
			CW_TH, // tetrahdral
			CW_TH_OR_UNSPECIFIED,
			CCW_TH,
			CCW_TH_OR_UNSPECIFIED,
			CW_AL, // allene-like
			CW_AL_OR_UNSPECIFIED,
			CCW_AL, 
			CCW_AL_OR_UNSPECIFIED,
			CW_SP, // square planar
			CW_SP_OR_UNSPECIFIED,
			CCW_SP,
			CCW_SP_OR_UNSPECIFIED,
			CW_TB, //trigonal bipyramidal
			CW_TB_OR_UNSPECIFIED,
			CCW_TB,
			CCW_TB_OR_UNSPECIFIED,
			CW_OH, // octahedral
			CW_OH_OR_UNSPECIFIED,
			CCW_OH,
			CCW_OH_OR_UNSPECIFIED
		};

		enum LogicalOperator
		{
			AND,
			OR,
			AND_LOW,
			NOOP
		};

		//typedef std::pair<ChiralClass, Position> ChiralDef;

		class SPAtom;
		class SPBond 
/*			:	public Bond*/
		{
			public:

				enum SPBondOrder
				{
					SINGLE = 1,
					SINGLE_UP,
					SINGLE_UP_OR_ANY,
					SINGLE_DOWN,
					SINGLE_DOWN_OR_ANY,
					AROMATIC,
					DOUBLE,
					TRIPLE,
					NOT_NECESSARILY_CONNECTED,
					IN_RING,
					ANY
				};
			
				SPBond();
				SPBond(SPBondOrder bond_order);
				SPBond(SPAtom* first, SPAtom* second, SPBondOrder bond_order);
				virtual ~SPBond() throw();

				ZEIsomerType getZEType() const { return ze_type_; }
				void setZEType(ZEIsomerType type) { ze_type_ = type; }
				void setBondOrder(SPBondOrder bond_order);
				SPBondOrder getBondOrder() const { return bond_order_; }
				bool getNot() const { return not_; }
				void setNot(bool is_not) { not_ = is_not; }

				// returns true if the SPBond matches the given bond
				bool equals(const Bond* bond) const;

			protected:
				ZEIsomerType	ze_type_;
				SPBondOrder bond_order_;
				bool not_;
		};
		
		class SPAtom
	/*		:	public Atom */
			: public PropertyManager
		{
			public:

				/** Common properties are:
				 *
				 * 	bool 		is_not
				 * 	Size		isotope 
				 * 	bool		not_isotope
				 * 	Index		charge
				 * 	bool 		not_charge
				 * 	bool		is_aromatic
				 * 	bool		not_aromatic
				 * 	bool		aliphatic
				 * 	bool		not_aliphatic
				 * 	Index		in_num_rings
				 * 	bool 		not_in_num_rings
				 * 	bool 		in_brackets
				 * 	Index		in_ring_size
				 * 	bool		not_in_ring_size
				 * 	Size		connected
				 * 	bool		not_connected_to
				 * 	Size		explicit_hydrogens
				 * 	bool		not_explicit_hydrogens
				 * 	Size		valence
				 * 	bool		not_valence
				 * 	Size		implicit_hydrogens
				 * 	bool		not_implicit_hydrogens
				 * 	Size		degree
				 * 	bool		not_degree
				 * 	Size		ring_connected
				 * 	bool		not_ring_connected
				 * 	bool		not_chirality
				 * 	String	symbol
				 *
				 * 	all this properties can be set
				 */
				
				SPAtom();
				SPAtom(const String& symbol);
				virtual ~SPAtom() throw();

				void addAtomProperty(NamedProperty property) throw(Exception::ParseError);

				Size getDefaultValence(const Atom* atom) const;
				Size countRealValences(const Atom* atom) const;
				Size getNumberOfImplicitHydrogens(const Atom* atom) const;

				//ChiralDef getChirality() const { return chirality_; }
				//void setChirality(const ChiralDef& chirality) { chirality_ = chirality; }
			
				// returns true if the local properties of the atoms match
				bool equals(const Atom* atom) const;
				
			protected:
			
				//ChiralDef chirality_;
				Atom* atom_;
				void init_();
		};

	
		class SPNode;

		class SPEdge
		{
			public:
				SPEdge();
				SPEdge(const SPEdge& sp_edge);
				~SPEdge();
				bool isInternal() const { return internal_; }
				void setInternal(bool internal) { internal_ = internal; }
				
				void setSPBond(SPBond* sp_bond) { bond_ = sp_bond; }
				SPBond* getSPBond() const { return bond_; }
				
				void setFirstSPNode(SPNode* first) { first_ = first; }
				SPNode* getFirstSPNode() const { return first_; } 
				
				void setSecondSPNode(SPNode* second) { second_ = second; }
				SPNode* getSecondSPNode() const { return second_; }
				
				SPNode* getPartnerSPNode(SPNode* node) { return node == first_ ? second_ : first_; }
				
				bool getNot() const { return is_not_; }
				void setNot(bool is_not) { is_not_ = is_not; }
			
				void setFirstSPEdge(SPEdge* first) { first_edge_ = first; }
				SPEdge* getFirstSPEdge() const { return first_edge_; }

				void setSecondSPEdge(SPEdge* second) { second_edge_ = second; }
				SPEdge* getSecondSPEdge() const { return second_edge_; }
			
				void setLogicalOperator(LogicalOperator log_op) { log_op_ = log_op; }
				LogicalOperator getLogicalOperator() const { return log_op_; }
			
			protected:
				bool internal_;
				bool is_not_;
				SPNode* first_;
				SPNode* second_;
				SPBond* bond_;
				SPEdge* first_edge_;
				SPEdge* second_edge_;
				LogicalOperator log_op_;
		};
	
		class SPNode
		{
			public:
		
				typedef std::vector<SPEdge*>::iterator EdgeIterator;
				typedef std::vector<SPEdge*>::const_iterator EdgeConstIterator;
		
				SPNode();
				SPNode(SPAtom* atom);
				SPNode(SPNode* first, LogicalOperator log_op, SPNode* second);
				SPNode(const SPNode& sp_node);
				~SPNode();
				bool isInternal() const { return internal_; }
				void setInternal(bool internal) { internal_ = internal; }
				
				bool isRecursive() const { return recursive_; }
				void setRecursive(bool recursive); 
				/*{ 
					for (EdgeIterator it = begin(); it != end(); ++it)
					{
						rec_edges_.insert(*it);
					}
					recursive_ = recursive; 
				}*/
			
				void setComponentNumber(int no) { component_no_ = no; }
				Size getComponentNumber() { return component_no_; }
			
				SPAtom* getSPAtom() const { return sp_atom_; }
				void setSPAtom(SPAtom* sp_atom) { sp_atom_ = sp_atom; }
		
				//SPEdge* getPreEdge() const { return pre_edge_; }
				//void setPreEdge(SPEdge* pre_edge) { pre_edge_ = pre_edge; }
				
				SPEdge* getFirstEdge() const { return first_edge_; }
				void setFirstEdge(SPEdge* first) { first_edge_ = first; }

				SPEdge* getSecondEdge() const { return second_edge_; }
				void setSecondEdge(SPEdge* second) { second_edge_ = second; }
		
				bool getNot() const { return is_not_; }
				void setNot(bool is_not) { is_not_ = is_not; }

				void setInBrackets() { in_brackets_ = true; }

				void addSPEdge(SPEdge* sp_edge) { edges_.push_back(sp_edge); }

				void setLogicalOperator(LogicalOperator log_op) { log_op_ = log_op; }
				LogicalOperator getLogicalOperator() const { return log_op_; }	

				Size countEdges() const { return edges_.size(); }

				EdgeIterator begin() { return edges_.begin(); }
				EdgeIterator end() { return edges_.end(); }
				EdgeConstIterator begin() const { return edges_.begin(); }
				EdgeConstIterator end() const { return edges_.end(); }

			protected:
				
				bool internal_;
				bool is_not_;
				bool recursive_;
				bool in_brackets_;
				LogicalOperator log_op_;
				std::vector<SPEdge*> edges_;
				//SPEdge* pre_edge_;
				SPEdge* first_edge_;
				SPEdge* second_edge_;
				SPAtom* sp_atom_;
				int component_no_;
		};

	
		/**	@name Constructors and Destructors
		*/
		//@{

		///
		SmartsParser();
			
		///
		SmartsParser(const SmartsParser& parser);

		///
		virtual ~SmartsParser();
		//@}
		
		/**	@name	Parsing
		*/
		//@{
		/**	Parse a SMARTS string.
		*/
		void parse(const String& s)
			throw(Exception::ParseError);

		/**	@name Methods required by the underlying YACC parser
		*/
		//@{
		///
		SPAtom* createAtom(const String& symbol, bool in_bracket = false);

		///
		//void createBond(SPAtom* left, SPAtom* right, SPBond::SPBondOrder bond_order);
			
		void setRoot(SPNode* root) { root_ = root; }

		SPNode* getRoot() const { return root_; }

		// dumps the tree to cerr
		void dumpTree();
		
		
		void clear();
		//@}
		
		void addRingConnection(SPNode* spnode, Size index);
		
		HashMap<Size, std::vector<SPNode*> > getRingConnections() const;
	
		void setSSSR(const std::vector<std::vector<Atom*> >& sssr);

		void setNeedsSSSR(bool needs_sssr) { needs_SSSR_ = needs_sssr; }

		bool getNeedsSSSR() const { return needs_SSSR_; }

		void setRecursive(bool recursive) { recursive_ = recursive; }

		bool isRecursive() const { return recursive_; }

		struct State
		{
			Size					char_count;
			SmartsParser*	current_parser;
			const char*		buffer;
		};
		
		static State state;

		void setNextComponentNumberToSubTree(SPNode* spnode);

		const HashSet<SPNode*>& getNodes() const { return nodes_; }

		const HashSet<SPEdge*>& getEdges() const { return edges_; }

		void addEdge(SPEdge* edge) { edges_.insert(edge); }

		void addNode(SPNode* node) { nodes_.insert(node); }

		bool hasRecursiveEdge(SPEdge* edge) const { return rec_edges_.has(edge); }

		void addRecursiveEdge(SPEdge* edge) { rec_edges_.insert(edge); }

		protected:

			bool needs_SSSR_;

			bool recursive_;

			static vector<HashSet<const Atom*> >* sssr_;

			void dumpTreeRecursive_(SPNode* node, Size depth);

			void dumpTreeRecursive_(SPEdge* edge, Size depth);
			
			HashMap<Size, std::vector<SPNode*>	> ring_connections_;
			
			static SmartsParser*	current_parser_;
			
			HashSet<SPEdge*> edges_;
			
			HashSet<SPNode*> nodes_;

			HashSet<SPEdge*> rec_edges_;

			SPNode* root_;

			int component_no_;
	};
  
} // namespace BALL

#endif // BALL_STRUCTURE_SMARTS_PARSER_H
