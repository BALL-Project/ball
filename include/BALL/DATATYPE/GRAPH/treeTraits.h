#ifndef BALL_DATATYPE_GRAPH_TREETRAITS_H
#define BALL_DATATYPE_GRAPH_TREETRAITS_H

#include <stack>
#include <iterator>
#include <vector>
#include <iostream>

#ifndef BALL_COMMON_EXCEPTION_H
# include <BALL/COMMON/exception.h>
#endif

// TODO: replace by boost::graph_as_tree

namespace BALL
{

	template<class T>
	class TreeCursor;

	template<class T>
	class DefaultTreeCursor;

	/**
	 * TreeTrait is a set of functions and types which build a minimum tree implementation.
	 * Each treenode has to provide at least a child_iterator type and a #beginChild and #endChild function
	 * which iterates above the children of the treenode. By default a tree should be a container of
	 * pointers to treenodes of the same type. Trees which consist of references or other structures instead
	 * of pointers have to redefine the #getPointer function, which takes a iterator to a child node and returns
	 * a pointer to this node.
	 *
	 * Each class which implements this trait can be used in generic tree functions like tree-traversal algorithms
	 * or the #postOrderFolding function.
	 */
	template<class T>
	struct TreeTrait
	{
		/**
		 * iterator to the child elements of this tree node. Generic tree algorithms shouldn't
		 * dereference this iterator itself and should use the #getPointer function instead.
		 * The iterator should implement the bidirectional_iterator_tag! Otherwise you have
		 * to define an own cursor class.
		 */
		typedef typename T::child_iterator child_iterator;

		/**
		 * A const iterator to the child elements.
		 * Is used to define a const variant of TreeTrait.
		 */
		typedef typename T::const_child_iterator const_child_iterator;

		/**
		 * The cursor for this tree. Generic tree functions should use
		 * the #getCursor function instead of building a cursor itself.
		 * For a description of cursors see TreeCursor class
		 * @see TreeCursor
		 */
		typedef DefaultTreeCursor<T> cursor;
		typedef DefaultTreeCursor<T const> const_cursor;

		/**
		 * returns a bidirectional iterator to the first child of a tree node.
		 * If the node is a leaf, this function should return an iterator
		 * which is equal to the iterator of the #endChild function.
		 * @param tree the tree node
		 */
		static child_iterator beginChild(T& tree)
		{
			return tree.beginChild();
		}
		static const_child_iterator beginChild(T const& tree)
		{
			return tree.beginChild();
		}

		/**
		 * returns a bidirectional iterator to the end of the tree nodes children.
		 * A child_iterator, created by #beginChild and increased by increment operations,
		 * should be equal to this iterator if there is no further child to iterate.
		 * @param tree the tree node
		 */
		static child_iterator endChild(T& tree)
		{
			return tree.endChild();
		}
		static const_child_iterator endChild(T const& tree)
		{
			return tree.endChild();
		}

		/**
		 * returns a child_iterator for a given tree node, represented
		 * as child_iterator itself.
		 * @see #beginChild(T&)
		 */
		static child_iterator beginChild(child_iterator tree)
		{
			return getPointer(tree)->beginChild();
		}
		static const_child_iterator beginChild(const_child_iterator tree)
		{
			return getPointer(tree)->beginChild();
		}

		/**
		 * returns a child_iterator for a given tree node, represented
		 * as child_iterator itself.
		 * @see #endChild(T&)
		 */
		static child_iterator endChild(child_iterator tree)
		{
			return getPointer(tree)->endChild();
		}
		static const_child_iterator endChild(const_child_iterator tree)
		{
			return getPointer(tree)->endChild();
		}

		/**
		 *	Return a pointer to the tree node, which is iterated by the
		 *	given iterator. This function is used, because there are maybe
		 *	some iterators which iterates above references or other structures
		 *	instead of pointers. If generic algorithms use this function instead of
		 *	dereferencing the iterator itself, they have the guarantee, that they
		 *	get a pointer to the tree node.
		 *	If your child_iterator iterates above other types than T*, you have to
		 *	redefine this function and transform *child_iterator into T*.
		 */
		static T* getPointer(child_iterator iter)
		{
			return *iter;
		}
		static T const * getPointer(const_child_iterator iter)
		{
			return *iter;
		}

