// $Id: BSDTree.h,v 1.15 2001/12/30 13:28:42 sturm Exp $

#ifndef BALL_STRUCTURE_BSDTREE_H
#define BALL_STRUCTURE_BSDTREE_H

#ifndef BALL_MATHS_BOX3_H
#	include <BALL/MATHS/box3.h>
#endif

#ifndef BALL_MATHS_VECTOR3_H
#	include <BALL/MATHS/vector3.h>
#endif

#ifndef BALL_DATATYPE_HASHSET_H
#	include <BALL/DATATYPE/hashSet.h>
#endif

#ifndef BALL_COMMON_EXCEPTION_H
#	include <BALL/COMMON/exception.h>
#endif

#include <vector>
#include <list>

namespace BALL 
{

	/** Generic BSD Tree Class.	
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
				This method creates a new TBSDTree object. It contains no points,	
				the direction of division is set to {\tt x}, the bounding box is	
				empty and there exist no children.
		*/
		TBSDTree()
			throw();

		// ?????: should honor the deep flag!
		/**	Copy constructor.
				Create a new BSDTree object from another.
				@param bsd_tree the BSDTree object to be copied
		*/	
		TBSDTree(const TBSDTree<T>& bsd_tree, bool = true)
			throw();

		/**	Detailed constructor.
				Create a new BSDTree object from a vector of TVector3, a HashSet of	
				Index, a Direction, a TBox3 and two pointers to BSDTrees.
				@param	point					assigned to the points
				@param	part					assigned to the indices of this part of the tree
				@param	division			assigned to the direction of division
				@param	bounding_box	assigned to the bounding box
				@param	left					assigned to the left child
				@param	right					assigned to the right child
		*/
		TBSDTree(const ::std::vector<TVector3<T> >& point,
				const HashSet<Index>& part,
				const Direction& division,
				const TBox3<T>& bounding_box,
				TBSDTree<T>* left,
				TBSDTree<T>* right)
			throw();

		/**	Detailed constructor.
				Create a new BSDTree object from a vector of TVector3, a list of	
				Index, a Direction, a TBox3 and two pointers to BSDTrees.
				@param	point					assigned to the points
				@param	part					assigned to the indices of this part of the tree
				@param	division			assigned to the direction of division
				@param	bounding_box	assigned to the bounding box
				@param	left					assigned to the left child
				@param	right					assigned to the right child
		*/
		TBSDTree(const ::std::vector<TVector3<T> >& point,
				const ::std::list<Index>& part,
				const Direction& division,
				const TBox3<T>& bounding_box,
				TBSDTree<T>* left,
				TBSDTree<T>* right)
			throw();

		/**	Detailed constructor.
				Create a new BSDTree object from a vector of TVector3
				@param	p assigned to the points
		*/
		TBSDTree(const ::std::vector< TVector3<T> >& point)
			throw();

		/**	Detailed constructor.
				Create a new BSDTree object from a vector of TVector3, a HashSet of	
				Index
				@param	point	assigned to the points
				@param	part	assigned to the indices of this part of the tree
		*/
		TBSDTree(const ::std::vector< TVector3<T> >& point,
				const HashSet<Index>& part)
			throw();

		/**	Detailed constructor.
				Create a new BSDTree object from a vector of TVector3, a list of	
				Index
				@param	point	assigned to the points
				@param	part	assigned to the indices of this part of the tree
		*/
		TBSDTree(const ::std::vector< TVector3<T> >& point,
				const ::std::list<Index>& part)
			throw();

		/**	Destructor.	
				Destructs the BSDTree object with all children.
		*/
		virtual ~TBSDTree()
			throw();

		//@}
		/**	@name	Assignment	
		*/
		//@{

		/**	Assign from another BSDTree.
				@param	bsd_tree	the BSDTree object to assign from
		*/
		void set(const TBSDTree<T>& bsd_tree)
			throw();

		//@}

		/**	@name	Acessors
		*/
		//@{
		
		/** Get the coordinates of the bounding box of the points containd in
				the node.
				@param	x_min	the minimal x value
				@param	y_min	the minimal y value
				@param	z_min	the minimal z value
				@param	x_max	the maximal x value
				@param	y_max	the maximal y value
				@param	z_max	the maximal z value
		*/
		void getExtrema(T& x_min, T& y_min, T& z_min,
										T& x_max, T& y_max, T& z_max)
			throw(Exception::SizeUnderflow);
	
