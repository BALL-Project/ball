#include <Shader.h>
#include <Global.h>
#include <BALL/COMMON/logStream.h>

#include <QtCore/QDir>

#include <string>

#define REPORT_UNIFORM_ERRORS

Shader::Shader(QGLShaderProgram* shader_program, QString name)
	: shader_program_(0),
	  uniform_dictionary_(0),
	  attribute_dictionary_(0),
	  needs_refesh_(true),
	  name_(name)
{
	shader_program_ = shader_program;
	attribute_dictionary_ = new AttributeDictionary;
	uniform_dictionary_ = new UniformDictionary;
}

Shader::~Shader()
{
	delete uniform_dictionary_;
	delete attribute_dictionary_;
	delete shader_program_;
}

void Shader::bind ()
{
	shader_program_->bind();
}

void Shader::release ( )
{
	shader_program_->release();
}


GLuint Shader::getAttributeLocation (const char* attribute_name )
{
	if(attribute_dictionary_->contains(attribute_name))
	{
		return *&*attribute_dictionary_->find(attribute_name);
	}
	else
	{
		return -1;
	}
}

void Shader::setAttributeLocation (GLuint location, const char* attribute_name )
{
	if(attribute_dictionary_->contains(attribute_name))
	{
		GLuint l = *&*(attribute_dictionary_->find(attribute_name));

		if(l == location)
		{
			//every thing up to date!!
			return;
		}
	}

	shader_program_->bindAttributeLocation(attribute_name, location);
	shader_program_->link();
	needs_refesh_ = true;
	attribute_dictionary_->insert(attribute_name, location);
}

GLuint Shader::getProgramId()
{
	return shader_program_->programId();
}

UniformDictionary* Shader::getUniformDictionary_()
{
	if(needs_refesh_)
	{
		createUniformDictionary_();
	}

	return uniform_dictionary_;
}

GLuint Shader::getUniformLocation ( const char* uniform_name )
{
	return shader_program_->uniformLocation(uniform_name);
	//TODO read it from the uniform_dictionary_
}

void Shader::setTextureUnit(const char* texture_name, int texture_unit)
{
	GLint linked;
	glGetProgramiv(getProgramId(), GL_LINK_STATUS, &linked);

	if (linked != GL_TRUE)
	{
		std::cerr << "[Shader] Info: Try to get uniform location for " << texture_name << std::endl;
		std::cerr << "[Shader] Error: setTextureUnit needs program to be linked - " << name_.toStdString() << " is not linked." << std::endl;
		exit(1);
	}

	GLuint tex_idx  = getUniformLocation(texture_name);

	if( (int) tex_idx == -1)
	{
#ifdef REPORT_UNIFORM_ERRORS
		std::cerr << "[Shader]: Warning: Invalid texture "  << texture_name << " in shader " << name_.toStdString() << std::endl;
#endif
	}

	glUniform1i(tex_idx, texture_unit);
}

void Shader::bindTexture( GLenum target, const char* texture_name, GLuint texture_id, int texture_unit)
{
	glActiveTexture(GL_TEXTURE0 + texture_unit);
	glBindTexture(target, texture_id);
	setTextureUnit(texture_name, texture_unit);
	glActiveTexture(GL_TEXTURE0);
}

void Shader::releaseTexture(GLenum target, int texture_unit)
{
	glActiveTexture(GL_TEXTURE0 + texture_unit);
	glBindTexture(target, 0);
}

void Shader::createUniformDictionary_()
{
	GLuint program_id = getProgramId();
	int n_uniforms = -1;

	uniform_dictionary_->clear();

	glGetProgramiv(program_id, GL_ACTIVE_UNIFORMS, &n_uniforms);

	for(int i = 0; i < n_uniforms; i++)
	{
		int name_len=-1, num=-1;
		GLenum type = GL_ZERO;
		char name[100];
		glGetActiveUniform( program_id, GLuint(i), sizeof(name)-1, &name_len, &num, &type, name );
		name[name_len] = 0;
		const GLuint location = glGetUniformLocation( program_id, name );
		boost::shared_ptr<Property> n = boost::shared_ptr<Property>();
		uniform_dictionary_->insert(QPair<QString, GLenum>(name, type), QPair<GLuint, boost::shared_ptr<Property> >(location, n));
	}

	needs_refesh_ = false;
}

void Shader::dump()
{
	std::cout << "[Shader]: dump:" << std::endl;
	std::cout << "\t shader name: " << name_.toStdString() << std::endl;
	std::cout << "\t needs_refesh_: " << needs_refesh_ << std::endl;

	//TODO print uniform and attribute dictionaries
}

