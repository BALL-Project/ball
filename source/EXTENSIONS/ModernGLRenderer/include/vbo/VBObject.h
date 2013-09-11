#ifndef VBOBJECT_H
#define VBOBJECT_H

#include <map>
#include <vector>

/**
 * @brief Stores three indices of the vertices forming a triangle.
 **/
struct Index
{
	/** First index. **/
	unsigned int a;

	/** Second index. **/
	unsigned int b;

	/** Third index. **/
	unsigned int c;
};

/**
 * @brief Stores three indices of the vertices forming a quad.
 **/
struct QuadIndex
{
	/** First index. **/
	unsigned int a;

	/** Second index. **/
	unsigned int b;

	/** Third index. **/
	unsigned int c;

	/** Forth index. **/
	unsigned int d;
};

/**
 * @brief Structure for an attribute with one element.
 **/
struct Attrib1
{
	/** First element of the attribute. **/
	float a;
};

/**
 * @brief Structure for an attribute with two elements.
 **/
struct Attrib2
{
	/** First element of the attribute. **/
	float a;
	/** Second element of the attribute. **/
	float b;
};

/**
 * @brief Structure for an attribute with three elements.
 **/
struct Attrib3
{
	/** First element of the attribute. **/
	float a;
	/** Second element of the attribute. **/
	float b;
	/** Third element of the attribute. **/
	float c;
};

/**
 * @brief Structure for an attribute with four elements.
 **/
struct Attrib4
{
	/** First element of the attribute. **/
	float a;
	/** Second element of the attribute. **/
	float b;
	/** Third element of the attribute. **/
	float c;
	/** Fourth element of the attribute. **/
	float d;
};

/**
 * @brief Structure for storing a buffer.
 **/
struct Buffer
{
	/** OpenGLs id of the buffer. **/
	GLuint buffer_id;
	/** Size of the buffer. **/
	size_t size;
	/** Number of elements per attribute. **/
	unsigned int elements;
};

/**
 * @brief Stores and manages vertex buffer objects (VBO).
 *
 * This class is used to make the rendering process of different structures consistent.
 *
 * The basic workflow is the following:
 * <ul>
 *   <li>Set number of elements to be rendered (e.g. number of the triangle times 3)
 *       <code>setNumberOfElements()</li>
 *   <li>Generate buffers by setting attributes with <code>setAttributeXf()</code> where X is
 *       [1,2,3,4].</li>
 *   <li>Generate an index buffer (if necessary) with <code>setIndices()</code>.</li>
 *   <li>Call <code>render()</code> for drawing the elements.</li>
 * </ul>
 *
 * @note To use this class one needs to derive from it and implement the render function.
 * @note Assignment and Copy constructor are private to avoid data races on the gpu.
 **/
class VBObject
{
	public:

		/**
		 * @brief Default constructor.
		 *
		 * Performs a setup.
		 *
		 **/
		VBObject();

		/**
		 * @brief Destructor
		 *
		 * Calls the cleanup_ method to delete buffers.
		 * After calling the destructor the initialized_ flag is set to false.
		 *
		 **/
		virtual ~VBObject();

		/**
		 * @brief Starting point for rendering.
		 * This method should be called whenever the window needs to be redrawn. It starts the rendering
		 * process of the VBO.
		 *
		 * It binds the index buffer (when used), enables all set attributes, calls an OpenGL render
		 * function and disables the attributes again.
		 *
		 * @return void
		 **/
		virtual void render() = 0;

		/**
		 * @brief Create a buffer with 4 elements per attribute.
		 *
		 * The generated buffer with its data will be transfered to the GPU.
		 *
		 * @param aid Attribute ID to be used for creating the buffer.
		 * @param attrib Data for this attribute buffer.
		 * @return void
		 * @note Previously set attribute data with the same <code>aid</code> will not be accessible
		 * anymore when calling this method twice.
		 **/
		virtual void setAttribute4f(GLuint aid, const std::vector<Attrib4>& attrib);

		/**
		 * @brief Create a buffer with 3 elements per attribute.
		 *
		 * The generated buffer with its data will be transfered to the GPU.
		 *
		 * @param aid Attribute ID to be used for creating the buffer.
		 * @param attrib Data for this attribute buffer.
		 * @return void
		 * @note Previously set attribute data with the same <code>aid</code> will not be accessible
		 * anymore when calling this method twice.
		 **/
		virtual void setAttribute3f(GLuint aid, const std::vector<Attrib3>& attrib);

