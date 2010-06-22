// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//

#ifndef BALL_STRUCTURE_SMARTES_PARSER_H
#define BALL_STRUCTURE_SMARTES_PARSER_H

#ifndef BALL_COMMON_H
	#	include <BALL/common.h>
#endif

#include <map>
#include <set>

// needed for MSVC:
#undef CW_DEFAULT

namespace BALL 
{
	// forward declarations
	class Bond;
	class Atom;
	class Element;

	/** SMARTS Parser.
	 
	 		This class implements the parser for SMARTS patterns. The SMARTS 
			string is converted into a tree, which is used for matching it
			to molecules. The tree has also some additional features, i.e. 
			additional edges which allows also for graph features (kind of cyclic
			structure).

			\ingroup StructureMatching
	*/
	class BALL_EXPORT SmartsParser
	{
		public:

		enum ZEIsomerType
		{
			ANY_ZE = 1,
			NONE,
			Z,
			E
		};

		/// chiral class definitions CW = clock wise, CCW = counter clock wise
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

		/** The logical operator supported by SMARTS-pattern <br>
		 		'&' -> and <br>
				',' -> or <br>
				';' -> low precedence and<br>

				NOOP is just provided for convenience.
		*/
		enum LogicalOperator
		{
			AND,
			OR,
			AND_LOW,
			NOOP
		};


		/// forward declaration
		class SPAtom;

		/** @brief Bond representation of the smarts parser
		
				This class represents a bond of the smarts parser. The normal
				bond representation of BALL is not sufficient in this case, 
				because many other properties are needed for the SMARTS-patterns.
				For example the "or any" orders
		*/
		class BALL_EXPORT SPBond 
		{
			public:

				/// the bond orders supported by SMARTS-patterns
				enum SPBondOrder
				{
					SINGLE = 1,
					SINGLE_UP,
					SINGLE_UP_OR_ANY,
					SINGLE_DOWN,
					SINGLE_DOWN_OR_ANY,
					SINGLE_OR_AROMATIC,
					AROMATIC,
					DOUBLE,
					TRIPLE,
					NOT_NECESSARILY_CONNECTED,
					IN_RING,
					ANY
				};
		
				/** @name Constructors and destructors
				*/
				//@{
				/// Default constructor
				SPBond();

				/// Detailed constructor with bond order
				SPBond(SPBondOrder bond_order);

				/// Detailed constructor with 
				SPBond(SPAtom* first, SPAtom* second, SPBondOrder bond_order);

				/// Destructor
				virtual ~SPBond() ;
				//@}

				/** @name Accessors
				*/
				//@{
				/// returns the Z/E isomer type
				ZEIsomerType getZEType() const { return ze_type_; }

				/// sets the Z/E isomer type
				void setZEType(ZEIsomerType type) { ze_type_ = type; }

				/// sets the bond order
				void setBondOrder(SPBondOrder bond_order);

				/// returns the bond order
				SPBondOrder getBondOrder() const { return bond_order_; }

				/// return true if a general negation is set
				bool isNot() const { return not_; }

				/// set the general negation to the bool given
				void setNot(bool is_not) { not_ = is_not; }

				// returns true if the SPBond matches the given bond
				bool equals(const Bond* bond) const;
				//@}

			protected:

				/// Z/E isomer type
				ZEIsomerType	ze_type_;

				/// the bond order
				SPBondOrder bond_order_;

				/// general negation flag
				bool not_;
		};
		
		/** @brief Smarts Parser Atom class
		
				This class implements the representation of 
				a atom of the smarts parser. It is faster than 
				using the BALL Atom class with Properties. As all
				the possible properties are known from the definition
				of the SMARTS language, the properties are listed
				within the enumeration.
		*/
		class BALL_EXPORT SPAtom
		{
			public:

				/// enum of all properties possible for a smarts parser atom
				enum PropertyType
				{
					ISOTOPE = 1,
					CHARGE,
					AROMATIC,
					ALIPHATIC,
					IN_NUM_RINGS,
					IN_RING_SIZE,
					IN_BRACKETS,
					CONNECTED,
					EXPLICIT_HYDROGENS,
					VALENCE,
					IMPLICIT_HYDROGENS,
					DEGREE,
					RING_CONNECTED,
					CHIRALITY,
					SYMBOL
				};

				/// possible types of the properties
				union PropertyValue
				{
					int int_value;
					bool bool_value;
					const Element* element_value;
					ChiralClass chiral_class_value;
				};

				/// Property struct of smarts parser atom
				struct Property
				{
					public:
				
						/** @name Constructors and desctructors
						*/
						//@{
						/// Detailed constructor with type and int value
						Property(PropertyType type, int value);
						
						/// Detailed constructor with type and flag
						Property(PropertyType type, bool value);
						
