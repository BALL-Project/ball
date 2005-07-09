// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: smartsMatcher.h,v 1.1 2005/07/09 18:35:04 bertsch Exp $
//

#ifndef BALL_STRUCTURE_SMARTSMATCHER_H
#define BALL_STRUCTURE_SMARTSMATCHER_H

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

#ifndef BALL_DATATYPE_HASHSET_H
# include <BALL/DATATYPE/hashSet.h>
#endif

#ifndef BALL_STRUCTURE_SMARTSPARSER_H
# include <BALL/STRUCTURE/smartsParser.h>
#endif

#include <vector>

namespace BALL 
{

	/** @name	\brief SMARTS Parser.
	*/
	class SmartsMatcher
	{
		public: 
		typedef SmartsParser::SPNode SPNode;
		typedef SmartsParser::SPEdge SPEdge;
		typedef SmartsParser::SPAtom SPAtom;
		typedef SmartsParser::SPBond SPBond;
		
		/**	@name Constructors and Destructors
		*/
		//@{

		///
		SmartsMatcher();
			
		///
		SmartsMatcher(const SmartsMatcher& matcher);

		///
		std::vector<HashSet<const Atom*> > match(const Molecule& mol, const String& smarts)
			throw(Exception::ParseError);

		///
		virtual ~SmartsMatcher();
		//@}
		
		protected:
		
		/// method for the evaluation of a pseudo-tree
		void evaluate_(vector<HashSet<const Atom*> >& matched_atoms, SPNode* start_node, const Atom* start_atom);

		/// method for evaluating a node of a pseudo-tree
		bool evaluate_node_(vector<HashSet<const Atom*> >& matched_atoms, SPNode* start_node, const Atom* start_atom,
				vector<HashSet<const Atom*> >& visited_atoms, vector<HashSet<SPNode*> >& visited_nodes);

		/// method for evaluating a edge of a pseudo-tree 
		bool evaluate_edge_(vector<HashSet<const Atom*> >& matched_atoms, SPEdge* start_node, const Atom* start_atom, const Bond* start_bond,
         vector<HashSet<const Atom*> >& visited_atoms, vector<HashSet<SPNode*> >& visited_nodes);
		
	};
  
} // namespace BALL

#endif // BALL_STRUCTURE_SMARTSMATCHER_H