		/**
		 * Returns a cursor, pointing to the given tree node.
		 * @see TreeCursor
		 */
		static cursor getCursor(T& tree)
		{
			return cursor(tree);
		}
		static const_cursor getCursor(T const& tree)
		{
			return cursor(tree);
		}
	};

	/**
	 * Specialize TreeTrait for const data types
	 */
	template<class T>
		struct TreeTrait<const T> : TreeTrait<T>
		{
			typedef typename TreeTrait<T>::const_child_iterator child_iterator;
			typedef typename TreeTrait<T>::const_cursor cursor;
			static child_iterator beginChild(T const& tree)
			{
				return TreeTrait<T>::beginChild(tree);
			}
			static child_iterator endChild(T const& tree)
			{
				return TreeTrait<T>::endChild(tree);;
			}
			static child_iterator beginChild(child_iterator tree)
			{
				return TreeTrait<T>::beginChild(tree);
			}
			static child_iterator endChild(child_iterator tree)
			{
				return TreeTrait<T>::endChild(tree);
			}
			static T const * getPointer(child_iterator iter)
			{
				return TreeTrait<T>::getPointer(iter);
			}
			static cursor getCursor(T const& tree)
			{
				return TreeTrait<T>::getCursor(tree);
			}
		};


	/**
	 * Gives information about the position of the vertex in the tree.
	 * Leafs have no childs, the root has no parent. Remark that a vertex,
	 * which has neither parents nor childs is a root, not a leaf, because each
	 * vertex has just one type. An inner vertex has childs and a parent. An end vertex
	 * doesn't exist. Nevertheless this identifier can be used to mark the end of a traversion or to
	 * mark an undefined vertex.
	 */
	enum NodeType
	{
		LEAF_VERTEX, INNER_VERTEX, ROOT_VERTEX, END_VERTEX
	};

	/**
	 * A TreeCursor is a special iterator which has more than two directions. It can
	 * move to child, sisters and parent elements. While the implementation of
	 * child and parent movements is easy with the functions provided by TreeTrait,
	 * there is no possibility to move to the parent of an element. There is a default
	 * implementation named DefaultTreeCursor, which uses a stack to remember the
	 * parent elements and provide a parent-function. Nevertheless it is possible
	 * to overwrite this cursor by a template specific, more efficient implementation.
	 *
	 * You should overwrite this class and redefine the TreeTrait::cursor type
	 * and TreeTrait::getCursor function, if:
	 * - your tree implementation provides an efficient way to return the parent of a vertex
	 * - your child_iterator is just an forward_iterator, so that the #previousSibling and
	 * 		#hasPreviousSibling functions doesn't work
	 */
	template<class T>
		class TreeCursor
		{
			public:
				typedef TreeTrait<T> trait;

				/**
				 * Builds an invalid tree_cursor, which should throw BALL::Exception::IllegalTreeOperation
				 * on each function and has NodeType END_VERTEX
				 */
				TreeCursor();

				/**
				 *	Build a tree_cursor which points on the given tree node
				 */
				TreeCursor(T& tree);

				/**
				 * copy constructor
				 */
				TreeCursor(TreeCursor<T> const& cursor);

				/**
				 * Assignment operation
				 */
				TreeCursor<T>& operator=(TreeCursor<T> const& cursor);

				/**
				 * returns the type of the vertex. If cursor is invalid, return END_VERTEX
				 */
				NodeType getType() const;

				/**
				 * dereferencing of the cursor. Returns the vertex the cursor is pointing to
				 * @throw Exception::IllegalTreeOperation if cursor is invalid
				 */
				T& getVertex();

				/**
				 * returns the parent node of the current node
				 * @throw Exception::IllegalTreeOperation if cursor is invalid or node is a root
				 */
				T& getParent();

				/**
				 * returns the right sibling of the current node
				 * @throw Exception::IllegalTreeOperation if cursor is invalid or node has no right sibling
				 */
				T& getNextSibling();

				/**
				 * returns the left sibling of the current node
				 * @throw Exception::IllegalTreeOperation if cursor is invalid or node has no left sibling
				 */
				T& getPreviousSibling();

				/**
				 * returns the first child of the current node
				 * @throw Exception::IllegalTreeOperation if cursor is invalid or node is leaf or childless root
				 */
				T& getFirstChild();

				/**
				 * returns true if node has a parent
				 * @throw Exception::IllegalTreeOperation if cursor is invalid
				 */
				bool hasParent() const;