						/// Detailed constructor with type and Element
						Property(PropertyType type, const Element* value);
						
						/// Detailed constructor with type and chiral class definition
						Property(PropertyType type, ChiralClass value);

						/// Destructor
						virtual ~Property();
						//@}

						/// assignment operator
						void operator = (const Property& rhs);

						/** @name Accessors
						*/
						//@{
						/// returns the type of the property
						PropertyType getType() const { return type_; }

						/// returns the value of the property
						PropertyValue getValue() const { return value_; }
						//@}

					private:
						
						/// Default constructor
						Property();
						
						/// type of the property
						PropertyType type_;

						/// value of the property
						PropertyValue value_;
				};
				
				/** Common properties are:
				 
				  	bool 		is_not
				  	Size		isotope 
				  	bool		not_isotope
				  	Index		charge
				  	bool 		not_charge
				  	bool		is_aromatic
				  	bool		not_aromatic
				  	bool		aliphatic
				  	bool		not_aliphatic
				  	Index		in_num_rings
				  	bool 		not_in_num_rings
				  	bool 		in_brackets
				  	Index		in_ring_size
				  	bool		not_in_ring_size
				  	Size		connected
				  	bool		not_connected_to
				  	Size		explicit_hydrogens
				  	bool		not_explicit_hydrogens
				  	Size		valence
				  	bool		not_valence
				  	Size		implicit_hydrogens
				  	bool		not_implicit_hydrogens
				  	Size		degree
				  	bool		not_degree
				  	Size		ring_connected
				  	bool		not_ring_connected
				  	bool		not_chirality
				  	String	symbol
				 
				 		all this properties can be set
				 */
			
				/** @name Constructors and destructors
				*/
				//@{
				/// Default constructor
				SPAtom();

				/// copy constructor
				SPAtom(const String& symbol);

				/// destructor
				virtual ~SPAtom() ;
				//@}


				/** @name Accessors
				*/
				//@{
				/// sets an int property
				void setProperty(PropertyType type, int int_value);
				
				/// sets a flag
				void setProperty(PropertyType type, bool  flag);

				/// sets a Element
				void setProperty(PropertyType type, const Element* element);

				/// sets a chirality value
				void setProperty(PropertyType type, ChiralClass chirality);

				/// sets a property
				void setProperty(Property property);

				/// adds properties from another SPAtom
				void addPropertiesFromSPAtom(SPAtom* sp_atom);

				/// negotiates the property definition
				void setNotProperty(PropertyType type);

				/// returns true if the property is set
				bool hasProperty(PropertyType type) const;

				/// returns a value of the given property type
				PropertyValue getProperty(PropertyType type);

				/// returns the number of properties
				Size countProperties() const;

				/// return the number of valences of the given atom
				Size getDefaultValence(const Atom* atom) const;
				
				/// returns the number of available valences of the given atom
				Size countRealValences(const Atom* atom) const;
				
				/// returns the number of implicit hydrogens of the given atom
				Size getNumberOfImplicitHydrogens(const Atom* atom) const;
				//@}

				/** @name Predicates
				*/
				//@{
				/// returns true if the local properties of the atoms match
				bool equals(const Atom* atom) const;
				//@}
				
			protected:

				/// the atom which this sp_atom belongs to
				Atom* atom_;

				/// the properties of this SPAtom
				std::map<PropertyType, PropertyValue> properties_;

				/// the properties which are negated
				std::set<PropertyType> not_properties_;
		};

		/// forward declaration	
		class SPNode;

		/** @brief Edge representation of the smarts parser graph
		*/
		class BALL_EXPORT SPEdge
		{
			public:

				/** @name Constructors and destructors
				*/
				//@{
				/// Default constructor
				SPEdge();

				/// Copy constructor
				SPEdge(const SPEdge& sp_edge);

				/// Destructor
				virtual ~SPEdge();
				//@}
				
				/** @name Acessors
				*/
				//@{
				/// returns true if this is a internal edge
				bool isInternal() const { return internal_; }

				/// set this edge to a internal edge
				void setInternal(bool internal) { internal_ = internal; }
				
				/// sets the corresponding SPBond of this edge 
				void setSPBond(SPBond* sp_bond) { bond_ = sp_bond; }

				/// returns the corresponding SPBond of this edge
				SPBond* getSPBond() const { return bond_; }
				
				/// set the first SPNode of this edge
				void setFirstSPNode(SPNode* first) { first_ = first; }

				/// returns the first SPNode of this edge
				SPNode* getFirstSPNode() const { return first_; } 
				
				/// sets the second SPNode of this edge
				void setSecondSPNode(SPNode* second) { second_ = second; }

				/// returns the second SPNode of this edge
				SPNode* getSecondSPNode() const { return second_; }
				
