#ifndef SHADER_H
#define SHADER_H

#include <GL/glew.h>

#include <QtCore/QHash>
#include <QtOpenGL/QGLShader>

#include <boost/shared_ptr.hpp>


class Property
{
	public:
		/**
		 * @brief Default Constructor
		 *
		 **/
		Property() {}

		/**
		 * @brief Destructor
		 *
		 **/
		virtual ~Property() {}
};

template< typename T >
/**
 * @brief Property which stores data of a given type T.
 **/
class TypedProperty : public Property
{
	public:
		/**
		 * @brief Constructor which stores data <code>data</code> of type <code>T</code>.
		 *
		 * @param data ...
		 **/
		TypedProperty (const T& data)
			: m_data(data) {};

		/**
		 * @brief Getter for the data.
		 *
		 * @return T The data to return.
		 **/
		T getData()
		{
			return m_data;
		}

		/**
		 * @brief Setter for the data.
		 *
		 * @param data The data to be set.
		 * @return void
		 **/
		void setData(T data)
		{
			m_data = data;
		}
	private:
		T m_data;
};

/**
 * @brief Stores the mapping from uniform name and type on location.
 *
 * Key of the hash is a pair of name and type of the uniform. The value is a pair of the location
 * of the uniform and it's value.
 **/
typedef QHash<QPair<QString, GLenum>, QPair<GLuint, boost::shared_ptr<Property> > > UniformDictionary;

/**
 * @brief Stores the mapping from uniform name and type on location.
 *
 * Key of the hash is the name of the attribute. The value is the location of the attribute.
 **/
typedef QHash<QString, GLuint> AttributeDictionary;

/**
 * A singleton that takes care of handling all shaders.
 * It is responsible for compiling and binding shaders.
 * It also returns uniform and attribute locations of the shader programs.
 *
 * Do not forget to set the basepath using initialize() for compiling shaders.
 */
class Shader
{
	public:

		Shader(QGLShaderProgram* shader_program, QString name);

		virtual ~Shader();

		/**
		 * @brief Binds a shader program.
		 *
		 * Binds the shader program.
		 *
		 * @return void
		 *
		 * @note Release the shader program after usage by calling <code>release()</code>.
		 **/
		void bind();

		/**
		 * @brief Releases the active shader program from the current context. This is equivalent to
		 * calling <code>glUseProgram(0)</code>.
		 *
		 * @return void
		 **/
		void release();

		/**
		 * @brief Returns an uniform location.
		 *
		 * Returns the uniform location of <code>uniform_name</code>.
		 *
		 * Returns -1 if the program or the uniform_name does not exist.
		 *
		 * @param uniform_name The name of the uniform.
		 * @return GLuint The uniform location.
		 **/
		GLuint getUniformLocation(const char* uniform_name);

		/**
		 * @brief Returns an attribute location.
		 *
		 * Returns the attribute location of <code>attribute_name</code>.
		 *
		 * Returns -1 if the program does not exist or the attribute_name is not valid.
		 *
		 * @param attribute_name The name of the attribute.
		 * @return GLuint The attribute location.
		 **/
		GLuint getAttributeLocation(const char* attribute_name);

		/**
		 * @brief Set the attribute location for the given program and attribute name.
		 *
		 * If the program is invalid nothing happens.
		 *
		 * @note The uniform_dictionary is removed due to linking the shaders.
		 *
		 * @param location The location to be set.
		 * @param attribute_name The name of the attribute the location should be set for.
		 * @return void
		 **/
		void setAttributeLocation (GLuint location, const char* attribute_name );

		/**
		 * @brief Returns the program id of the shader program.
		 *
		 *
		 * @return GLuint The id of the program.
		 **/
		GLuint getProgramId();

		/**
		 * @brief Querries the uniform location of the the texture <code>texture_name</code> and sets
		 * the texture unit <code>texture_unit</code>.
		 *
		 * @param texture_name The name of the texture unit to be set.
		 * @param texture_unit The texture unit to be set.
		 * @return void
		 **/
		void setTextureUnit(const char* texture_name, int texture_unit);

		/**
		 * @brief Binds a given texture <code>texture_id</code> to a given texturing target
		 * <code>target</code>. The texture unit <code>texture_unit</code> specifies to which unit the
		 * texture should be bound to.
		 *
		 * @param target Target to which the texture is bound to. Must be either GL_TEXTURE_1D,
		 * GL_TEXTURE_2D, GL_TEXTURE_3D, or GL_TEXTURE_1D_ARRAY, GL_TEXTURE_2D_ARRAY,
		 * GL_TEXTURE_RECTANGLE, GL_TEXTURE_CUBE_MAP, GL_TEXTURE_2D_MULTISAMPLE or
		 * GL_TEXTURE_2D_MULTISAMPLE_ARRAY.
		 * @param texture_name Name of the texture (string in shader).
		 * @param texture_id Name of the texture (GLuint in code).
		 * @param texture_unit Texture unit to be bound to.
		 * @return void
		 **/
		void bindTexture(GLenum target, const char* texture_name, GLuint texture_id, int texture_unit);

		/**
		 * @brief Binds a given texture <code>texture_id</code> to the texturing target
		 * <code>GL_TEXTURE_2D</code>. The texture unit <code>texture_unit</code> specifies to which unit
		 * the texture should be bound to.
		 *
		 * @param texture_name Name of the texture (string in shader).
		 * @param texture_id Name of the texture (GLuint in code).
		 * @param texture_unit Texture unit to be bound to.
		 * @return void
		 **/
		void bindTexture2D(const char* texture_name, GLuint texture_id, int texture_unit)
		{
			bindTexture(GL_TEXTURE_2D, texture_name, texture_id, texture_unit);
		}

