// $Id: BSDTree.h,v 1.4 2000/10/30 00:19:26 amoll Exp $

#ifndef BALL_STRUCTURE_BSDTREE_H
#define BALL_STRUCTURE_BSDTREE_H

#ifndef BALL_MATHS_BOX3_H
#	include <BALL/MATHS/box3.h>
#endif
#ifndef BALL_MATHS_VECTOR3_H
#	include <BALL/MATHS/vector3.h>
#endif

#include <vector>
#include <list>
#include <string>

namespace BALL 
{

	/** Generic BSD Tree Class.

			\\
			{\bf Definition:}\URL{BALL/STRUCTURE/BSDTree.h}
			\\
	*/
	template <typename T>
	class TBSDTree
	{
		public:

		BALL_CREATE(TBSDTree)
		
		/**	@name	Enums
		*/
		//@{

		/** direction of division:
				{\tt 0} x
				{\tt 1} y
				{\tt 2} z
		*/
		enum Direction
		{
			DIRECTION_X  = 0,
			DIRECTION_Y  = 1,
			DIRECTION_Z  = 2
		};
		//@}

		/**	@name	Constructors and Destructors
		*/
		//@{

		/**	Default constructor.
				This method creates a new TBSDTree object. The three components
				are initialized to {\tt (T)0}.
		*/
		TBSDTree()
			: point(), part(), division(TBSDTree<T>::DIRECTION_X), bounding_box(), left(NULL), right(NULL)
		{
		}


		// BAUSTELLE: should honor the deep flag!
		/**	Copy constructor.
				Create a new BSDTree object from another.
				@param bsd_tree the BSDTree object to be copied
		*/	
		TBSDTree(const TBSDTree& bsd_tree, bool /* deep */ = true)
			: point(bsd_tree.point), part(bsd_tree.part), division(bsd_tree.division),
				bounding_box(bsd_tree.bounding_box), left(bsd_tree.left), right(bsd_tree.right)
		{
		}

		/**	Detailed constructor.
				Create a new BSDTree object from a vector of TVector3, a list of Index, a Direction and two
				pointers to BSDTrees.
				@param	p assigned to the points
				@param	prt assigned to the indices of this part of the tree
				@param	d assigned to the direction of division
				@param	bb assigned to the bounding box
				@param	l assigned to the left child
				@param	r assigned to the right child
		*/
		TBSDTree(const vector<TVector3<T> >& p, const list<Index>& prt, 
						 const Direction& d,
						 const TBox3<T>& bb, TBSDTree<T>* l, TBSDTree<T>* r)
			: point(p), part(prt), division(d), bounding_box(bb), left(l), right(r)
		{
		}

		/**	Detailed constructor.
				Create a new BSDTree object from a vector of TVector3, a list of Index, a Direction and two
				pointers to BSDTrees.
				@param	p assigned to the points
				@param	prt assigned to the indices of this part of the tree
		*/
		TBSDTree(const vector< TVector3<T> >& p, const list<Index>& prt, const string& pr, const string& s)
		{
			point = p;
			part = prt;
			if (part.size() > 0)
			{
				bounding_box = TBox3<T>(TVector3<T>(getMin(TBSDTree<T>::DIRECTION_X),
																					 	getMin(TBSDTree<T>::DIRECTION_Y),
																					 	getMin(TBSDTree<T>::DIRECTION_Z)	),
															 	TVector3<T>(getMax(TBSDTree<T>::DIRECTION_X),
															 						 	getMax(TBSDTree<T>::DIRECTION_Y),
										 											 	getMax(TBSDTree<T>::DIRECTION_Z)	)	);
			}
			left = NULL;
			right = NULL;
			pre = pr;
			side = s;
		}

		/**	Destructor.	
				Destructs the BSDTree object.
		*/
		virtual ~TBSDTree()
		{
			if (left != NULL)
			{
				delete left;
			}
			if (right != NULL)
			{
				delete right;
			}
		}
		//@}

		/**	@name	Assignment	
		*/
		//@{

		/**	Assign from another BSDTree.
				@param bsd_tree	the BSDTree object to assign from
		*/
		void set(const TBSDTree& bsd_tree)
		{
			point = bsd_tree.point; part = bsd_tree.part; division = bsd_tree.division;
			bounding_box = bsd_tree.bounding_box; left = bsd_tree.left; right = bsd_tree.right;
		}

		//@}

		/**	@name	Acessors
		*/
		//@{
		T getMin(Direction direction)
		{
			Index d;
			switch (direction)
			{
				case TBSDTree<T>::DIRECTION_X :	d = 0; break;
				case TBSDTree<T>::DIRECTION_Y :	d = 1; break;
				case TBSDTree<T>::DIRECTION_Z :	d = 2; break;
				default												:	d = 0;
			}
			if (part.size() == 0)
			{
				throw Exception::SizeUnderflow(__FILE__, __LINE__);
			}
			else
			{
				T min = point[*part.begin()][d];
				for (list<Index>::iterator i = part.begin(); i != part.end(); i++)
				{
					min = Maths::min(point[*i][d],min);
				}
				return min;
			}
		}

		T getMax(Direction direction)
		{
			Index d;
			switch (direction)
			{
				case TBSDTree<T>::DIRECTION_X :	d = 0; break;
				case TBSDTree<T>::DIRECTION_Y :	d = 1; break;
				case TBSDTree<T>::DIRECTION_Z :	d = 2; break;
				default												:	d = 0;
			}
			if (part.size() == 0)
			{
				throw Exception::SizeUnderflow(__FILE__, __LINE__);
			}
			else
			{
				T max = point[*part.begin()][d];
				for (list<Index>::iterator i = part.begin(); i != part.end(); i++)
				{
					max = Maths::max(point[*i][d],max);
				}
				return max;
			}
		}