				/// returns the partner; either the first or the second SPNode
				SPNode* getPartnerSPNode(SPNode* node) { return node == first_ ? second_ : first_; }
				
				/// returns true if negation is enabled
				bool isNot() const { return is_not_; }

				/// set the negation flag
				void setNot(bool is_not) { is_not_ = is_not; }
			
				/// set the first SPEdge (first tree child)
				void setFirstSPEdge(SPEdge* first) { first_edge_ = first; }

				/// returns the first SPEdge (first tree child)
				SPEdge* getFirstSPEdge() const { return first_edge_; }

				/// set the second SPEdge (second tree child)
				void setSecondSPEdge(SPEdge* second) { second_edge_ = second; }

				/// returns the second SPEdge (second tree child)
				SPEdge* getSecondSPEdge() const { return second_edge_; }
		
				/// sets the associated logical operator (for the child edges)
				void setLogicalOperator(LogicalOperator log_op) { log_op_ = log_op; }

				/// returns the asociated logical operator (for the child edges)
				LogicalOperator getLogicalOperator() const { return log_op_; }
				//@}
			
			protected:

				/// internal flag
				bool internal_;

				/// negation flag
				bool is_not_;

				/// first SPNode
				SPNode* first_;

				/// second SPNode
				SPNode* second_;

				/// associated bond
				SPBond* bond_;

				/// first SPEdge
				SPEdge* first_edge_;

				/// second SPEdge
				SPEdge* second_edge_;

				/// logical operator associated with the SPEdges
				LogicalOperator log_op_;
		};

		/** @brief Representation of a node in the smarts parser graph
		*/
		class BALL_EXPORT SPNode
		{
			public:
		
				/** @name Iterators
				*/
				//@{
				/// non-constant edge iterator
				typedef std::vector<SPEdge*>::iterator EdgeIterator;

				/// constant edge iterator
				typedef std::vector<SPEdge*>::const_iterator EdgeConstIterator;
				//@}
	

				/** @name Constructors and destructors
				*/
				//@{
				/// Default constructor
				SPNode();

				/// Detailed constructor with an atom
				SPNode(SPAtom* atom);

				/// Detailed constructor with two nodes and a logical operator
				SPNode(SPNode* first, LogicalOperator log_op, SPNode* second);

				/// Copy constructor
				SPNode(const SPNode& sp_node);

				/// Destructor
				virtual ~SPNode();
				//@}
				

				/** @name Accessors
				*/
				//@{
				/// returns true if the SPNode is an internal node
				bool isInternal() const { return internal_; }

				/// sets the internal flag
				void setInternal(bool internal) { internal_ = internal; }
				
				/// returns true if the SPNode is a recursive node (from recursive SMARTS)
				bool isRecursive() const { return recursive_; }

				/// sets the recursive flag
				void setRecursive(bool recursive); 
			
				/// set the component no of the component level grouping
				void setComponentNumber(int no) { component_no_ = no; }

				/// returns the component number
				Size getComponentNumber() const { return component_no_; }
			
				/// returns the associated SPAtom
				SPAtom* getSPAtom() const { return sp_atom_; }

				/// set the associated SPAtom
				void setSPAtom(SPAtom* sp_atom) { sp_atom_ = sp_atom; }
		
				/// returns the first edge (for tree use)
				SPEdge* getFirstEdge() const { return first_edge_; }

				/// sets the first edge (for tree use)
				void setFirstEdge(SPEdge* first) { first_edge_ = first; }

				/// returns the second edge (for tree use)
				SPEdge* getSecondEdge() const { return second_edge_; }

				/// sets the second edge (for tree use)
				void setSecondEdge(SPEdge* second) { second_edge_ = second; }
		
				/// returns the negation flag
				bool getNot() const { return is_not_; }

				/// sets the negation flag
				void setNot(bool is_not) { is_not_ = is_not; }
	

				/// flag whether the pattern is in brackets
				//void setInBrackets() { in_brackets_ = true; }

				/// adds a new SPEdge (not for tree use)
				void addSPEdge(SPEdge* sp_edge) { edges_.push_back(sp_edge); }

				/// sets the logical operator associated with the SPNode
				void setLogicalOperator(LogicalOperator log_op) { log_op_ = log_op; }

				/// returns the logical operator of the SPNode
				LogicalOperator getLogicalOperator() const { return log_op_; }	

				/// counts the number of edges
				Size countEdges() const { return edges_.size(); }
				//@}

				/** @name Iterators
				*/
				//@{
				/// mutable access to begin of edges list
				EdgeIterator begin() { return edges_.begin(); }

				/// mutable access to end of edges list
				EdgeIterator end() { return edges_.end(); }

				/// non-mutable access to begin of edges list
				EdgeConstIterator begin() const { return edges_.begin(); }