				/**
				 * returns true if node has a right sibling
				 * @throw Exception::IllegalTreeOperation if cursor is invalid
				 */
				bool hasNextSibling() const;

				/**
				 * returns true if node has a left sibling
				 * @throw Exception::IllegalTreeOperation if cursor is invalid
				 */
				bool hasPreviousSibling() const;

				/**
				 * returns true if node has at least one child
				 * @throw Exception::IllegalTreeOperation if cursor is invalid
				 */
				bool hasChild() const;

				/**
				 * two cursors are equal, if they point to the same vertex.
				 * @throw Exception::IllegalTreeOperation if cursor is invalid
				 */
				bool operator==(TreeCursor<T> const& cursor) const;
				bool operator!=(TreeCursor<T> const& cursor) const;

				/**
				 * moves the cursor to the first child of the current node
				 * @throw Exception::IllegalTreeOperation if cursor is invalid or node has no childs
				 */
				void gotoFirstChild();

				/**
				 * moves the cursor to the parent of the current node
				 * @throw Exception::IllegalTreeOperation if cursor is invalid or node has no parent
				 */
				void gotoParent();

				/**
				 * moves the cursor to the right sibling of the current node
				 * @throw Exception::IllegalTreeOperation if cursor is invalid or node has no right sibling
				 */
				void gotoNextSibling();

				/**
				 * moves the cursor to the left sibling of the current node
				 * @throw Exception::IllegalTreeOperation if cursor is invalid or node has no left sibling
				 */
				void gotoPreviousSibling();

			protected:
				/**
				 * the current vertex this cursor is pointing to
				 */
				T* vertex;
				/**
				 * the type of the current vertex
				 */
				NodeType type;
				typename TreeTrait<T>::child_iterator iterator;
				typename TreeTrait<T>::child_iterator begin;
				typename TreeTrait<T>::child_iterator end;

				/**
				 * returns true if this cursor can be dereferenced
				 */
				bool isValid() const;
				/**
				 * returns true if the current vertex has childs.
				 */
				bool checkForChilds() const;

		};

	/**
	 * Default cursor implementation for tree nodes which doesn't have a
	 * pointer to their parent node. This implementation uses a stack to
	 * remember the path from root to current node. So it can return the
	 * parent of a vertex by pop the vertex on the top of the stack.
	 */
	template<class T>
		class DefaultTreeCursor : public TreeCursor<T>
	{
		public:
			typedef TreeTrait<T> trait;
			/**
			 * Builds an invalid cursor
			 */
			DefaultTreeCursor();

			/**
			 * builds a cursor which points to the given tree node
			 */
			DefaultTreeCursor(T& tree);

			/**
			 * copy constructor
			 */
			DefaultTreeCursor(DefaultTreeCursor<T> const& cursor);
			DefaultTreeCursor<T>& operator=(DefaultTreeCursor<T> const& cursor);

			/**
			 * @see TreeCursor::gotoParent
			 */
			void gotoParent();

			/**
			 * @see TreeCursor::gotoFirstChild
			 */
			void gotoFirstChild();

			/**
			 * @see TreeCursor::getParent
			 */
			T& getParent();

		protected:
			/**
			 * stack with the ancestors of the current vertex
			 */
			std::stack<T*> parents;
	};

	/**
	 * @name Traversals
	 * A traversal is an iterator, which iterates each node in the tree in a special order.
	 */
	//@{