void Shader::setUniform1b(QString uniform_name, bool v1)
{
	UniformDictionary* ud = getUniformDictionary_();
	QPair< QString, GLenum > key = QPair<QString, GLenum>(uniform_name, GL_BOOL);

	if (ud->contains(key))
	{
		GLuint uniform_id = ud->find(key).value().first;

		if(uniformNeedsUpdate_<bool>(key, v1))
		{
			bind();
			glUniform1i(uniform_id, v1);
		}
	}
}

void Shader::setUniform2b(QString uniform_name, bool v1, bool v2)
{
	QVector<bool> vec;
	vec.push_back(v1);
	vec.push_back(v2);
	UniformDictionary* ud = getUniformDictionary_();
	QPair< QString, GLenum > key = QPair<QString, GLenum>(uniform_name, GL_BOOL_VEC2);

	if (ud->contains(key))
	{
		GLuint uniform_id = ud->find(key).value().first;

		if(uniformNeedsUpdate_<QVector<bool> >(key, vec))
		{
			bind();
			glUniform2i(uniform_id, v1, v2);
		}
	}
}

void Shader::setUniform3b(QString uniform_name, bool v1, bool v2, bool v3)
{
	QVector<bool> vec;
	vec.push_back(v1);
	vec.push_back(v2);
	vec.push_back(v3);
	UniformDictionary* ud = getUniformDictionary_();
	QPair< QString, GLenum > key = QPair<QString, GLenum>(uniform_name, GL_BOOL_VEC3);

	if (ud->contains(key))
	{
		GLuint uniform_id = ud->find(key).value().first;

		if(uniformNeedsUpdate_<QVector<bool> >(key, vec))
		{
			bind();
			glUniform3i(uniform_id, v1, v2, v3);
		}
	}
}

void Shader::setUniform4b(QString uniform_name, bool v1, bool v2, bool v3, bool v4)
{
	QVector<bool> vec;
	vec.push_back(v1);
	vec.push_back(v2);
	vec.push_back(v3);
	vec.push_back(v4);
	UniformDictionary* ud = getUniformDictionary_();
	QPair< QString, GLenum > key = QPair<QString, GLenum>(uniform_name, GL_BOOL_VEC4);

	if (ud->contains(key))
	{
		GLuint uniform_id = ud->find(key).value().first;

		if(uniformNeedsUpdate_<QVector<bool> >(key, vec))
		{
			bind();
			glUniform4i(uniform_id, v1, v2, v3, v4);
		}
	}
}

void Shader::setUniform1i(QString uniform_name, int v1)
{
	UniformDictionary* ud = getUniformDictionary_();
	QPair< QString, GLenum > key = QPair<QString, GLenum>(uniform_name, GL_INT);

	if (ud->contains(key))
	{
		GLuint uniform_id = ud->find(key).value().first;

		if(uniformNeedsUpdate_<int>(key, v1))
		{
			bind();
			glUniform1i(uniform_id, v1);
		}
	}
}

void Shader::setUniform2i(QString uniform_name, int v1, int v2)
{
	QVector<int> vec;
	vec.push_back(v1);
	vec.push_back(v2);
	UniformDictionary* ud = getUniformDictionary_();
	QPair< QString, GLenum > key = QPair<QString, GLenum>(uniform_name, GL_INT_VEC2);

	if (ud->contains(key))
	{
		GLuint uniform_id = ud->find(key).value().first;

		if(uniformNeedsUpdate_<QVector<int> >(key, vec))
		{
			bind();
			glUniform2i(uniform_id, v1, v2);
		}
	}
}

void Shader::setUniform3i(QString uniform_name, int v1, int v2, int v3)
{
	QVector<int> vec;
	vec.push_back(v1);
	vec.push_back(v2);
	vec.push_back(v3);
	UniformDictionary* ud = getUniformDictionary_();
	QPair< QString, GLenum > key = QPair<QString, GLenum>(uniform_name, GL_INT_VEC3);

	if (ud->contains(key))
	{
		GLuint uniform_id = ud->find(key).value().first;

		if(uniformNeedsUpdate_<QVector<int> >(key, vec))
		{
			bind();
			glUniform3i(uniform_id, v1, v2, v3);
		}
	}
}

void Shader::setUniform4i(QString uniform_name, int v1, int v2, int v3, int v4)
{
	QVector<int> vec;
	vec.push_back(v1);
	vec.push_back(v2);
	vec.push_back(v3);
	vec.push_back(v4);
	UniformDictionary* ud = getUniformDictionary_();
	QPair< QString, GLenum > key = QPair<QString, GLenum>(uniform_name, GL_INT_VEC4);

	if (ud->contains(key))
	{
		GLuint uniform_id = ud->find(key).value().first;

		if(uniformNeedsUpdate_<QVector<int> >(key, vec))
		{
			bind();
			glUniform4i(uniform_id, v1, v2, v3, v4);
		}
	}
}

