// $Id: SESFace.h,v 1.14 2001/12/08 17:03:04 strobel Exp $

#ifndef BALL_STRUCTURE_SESFACE_H
#define BALL_STRUCTURE_SESFACE_H

#ifndef BALL_MATHS_VECTOR3_H
#	include <BALL/MATHS/vector3.h>
#endif

#ifndef BALL_STRUCTURE_GRAPHEDGE_H
#	include <BALL/STRUCTURE/graphEdge.h>
#endif

#ifndef BALL_STRUCTURE_GRAPHFACE_H
#	include <BALL/STRUCTURE/graphFace.h>
#endif

#ifndef BALL_STRUCTURE_GRAPHVERTEX_H
#	include <BALL/STRUCTURE/graphVertex.h>
#endif

namespace BALL
{

	template <typename T>
	class TSolventExcludedSurface;

	template <typename T>
	class TSESEdge;

	template <typename T>
	class TSESVertex;

	template <typename T>
	class TRSEdge;

	template <typename T>
	class TRSVertex;

	template <typename T>
	class TRSFace;

	template <typename T>
	class TTriangulatedSES;

	/** Generic SESFace Class. 
			{\bf Definition:} \URL{BALL/STRUCTURE/SESFace.h} 
	*/
	template <typename T>
	class TSESFace	:	public GraphFace< TSESVertex<T>,TSESEdge<T> >
	{
		public:

		/** @name Class friends
				\begin{itemize}
					\item class GraphEdge< TRSVertex<T>,TRSFace<T> >
					\item class GraphFace< TRSVertex<T>,TRSEdge<T> >
					\item class GraphVertex< TRSEdge<T>,TRSFace<T> >
					\item class TSolventExcludedSurface<T>
					\item class TSESEdge<T>
					\item class TSESVertex<T>
					\item class TTriangulatedSES<T>
				\end{itemize}
		*/
		friend class GraphEdge< TSESVertex<T>,TSESFace<T> >;
		friend class GraphFace< TSESVertex<T>,TSESEdge<T> >;
		friend class GraphVertex< TSESEdge<T>,TSESFace<T> >;
		friend class TSolventExcludedSurface<T>;
		friend class TSESEdge<T>;
		friend class TSESVertex<T>;
		friend class TTriangulatedSES<T>;

		BALL_CREATE(TSESFace)

		/**	@name	Enums
		*/
		//@{

		/** type of SESFace:
				{\tt 0} spheric reentrant face
				{\tt 1} toric reentrant face
				{\tt 2} contact face
		*/
		enum Type
		{
			TYPE_SPHERIC  = 0,
			TYPE_TORIC = 1,
			TYPE_CONTACT = 2,
			TYPE_TORIC_SINGULAR = 3
		};

		//@}
		/**	@name	Constructors and Destructors
		*/
		//@{

		/**	Default constructor.
				This method creates a new SESFace object.
		*/
		TSESFace()
			throw();

		/**	Copy constructor.
				Create a new SESFace object from another.
				@param	sesface the SESFace object to be copied
				@param	deep		if deep = false, all pointers are set to NULL (default). Otherwise the new
												SESFace object is linked to the neighbours of the old SESFace object.
		*/
		TSESFace(const TSESFace<T>& sesface, bool deep = false)
			throw();

		/**	Detailled constructor.
				Create a new SESFace object
				@param	type				assigned the type
				@param	rsvertex		assigned to the rsvertex
				@param	rsedge			assigned to the rsedge
				@param	rsface			assigned to the rsface
				@param	vertex			assigned to the list of vertices
				@param	edge				assigned to the list of edges
				@param	orientation	assigned to the list of orientations
				@param	index				assigned to the index
		*/
		TSESFace(Type type,
				TRSVertex<T>* rsvertex,
				TRSEdge<T>* rsedge,
				TRSFace<T>* rsface,
				const ::std::vector<TSESVertex<T>*>& vertex,
				const ::std::vector<TSESEdge<T>*> edge,
				const ::std::vector<bool> orientation,
				Index index)
			throw();

