// $Id:

#ifndef BALL_STRUCTURE_GRAPHEDGE_H
#define BALL_STRUCTURE_GRAPHEDGE_H

#ifndef BALL_COMMON_H
#	include <BALL/common.h>
#endif

#include <vector>

namespace BALL
{

	/** Generic GraphEdge Class.	
      {\bf Definition:} \URL{BALL/STRUCTURE/GraphEdge.h}
	*/
	template <class Vertex, class Face>
	class GraphEdge
	{
		public:

		BALL_CREATE(GraphEdge)

		/**	@name	Constructors and Destructors
		*/
		//@{

		/**	Default constructor.
				This method creates a new GraphEdge object.
		*/
		GraphEdge()
			throw();

		/**	Copy constructor.
				Create a new GraphEdge object from another.
				@param	edge	the GraphEdge object to be copied
				@param	deep	if deep = false, all pointers are set to NULL (default). Otherwise the new	
											GraphEdge object is linked to the neighbours of the old GraphEdge object.
		*/
		GraphEdge(const GraphEdge<Vertex,Face>& edge, bool deep = false)
			throw();

		/**	Detailled constructor.
				Create a new GraphEdge object from a lot of nice objects
				@param	vertex1	assigned to the first vertex
				@param	vertex2	assigned to the second vertex
				@param	face1		assigned to the first face
				@param	face2		assigned to the second face
				@param	index		assigned to the index
		*/
		GraphEdge(Vertex* vertex1, Vertex* vertex2, Face* face1, Face* face2, Index index)
			throw();

		/**	Destructor.
				Destructs the GraphEdge object.
				The neighboured faces and vertices are {/bf not} destructed.
		*/
		virtual ~GraphEdge()
			throw();

		//@}
		/**	@name	Assignment
		*/
		//@{

		/**	Assign from another GraphEdge.
				@param	edge	the GraphEdge object to assign from
				@param	deep	if deep = false, all pointers are set to NULL (default). Otherwise the	
											GraphEdge object is linked to the neighbours of the GraphEdge object to be copied.
		*/
		virtual void set(const GraphEdge<Vertex,Face>& edge, bool deep = false)
			throw();

		/**	Assign from a lot of nice objects
				@param	vertex1	assigned to the first vertex
				@param	vertex2	assigned to the second vertex
				@param	face1		assigned to the first face
				@param	face2		assigned to the second face
				@param	index		assigned to the index
		*/
		virtual void set(Vertex* vertex0, Vertex* vertex1, Face* face0, Face* face1, Index index)
			throw();
		
		//@}
		/**	@name	Accessors
		*/
		//@{

		/** Set one of the two vertices of the GraphEdge.
				@param	i				the first vertex is changed if i = 0, the second	
												otherwise
				@param	vertex	a pointer to the new vertex
		*/
		void setVertex(Position i, Vertex* vertex)
			throw();

		/** Return one of the two vertices of the GraphEdge.
				@param	i
				@return	Vertex*	a pointer to the first vertex if i = 0,	
												a pointer to the second vertex otherwise
		*/
		Vertex* getVertex(Position i) const
			throw();

		/** Set one of the two faces of the GraphEdge.
				@param	i			change the first face, if i = 0, the second otherwise
				@param	face	a pointer to the new face
		*/
		void setFace(Position i, Face* face)
			throw();

		/** Return one of the two faces of the GraphEdge.
				@param	i
				@return	Face*	a pointer to the first face if i = 0,	
											a pointer to the second face otherwise
		*/
		Face* getFace(Position i) const
			throw();

		/** Set the index of the GraphEdge.
				@param	index	the new index
		*/
		void setIndex(Index index)
			throw();

		/** Return the index of the GraphEdge.
				@return	Index	the index of the GraphEdge
		*/
		Index getIndex() const
			throw();
		
		/** Return a pointer to the other vertex of the GraphEdge.
				If the given vertex is not incident to the GraphEdge, an exception is thrown.
				@param	Vertex*	one of the vertices of the GraphEdge
				@return	Vertex*	the other vertex
		*/
		Vertex* otherVertex(Vertex* face) const
			throw(Exception::GeneralException);

