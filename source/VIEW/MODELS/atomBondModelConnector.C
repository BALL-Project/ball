// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: atomBondModelConnector.C,v 1.1 2003/08/26 12:38:37 oliver Exp $

#include <BALL/MOLVIEW/FUNCTOR/atomBondModelConnector.h>
#include <BALL/KERNEL/atom.h>
#include <BALL/KERNEL/bond.h>
#include <BALL/MOLVIEW/COMMON/common.h>
#include <BALL/VIEW/GUI/FUNCTOR/colorProcessor.h>

using namespace std;

namespace BALL
{
	using VIEW::ColorRGBA;

	namespace MOLVIEW
	{
				
		AtomBondModelConnector::AtomBondModelConnector()
			throw()
			:	BaseModelConnector()
		{
		}

		AtomBondModelConnector::AtomBondModelConnector(const AtomBondModelConnector& connector)
			throw()
			:	BaseModelConnector(connector)
		{
		}

		AtomBondModelConnector::~AtomBondModelConnector()
			throw()
		{
			#ifdef BALL_VIEW_DEBUG
				Log.info() << "Destructing object " << (void *)this 
									 << " of class " << RTTI::getName<AtomBondModelConnector>() << std::endl;
			#endif 
		}

  	void AtomBondModelConnector::visit(Composite& composite)
		{
			if (!RTTI::isKindOf<Bond>(composite)) return;

			Bond* bond = (Bond*)(&composite);

			switch (model_type_)
			{
				case MODEL_LINES:
					createLineRepresentation_(*bond);
					break;
				case MODEL_BALL_AND_STICK:
				case MODEL_STICK:
					createTubeRepresentation_(*bond);
					break;
			}
			
/*
			// if any model is unkown => do nothing
			if (first_model == MODEL_UNKNOWN || second_model == MODEL_UNKNOWN)
			{
				return;
			}

			// is any model a line model => bond will be shown as a line
			if (first_model == MODEL_LINES || second_model == MODEL_LINES)
			{
				createLineRepresentation_(*bond);
			}
			// now is it not a line model present
			// if there is a ball and stick model present => create tube representation
			else if (first_model == MODEL_BALL_AND_STICK || second_model == MODEL_BALL_AND_STICK) 
			{	
				createTubeRepresentation_(*bond);
			}		
*/
		}

		Property AtomBondModelConnector::getModel_(Atom& atom)
			throw()
    {
    }

		void AtomBondModelConnector::createLineRepresentation_(Bond& bond)
			throw(Exception::OutOfMemory)
    {
			// get colors from both atoms
			ColorRGBA first_color, second_color;

			if (getColorProcessor() != 0)
			{
				getColorProcessor()->operator() (bond.getFirstAtom());
				first_color = getColorProcessor()->getColor();
								
				getColorProcessor()->operator() (bond.getSecondAtom());
				second_color = getColorProcessor()->getColor();
			}
							
			// if both colors are identical
			if (first_color == second_color)
			{
				// generate single colored line
				Line *line = new Line;

				if (line == 0) throw Exception::OutOfMemory(__FILE__, __LINE__, sizeof(Line));
								
				line->setVertex1Address(bond.getFirstAtom()->getPosition());
				line->setVertex2Address(bond.getSecondAtom()->getPosition());
				line->setColor(first_color);
				line->setComposite(&bond);
								
				geometric_objects_->push_back(line);
			}
			else
			{
				// generate two colored tube
				TwoColoredLine *line = new TwoColoredLine;

				if (line == 0) throw Exception::OutOfMemory(__FILE__, __LINE__, sizeof(TwoColoredLine));
								
				line->setVertex1Address(bond.getFirstAtom()->getPosition());
				line->setVertex2Address(bond.getSecondAtom()->getPosition());
				line->setColor(first_color);
				line->setColor2(second_color);
				line->setComposite(&bond);
				geometric_objects_->push_back(line);
			}
    }

		void AtomBondModelConnector::createTubeRepresentation_(Bond& bond)
			throw(Exception::OutOfMemory)
    {
			float stick_radius = 0.4;

			if (hasProperty("STICK_RADIUS"))
			{
				stick_radius = getProperty(String("STICK_RADIUS")).getFloat();
			}

			// get colors from both atoms
			ColorRGBA first_color, second_color;

			if (getColorProcessor() != 0)
			{
				getColorProcessor()->operator() (bond.getFirstAtom());
				first_color = getColorProcessor()->getColor();
								
				getColorProcessor()->operator() (bond.getSecondAtom());
				second_color = getColorProcessor()->getColor();
			}
							
			// if both colors are identical
			if (first_color == second_color)
			{
				// generate single colored tube
				Tube *tube = new Tube;

				if (tube == 0) throw Exception::OutOfMemory (__FILE__, __LINE__, sizeof(Tube));
								
				tube->setRadius(stick_radius);
				tube->setVertex1Address(bond.getFirstAtom()->getPosition());
				tube->setVertex2Address(bond.getSecondAtom()->getPosition());
				tube->setColor(first_color);
				tube->setComposite(&bond);
				geometric_objects_->push_back(tube);
			}
			else
			{
				// generate two colored tube
				TwoColoredTube *tube = new TwoColoredTube;
								
				if (tube == 0) throw Exception::OutOfMemory(__FILE__, __LINE__, sizeof(TwoColoredTube));
								
				tube->setRadius(stick_radius);
				tube->setVertex1Address(bond.getFirstAtom()->getPosition());
				tube->setVertex2Address(bond.getSecondAtom()->getPosition());
				tube->setColor(first_color);
				tube->setColor2(second_color);
				tube->setComposite(&bond);
				geometric_objects_->push_back(tube);
			}
    }

	} // namespace MOLVIEW
} // namespace BALL