		/**
		 * @brief Create a buffer with 2 elements per attribute.
		 *
		 * The generated buffer with its data will be transfered to the GPU.
		 *
		 * @param aid Attribute ID to be used for creating the buffer.
		 * @param attrib Data for this attribute buffer.
		 * @return void
		 * @note Previously set attribute data with the same <code>aid</code> will not be accessible
		 * anymore when calling this method twice.
		 **/
		virtual void setAttribute2f(GLuint aid, const std::vector<Attrib2>& attrib);

		/**
		 * @brief Create a buffer with 1 elements per attribute.
		 *
		 * The generated buffer with its data will be transfered to the GPU.
		 *
		 * @param aid Attribute ID to be used for creating the buffer.
		 * @param attrib Data for this attribute buffer.
		 * @return void
		 * @note Previously set attribute data with the same <code>aid</code> will not be accessible
		 * anymore when calling this method twice.
		 **/
		virtual void setAttribute1f(GLuint aid, const std::vector<Attrib1>& attrib);

		/**
		 * @brief Generates an index buffer (3 elements per entry).
		 *
		 * The generated buffer with its data will be transfered to the GPU.
		 *
		 * @param indices Vector of indices to be set.
		 * @return void
		 * @note Previously set indices will not be accessible anymore when calling this method twice.
		 **/
		virtual void setIndices(const std::vector<Index>& indices);

		/**
		 * @brief Generates an index buffer (4 elements per entry).
		 *
		 * The generated buffer with its data will be transfered to the GPU.
		 *
		 * @param indices Vector of indices to be set.
		 * @return void
		 * @note Previously set indices will not be accessible anymore when calling this method twice.
		 **/
		virtual void setIndices(const std::vector<QuadIndex>& indices);

		/**
		 * @brief Binds the index buffer.
		 *
		 * @return void
		 **/
		virtual void bindIndices();

		/**
		 * @brief Enables the given attribute.
		 *
		 * @param aid Attribute ID to be enabled.
		 * @return void
		 **/
		void enableAttribute(GLuint aid);

		/**
		 * @brief Disables the given attribute.
		 *
		 * @param aid Attribute ID to be disabled.
		 * @return void
		 **/
		void disableAttribute(GLuint aid);

		/**
		 * @brief Setup the VBO.
		 *
		 * Perfomes an cleanup and calles the initialize methode.
		 * @return void
		 **/
		void setup ();

		/**
		 * @brief Set the number of elements to be rendered.
		 *
		 * @param number Number of elements to be rendered.
		 * @return void
		 **/
		virtual void setNumberOfElements(unsigned int number);

	private:
		/**
		 * @brief Copy constructor.
		 *
		 **/
		VBObject(const VBObject& vb_object);

		/**
		 * @brief Assignment operator.
		 *
		 * @param vb_object
		 * @return VBObject& Returns this.
		 **/
		VBObject& operator = (const VBObject& vb_object);

	protected:

		/**
		 * @brief Initializes the VBO.
		 *
		 * Only initializes the system when the initialized_ flag is not already set to true.
		 * It performs the OpenGL generation calls for nedded buffers (vertex and index buffer objects).
		 * The initialized_ flag is set to true.
		 *
		 * @return void
		 **/
		void initialize_();

		/**
		 * @brief Performs an cleanup.
		 *
		 * Deletes buffers.
		 * After calling cleanup_() the initialized_ flag is set to false.
		 *
		 * @return void
		 **/
		void cleanup_();

		/**
		 * @brief Creates a buffer object.
		 *
		 * @param size Size (total number of elements) of the buffer to be created.
		 * @param elements Number of elements per attribute.
		 * @return Buffer The generated buffer.
		 **/
		Buffer createBuffer(size_t size, unsigned int elements);

		/**
		 * @brief Is the VBObject initialized?
		 *
		 * Is set to true if VBObject is initialized.
		 **/
		bool initialized_;

		/**
		 * @brief Maps an ID onto a buffer object.
		 **/
		std::map<GLuint, Buffer> buffers_;

		/**
		 * @brief Buffer object for storing the indices.
		 **/
		Buffer index_buffer_;


		/**
		 * @brief Number of elements used to render.
		 *
		 **/
		unsigned int n_elements_;
};

#endif //VBOBJECT_H