		/** Return a pointer to the other face of the GraphEdge.
				If the given face is not neighboured to the GraphEdge, an exception is thrown.
				@param	Face*	one of the faces of the GraphEdge
				@return	Face*	the other face
		*/
		Face* otherFace(Face* face) const
			throw(Exception::GeneralException);

		/** Substitute a vertex by an other one.
				@param	old_vertex	the vertex that has to be substituted
				@param	new_vertex	the new vertex
				@return	bool				{\bf true}, if the vertex can be substituted, {\bf false} otherwise
		*/
		bool substituteVertex(Vertex* old_vertex, Vertex* new_vertex)
			throw();

		/** Substitute a face by an other one.
				@param	old_face	the face that has to be substituted
				@param	new_face	the new face
				@return	bool			{\bf true}, if the face can be substituted, {\bf false} otherwise
		*/
		bool substituteFace(Face* old_vertex, Face* new_vertex)
			throw();

		/*void remove()
		{	
			vertex_[0]->deleteEdge(this);
			vertex_[1]->deleteEdge(this);
			//if (face0_ != NULL)
			//{
			//	face0_->deleteEdge(this);
			//}
			//if (face1_ != NULL)
			//{
			//	face1_->deleteEdge(this);
			//}
		}*/

		/**	Delete a face of the GraphEdge.
				If the second face of the GraphEdge should be deleted, it is set to NULL. If the first one should	
				be deleted, the second one is copied to first position and then set to NULL.
				@param	face	the face to delete
				@return	Face*	a pointer to the other face
		*/
		Face* deleteFace(Face* face)
			throw();

		//@}


		/**	@name	Predicates
		*/
		//@{

		/**	Equality operator.
		*/
		virtual bool operator == (const GraphEdge<Vertex,Face>&) const
			throw();// = 0;

		/**	Inequality operator.
		*/
		virtual bool operator != (const GraphEdge<Vertex,Face>&) const
			throw();// = 0;
		
		//@}


		protected:

		/*_ The vertices of the GraphEdge
		*/
		std::vector<Vertex*> vertex_;
		/*_ The faces of the GraphEdge
		*/
		std::vector<Face*> face_;
		/*_ The index of the GraphEdge
		*/
		Index index_;

	};



	template <typename Vertex, typename Face>
	GraphEdge<Vertex,Face>::GraphEdge()
		throw()
		//:	vertex_(2,NULL),
		//	face_(2,NULL),
		:	vertex_(2),
			face_(2),
			index_(-1)
	{
	}


	template <typename Vertex, typename Face>
	GraphEdge<Vertex,Face>::GraphEdge(const GraphEdge<Vertex,Face>& edge, bool deep)
		throw()
		//:	vertex_(2,NULL),
		//	face_(2,NULL),
		:	vertex_(2),
			face_(2),
			index_(edge.index_)
	{
		if (deep)
		{
			vertex_[0] = edge.vertex_[0];
			vertex_[1] = edge.vertex_[1];
			face_[0] = edge.face_[0];
			face_[1] = edge.face_[1];
		}
	}


	template <typename Vertex, typename Face>
	GraphEdge<Vertex,Face>::GraphEdge
		(Vertex*	vertex1,
		 Vertex*	vertex2,
		 Face*		face1,
		 Face*		face2,
		 Index		index)
		throw()
		: vertex_(2),
			face_(2),
			index_(index)
	{
		vertex_[0] = vertex1;
		vertex_[1] = vertex2;
		face_[0] = face1;
		face_[1] = face2;
	}


	template <typename Vertex, typename Face>
	GraphEdge<Vertex,Face>::~GraphEdge()
		throw()
	{
	}


	template <typename Vertex, typename Face>
	void GraphEdge<Vertex,Face>::set(const GraphEdge<Vertex,Face>& edge, bool deep)
		throw()
	{
		if (deep)
		{
			vertex_[0] = edge.vertex_[0];
			vertex_[1] = edge.vertex_[1];
			face_[0] = edge.face_[0];
			face_[1] = edge.face_[1];
		}
		else
		{
			vertex_[0] = NULL;
			vertex_[1] = NULL;
			face_[0] = NULL;
			face_[1] = NULL;
		}
		index_ = edge.index_;
	}