		/**	Detailled constructor.
				Create a new SESFace object
				@param	type			assigned the type
				@param	rsvertex	assigned to the rsvertex
				@param	rsedge		assigned to the rsedge
				@param	rsface		assigned to the rsface
		*/
		TSESFace(Type type,
				TRSVertex<T>* rsvertex,
				TRSEdge<T>* rsedge,
				TRSFace<T>* rsface)
			throw();

		/**	Destructor.
				Destructs the SESFace object.
		*/
		virtual ~TSESFace()
			throw();

		//@}
		/**	@name	Assignments
		*/
		//@{

		/** Set the type of the SESFace.
				@param	type - the new type
		*/
		void setType(Type type)
			throw();

		/** Get the type of the SESFace
				@return	TSESFace<T>::Type	the type of the SESFace
		*/
		Type getType() const
			throw();

		/** Set the corresponding RSVertex of the SESFace.
				@param	TRSVertex<T>*	a pointer to the new RSVertex
		*/
		void setRSVertex(TRSVertex<T>* rsvertex)
			throw();

		/** Get the corresponding RSVertex of the SESFace.
				@return	TRSVertex<T>*	a pointer to the corresponding RSVertex
		*/
		TRSVertex<T>* getRSVertex() const
			throw();

		/** Set the corresponding RSEdge of the SESFace.
				@param	TRSEdge<T>*	a pointer to the new RSFace
		*/
		void setRSEdge(TRSEdge<T>* rsedge)
			throw();

		/** Get the corresponding RSEdge of the SESFace.
				@return	TRSEdge<T>*	a pointer to the corresponding RSEdge
		*/
		TRSEdge<T>* getRSEdge() const
			throw();

		/** Set the corresponding RSFace of the SESFace.
				@param	TRSFace<T>*	a pointer to the new RSFace
		*/
		void setRSFace(TRSFace<T>* rsface)
			throw();

		/** Get the corresponding RSFace of the SESFace.
				@return	TRSFace<T>*	a pointer to the corresponding RSFace
		*/
		TRSFace<T>* getRSFace() const
			throw();

		/** Set one of the orientations of the SESFace.
				@param	i						the relative index of the orientation which should be set. If i is greater	
														than the current number of orientations, an exception is thrown
				@param	orientation	a pointer to the new vertex
		*/
		void setOrientation(Position i, bool orientation)
			throw(Exception::IndexOverflow);

		/** Return one of the orientations of the SESFace.
				@param	i			the relative index of the orientation which should be given back. If i is greater	
											than the current number of orientation, an exception is thrown
				@return	bool	the asked orientation
		*/
		bool getOrientation(Position i) const
			throw(Exception::IndexOverflow);

		/** Push an new value to the vector of orientations of the SESFace.
				@param	bool	the new orientation value
		*/
		void pushOrientation(bool orientation)
			throw();
		

		//@}
		/**	@name	Accessors
		*/
		//@{

		/** Normalize the SESFace.
		*/
		void normalize(bool singular)
			throw();

		//@}
		/**	@name	Predicates
		*/
		//@{

		/** Test wether two faces are neighboured.
				Two faces are neighboured if they have a common edge.
				@param	face	the face to be tested
				@return	bool	{\bf true} if the faces are neighboured,	
											{\bf false} otherwise
		*/
		bool isNeighbouredTo(TSESFace<T>* face) const
			throw();

		/**	Test wether a toric face is free ore not.
				@return	bool	{\bf true} if the face is toric and free,	
											{\bf false} otherwise
		*/
		bool isFree() const
			throw();

		//@}

		protected:

		/**	@name	Attributes
		*/
		//@{

		/*_	The type of the face.
		*/
		Type type_;
		/*_	A pointer to the corresponding RSVertex (if type_ is TYPE_CONTACT).
		*/
		TRSVertex<T>* rsvertex_;
		/*_	A pointer to the corresponding RSEdge (if type_ is TYPE_TORIC or TYPE_TORIC_SINGULAR).
		*/
		TRSEdge<T>* rsedge_;
		/*_	A pointer to the corresponding RSFace (if type_ is TYPE_SPHERIC).
		*/
		TRSFace<T>* rsface_;
		/*_	A vector of booleans to indicate the orientation of each sesedge.
		*/
		::std::vector<bool> orientation_;

