#include <GL/glew.h>

#include <vbo/VBObject.h>


VBObject::VBObject()
	: initialized_(false)
{
	setup();
}

VBObject::~VBObject()
{
	cleanup_();
}

void VBObject::initialize_()
{
	if(initialized_)
	{
		return;
	}

	initialized_ = true;
}

void VBObject::cleanup_()
{
	if(!initialized_)
	{
		return;
	}

	for(  std::map< GLuint, Buffer >::const_iterator iter = buffers_.begin(); iter != buffers_.end(); iter++)
	{
		glDeleteBuffers(1, &iter->second.buffer_id);
	}

	buffers_.clear();

	initialized_ = false;
}

void VBObject::setup ()
{
	cleanup_();
	initialize_();
}

Buffer VBObject::createBuffer ( size_t size, unsigned int elements )
{
	GLuint buffer_id;
	glGenBuffers(1, &buffer_id);

	Buffer buffer;
	buffer.buffer_id = buffer_id;
	buffer.size = size;
	buffer.elements = elements;

	return buffer;
}


void VBObject::setAttribute4f ( GLuint aid, const std::vector< Attrib4 >& attrib )
{

	Buffer buffer = createBuffer(sizeof(Attrib4), 4);

	glBindBuffer(GL_ARRAY_BUFFER, buffer.buffer_id);
	glBufferData(GL_ARRAY_BUFFER, buffer.size * attrib.size(), &attrib[0], GL_STATIC_DRAW);

	buffers_.insert(std::pair<GLuint, Buffer>(aid, buffer));
}

void VBObject::setAttribute3f ( GLuint aid, const std::vector< Attrib3 >& attrib )
{
	Buffer buffer = createBuffer(sizeof(Attrib3), 3);

	glBindBuffer(GL_ARRAY_BUFFER, buffer.buffer_id);
	glBufferData(GL_ARRAY_BUFFER, buffer.size * attrib.size(), &attrib[0], GL_STATIC_DRAW);

	buffers_.insert(std::pair<GLuint, Buffer>(aid, buffer));
}

void VBObject::setAttribute2f ( GLuint aid, const std::vector< Attrib2 >& attrib )
{
	Buffer buffer = createBuffer(sizeof(Attrib2), 2);

	glBindBuffer(GL_ARRAY_BUFFER, buffer.buffer_id);
	glBufferData(GL_ARRAY_BUFFER, buffer.size * attrib.size(), &attrib[0], GL_STATIC_DRAW);

	buffers_.insert(std::pair<GLuint, Buffer>(aid, buffer));
}

void VBObject::setAttribute1f ( GLuint aid, const std::vector< Attrib1 >& attrib )
{
	Buffer buffer = createBuffer(sizeof(Attrib1), 1);

	glBindBuffer(GL_ARRAY_BUFFER, buffer.buffer_id);
	glBufferData(GL_ARRAY_BUFFER, buffer.size * attrib.size(), &attrib[0], GL_STATIC_DRAW);

	buffers_.insert(std::pair<GLuint, Buffer>(aid, buffer));
}

void VBObject::setIndices ( const std::vector< Index >& indices )
{
	index_buffer_ = createBuffer(sizeof(Index), 3);

	bindIndices();
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, index_buffer_.size * indices.size(), &indices[0], GL_STATIC_DRAW);
}

void VBObject::setIndices ( const std::vector< QuadIndex >& indices )
{
	index_buffer_ = createBuffer(sizeof(QuadIndex), 4);

	bindIndices();
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, index_buffer_.size * indices.size(), &indices[0], GL_STATIC_DRAW);
}

void VBObject::bindIndices()
{
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, index_buffer_.buffer_id);
}

void VBObject::enableAttribute ( GLuint aid )
{
	glEnableVertexAttribArray(aid);
	Buffer b = buffers_.find(aid)->second;
	glBindBuffer(GL_ARRAY_BUFFER, b.buffer_id);
	glVertexAttribPointer(aid, b.elements, GL_FLOAT, GL_FALSE, b.size, 0);
}

void VBObject::disableAttribute ( GLuint aid )
{
	glDisableVertexAttribArray(aid);
}

void VBObject::setNumberOfElements ( unsigned int number )
{
	n_elements_ = number;
}

void VBObject::render()
{
	if(n_elements_ == 0) {
		return;
	}

	bindIndices();

	for(std::map< GLuint, Buffer >::const_iterator iter = buffers_.begin(); iter != buffers_.end(); iter++)
	{
		enableAttribute(iter->first);
	}

	render_();

	for(std::map< GLuint, Buffer >::const_iterator iter = buffers_.begin(); iter != buffers_.end(); iter++)
	{
		disableAttribute(iter->first);
	}
}