	/**
	 * Abstract class for traversal algorithms.
	 * Implements an bidirectional iterator. Only the incrementing and decrementing operations
	 * have to be rewritten by subclasses.
	 */
	template<class T>
	class AbstractTraversal 
		: public std::iterator<std::bidirectional_iterator_tag, T>
	{
		protected:
			typedef TreeTrait<T> trait;
			typedef typename trait::cursor Cursor;
			/**
			 * tree cursor to navigating in the tree
			 */
			Cursor cursor;
			/**
			 * should be true if the iterator is invalid and pointing
			 * to the end of a tree
			 */
			bool atEnd;
		public:
			/**
			 * Empty constructor, builds an invalid iterator to the end of each tree
			 */
			AbstractTraversal();

			/**
			 * Iterator above the subtree which is rooted by the given node
			 */
			AbstractTraversal(T& tree);


			/**
			 * copy constructor
			 */
			AbstractTraversal(AbstractTraversal<T> const& copy);
			AbstractTraversal<T>& operator=(AbstractTraversal<T> const& copy);

			/**
			 * two iterators are equal if they both are invalid, or if their cursors
			 * are equal
			 */
			bool operator==(AbstractTraversal<T> const& other) const;
			bool operator!=(AbstractTraversal<T> const& other) const;

			/**
			 * returns the type of the current vertex or END_VERTEX if the iterator is invalid
			 */
			NodeType getVertexType() const;

			/**
			 * returns a reference to the current vertex.
			 * @throw BALL::Exception::InvalidIterator if iterator is invalid
			 */
			T& operator * ();

			/**
			 * @throw BALL::Exception::OutOfRange if iterator is on the end of a tree
			 */
			AbstractTraversal<T>& operator++();

			/**
			 * @throw BALL::Exception::OutOfRange if iterator is on the end of a tree
			 */
			AbstractTraversal<T> operator++(int);

			/**
			 * @throw BALL::Exception::OutOfRange if iterator is on the begin of a tree
			 */
			AbstractTraversal<T>& operator--();

			/**
			 * @throw BALL::Exception::OutOfRange if iterator is on the begin of a tree
			 */
			AbstractTraversal<T> operator--(int);

			Cursor& getCursor() { return cursor; }
	};

	/**
	 * Iterates a tree in post-order. That means: it iterates first the left subtree, later
	 * the right subtree and finally the root itself.
	 */
	template<class T>
		class PostOrderTraversal : public AbstractTraversal<T>
	{
		public:
			PostOrderTraversal();
			PostOrderTraversal(T& tree);
			PostOrderTraversal(PostOrderTraversal<T> const& copy);
			PostOrderTraversal<T>& operator=(PostOrderTraversal<T> const& copy);
			PostOrderTraversal<T>& operator++();
			PostOrderTraversal<T> operator++(int);
			PostOrderTraversal<T>& operator--();
			PostOrderTraversal<T> operator--(int);
	};

	/**
	 * Iterates a tree in pre-order. That means: it iterates first the root, then the
	 * left subtree and finally the right subtree.
	 */
	template<class T>
		class PreOrderTraversal : public AbstractTraversal<T>
	{
		public:
			PreOrderTraversal();
			PreOrderTraversal(T& tree);
			PreOrderTraversal(PreOrderTraversal<T> const& copy);
			PreOrderTraversal<T>& operator=(PreOrderTraversal<T> const& copy);
			PreOrderTraversal<T>& operator++();
			PreOrderTraversal<T> operator++(int);
			PreOrderTraversal<T>& operator--();
			PreOrderTraversal<T> operator--(int);
	};

	/**
	 *	Apply a function <f: (From x NodeType x vector<To>::const_iterator x vector<To>::const_iterator) -> To> on the tree,
	 *	where From is the type of the tree node and To is the return value of the function.
	 *	You could define postOrderFolding functional as:
	 *	postOrderFolding(root, f) := f(root, ROOT_VERTEX, [postOrderFolding(v, f) | v <- children(root)] )
	 *	postOrderFolding(inner, f) := f(inner, INNER_VERTEX, [postOrderFolding(v, f) | v <- children(inner)] )
	 *	postOrderFolding(leaf, f) := f(leaf, LEAF_VERTEX, [])
	 *	Which means: This function is applied first on the leafs with an empty argument list. Then it is applied
	 *	on the parent of a set of computed leafs with the return-values of the leafs as arguments, and so on.
	 *	@return the return value for the root vertex
	 */
	template<class From, class To, class Functor>
	To postOrderFolding(From& tree, Functor& f);

	//@}


	/*
	 * Implementation
	 */

	template<class T>
		TreeCursor<T>::TreeCursor() : vertex(NULL), type(END_VERTEX), iterator(), begin(iterator), end(iterator)
	{

	}

	template<class T>
		TreeCursor<T>::TreeCursor(T& tree) : vertex(&tree), type(ROOT_VERTEX), iterator(), begin(iterator), end(iterator)
	{
	}

	template<class T>
		TreeCursor<T>::TreeCursor(TreeCursor<T> const& cursor) :
			vertex(cursor.vertex), type(cursor.type), iterator(cursor.iterator), begin(cursor.begin), end(cursor.end)
	{

	}

