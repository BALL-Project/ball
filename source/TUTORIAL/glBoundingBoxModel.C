// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: glBoundingBoxModel.C,v 1.5 2004/04/16 11:49:30 amoll Exp $
// BALLView tutorial example
// ------------------------
// create a processor that computes the bounding box of a molecular structure

#include "glBoundingBoxModel.h"
#include <BALL/KERNEL/system.h>
#include <BALL/VIEW/KERNEL/molecularInformation.h>

namespace BALL
{

	namespace VIEW
	{

		GLBoundingBoxModel::GLBoundingBoxModel() 
			throw()
			: color_(),
				new_start_(true),
				start_composite_(0),
				bbox_()
		{
		}

		GLBoundingBoxModel::~GLBoundingBoxModel()
			throw()
		{
		}

		void GLBoundingBoxModel::setColor(const ColorRGBA &color)
			throw()
		{ 
			color_ = color; 
		}

		bool GLBoundingBoxModel::start()
			throw()
		{
			new_start_ = true;
			start_composite_ = 0;

			return ModelProcessor::start();
		}


		bool GLBoundingBoxModel::finish()
			throw()
		{
			Composite *root = &(start_composite_->getRoot());

			// if the bounding box is degenerated or if the root of the composite is no
			// System, do not create a bounding box
			if (bbox_.a == bbox_.b || !RTTI::isKindOf<System>(*root))
			{
				return false;
			}

			// use a MolecularInformation visitor to gain information about the
			// molecular structure wa are processing at the moment
			MolecularInformation molecular_information;
			start_composite_->host(molecular_information);                        

			// create a SimpleBox with the boundaries of the calculated box and set
			// its properties with help from MolecularInformation
			SimpleBox* pbox = new SimpleBox();
			pbox->a = bbox_.a ;
 			pbox->b = bbox_.b;
			pbox->setColor(color_);
			pbox->setName(String("BoundingBox of ")
										+ molecular_information.getTypeName()
										+ String(" (")
										+ molecular_information.getName()
										+ String(")"));
			pbox->setColor(color_);

			geometric_objects_.push_back(pbox);

			// TODO
			// move that to the main program
			Log.info() << "Bounding Box: " << bbox_.a << " " << bbox_.b << endl;

			return true;
		}


		Processor::Result GLBoundingBoxModel::operator() (Composite &composite)
			throw()
		{
			// store the composite we started at because we will need it when finishing
			if (start_composite_ == 0)
			{
				start_composite_ = &composite;
			}

			// if we do not encounter an atom, skip it
			if (!RTTI::isKindOf<Atom>(composite))
			{
				return Processor::CONTINUE;
			}

			// cast the current item to Atom in order to use Atom related methods
			Atom *atom = RTTI::castTo<Atom>(composite);

			// Create a three dimensional Box defined by the atom position
			SimpleBox3 bbox(atom->getPosition(), atom->getPosition());

			// if we just started, use the just generated box as starting point, else
			// join it with the previously calculated one to create a box containing all
			// atoms we encountered until now
			if (new_start_)
			{
				bbox_ = bbox;
				new_start_ = false;
			}
			else
			{
				bbox_.join(bbox);
			}

			return Processor::CONTINUE;
		}

	}

}
