#ifndef SHADERMANAGER_H
#define SHADERMANAGER_H

#include <shader.h>

#include <QtCore/QHash>
#include <QtOpenGL/QGLShader>


/**
 * A singleton that takes care of handling all shaders.
 * It is responsible for compiling and binding shaders.
 * It also returns uniform and attribute locations of the shader programs.
 *
 * Do not forget to set the basepath using initialize() for compiling shaders.
 */
class ShaderManager
{
	public:

		virtual ~ShaderManager();

		/**
		 * Use this method to obtain the ShaderManager instance.
		 */
		static ShaderManager& instance();

		/**
		 * @brief Initializes the ShaderManager.
		 *
		 * Initializes the ShaderManager using the shader_path.
		 *
		 * @param shader_path Path to the shaders.
		 * @return void
		 **/
		void initialize(const char* shader_path);

		/**
		 * @brief Creates a shader program.
		 *
		 * The shaders in the shader_list are compiled and linked the a shader program called
		 * <code>name</code>. On error the method returns and outputs an error message.
		 * The method only compiles a shader if it has not been compilied earlier. (Except,
		 * <code>recompile</code> is set to true).
		 *
		 * @note The uniform_dictionary is removed due to linking the shaders.
		 *
		 * @param name Name of the shader program. This will be used to identify the shader program.
		 * @param vertex_shader List of relative pathes to the shader files from the basepath.
		 * @param recompile If true, shaders are forced to be recompiled.

		 * @return void
		 **/
		void createShaderProgram(const char* name, std::vector<const char*> shader_list, bool recompile = false);

		/**
		 * @brief Releases the active shader program from the current context. This is equivalent to
		 * calling <code>glUseProgram(0)</code>.
		 *
		 * @return void
		 **/
		void release();

		/**
		 * @brief Deletes a shader program.
		 *
		 * Deletes the shader program <code>name</code> if it exists. Returns true, if shader program
		 * exists and could be deleted.
		 *
		 * @param name The name of the shader program to delete.
		 * @return bool True if successful deleted.
		 **/
		bool deleteShaderProgram(const char* name);

		/**
		 * @brief Reloads a shader program.
		 *
		 * @param name The name of the shader to be reloaded.
		 * @param shader_list List of relative pathes to the shader files from the basepath.
		 * @return void
		 **/
		void reloadShaderProgram(const char* name, std::vector<const char*> shader_list);

		/**
		 * @brief Returns the number of stored shader programs.
		 *
		 * @return int Number of stored shader programs.
		 **/
		int getShaderProgramsCount();

		/**
		 * @brief Prints the current state of the shader manager to std::cout.
		 *
		 * @return void
		 **/
		void dump();

		virtual Shader* getShader(const char* program)
		{
			return (*&*shader_programs_.find(program)); //TODO what if shader does not exist?
		}

	protected:

		//avoid object creation from outsite.
		ShaderManager();

		//avoid creating objects using the copy constructor.
		ShaderManager(const ShaderManager&);

		//avoid creating objects using the assignment operator.
		const ShaderManager& operator=(const ShaderManager&);

		/**
		 * @brief Performs an cleanup.
		 *
		 * Shader programs and shaders are deleted.
		 *
		 * @return void
		 **/
		void cleanup_();

		/**
		 * @brief Sets the basepath.
		 *
		 * @param basepath The basepath to be set.
		 * @return void
		 **/
		void setBasePath_(const char* basepath);

		/**
		 * @brief Compile a single shader.
		 *
		 * The pointer the the shader is stored in shaders_ with its absolute path as key.
		 *
		 * @param path relative path to shader file from basepath.
		 * @return void
		 **/
		void compileSingleShader_(const QString& path);

		/**
		 * @brief Returns the type of the shader name based on the file name.
		 *
		 * Files ending with vertex are vertex shader, files ending with fragment are fragment shader.
		 *
		 * @param filename The filename of the shadertype to be determined.
		 * @return :ShaderType Type of the shader.
		 **/
		QGLShader::ShaderType getShaderType_(const QString& filename);

		/**
		 * @brief Stores the linked shader programs.
		 **/
		QHash<QString, Shader*> shader_programs_;

		/**
		 * @brief Stores the compiled shaders.
		 **/
		QHash<QString, QGLShader*> shaders_;

		/**
		 * @brief Path the shaders.
		 **/
		QString basepath_;

		/**
		 * @brief The shadermanager instance. It is NULL if it is not instanced.
		 **/
		static ShaderManager* shadermanager_;
};

#endif //SHADERMANAGER_H