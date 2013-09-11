#include <ShaderManager.h>
#include <Global.h>
#include <BALL/COMMON/logStream.h>

#include <QtCore/QDir>

#include <string>

#define REPORT_UNIFORM_ERRORS

ShaderManager* ShaderManager::shadermanager_;


ShaderManager::ShaderManager()
{

}

ShaderManager::~ShaderManager()
{
	cleanup_();
}

ShaderManager& ShaderManager::instance()
{
	if (!shadermanager_)
	{
		shadermanager_ = new ShaderManager();
	}

	return *shadermanager_;
}


void ShaderManager::initialize(const char* shader_path)
{
	setBasePath_(shader_path);
}

void ShaderManager::createShaderProgram ( const char* name, std::vector<const char*> shader_list, bool recompile)
{
	if(!recompile && shader_programs_.contains(name))
	{
		return;
	}

	QGLShaderProgram* sp = new QGLShaderProgram;
	std::cout << "[ShaderManager] Create Program for " << name << std::endl;

	for( std::vector< const char* >::iterator it=shader_list.begin(); it < shader_list.end(); it++)
	{
		if(recompile || !shaders_.contains(basepath_ + *it))
		{
			compileSingleShader_(*it);
		}

		sp->addShader(shaders_[basepath_ + *it]);
	}


	if(!sp->link())
	{
		std::cerr << "[ShaderManager] ERROR while linking shaders for " << name << ": " << sp->log().toStdString() << std::endl;
		return;
	}

	Shader* shader = new Shader(sp, name);
	shader_programs_.insert(name, shader);
}

void ShaderManager::release ( )
{
	if(shader_programs_.begin() != shader_programs_.end())
	{
		(*&*shader_programs_.begin())->release();
	}
}

bool ShaderManager::deleteShaderProgram ( const char* name )
{
	if(shader_programs_.contains(name))
	{
		delete shader_programs_[name];
	}

	return shader_programs_.remove(name);
}

void ShaderManager::reloadShaderProgram ( const char* name, std::vector< const char* > shader_list )
{
	deleteShaderProgram(name);
	createShaderProgram(name, shader_list, true);
}

int ShaderManager::getShaderProgramsCount()
{
	return shader_programs_.count();
}

void ShaderManager::cleanup_()
{
	if(shadermanager_)
	{
		//delete the compiled shaders
		QGLShader* shader;
		foreach(shader, shaders_.values())
		{
			delete shader;
		}

		//be sure that no shader is active anymore
		release();

		//delete the linked shader programs
		Shader* shader_program;
		foreach(shader_program, shader_programs_.values())
		{
			delete shader_program;
		}
		shadermanager_ = NULL;
	}
}

void ShaderManager::setBasePath_ ( const char* path )
{
	std::cout << "[ShaderManager] Set basedir to " << path << std::endl;
	basepath_ = QString(path);
}


void ShaderManager::compileSingleShader_(const QString& path)
{
	QFile file(basepath_ + path);

	if(!file.exists())
	{
		std::cerr << "[ShaderManager] ERROR: Could not open file " << file.fileName().toStdString() << std::endl;
		return;
	}

	QFileInfo finfo(file);

	if(finfo.completeSuffix() == "glsl")
	{
		std::cout << "[ShaderManager] Compiling shader " << finfo.absoluteFilePath().toStdString() << "\n";

		QGLShader* shader = new QGLShader(getShaderType_(finfo.baseName()));

		if(!shader->compileSourceFile(finfo.absoluteFilePath()))
		{
			std::cerr << "[ShaderManager] ERROR: Error while compiling shader: " << shader->log().toStdString() << std::endl;
			exit (0);
		}

		shaders_.insert(finfo.absoluteFilePath(), shader);
	}
}

QGLShader::ShaderType ShaderManager::getShaderType_ ( const QString& filename )
{
	if(filename.endsWith("fragment"))
	{
		return QGLShader::Fragment;
	}
	else if(filename.endsWith("vertex"))
	{
		return QGLShader::Vertex;
	}

	return 0;
}


void ShaderManager::dump()
{
	std::cout << "[ShaderManager]: dump:" << std::endl;
	std::cout << "\t basepath: " << basepath_.toStdString() << std::endl;
	std::cout << "\t shaders: " << std::endl;

	QHashIterator<QString, QGLShader *> is(shaders_);

	while (is.hasNext())
	{
		is.next();
		std::cout << "\t\t" << is.key().toStdString() << ": " << is.value();
	}

	std::cout << "\t shader_programs: " << std::endl;

	QHashIterator<QString, Shader *> isp(shader_programs_);

	while (isp.hasNext())
	{
		isp.next();
		std::cout << "\t\t" << isp.key().toStdString() << ": " << isp.value();
	}

}