		/**
		 * @brief Binds a given texture <code>texture_id</code> to the texturing target
		 * <code>GL_TEXTURE_3D</code>. The texture unit <code>texture_unit</code> specifies to which unit
		 * the texture should be bound to.
		 *
		 * @param texture_name Name of the texture (string in shader).
		 * @param texture_id Name of the texture (GLuint in code).
		 * @param texture_unit Texture unit to be bound to.
		 * @return void
		 **/
		void bindTexture3D(const char* texture_name, GLuint texture_id, int texture_unit)
		{
			bindTexture(GL_TEXTURE_3D, texture_name, texture_id, texture_unit);
		}

		/**
		 * @brief Binds a given texture <code>texture_id</code> to the texturing target
		 * <code>GL_TEXTURE_RECTANGLE_ARB</code>. The texture unit <code>texture_unit</code> specifies to
		 * which unit the texture should be bound to.
		 *
		 * @param texture_name Name of the texture (string in shader).
		 * @param texture_id Name of the texture (GLuint in code).
		 * @param texture_unit Texture unit to be bound to.
		 * @return void
		 **/
		void bindTextureRECT(const char* texture_name, GLuint texture_id, int texture_unit)
		{
			bindTexture(GL_TEXTURE_RECTANGLE_ARB, texture_name, texture_id, texture_unit);
		}

		/**
		 * @brief Release the texture <code>texture_unit</code>
		 *
		 * @param target Target to which the texture is bound to. Must be either GL_TEXTURE_1D,
		 * GL_TEXTURE_2D, GL_TEXTURE_3D, or GL_TEXTURE_1D_ARRAY, GL_TEXTURE_2D_ARRAY,
		 * GL_TEXTURE_RECTANGLE, GL_TEXTURE_CUBE_MAP, GL_TEXTURE_2D_MULTISAMPLE or
		 * GL_TEXTURE_2D_MULTISAMPLE_ARRAY.
		 * @param texture_unit Texture unit to release.
		 * @return void
		 **/
		void releaseTexture(GLenum target, int texture_unit);

		/**
		 * @brief Prints the current state of the shader to std::cout.
		 *
		 * @return void
		 **/
		void dump();

		/**
		 * @page setUniform
		 * @brief Set the uniform <code>uniform_name</code> for the shader program.
		 *
		 * If the the uniform <code>uniform_name</code> does not exist for the shader nothing happens.
		 * Otherwise, if the the uniform's value is different from the previously set value, the uniform
		 * is set the given value.
		 *
		 * @param program Name of the program to set the uniform for.
		 * @param uniform_name Name of the uniform to be set.
		 * @param v1 Value of the uniform to be set.
		 * @return void
		 **/
		virtual void setUniform1b(QString uniform_name, bool v1);

		/**
		 * @ref setUniform
		 **/
		virtual void setUniform2b(QString uniform_name, bool v1, bool v2);

		/**
		 * @ref setUniform
		 **/
		virtual void setUniform3b(QString uniform_name, bool v1, bool v2, bool v3);

		/**
		 * @ref setUniform
		 **/
		virtual void setUniform4b(QString uniform_name, bool v1, bool v2, bool v3, bool v4);

		/**
		 * @ref setUniform
		 **/
		virtual void setUniform1i(QString uniform_name, int v1);

		/**
		 * @ref setUniform
		 **/
		virtual void setUniform2i(QString uniform_name, int v1, int v2);

		/**
		 * @ref setUniform
		 **/
		virtual void setUniform3i(QString uniform_name, int v1, int v2, int v3);

		/**
		 * @ref setUniform
		 **/
		virtual void setUniform4i(QString uniform_name, int v1, int v2, int v3, int v4);

		/**
		 * @ref setUniform
		 **/
		virtual void setUniform1f(QString uniform_name, float v1);

		/**
		 * @ref setUniform
		 **/
		virtual void setUniform2f(QString uniform_name, float v1, float v2);

		/**
		 * @ref setUniform
		 **/
		virtual void setUniform3f(QString uniform_name, float v1, float v2, float v3);

		/**
		 * @ref setUniform
		 **/
		virtual void setUniform4f(QString uniform_name, float v1, float v2, float v3, float v4);

		/**
		 * @ref setUniform
		 **/
		virtual void setUniform4fv(QString uniform_name, int count, float* v1);

		/**
		 * @ref setUniform
		 **/
		virtual void setUniformMatrix4fv(QString uniform_name, float* v1);

		//TODO add the other setters

	protected:

		template< typename T >
		bool uniformNeedsUpdate_(QPair< QString, GLenum > key, T data);

		//avoid creating objects using the copy constructor.
		Shader(const Shader&);

		//avoid creating objects using the assignment operator.
		const Shader& operator=(const Shader&);

		/**
		 * @brief Create uniform dictionary and add it to map of uniform dictionaries.
		 *
		 * If the uniform dictionary  already exists, it is cleared and then
		 * refilled.
		 *
		 **/
		void createUniformDictionary_();

		/**
		 * @brief Returns a uniform dictionary of all active uniforms.
		 *
		 * @return void
		 **/
		UniformDictionary* getUniformDictionary_();

		/**
		 * @brief Stores the linked shader program.
		 **/
		QGLShaderProgram* shader_program_;

		/**
		 * @brief Mapping from name of shader to uniform dictionary.
		 *
		 * Do not access this directly. Use the getUniformDictionary_ function.
		 **/
		UniformDictionary* uniform_dictionary_;

		/**
		 * @brief Mapping from name of shader to attribute dictionary.
		 **/
		AttributeDictionary* attribute_dictionary_;

		/**
		 * @brief True, if uniform_dictionary needs to be refreshed due to relinking the shaders.
		 **/
		bool needs_refesh_;

		QString name_;
};

#endif //SHADER_H