	template<class T>
		TreeCursor<T>& TreeCursor<T>::operator=(TreeCursor<T> const& cursor)
		{
			if (&cursor == this)
			{
				return *this;
			}
			vertex = cursor.vertex;
			type = cursor.type;
			iterator = cursor.iterator;
			begin = cursor.begin;
			end = cursor.end;
			return *this;
		}

	template<class T>
		bool TreeCursor<T>::isValid() const
		{
			return type != END_VERTEX && vertex != NULL;
		}

	template<class T>
		NodeType TreeCursor<T>::getType() const
		{
			return type;
		}

	template<class T>
		T& TreeCursor<T>::getVertex()
		{
			if (!isValid()) {
				throw Exception::IllegalTreeOperation(__FILE__, __LINE__);
			}
			return *vertex;
		}

	template<class T>
		T& TreeCursor<T>::getParent()
		{
			throw Exception::NotImplemented(__FILE__, __LINE__);
		}

	template<class T>
		T& TreeCursor<T>::getFirstChild()
		{
			if (!hasChild())
			{
				throw Exception::IllegalTreeOperation(__FILE__, __LINE__);
			} else
			{
				return *trait::getPointer(trait::beginChild(*vertex));
			}
		}

	template<class T>
		T& TreeCursor<T>::getNextSibling()
		{
			if (!isValid() || type == LEAF_VERTEX || type == ROOT_VERTEX)
			{
				throw Exception::IllegalTreeOperation(__FILE__, __LINE__);
			} else
			{
				typename trait::child_iterator iter (iterator);
				++iter;
				if (iter != end)
				{
					return (*trait::getPointer(iter));
				} else
				{
					throw Exception::IllegalTreeOperation(__FILE__, __LINE__);
				}
			}
		}

	template<class T>
		T& TreeCursor<T>::getPreviousSibling()
		{
			if (!isValid() || type == LEAF_VERTEX || type == ROOT_VERTEX || iterator == begin)
			{
				throw Exception::IllegalTreeOperation(__FILE__, __LINE__);
			} else
			{
				typename trait::child_iterator iter (iterator);
				--iter;
				return (*trait::getPointer(iter));
			}
		}

	template<class T>
		bool TreeCursor<T>::hasParent() const
		{
			if (!isValid())
			{
				throw Exception::IllegalTreeOperation(__FILE__, __LINE__);
			}
			return type != ROOT_VERTEX;
		}
	template<class T>
		bool TreeCursor<T>::hasChild() const
		{
			if (!isValid())
			{
				throw Exception::IllegalTreeOperation(__FILE__, __LINE__);
			}
			if (type == LEAF_VERTEX)
			{
				return false;
			} else if (type == ROOT_VERTEX)
			{
				return (trait::beginChild(*vertex) != trait::endChild(*vertex));
			} else
			{
				return true;
			}
		}

	template<class T>
		bool TreeCursor<T>::checkForChilds() const
		{
			if (!isValid())
			{
				throw Exception::IllegalTreeOperation(__FILE__, __LINE__);
			} else {
				return (trait::beginChild(*vertex) != trait::endChild(*vertex));
			}
		}

	template<class T>
		bool TreeCursor<T>::hasPreviousSibling() const
		{
			if (!isValid())
			{
				throw Exception::IllegalTreeOperation(__FILE__, __LINE__);
			}
			if (type == ROOT_VERTEX)
			{
				return false;
			} else
			{
				return iterator != begin;
			}
		}
	template<class T>
		bool TreeCursor<T>::hasNextSibling() const
		{
			if (!isValid())
			{
				throw Exception::IllegalTreeOperation(__FILE__, __LINE__);
			}
			if (type == ROOT_VERTEX)
			{
				return false;
			} else
			{
				typename trait::child_iterator iter (iterator);
				return (++iter != end);
			}
		}

	template<class T>
		bool TreeCursor<T>::operator==(TreeCursor<T> const& cursor) const
		{
			if (!isValid())
			{
				return !cursor.isValid();
			}
			return vertex == cursor.vertex;
		}

	template<class T>
		bool TreeCursor<T>::operator!=(TreeCursor<T> const& cursor) const
		{
			return !(*this == cursor);
		}

	template<class T>
		void TreeCursor<T>::gotoParent()
		{
			throw Exception::NotImplemented(__FILE__, __LINE__);
		}

