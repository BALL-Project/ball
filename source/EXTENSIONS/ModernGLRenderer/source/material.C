#include <material.h>
#include <renderObjects/renderObject.h>

#include <iostream>


Material* Material::material_;

Material::Material()
	: QObject()
{

}

Material::~Material()
{
}

Material& Material::instance()
{
	if (! material_)
	{
		material_ = new Material();
	}

	return *material_;
}

void Material::setMaterialShininess(float shininess)
{
	m_.shininess = shininess;

	Q_EMIT materialShininessChanged(shininess);
}

void Material::setMaterialDiffuse(float r, float g, float b, float a)
{
	m_.diffuse.r = r;
	m_.diffuse.g = g;
	m_.diffuse.b = b;
	m_.diffuse.a = a;

	Q_EMIT materialDiffuseChanged(r,g,b,a);
}

void Material::setMaterialAmbient(float r, float g, float b, float a)
{
	m_.ambient.r = r;
	m_.ambient.g = g;
	m_.ambient.b = b;
	m_.ambient.a = a;

	Q_EMIT materialAmbientChanged(r, g, b, a);
}

void Material::setMaterialSpecular(float r, float g, float b, float a)
{
	m_.specular.r = r;
	m_.specular.g = g;
	m_.specular.b = b;
	m_.specular.a = a;

	Q_EMIT materialSpecularChanged(r, g, b, a);
}

void Material::registerMaterial(const RenderObject* slot)
{
	connect(this, &Material::materialShininessChanged, slot, &RenderObject::setMaterialShininess);
	connect(this, &Material::materialDiffuseChanged,   slot, &RenderObject::setMaterialDiffuse);
	connect(this, &Material::materialAmbientChanged,   slot, &RenderObject::setMaterialAmbient);
	connect(this, &Material::materialSpecularChanged,  slot, &RenderObject::setMaterialSpecular);
}

void Material::unregisterMaterial(const RenderObject* slot)
{
	disconnect(this, &Material::materialShininessChanged, slot, &RenderObject::setMaterialShininess);
	disconnect(this, &Material::materialDiffuseChanged,   slot, &RenderObject::setMaterialDiffuse);
	disconnect(this, &Material::materialAmbientChanged,   slot, &RenderObject::setMaterialAmbient);
	disconnect(this, &Material::materialSpecularChanged,  slot, &RenderObject::setMaterialSpecular);
}

void Material::dump()
{
	std::cout << "[Material]: dump:" << std::endl;
	std::cout << "\t shininess: " << m_.shininess << std::endl;
	std::cout << "\t diffuse: " << m_.diffuse.r <<  ", " << m_.diffuse.g <<  ", " << m_.diffuse.b <<  ", " << m_.diffuse.a <<  ", " << std::endl;
	std::cout << "\t ambient: " << m_.ambient.r <<  ", " << m_.ambient.g <<  ", " << m_.ambient.b <<  ", " << m_.ambient.a <<  ", " << std::endl;
	std::cout << "\t specular: " << m_.specular.r <<  ", " << m_.specular.g <<  ", " << m_.specular.b <<  ", " << m_.specular.a <<  ", " << std::endl;
}