		void build()
		{
			if ((left == NULL) && (right == NULL))
				{
					if (part.size() > 10)
						{
							T x_min = getMin(TBSDTree<T>::DIRECTION_X);
							T y_min = getMin(TBSDTree<T>::DIRECTION_Y);
							T z_min = getMin(TBSDTree<T>::DIRECTION_Z);
							T x_max = getMax(TBSDTree<T>::DIRECTION_X);
							T y_max = getMax(TBSDTree<T>::DIRECTION_Y);
							T z_max = getMax(TBSDTree<T>::DIRECTION_Z);
							T middle;
							if (Maths::isLess(x_max-x_min,y_max-y_min))
							{
								if (Maths::isLess(y_max-y_min,z_max-z_min))
								{
									division = TBSDTree<T>::DIRECTION_Z;
									middle = (z_max+z_min)/2;
								}
								else
								{
									division = TBSDTree<T>::DIRECTION_Y;
									middle = (y_max+y_min)/2;
								}
							}
							else
							{
								if (Maths::isLess(x_max-x_min,z_max-z_min))
								{
									division = TBSDTree<T>::DIRECTION_Z;
									middle = (z_max+z_min)/2;
								}
								else
								{
									division = TBSDTree<T>::DIRECTION_X;
									middle = (x_max+x_min)/2;
								}
							}
							Index d;
							switch (division)
							{
								case TBSDTree<T>::DIRECTION_X :	d = 0; break;
								case TBSDTree<T>::DIRECTION_Y :	d = 1; break;
								case TBSDTree<T>::DIRECTION_Z :	d = 2; break;
							}
							list<Index> left_part;
							list<Index> right_part;
							for (list<Index>::iterator i = part.begin(); i != part.end(); i++)
							{
								if (Maths::isLess(point[*i][d],middle))
								{
									left_part.push_back(*i);
								}
								else
								{
									right_part.push_back(*i);
								}
							}
							left = new TBSDTree<T>(point,left_part,pre+"  ","l");
							right = new TBSDTree<T>(point,right_part,pre+"  ","r");
							left->build();
							right->build();
						}
						else
						{
							left = NULL;
							right = NULL;
						}
				}
		}

		list<Index> get(const TVector3<T>& p, const T& length)
		{
			TBox3<T> test_box(TVector3<T>(p.x-length,p.y-length,p.z-length),
												TVector3<T>(p.x+length,p.y+length,p.z+length));
			if (left != NULL)
			{
				if (bounding_box.isIntersecting(test_box))
				{
					list<Index> temp1 = left->get(p,length);
					list<Index> temp2 = right->get(p,length);
					temp1.merge(temp2);
					return temp1;
				}
				else
				{
					list<Index> empty;
					empty.erase(empty.begin(),empty.end());
					return empty;
				}
			}
			else
			{
				if (bounding_box.isIntersecting(test_box))
				{
					return part;
				}
				else
				{
					list<Index> empty;
					empty.erase(empty.begin(),empty.end());
					return empty;
				}
			}
		}

		void remove(Index i)
		{
			part.remove(i);
			if (left != NULL)
			{
				if (left->bounding_box.has(point[i]))
				{
					left->remove(i);
				}
			}
			if (right != NULL)
			{
				if (right->bounding_box.has(point[i]))
				{
					right->remove(i);
				}
			}
		}
		//@}


		/**	@name	Attributes
		*/
		//@{

		/**	The points stored in the tree.
		*/
		vector<TVector3<T> > point;

		/** The points stored in this part of the tree.
		*/
		list<Index> part;
		
		/** The direction of division.
		*/
		Direction division;
		
		/** The bounding box of the points.
		*/
		TBox3<T> bounding_box;
		
		/** left child.
		*/
		TBSDTree<T>* left;
		
		/** right child.
		*/
		TBSDTree<T>* right;
		
		String pre;
		String side;
		//@}
	};

	/**	@name	Storers
	*/
	//@{

	/**	Output Operator.
			Write the contentsp of {\tt p} and {\tt radius} to an output stream.
			The values are enclose in brackets.\\
			{\bf Example:}\\
			{\tt ((0 1 1.5) 0.4)}
	*/
	template <typename T>
	std::ostream& operator << (std::ostream& s, const TBSDTree<T>& bsd_tree)
	{
		s << bsd_tree.pre << bsd_tree.side << ": ";
		switch (bsd_tree.division)
		{
			case TBSDTree<T>::DIRECTION_X :	s << "x "; break;
			case TBSDTree<T>::DIRECTION_Y :	s << "y "; break;
			case TBSDTree<T>::DIRECTION_Z :	s << "z "; break;
		}
		s << bsd_tree.bounding_box << "[";
		list<Index> liste = bsd_tree.part;
		for (list<Index>::iterator i = liste.begin(); i != liste.end(); i++)
		{
			s << *i << " ";
		}
		s << "]\n";
		if (bsd_tree.left != NULL)
		{
			s << *(bsd_tree.left);
		}
		else
		{
			s << bsd_tree.pre << "  l: --\n";
		}
		if (bsd_tree.right != NULL)
		{
			s << *(bsd_tree.right);
		}
		else
		{
			s << bsd_tree.pre << "  r: --\n";
		}
		return s;
	}
	//@}


	/**	The Default TBSDTree Type.
			If double precision is not needed, {\tt TBSDTree<float>} should
			be used. It is predefined as {\tt BSDTree} for convenience.
	*/
	typedef TBSDTree<float> BSDTree;

} // namespace BALL

#endif // BALL_STRUCTURE_BSDTREE_H
