// $Id: BSDTree.h,v 1.9 2001/03/06 00:43:42 amoll Exp $

#define DEBUG_BSDTREE

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

	/** Generic BSD Tree Class.	\\
			{\bf Definition:}\URL{BALL/STRUCTURE/BSDTree.h}
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
			: point_(), part_(), division_(TBSDTree<T>::DIRECTION_X),
				bounding_box_(), left_(NULL), right_(NULL)
		{
		}

		// BAUSTELLE: should honor the deep flag!
		/**	Copy constructor.
				Create a new BSDTree object from another.
				@param bsd_tree the BSDTree object to be copied
		*/	
		TBSDTree(const TBSDTree& bsd_tree, bool /* deep */ = true)
			: point_(bsd_tree.point_), part_(bsd_tree.part_),
				division_(bsd_tree.division_),
				bounding_box_(bsd_tree.bounding_box_),
				left_(NULL), right_(NULL)
		{
			if (bsd_tree.left_ != NULL)
			{
				left_ = new TBSDTree<T>(*bsd_tree.left_);
			}
			if (bsd_tree.right_ != NULL)
			{
				right_ = new TBSDTree<T>(*bsd_tree.right_);
			}
		}

		/**	Detailed constructor.
				Create a new BSDTree object from a vector of TVector3, a list of
				Index, a Direction and two pointers to BSDTrees.
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
			: point_(p), part_(prt), division_(d), bounding_box_(bb),
				left_(l), right_(r)
		{
		}

		/**	Detailed constructor.
				Create a new BSDTree object from a vector of TVector3
				@param	p assigned to the points
		*/
		TBSDTree(const vector< TVector3<T> >& p)
			:	point_(p), part_(), bounding_box_(), left_(NULL), right_(NULL)
		{
			for (Position i = 0; i < p.size(); i++)
			{
				part_.push_back(i);
			}
			if (part_.size() > 0)
			{
				T x_min, y_min, z_min, x_max, y_max, z_max;
				getExtrema(x_min,y_min,z_min,x_max,y_max,z_max);
				bounding_box_ = TBox3<T>(TVector3<T>(x_min,y_min,z_min),
																 TVector3<T>(x_max,y_max,z_max)	);
			}
		}

		/**	Detailed constructor.
				Create a new BSDTree object from a vector of TVector3, a list of
				Index
				@param	p assigned to the points
				@param	prt assigned to the indices of this part of the tree
		*/
		TBSDTree(const vector< TVector3<T> >& p, const list<Index>& prt)
			:	point_(p), part_(prt), bounding_box_(), left_(NULL), right_(NULL)
		{
			if (part_.size() > 0)
			{
				T x_min, y_min, z_min, x_max, y_max, z_max;
				getExtrema(x_min,y_min,z_min,x_max,y_max,z_max);
				bounding_box_ = TBox3<T>(TVector3<T>(x_min,y_min,z_min),
																 TVector3<T>(x_max,y_max,z_max)	);
			}
		}

		/**	Destructor.	
				Destructs the BSDTree object.
		*/
		virtual ~TBSDTree()
		{
			if (left_ != NULL)
			{
				delete left_;
			}
			if (right_ != NULL)
			{
				delete right_;
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
			point_ = bsd_tree.point_; part_ = bsd_tree.part_;
			division_ = bsd_tree.division_;
			bounding_box_ = bsd_tree.bounding_box_;
			left_ = bsd_tree.left_; right_ = bsd_tree.right_;
		}

		//@}
		/**	@name	Acessors
		*/
		//@{
		void getExtrema(T& x_min, T& y_min, T& z_min,
										T& x_max, T& y_max, T& z_max)
		{
			if (part_.size() == 0)
			{
				throw Exception::SizeUnderflow(__FILE__, __LINE__);
			}
			x_min = point_[*part_.begin()].x;
			y_min = point_[*part_.begin()].y;
			z_min = point_[*part_.begin()].z;
			x_max = point_[*part_.begin()].x;
			y_max = point_[*part_.begin()].y;
			z_max = point_[*part_.begin()].z;
			for (list<Index>::iterator i = part_.begin(); i != part_.end(); i++)
			{
				x_min = Maths::min(point_[*i].x,x_min);
				y_min = Maths::min(point_[*i].y,y_min);
				z_min = Maths::min(point_[*i].z,z_min);
				x_max = Maths::max(point_[*i].x,x_max);
				y_max = Maths::max(point_[*i].y,y_max);
				z_max = Maths::max(point_[*i].z,z_max);
			}
		}

		void build()
		{
			if ((left_ == NULL) && (right_ == NULL))
			{
				if (part_.size() > 10)
				{
					T x_min, y_min, z_min, x_max, y_max, z_max;
					getExtrema(x_min,y_min,z_min,x_max,y_max,z_max);
					T middle;
					if (Maths::isLess(x_max-x_min,y_max-y_min))
					{
						if (Maths::isLess(y_max-y_min,z_max-z_min))
						{
							division_ = TBSDTree<T>::DIRECTION_Z;
							middle = (z_max+z_min)/2;
						}
						else
						{
							division_ = TBSDTree<T>::DIRECTION_Y;
							middle = (y_max+y_min)/2;
						}
					}
					else
					{
						if (Maths::isLess(x_max-x_min,z_max-z_min))
						{
							division_ = TBSDTree<T>::DIRECTION_Z;
							middle = (z_max+z_min)/2;
						}
						else
						{
							division_ = TBSDTree<T>::DIRECTION_X;
							middle = (x_max+x_min)/2;
						}
					}
					Index d;
					switch (division_)
					{
						case TBSDTree<T>::DIRECTION_X :	d = 0; break;
						case TBSDTree<T>::DIRECTION_Y :	d = 1; break;
						case TBSDTree<T>::DIRECTION_Z :	d = 2; break;
					}
					list<Index> left_part;
					list<Index> right_part;
					list<Index>::iterator i;
					for (i = part_.begin(); i != part_.end(); i++)
					{
						if (Maths::isLess(point_[*i][d],middle))
						{
							left_part.push_back(*i);
						}
						else
						{
							right_part.push_back(*i);
						}
					}
					left_ = new TBSDTree<T>(point_,left_part);
					right_ = new TBSDTree<T>(point_,right_part);
					left_->build();
					right_->build();
				}
				else
				{
					left_ = NULL;
					right_ = NULL;
				}
			}
		}

		list<Index> get(const TVector3<T>& p, const T& length)
		{
			TBox3<T> test_box(TVector3<T>(p.x-length,p.y-length,p.z-length),
												TVector3<T>(p.x+length,p.y+length,p.z+length));
			if (bounding_box_.isIntersecting(test_box) == false)
			{
				list<Index> empty;
				empty.clear();
				return empty;
			}
			if (left_ == NULL)
			{
				return part_;
			}
			list<Index> temp1 = left_->get(p,length);
			list<Index> temp2 = right_->get(p,length);
			temp1.merge(temp2);
			return temp1;
		}


		void remove(Index i)
		{
			part_.remove(i);
			if (left_ != NULL)
			{
				if (left_->bounding_box_.has(point_[i]))
				{
					left_->remove(i);
				}
			}
			if (right_ != NULL)
			{
				if (right_->bounding_box_.has(point_[i]))
				{
					right_->remove(i);
				}
			}
		}

		//@}

		protected:

		vector< TVector3<T> > point_;
		list<Index> part_;
		Direction division_;
		TBox3<T> bounding_box_;
		TBSDTree<T>* left_;
		TBSDTree<T>* right_;

	};


	/**	The Default TBSDTree Type.
			If double precision is not needed, {\tt TBSDTree<float>} should
			be used. It is predefined as {\tt BSDTree} for convenience.
	*/
	typedef TBSDTree<float> BSDTree;

} // namespace BALL

#endif // BALL_STRUCTURE_BSDTREE_H
