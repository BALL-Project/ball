#include <Material.h>
#include <Global.h>

#include <QtCore/QDir>

#include <string>

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

	emit materialShininessChanged(shininess);
}

void Material::setMaterialDiffuse(float r, float g, float b, float a)
{
	m_.diffuse.r = r;
	m_.diffuse.g = g;
	m_.diffuse.b = b;
	m_.diffuse.a = a;

	emit materialDiffuseChanged(r,g,b,a);
}

void Material::setMaterialAmbient(float r, float g, float b, float a)
{
	m_.ambient.r = r;
	m_.ambient.g = g;
	m_.ambient.b = b;
	m_.ambient.a = a;

	emit materialAmbientChanged(r, g, b, a);
}

void Material::setMaterialSpecular(float r, float g, float b, float a)
{
	m_.specular.r = r;
	m_.specular.g = g;
	m_.specular.b = b;
	m_.specular.a = a;

	emit materialSpecularChanged(r, g, b, a);
}

void Material::registerMaterial( const QObject* slot )
{
	connect(this, SIGNAL(materialShininessChanged(float)),
	        slot, SLOT(setMaterialShininess(float)));
	connect(this, SIGNAL(materialDiffuseChanged(float, float, float, float)),
	        slot, SLOT(setMaterialDiffuse(float, float, float, float)));
	connect(this, SIGNAL(materialAmbientChanged(float, float, float, float)),
	        slot, SLOT(setMaterialAmbient(float, float, float, float)));
	connect(this, SIGNAL(materialSpecularChanged(float, float, float, float)),
	        slot, SLOT(setMaterialSpecular(float, float, float, float)));
}

void Material::unregisterMaterial( const QObject* slot )
{
	disconnect(this, SIGNAL(materialShininessChanged(float)),
	           slot, SLOT(setMaterialShininess(float)));
	disconnect(this, SIGNAL(materialDiffuseChanged(float, float, float, float)),
	           slot, SLOT(setMaterialDiffuse(float, float, float, float)));
	disconnect(this, SIGNAL(materialAmbientChanged(float, float, float, float)),
	           slot, SLOT(setMaterialAmbient(float, float, float, float)));
	disconnect(this, SIGNAL(materialSpecularChanged(float, float, float, float)),
	           slot, SLOT(setMaterialSpecular(float, float, float, float)));
}

void Material::dump()
{
	std::cout << "[Material]: dump:" << std::endl;
	std::cout << "\t shininess: " << m_.shininess << std::endl;
	std::cout << "\t diffuse: " << m_.diffuse.r <<  ", " << m_.diffuse.g <<  ", " << m_.diffuse.b <<  ", " << m_.diffuse.a <<  ", " << std::endl;
	std::cout << "\t ambient: " << m_.ambient.r <<  ", " << m_.ambient.g <<  ", " << m_.ambient.b <<  ", " << m_.ambient.a <<  ", " << std::endl;
	std::cout << "\t specular: " << m_.specular.r <<  ", " << m_.specular.g <<  ", " << m_.specular.b <<  ", " << m_.specular.a <<  ", " << std::endl;
}