	template<class T>
		void TreeCursor<T>::gotoFirstChild()
		{
			if (hasChild())
			{
				begin = trait::beginChild(*vertex);
				end = trait::endChild(*vertex);
				iterator = begin;
				vertex = trait::getPointer(iterator);
				type = (checkForChilds() ? INNER_VERTEX : LEAF_VERTEX);
			} else
			{
				throw Exception::IllegalTreeOperation(__FILE__, __LINE__);
			}
		}

	template<class T>
		void TreeCursor<T>::gotoNextSibling()
		{

			if (!isValid() || type == ROOT_VERTEX)
			{
				throw Exception::IllegalTreeOperation(__FILE__, __LINE__);
			} else
			{
				if (++iterator == end)
				{
					throw Exception::IllegalTreeOperation(__FILE__, __LINE__);
				}
				vertex = trait::getPointer(iterator);
				type = (checkForChilds() ? INNER_VERTEX : LEAF_VERTEX);
			}
		}

	template<class T>
		void TreeCursor<T>::gotoPreviousSibling()
		{
			if (hasPreviousSibling())
			{
				--iterator;
				vertex = trait::getPointer(iterator);
				type = (checkForChilds() ? INNER_VERTEX : LEAF_VERTEX);
			} else
			{
				throw Exception::IllegalTreeOperation(__FILE__, __LINE__);
			}
		}

	template<class T>
		DefaultTreeCursor<T>::DefaultTreeCursor() : TreeCursor<T>(), parents()
	{

	}

	template<class T>
		DefaultTreeCursor<T>::DefaultTreeCursor(T& tree) : TreeCursor<T>(tree), parents()
	{

	}

	template<class T>
		DefaultTreeCursor<T>::DefaultTreeCursor(DefaultTreeCursor<T> const& cursor) : TreeCursor<T>(cursor), parents()
	{

	}

	template<class T>
		DefaultTreeCursor<T>& DefaultTreeCursor<T>::operator=(DefaultTreeCursor<T> const& cursor)
		{
			if (this == &cursor)
			{
				return *this;
			}
			TreeCursor<T>::operator=(cursor);
			parents = cursor.parents;
			return *this;
		}

	template<class T>
		T& DefaultTreeCursor<T>::getParent()
		{
			if (parents.empty())
			{
				throw Exception::IllegalTreeOperation(__FILE__, __LINE__);
			} else
			{
				return *(parents.top());
			}
		}

	template<class T>
		void DefaultTreeCursor<T>::gotoParent()
		{
			if (!TreeCursor<T>::isValid() || parents.empty())
			{
				throw Exception::IllegalTreeOperation(__FILE__, __LINE__);
			} else
			{
				TreeCursor<T>::vertex = parents.top();
				parents.pop();
				if (parents.empty())
				{
					TreeCursor<T>::begin = typename trait::child_iterator();
					TreeCursor<T>::end = typename trait::child_iterator();
					TreeCursor<T>::iterator = TreeCursor<T>::begin;
					TreeCursor<T>::type = ROOT_VERTEX;
				} else
				{
					T* pnt (parents.top());
					if (pnt == NULL)
					{
						throw Exception::NullPointer(__FILE__, __LINE__);
					}
					TreeCursor<T>::begin = trait::beginChild(*pnt);
					TreeCursor<T>::end = trait::endChild(*pnt);
					TreeCursor<T>::iterator = TreeCursor<T>::begin;
					if (TreeCursor<T>::iterator == TreeCursor<T>::end)
					{
						throw Exception::IllegalTreeOperation(__FILE__, __LINE__);
					}
					while (trait::getPointer(TreeCursor<T>::iterator) != TreeCursor<T>::vertex)
					{
						if (++TreeCursor<T>::iterator == TreeCursor<T>::end)
						{
							throw Exception::IllegalTreeOperation(__FILE__, __LINE__);
						}
					}
					TreeCursor<T>::type = INNER_VERTEX;
				}
			}
		}

	template<class T>
		void DefaultTreeCursor<T>::gotoFirstChild()
		{
			if (!TreeCursor<T>::isValid())
			{
				throw Exception::IllegalTreeOperation(__FILE__, __LINE__);
			}
			parents.push(TreeCursor<T>::vertex);
			TreeCursor<T>::gotoFirstChild();
		}

	template<class T>
		AbstractTraversal<T>::AbstractTraversal() : cursor(), atEnd(true)
	{

	}

	template<class T>
		AbstractTraversal<T>::AbstractTraversal(T& tree) : cursor(tree), atEnd(true)
	{
		atEnd = (cursor.getType() == END_VERTEX);
	}