		/** Build the BSDTree.
				If the node contains more than 10 points it gets two children: The
				bounding box is split along the widest dimension. Each child
				contains the points of one of these two parts. This procedure is
				recursively translated.
		*/
		void build()
			throw();

		/** Get all points close enougth to p.
				The indices of all points of all leafs wich bounding box is
				intersecting the box with center p and width 2*length are returned.
				@param	p				the center of the tested box
				@param	length	the width of the tested box
				@param	indices	a HashSet containing the indices of all found points
		*/
		void get(const TVector3<T>& p, const T& length, HashSet<Index>& indices)
			throw();

		/** Get all points close enougth to p.
				The indices of all points of all leafs wich bounding box is
				intersecting the box with center p and width 2*length are returned.
				@param	p				the center of the tested box
				@param	length	the width of the tested box
				@param	indices	a list containing the indices of all found points
		*/
		void get(const TVector3<T>& p, const T& length, ::std::list<Index>& indices)
			throw();

		/** Remove a point from the BSDTree.
				@param	i	the index ofthe pointto be removed
		*/
		//void remove(Index i)
		//	throw(Exception::IndexOverflow);

		//@}

		protected:

		/*_ the points stored in the BSDTree
		*/
		::std::vector< TVector3<T> > point_;
		/*_ the indices of the points stored in this part of the BSDTree
		*/
		//HashSet<Index> part_;
		::std::list<Index> part_;
		/*_ the direction of division of this node of the BSDTree
		*/
		Direction division_;
		/*_ the bounding box of all points stored in this part of the BSDTree
		*/
		TBox3<T> bounding_box_;
		/*_ the left child of this node
		*/
		TBSDTree<T>* left_;
		/*_ the right child of this node
		*/		
		TBSDTree<T>* right_;

	};


	/**	The Default TBSDTree Type.
			If double precision is not needed, {\tt TBSDTree<float>} should
			be used. It is predefined as {\tt BSDTree} for convenience.
	*/
	typedef TBSDTree<float> BSDTree;


	template <typename T>
	TBSDTree<T>::TBSDTree()
		throw()
		: point_(),
			part_(),
			division_(TBSDTree<T>::DIRECTION_X),
			bounding_box_(),
			left_(NULL),
			right_(NULL)
	{
	}


	template <typename T>
	TBSDTree<T>::TBSDTree(const TBSDTree<T>& bsd_tree, bool)
		throw()
		: point_(bsd_tree.point_),
			part_(bsd_tree.part_),
			division_(bsd_tree.division_),
			bounding_box_(bsd_tree.bounding_box_),
			left_(NULL),
			right_(NULL)
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


	template <typename T>
	TBSDTree<T>::TBSDTree
		 (const ::std::vector<TVector3<T> >& point,
			const HashSet<Index>&							 part,
			const Direction&									 division,
			const TBox3<T>&										 bounding_box,
			TBSDTree<T>*											 left,
			TBSDTree<T>*											 right)
		throw()
		: point_(point),
			part_(),
			division_(division),
			bounding_box_(bounding_box),
			left_(left),
			right_(right)
	{
		HashSet<Index>::ConstIterator i;
		for (i = part.begin(); i != part.end(); i++)
		{
			part_.push_back(*i);
		}
	}


	template <typename T>
	TBSDTree<T>::TBSDTree
		 (const ::std::vector<TVector3<T> >& point,
			const ::std::list<Index>&					 part,
			const Direction&									 division,
			const TBox3<T>&										 bounding_box,
			TBSDTree<T>*											 left,
			TBSDTree<T>*											 right)
		throw()
		: point_(point),
			part_(part),
			division_(division),
			bounding_box_(bounding_box),
			left_(left),
			right_(right)
	{
	}


	template <typename T>
	TBSDTree<T>::TBSDTree(const ::std::vector< TVector3<T> >& point)
		throw()
		:	point_(point),
			part_(),
			bounding_box_(),
			left_(NULL),
			right_(NULL)
	{
		for (Position i = 0; i < point.size(); i++)
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


	template <typename T>
	TBSDTree<T>::TBSDTree(const ::std::vector< TVector3<T> >& point,
			const HashSet<Index>& part)
		throw()
		:	point_(point),
			part_(),
			bounding_box_(),
			left_(NULL),
			right_(NULL)
	{
		HashSet<Index>::ConstIterator i;
		for (i = part.begin(); i != part.end(); i++)
		{
			part_.push_back(*i);
		}
		if (part_.size() > 0)
		{
			T x_min, y_min, z_min, x_max, y_max, z_max;
			getExtrema(x_min,y_min,z_min,x_max,y_max,z_max);
			bounding_box_ = TBox3<T>(TVector3<T>(x_min,y_min,z_min),
															 TVector3<T>(x_max,y_max,z_max)	);
		}
	}


	template <typename T>
	TBSDTree<T>::TBSDTree(const ::std::vector< TVector3<T> >& point,
			const ::std::list<Index>& part)
		throw()
		:	point_(point),
			part_(part),
			bounding_box_(),
			left_(NULL),
			right_(NULL)
	{
		if (part_.size() > 0)
		{
			T x_min, y_min, z_min, x_max, y_max, z_max;
			getExtrema(x_min,y_min,z_min,x_max,y_max,z_max);
			bounding_box_ = TBox3<T>(TVector3<T>(x_min,y_min,z_min),
															 TVector3<T>(x_max,y_max,z_max)	);
		}
	}


	template <typename T>
	TBSDTree<T>::~TBSDTree()
		throw()
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


	template <typename T>
	void TBSDTree<T>::set(const TBSDTree<T>& bsd_tree)
		throw()
	{
		point_ = bsd_tree.point_;
		part_ = bsd_tree.part_;
		division_ = bsd_tree.division_;
		bounding_box_ = bsd_tree.bounding_box_;
		left_ = bsd_tree.left_;
		right_ = bsd_tree.right_;
	}


	template <typename T>
	void TBSDTree<T>::getExtrema(T& x_min, T& y_min, T& z_min,
															 T& x_max, T& y_max, T& z_max)
		throw(Exception::SizeUnderflow)
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
		::std::list<Index>::iterator i;
		for (i = part_.begin(); i != part_.end(); i++)
		{
			x_min = Maths::min(point_[*i].x,x_min);
			y_min = Maths::min(point_[*i].y,y_min);
			z_min = Maths::min(point_[*i].z,z_min);
			x_max = Maths::max(point_[*i].x,x_max);
			y_max = Maths::max(point_[*i].y,y_max);
			z_max = Maths::max(point_[*i].z,z_max);
		}
	}


	template <typename T>
	void TBSDTree<T>::build()
		throw()
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
				::std::list<Index> left_part;
				::std::list<Index> right_part;
				::std::list<Index>::iterator i;
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


	template <typename T>
	void TBSDTree<T>::get(const TVector3<T>& p, const T& length,
												HashSet<Index>& indices)
		throw()
	{
		bool is_intersecting = true;
		for (Position i = 0; i < 3; i++)
		{
			if ((p[i]+length < bounding_box_.a[i]) ||
					(p[i]-length > bounding_box_.b[i])		)
			{
				is_intersecting = false;
			}
		}
		if (is_intersecting)
		{
			if (left_ == NULL)
			{
				::std::list<Index>::const_iterator i;
				for (i = part_.begin(); i != part_.end(); i++)
				{
					indices.insert(*i);
				}
			}
			else
			{
				left_->get(p,length,indices);
				right_->get(p,length,indices);
			}
		}
	}


	template <typename T>
	void TBSDTree<T>::get(const TVector3<T>& p, const T& length,
												::std::list<Index>& indices)
		throw()
	{
		bool is_intersecting = true;
   	for (Position i = 0; i < 3; i++)
 	  {
      if ((p[i]+length < bounding_box_.a[i]) ||
      		(p[i]-length > bounding_box_.b[i])		)
     	{
   	    is_intersecting = false;
 	    }
    }
		if (is_intersecting)
		{
			if (left_ == NULL)
			{
				indices.insert(indices.end(),part_.begin(),part_.end());
			}
			else
			{
				left_->get(p,length,indices);
				right_->get(p,length,indices);
			}
		}
	}


	/*template <typename T>
	void TBSDTree<T>::remove(Index i)
		throw(Exception::IndexOverflow)
	{
		if (i >= point_.size())
		{
			throw Exception::SizeOverflow(__FILE__, __LINE__);
		}
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
	}*/




} // namespace BALL

#endif // BALL_STRUCTURE_BSDTREE_H
