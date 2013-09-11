#ifndef MATERIAL_H
#define MATERIAL_H
#include <GL/glew.h>

#include <iostream>
#include <QObject>



/**
 * @brief Stores the parameters of the materials.
 *
 * Signals all connected slots when the material changes.
 **/
class Material : public QObject
{
		Q_OBJECT
	public:

		virtual ~Material();

		/**
		 * Use this method to obtain the Material instance.
		 */
		static Material& instance();

		/**
		 * @brief Dump the current material parameters to std::cout
		 *
		 * @return void
		 **/
		void dump();

		/**
		 * @brief Set the shininess material.
		 *
		 * @emits materialShininessChanged
		 * @param shininess The shininess to be set.
		 * @return void
		 **/
		void setMaterialShininess(float shininess);

		/**
		 * @brief Set the diffuse material.
		 *
		 * @emit materialDiffuseChanged
		 * @param r Red channel of the diffuse material.
		 * @param g Green channel of the diffuse material.
		 * @param b Blue channel of the diffuse material.
		 * @param a Alpha channel of the diffuse material.
		 * @return void
		 **/
		void setMaterialDiffuse(float r, float g, float b, float a);

		/**
		 * @brief Set the ambient material.
		 *
		 * @emit materialAmbientChanged
		 * @param r Red channel of the ambient material.
		 * @param g Green channel of the ambient material.
		 * @param b Blue channel of the ambient material.
		 * @param a Alpha channel of the ambient material.
		 * @return void
		 **/
		void setMaterialAmbient(float r, float g, float b, float a);

		/**
		 * @brief Set the specular material.
		 *
		 * @emit materialSpecularChanged
		 * @param r Red channel of the specular material.
		 * @param g Green channel of the specular material.
		 * @param b Blue channel of the specular material.
		 * @param a Alpha channel of the specular material.
		 * @return void
		 **/
		void setMaterialSpecular(float r, float g, float b, float a);

		/**
		 * @brief Register the slot for changes of the material.
		 *
		 * Connects the signals of this Material with the slots of the slot object.
		 *
		 * @param slot The slot to be registered.
		 * @return void
		 **/
		void registerMaterial( const QObject* slot );

		/**
		 * @brief Disconnects the Material from the given <code>slot</code> object.
		 *
		 * The slot will no longer be notified on material changes.
		 *
		 * @param slot The slot to disconnect.
		 * @return void
		 **/
		void unregisterMaterial( const QObject* slot );

	signals:

		void materialShininessChanged(float shininess);
		void materialDiffuseChanged (float r, float g, float b, float a);
		void materialAmbientChanged (float r, float g, float b, float a);
		void materialSpecularChanged(float r, float g, float b, float a);

	protected:

		struct material
		{
			float shininess;
			struct diffuse
			{
				float r,g,b,a;
			} diffuse;
			struct ambient
			{
				float r,g,b,a;
			} ambient;
			struct specualr
			{
				float r,g,b,a;
			} specular;
		} m_;

		//avoid object creation from outsite.
		Material();

		//avoid creating objects using the copy constructor.
		Material(const Material&);

		//avoid creating objects using the assignment operator.
		const Material& operator=(const Material&);

		/**
		 * @brief The material instance. It is NULL if it is not instanced.
		 **/
		static Material* material_;

};

#endif //MATERIAL_H