	template<class T>
		AbstractTraversal<T>::AbstractTraversal(AbstractTraversal<T> const& copy) : cursor(copy.cursor), atEnd(copy.atEnd)
	{

	}

	template<class T>
		AbstractTraversal<T>& AbstractTraversal<T>::operator=(AbstractTraversal<T> const& copy)
		{
			if (this == &copy)
			{
				return *this;
			}
			cursor = copy.cursor;
			atEnd = copy.atEnd;
			return *this;
		}

	template<class T>
		NodeType AbstractTraversal<T>::getVertexType() const
		{
			return atEnd ? END_VERTEX : cursor.getType();
		}

	template<class T>
		bool AbstractTraversal<T>::operator==(AbstractTraversal<T> const& other) const
		{
			return atEnd ? other.atEnd : (cursor == other.cursor);
		}

	template<class T>
		bool AbstractTraversal<T>::operator!=(AbstractTraversal<T> const& other) const
		{
			return !(*this == other);
		}

	template<class T>
		T& AbstractTraversal<T>::operator * ()
		{
			if (!atEnd && cursor.getType() != END_VERTEX)
			{
				return cursor.getVertex();
			} else
			{
				throw Exception::InvalidIterator(__FILE__, __LINE__);
			}
		}

	template<class T>
		PostOrderTraversal<T>::PostOrderTraversal() : AbstractTraversal<T>()
	{

	}

	template<class T>
		PostOrderTraversal<T>::PostOrderTraversal(PostOrderTraversal const& copy) : AbstractTraversal<T>(copy)
	{

	}

	template<class T>
		PostOrderTraversal<T>::PostOrderTraversal(T& tree) : AbstractTraversal<T>(tree)
	{
		while (AbstractTraversal<T>::cursor.hasChild())
		{
			AbstractTraversal<T>::cursor.gotoFirstChild();
		}
	}


	template<class T>
		PostOrderTraversal<T>& PostOrderTraversal<T>::operator++()
		{
			if (AbstractTraversal<T>::atEnd == true)
			{
				throw Exception::OutOfRange(__FILE__, __LINE__);
			} else if (AbstractTraversal<T>::cursor.hasNextSibling())
			{
				AbstractTraversal<T>::cursor.gotoNextSibling();
				while (AbstractTraversal<T>::cursor.hasChild())
				{
					AbstractTraversal<T>::cursor.gotoFirstChild();
				}
			} else
			{
				if (AbstractTraversal<T>::cursor.hasParent())
				{
					AbstractTraversal<T>::cursor.gotoParent();
				} else
				{
					AbstractTraversal<T>::atEnd = true;
				}
			}
			return *this;
		}

	template<class T>
		PostOrderTraversal<T> PostOrderTraversal<T>::operator++(int)
		{
			PostOrderTraversal<T> cpy (*this);
			++this;
			return cpy;
		}

	template<class T>
		PostOrderTraversal<T>& PostOrderTraversal<T>::operator--()
		{
			if (AbstractTraversal<T>::atEnd)
			{
				AbstractTraversal<T>::atEnd = false;
			} else if (AbstractTraversal<T>::cursor.hasChild())
			{
				AbstractTraversal<T>::cursor.gotoFirstChild();
				while (AbstractTraversal<T>::cursor.hasNextSibling()) {
					AbstractTraversal<T>::cursor.gotoNextSibling();
				}
			} else
			{
				if (AbstractTraversal<T>::cursor.hasPreviousSibling())
				{
					AbstractTraversal<T>::cursor.gotoPreviousSibling();
				} else
				{
					do {
						if (AbstractTraversal<T>::cursor.hasParent())
						{
							AbstractTraversal<T>::cursor.gotoParent();
						} else
						{
							AbstractTraversal<T>::cursor = typename TreeTrait<T>::cursor();
							throw Exception::OutOfRange(__FILE__, __LINE__);
						}
					} while (!AbstractTraversal<T>::cursor.hasPreviousSibling());
					AbstractTraversal<T>::cursor.gotoPreviousSibling();
				}
			}
			return *this;
		}

	template<class T>
		PostOrderTraversal<T> PostOrderTraversal<T>::operator--(int)
		{
			PostOrderTraversal<T> cpy (*this);
			--this;
			return cpy;
		}

	template<class T>
		PreOrderTraversal<T>::PreOrderTraversal() : AbstractTraversal<T>()
	{

	}