void Shader::setUniform1f(QString uniform_name, float v1)
{
	UniformDictionary* ud = getUniformDictionary_();
	QPair< QString, GLenum > key = QPair<QString, GLenum>(uniform_name, GL_FLOAT);

	if (ud->contains(key))
	{
		GLuint uniform_id = ud->find(key).value().first;

		if(uniformNeedsUpdate_<float>(key, v1))
		{
			bind();
			glUniform1f(uniform_id, v1);
		}
	}
}

void Shader::setUniform2f(QString uniform_name, float v1, float v2)
{
	QVector<float> vec;
	vec.push_back(v1);
	vec.push_back(v2);
	UniformDictionary* ud = getUniformDictionary_();
	QPair< QString, GLenum > key = QPair<QString, GLenum>(uniform_name, GL_FLOAT_VEC2);

	if (ud->contains(key))
	{
		GLuint uniform_id = ud->find(key).value().first;

		if(uniformNeedsUpdate_<QVector<float> >(key, vec))
		{
			bind();
			glUniform2f(uniform_id, v1, v2);
		}
	}
}

void Shader::setUniform3f(QString uniform_name, float v1, float v2, float v3)
{
	QVector<float> vec;
	vec.push_back(v1);
	vec.push_back(v2);
	vec.push_back(v3);
	UniformDictionary* ud = getUniformDictionary_();
	QPair< QString, GLenum > key = QPair<QString, GLenum>(uniform_name, GL_FLOAT_VEC3);

	if (ud->contains(key))
	{
		GLuint uniform_id = ud->find(key).value().first;

		if(uniformNeedsUpdate_<QVector<float> >(key, vec))
		{
			bind();
			glUniform3f(uniform_id, v1, v2, v3);
		}
	}

}

void Shader::setUniform4f(QString uniform_name, float v1, float v2, float v3, float v4)
{
	QVector<float> vec;
	vec.push_back(v1);
	vec.push_back(v2);
	vec.push_back(v3);
	vec.push_back(v4);
	UniformDictionary* ud = getUniformDictionary_();
	QPair< QString, GLenum > key = QPair<QString, GLenum>(uniform_name, GL_FLOAT_VEC4);

	if (ud->contains(key))
	{
		GLuint uniform_id = ud->find(key).value().first;

		if(uniformNeedsUpdate_<QVector<float> >(key, vec))
		{
			bind();
			glUniform4f(uniform_id, v1, v2, v3, v4);
		}
	}
}

void Shader::setUniform4fv(QString uniform_name, int count, float* v1)
{
	QVector<float> vec;

	for(int i = 0; i < count*4; i++)
	{
		vec.push_back(v1[i]);
	}

	UniformDictionary* ud = getUniformDictionary_();
	QPair< QString, GLenum > key = QPair<QString, GLenum>(uniform_name, GL_FLOAT_VEC4);

	if (ud->contains(key))
	{
		GLuint uniform_id = ud->find(key).value().first;

		if(uniformNeedsUpdate_<QVector<float> >(key, vec))
		{
			bind();
			glUniform4fv(uniform_id, count, v1);
		}
	}
}

void Shader::setUniformMatrix4fv(QString uniform_name, float* v1)
{
	QVector<float> vec;

	for(int i = 0; i < 16; i++)
	{
		vec.push_back(v1[i]);
	}

	UniformDictionary* ud = getUniformDictionary_();
	QPair< QString, GLenum > key = QPair<QString, GLenum>(uniform_name, GL_FLOAT_MAT4);

	if (ud->contains(key))
	{
		GLuint uniform_id = ud->find(key).value().first;

		if(uniformNeedsUpdate_<QVector<float> >(key, vec))
		{
			bind();
			glUniformMatrix4fv(uniform_id, 1, GL_FALSE, v1);
		}
	}
}

template< typename T >
bool Shader::uniformNeedsUpdate_(QPair< QString, GLenum> key, T data)
{
	UniformDictionary* ud = getUniformDictionary_();
	UniformDictionary::iterator udi = ud->find(key);
	boost::shared_ptr<Property> p = udi.value().second;

	if(p)  //is the pointer valid?
	{
		boost::shared_ptr<TypedProperty<T> > tp =  boost::dynamic_pointer_cast<TypedProperty<T > >(p);

		if(tp)
		{
			if(tp->getData() == data)
			{
				//data are already up to date
				return false;
			}
			else
			{
				//set the new data
				tp->setData(data);
			}
		}
	}
	else
	{
		//create a new pointer, last one was null
		boost::shared_ptr<Property> new_tp = boost::shared_ptr<TypedProperty<T> >(new TypedProperty<T>(data));
		udi.value().second = boost::dynamic_pointer_cast<TypedProperty<T> >(new_tp);
	}

	return true;
}