		//@}
	};

	/**	@name	Storers
	*/
	//@{

	/**	Output- Operator
	*/
	template <typename T>
	std::ostream& operator << (std::ostream& s, const TSESFace<T>& sesface)
	{
		s << "SESFACE" << sesface.getIndex() << "("
			<< (sesface.getType() == TSESFace<T>::TYPE_CONTACT ? "contact " :
						 (sesface.getType() == TSESFace<T>::TYPE_SPHERIC ? "spheric " : "toric "));
		if (sesface.getRSVertex() == NULL)
		{
			s << "(nil) ";
		}
		else
		{
			s << "(" << sesface.getRSVertex()->getIndex() << ") ";
		}
		if (sesface.getRSEdge() == NULL)
		{
			s << "(nil) ";
		}
		else
		{
			s << "(" << sesface.getRSEdge()->getIndex() << ") ";
		}
		if (sesface.getRSFace() == NULL)
		{
			s << "(nil) [";
		}
		else
		{
			s << "(" << sesface.getRSFace()->getIndex() << ") [";
		}
		for (Position i = 0; i < sesface.numberOfVertices(); i++)
		{
			s << sesface.getVertex(i)->getIndex() << ' ';
		}
		s << "] [";
		for (Position i = 0; i < sesface.numberOfEdges(); i++)
		{
			if (sesface.getEdge(i) == NULL)
			{
				s << "(nil) ";
			}
			else
			{
				s << sesface.getEdge(i)->getIndex() << ' ';
			}
		}
		s << "] [";
		//for (Position i = 0; i < sesface.orientation_.size(); i++)
		//{
		//	s << sesface.orientation_[i] << ' ';
		//}
		s << "])";
		return s;
	}

	//@}

	/**	The Default SESFace Type.
			If double precision is not needed, {\tt SESFace<float>} should
			be used. It is predefined as {\tt SESFace} for convenience.
	*/
	typedef TSESFace<float> SESFace;


	template <class T>
	TSESFace<T>::TSESFace()
		throw()
		: GraphFace< TSESVertex<T>,TSESEdge<T> >(),
			type_(TSESFace<T>::TYPE_SPHERIC),
			rsvertex_(NULL),
			rsedge_(NULL),
			rsface_(NULL),
			orientation_()
	{
	}


	template <class T>
	TSESFace<T>::TSESFace(const TSESFace<T>& sesface, bool deep)
		throw()
		: GraphFace< TSESVertex<T>,TSESEdge<T> >(sesface,deep),
			type_(sesface.type_),
			rsvertex_(NULL),
			rsedge_(NULL),
			rsface_(NULL),
			orientation_()
	{
		if (deep)
		{
			rsvertex_  = sesface.rsvertex_;
			rsedge_ =  sesface.rsedge_;
			rsface_ =  sesface.rsface_;
			orientation_ = sesface.orientation_;
		}
	}


	template <class T>
	TSESFace<T>::TSESFace(Type type,
			TRSVertex<T>* rsvertex,
			TRSEdge<T>* rsedge,
			TRSFace<T>* rsface,
			const ::std::vector<TSESVertex<T>*>& vertex,
			const ::std::vector<TSESEdge<T>*> edge,
			const ::std::vector<bool> orientation,
			Index index)
		throw()
		: GraphFace< TSESVertex<T>,TSESEdge<T> >(vertex,edge,index),
			type_(type),
			rsvertex_(rsvertex),
			rsedge_(rsedge),
			rsface_(rsface),
			orientation_(orientation)
	{
	}


	template <class T>
	TSESFace<T>::TSESFace(Type type,
			TRSVertex<T>* rsvertex,
			TRSEdge<T>* rsedge,
			TRSFace<T>* rsface)
		throw()
		: GraphFace< TSESVertex<T>,TSESEdge<T> >(),
			type_(type),
			rsvertex_(rsvertex),
			rsedge_(rsedge),
			rsface_(rsface),
			orientation_()
	{
	}