	template<class T>
		PreOrderTraversal<T>::PreOrderTraversal(PreOrderTraversal const& copy) : AbstractTraversal<T>(copy)
	{

	}

	template<class T>
		PreOrderTraversal<T>::PreOrderTraversal(T& tree) : AbstractTraversal<T>(tree)
	{

	}

	template<class T>
		PreOrderTraversal<T>& PreOrderTraversal<T>::operator++()
		{
			if (AbstractTraversal<T>::atEnd)
			{
				throw Exception::OutOfRange(__FILE__, __LINE__);
			} else if (AbstractTraversal<T>::cursor.hasChild())
			{
				AbstractTraversal<T>::cursor.gotoFirstChild();
			} else if (AbstractTraversal<T>::cursor.hasNextSibling())
			{
				AbstractTraversal<T>::cursor.gotoNextSibling();
			} else
			{
				do
				{
					if (AbstractTraversal<T>::cursor.hasParent())
					{
						AbstractTraversal<T>::cursor.gotoParent();
					} else
					{
						AbstractTraversal<T>::atEnd = true;
						return *this;
					}
				} while (!AbstractTraversal<T>::cursor.hasNextSibling());
				AbstractTraversal<T>::cursor.gotoNextSibling();
			}
			return *this;
		}

	template<class T>
		PreOrderTraversal<T> PreOrderTraversal<T>::operator++(int)
		{
			PreOrderTraversal<T> cpy (*this);
			++this;
			return cpy;
		}

	template<class T>
		PreOrderTraversal<T>& PreOrderTraversal<T>::operator--()
		{
			if (AbstractTraversal<T>::atEnd)
			{
				AbstractTraversal<T>::atEnd = false;
				while (AbstractTraversal<T>::cursor.hasChild())
				{
					AbstractTraversal<T>::cursor.gotoFirstChild();
					while (AbstractTraversal<T>::cursor.hasNextSibling())
					{
						AbstractTraversal<T>::cursor.gotoNextSibling();
					}
				}
			} else if (AbstractTraversal<T>::cursor.hasPreviousSibling())
			{
				AbstractTraversal<T>::cursor.gotoPreviousSibling();
				while (AbstractTraversal<T>::cursor.hasChild())
				{
					AbstractTraversal<T>::cursor.gotoFirstChild();
					while (AbstractTraversal<T>::cursor.hasNextSibling())
					{
						AbstractTraversal<T>::cursor.gotoNextSibling();
					}
				}
			} else
			{
				if (AbstractTraversal<T>::cursor.hasParent())
				{
					AbstractTraversal<T>::cursor.gotoParent();
				} else
				{
					throw Exception::OutOfRange(__FILE__, __LINE__);
				}
			}
			return *this;
		}

	template<class T>
		PreOrderTraversal<T> PreOrderTraversal<T>::operator--(int)
		{
			PreOrderTraversal<T> cpy (*this);
			--this;
			return cpy;
		}

	template<class From, class To, class Functor>
		To postOrderFolding(From& tree, Functor& f)
		{
			typedef TreeTrait<From> trait;
			typedef typename trait::child_iterator iterator;
			typedef typename std::vector<To>::iterator argIterator;
			typedef typename std::vector<To>::const_iterator const argByValueIterator;
			PostOrderTraversal<From> traversal(tree);
			PostOrderTraversal<From> end;
			std::vector<To> returnStack;
			for (; traversal != end; ++traversal)
			{
				NodeType vtype (traversal.getVertexType());
				argIterator begin (returnStack.end());
				argIterator endit (returnStack.end());
				switch (vtype) {
					case LEAF_VERTEX:
						break;
					case INNER_VERTEX:
					case ROOT_VERTEX:
						for (iterator iter(trait::beginChild(*traversal)); iter != trait::endChild(*traversal); ++iter)
						{
							--begin;
						}
						break;
					case END_VERTEX:
					default:
						throw Exception::IllegalTreeOperation(__FILE__, __LINE__);
				}
				To value (f(*traversal, vtype,
							static_cast<argByValueIterator>(begin), static_cast<argByValueIterator>(endit)));
				if (begin != endit)
				{
					returnStack.erase(begin, endit);
				}
				returnStack.push_back(value);
			}
			return returnStack.back();
		}

}

#endif // BALL_DATATYPE_GRAPH_TREETRAITS_H