				/// non-mutable access to end of edges list
				EdgeConstIterator end() const { return edges_.end(); }
				//@}

			protected:
				
				/// internal flag
				bool internal_;

				/// negotiation flag
				bool is_not_;

				/// recursive flag
				bool recursive_;

				/// in brackets flag
				//bool in_brackets_;

				/// logical operator associated with this SPNode
				LogicalOperator log_op_;

				/// edges list
				std::vector<SPEdge*> edges_;

				/// first edge
				SPEdge* first_edge_;

				/// second edge
				SPEdge* second_edge_;

				/// SPAtom associated with this SPNode
				SPAtom* sp_atom_;

				/// component level
				int component_no_;
		};

	
		/**	@name Constructors and Destructors
		*/
		//@{
		/// Default constructor
		SmartsParser();
			
		/// Copy constructor
		SmartsParser(const SmartsParser& parser);

		/// Destructor
		virtual ~SmartsParser();
		//@}
		
		/**	@name	Parsing
		*/
		//@{
		/**	Parse a SMARTS string.
		*/
		void parse(const String& s)
			throw(Exception::ParseError);

		/**	@name Accessors
		*/
		//@{
		/// creates a new atom of symbol
		SPAtom* createAtom(const String& symbol, bool in_bracket = false);

		/// sets the root SPNode of the tree
		void setRoot(SPNode* root) { root_ = root; }

		/// returns the root SPNode of the tree
		SPNode* getRoot() const { return root_; }

		/// dumps the tree to cerr
		void dumpTree();
		
		/// clear the tree
		void clear();
		
		/// adds a ring connection, SPNode with an index used in the SMARTS pattern
		void addRingConnection(SPNode* spnode, Size index);
		
		/// returns the ring connections sorted by index from SMARTS pattern
		std::map<Size, std::vector<SPNode*> > getRingConnections() const;
	
		/// sets the SSSR
		void setSSSR(const std::vector<std::vector<Atom*> >& sssr);

		/// sets the sssr needed flag
		void setNeedsSSSR(bool needs_sssr) { needs_SSSR_ = needs_sssr; }

		/// returns true if the SMARTS pattern contains ring related parts
		bool getNeedsSSSR() const { return needs_SSSR_; }

		/// sets the recursive flag
		void setRecursive(bool recursive) { recursive_ = recursive; }

		/// returns true if the tree represents a recursive SMARTS pattern
		bool isRecursive() const { return recursive_; }

		/// sets the component level flag
		void setComponentGrouping(bool component_grouping) { component_grouping_ = component_grouping; }

		/// returns true if the component level grouping was enabled
		bool hasComponentGrouping() const { return component_grouping_; }

		/// Parser state (used by the parser itself)
		struct State
		{
			Size					char_count;
			SmartsParser*	current_parser;
			const char*		buffer;
		};
		
		/// static member for the parser itself 
		static State state;

		/// returns the eodes stored in the tree
		const std::set<SPNode*>& getNodes() const { return nodes_; }

		/// returns the edges stored in the tree
		const std::set<SPEdge*>& getEdges() const { return edges_; }

		/// adds an edge to the tree
		void addEdge(SPEdge* edge) { edges_.insert(edge); }

		/// adds a node to the tree
		void addNode(SPNode* node) { nodes_.insert(node); }

		/// returns true if the tree has the given recursive edge
		bool hasRecursiveEdge(SPEdge* edge) const { return rec_edges_.find(edge) != rec_edges_.end(); }

		/// adds a recursive edge to the tree
		void addRecursiveEdge(SPEdge* edge) { rec_edges_.insert(edge); }

		/// gets the next component no and assigns it to the subtree 
		void setNextComponentNumberToSubTree(SPNode* spnode);
		//@}

		protected:

			/// sssr needed flag
			bool needs_SSSR_;

			/// recursive flag
			bool recursive_;

			/// component level grouping flag
			bool component_grouping_;

			/// the sssr 
			static vector<std::set<const Atom*> >* sssr_;

			/// dump method for the tree
			void dumpTreeRecursive_(SPNode* node, Size depth);

			/// dump method for the tree
			void dumpTreeRecursive_(SPEdge* edge, Size depth);
			
			/// the ring connection sorted by index of the SMARTS pattern
			std::map<Size, std::vector<SPNode*>	> ring_connections_;
		
			/// current instance
			static SmartsParser* current_parser_;
			
			/// the edges 
			std::set<SPEdge*> edges_;
			
			/// the nodes
			std::set<SPNode*> nodes_;

			/// the recursive edges
			std::set<SPEdge*> rec_edges_;

			/// the root node of the tree
			SPNode* root_;

			/// the actual component number
			int component_no_;
	};
  
} // namespace BALL

#endif // BALL_STRUCTURE_SMARTS_PARSER_H