	template <class T>
	TSESFace<T>::~TSESFace()
		throw()
	{
	}


	template <class T>
	void TSESFace<T>::setType(TSESFace<T>::Type type)
		throw()
	{
		type_ = type;
	}


	template <class T>
	TSESFace<T>::Type TSESFace<T>::getType() const
		throw()
	{
		return type_;
	}


	template <class T>
	void TSESFace<T>::setRSVertex(TRSVertex<T>* rsvertex)
		throw()
	{
		rsvertex_ = rsvertex;
	}


	template <class T>
	TRSVertex<T>* TSESFace<T>::getRSVertex() const
		throw()
	{
		return rsvertex_;
	}


	template <class T>
	void TSESFace<T>::setRSEdge(TRSEdge<T>* rsedge)
		throw()
	{
		rsedge_ = rsedge;
	}


	template <class T>
	TRSEdge<T>* TSESFace<T>::getRSEdge() const
		throw()
	{
		return rsedge_;
	}


	template <class T>
	void TSESFace<T>::setRSFace(TRSFace<T>* rsface)
		throw()
	{
		rsface_ = rsface;
	}


	template <class T>
	TRSFace<T>* TSESFace<T>::getRSFace() const
		throw()
	{
		return rsface_;
	}


	template <class T>
	bool TSESFace<T>::getOrientation(Position i) const
		throw(Exception::IndexOverflow)
	{
		if (i < orientation_.size())
		{
			return orientation_[i];
		}
		else
		{
			throw Exception::IndexOverflow(__FILE__,__LINE__,
																		 i,orientation_.size()-1);
		}
	}

		
	template <class T>
	void TSESFace<T>::setOrientation(Position i, bool orientation)
		throw(Exception::IndexOverflow)
	{
		if (i < orientation_.size())
		{
			orientation_[i] = orientation;
		}
		else
		{
			throw Exception::IndexOverflow(__FILE__,__LINE__,
																		 i,orientation_.size()-1);
		}
	}

		
	template <class T>
	void TSESFace<T>::pushOrientation(bool orientation)
		throw()
	{
		orientation_.push_back(orientation);
	}