	template <typename Vertex, typename Face>
	void GraphEdge<Vertex,Face>::set
		(Vertex*	vertex0,
		 Vertex*	vertex1,
		 Face*		face0,
		 Face*		face1,
		 Index		index)
		throw()
	{
		vertex_[0] = vertex0;
		vertex_[1] = vertex1;
		face_[0] = face0;
		face_[1] = face1;
		index_ = index;
	}


	template <typename Vertex, typename Face>
	void GraphEdge<Vertex,Face>::setVertex(Position i, Vertex* vertex)
		throw()
	{
		if (i == 0)
		{
			vertex_[0] = vertex;
		}
		else
		{
			vertex_[1] = vertex;
		}
	}


	template <typename Vertex, typename Face>
	Vertex* GraphEdge<Vertex,Face>::getVertex(Position i) const
		throw()
	{
		if (i == 0)
		{
			return vertex_[0];
		}
		else
		{
			return vertex_[1];
		}
	}


	template <typename Vertex, typename Face>
	void GraphEdge<Vertex,Face>::setFace(Position i, Face* face)
		throw()
	{
		if (i == 0)
		{
			face_[0] = face;
		}
		else
		{
			face_[1] = face;
		}
	}


	template <typename Vertex, typename Face>
	Face* GraphEdge<Vertex,Face>::getFace(Position i) const
		throw()
	{
		if (i == 0)
		{
			return face_[0];
		}
		else
		{
			return face_[1];
		}
	}


	template <typename Vertex, typename Face>
	void GraphEdge<Vertex,Face>::setIndex(Index index)
		throw()
	{
		index_ = index;
	}


	template <typename Vertex, typename Face>
	Index GraphEdge<Vertex,Face>::getIndex() const
		throw()
	{
		return index_;
	}


	template <typename Vertex, typename Face>
	Vertex* GraphEdge<Vertex,Face>::otherVertex(Vertex* vertex) const
		throw(Exception::GeneralException)
	{
		if (vertex_[0] == vertex)
		{
			return vertex_[1];
		}
		else
		{
			if (vertex_[1] == vertex)
			{
				return vertex_[0];
			}
			else
			{
				throw Exception::GeneralException(__FILE__, __LINE__);
			}
		}
	}


	template <typename Vertex, typename Face>
	Face* GraphEdge<Vertex,Face>::otherFace(Face* face) const
		throw(Exception::GeneralException)
	{
		if (face_[0] == face)
		{
			return face_[1];
		}
		else
		{
			if (face_[1] == face)
			{
				return face_[0];
			}
			else
			{
				throw Exception::GeneralException(__FILE__, __LINE__);
			}
		}
	}


	template <typename Vertex, typename Face>
	bool GraphEdge<Vertex,Face>::substituteVertex(Vertex* old_vertex, Vertex* new_vertex)
		throw()
	{
		if (vertex_[0] == old_vertex)
		{
			vertex_[0] = new_vertex;
		}
		else
		{
			if (vertex_[1] == old_vertex)
			{
				vertex_[1] = new_vertex;
			}
			else
			{
				return false;
			}
		}
		return true;
	}


	template <typename Vertex, typename Face>
	bool GraphEdge<Vertex,Face>::substituteFace(Face* old_face, Face* new_face)
		throw()
	{
		if (face_[0] == old_face)
		{
			face_[0] = new_face;
		}
		else
		{
			if (face_[1] == old_face)
			{
				face_[1] = new_face;
			}
			else
			{
				return false;
			}
		}
		return true;
	}


	template <typename Vertex, typename Face>
	bool GraphEdge<Vertex,Face>::operator == (const GraphEdge<Vertex,Face>&) const
		throw()
	{
		return false;
	}


	template <typename Vertex, typename Face>
	bool GraphEdge<Vertex,Face>::operator != (const GraphEdge<Vertex,Face>&) const
		throw()
	{
		return true;
	}


	template <typename Vertex, typename Face>
	Face* GraphEdge<Vertex,Face>::deleteFace(Face* face)
		throw()
	{
		if (face_[1] == face)
		{
			face_[1] = NULL;
		}
		else
		{
			if (face_[0] == face)
			{
				face_[0] = face_[1];
				face_[1] = NULL;
			}
		}
		return face_[0];
	}





} // namespace BALL

#endif // BALL_STRUCTURE_GRAPHEDGE_H