	template <class T>
	void TSESFace<T>::normalize(bool singular)
		throw()
	{
		if ((type_ != TSESFace<T>::TYPE_TORIC) &&
				(type_ != TSESFace<T>::TYPE_TORIC_SINGULAR))
		{
			return;
		}
		if (rsedge_->isFree())
		{
			return;
		}
		if (singular == false)
		{
			TSESEdge<T>* edge0;
			TSESEdge<T>* edge1;
			TSESEdge<T>* edge2;
			TSESEdge<T>* edge3;
			TSESVertex<T>* p0;
			TSESVertex<T>* p1;
			TSESVertex<T>* p2;
			TSESVertex<T>* p3;
			Index i = 0;
			while (edge_[i]->type_ != TSESEdge<T>::TYPE_CONCAVE)
			{
				i++;
			}
			edge0 = edge_[i];								// edge0 = first concave edge
			i++;
			while (edge_[i]->type_ != TSESEdge<T>::TYPE_CONCAVE)
			{
				i++;
			}   	
			edge2 = edge_[i];								// edge2 = second concave edge
			p0 = edge0->vertex_[0];
			p1 = edge0->vertex_[1];
			TSESEdge<T>* e;
			if (getEdge(p1,edge2->vertex_[0],e))
			{
				edge1 = e;
				p2 = edge2->vertex_[0];
				p3 = edge2->vertex_[1];
			}
			else
			{
				getEdge(p1,edge2->vertex_[1],e);							//	  	____e3____
				edge1 = e;																		//     /          \			x
				p2 = edge2->vertex_[1];												//  p0 \          /p3
				p3 = edge2->vertex_[0];												//      \        /
			}																								//    e0|        |e2
			getEdge(p0,p3,e);																//      |        |
			edge3 = e;																			//      /________\			x
			edge_[0] = edge0;																//   p1/   e1     \p2
			edge_[1] = edge1;
			edge_[2] = edge2;
			edge_[3] = edge3;
			vertex_[0] = p0;
			vertex_[1] = p1;
			vertex_[2] = p2;
			vertex_[3] = p3;
		}
		else
		{
			TSESEdge<T>* edge0;
			TSESEdge<T>* edge1;
			TSESEdge<T>* edge2;
			TSESEdge<T>* edge3;
			TSESEdge<T>* edge4;
			TSESEdge<T>* edge5;
			TSESVertex<T>* p0;
			TSESVertex<T>* p1;
			TSESVertex<T>* p2;
			TSESVertex<T>* p3;
			TSESVertex<T>* p4;
			TSESVertex<T>* p5;
			Position i = 0;
			while (edge_[i]->type_ != TSESEdge<T>::TYPE_CONVEX)
			{
				i++;
			}
			edge0 = edge_[i];								// edge0 = first convex edge
			p0 = edge0->vertex_[0];
			p2 = edge0->vertex_[1];
			i++;
			while (edge_[i]->type_ != TSESEdge<T>::TYPE_CONVEX)
			{
				i++;
			}   	
			edge3 = edge_[i];								// edge3 = second convex edge
			p3 = edge3->vertex_[0];
			p5 = edge3->vertex_[1];
			for (i = 0; i < edge_.size(); i++)
			{
				if ((edge_[i]->vertex_[0] == p0) && (edge_[i] != edge0))
				{
					edge1 = edge_[i];
					p1 = edge1->vertex_[1];
				}
				else
				{
					if (edge_[i]->vertex_[1] == p0)
					{
						edge1 = edge_[i];
						p1 = edge1->vertex_[0];
					}
				}
			}
			for (i = 0; i < edge_.size(); i++)
			{
				if (((edge_[i]->vertex_[0] == p1) && (edge_[i]->vertex_[1] == p2)) ||
						((edge_[i]->vertex_[1] == p1) && (edge_[i]->vertex_[0] == p2))		)
				{
					edge2 = edge_[i];
				}
			}
			for (i = 0; i < edge_.size(); i++)
			{
				if ((edge_[i]->vertex_[0] == p3) && (edge_[i] != edge3))
				{
					edge4 = edge_[i];
					p4 = edge4->vertex_[1];
				}
				else
				{
					if (edge_[i]->vertex_[1] == p3)
					{
						edge4 = edge_[i];
						p4 = edge4->vertex_[0];
					}
				}
			}
			for (i = 0; i < edge_.size(); i++)
			{
				if (((edge_[i]->vertex_[0] == p5) && (edge_[i]->vertex_[1] == p4)) ||
						((edge_[i]->vertex_[1] == p5) && (edge_[i]->vertex_[0] == p4))		)
				{
					edge5 = edge_[i];
				}
			}
			if (edge1->circle_ != edge4->circle_)
			{
				TSESEdge<T>* temp = edge5;
				edge5 = edge4;
				edge4 = temp;
				TSESVertex<T>* tmp = p5;
				p5 = p3;
				p3 = tmp;
			}
			edge_[0] = edge0;
			edge_[1] = edge1;
			edge_[2] = edge2;
			edge_[3] = edge3;
			edge_[4] = edge4;
			edge_[5] = edge5;
			vertex_[0] = p0;
			vertex_[1] = p1;
			vertex_[2] = p2;
			vertex_[3] = p3;
			vertex_[4] = p4;
			vertex_[5] = p5;
		}
	}


	template <class T>
	bool TSESFace<T>::isNeighbouredTo(TSESFace<T>* face) const
		throw()
	{
		for (Position i = 0; i < edge_.size(); i++)
		{
			if (edge_[i] != NULL)
			{
				if (edge_[i]->face_[0] == face)
				{
					return true;
				}
				if (edge_[i]->face_[1] == face)
				{
					return true;
				}
			}
		}
		return false;
	}


	template <class T>
	bool TSESFace<T>::isFree() const
		throw()
	{
		if (type_ != TYPE_TORIC)
		{
			return false;
		}
		return rsedge_->isFree();
	}





} // namespace BALL

#endif // BALL_STRUCTURE_SESFACE_H
