// $Id: atomBondModelConnector.C,v 1.4 2001/05/13 16:27:25 hekl Exp $

#include <BALL/MOLVIEW/FUNCTOR/atomBondModelConnector.h>

using namespace std;

namespace BALL
{

	namespace MOLVIEW
	{
		/*
    AtomBondModelConnector::MissingProperty::MissingProperty
		  (const char* file, int line, const string& message)
			throw()
				: Exception::GeneralException(file, line, "MissingProperty", message)
		{
    }
		*/

		AtomBondModelConnector::AtomBondModelConnector()
			throw()
			:	BaseModelConnector()
		{
		}

		AtomBondModelConnector::AtomBondModelConnector
			(const AtomBondModelConnector& connector, bool deep)
			throw()
			:	BaseModelConnector(connector, deep)
		{
		}

		AtomBondModelConnector::~AtomBondModelConnector()
			throw()
		{
			#ifdef BALL_VIEW_DEBUG
				cout << "Destructing object " << (void *)this 
					<< " of class " << RTTI::getName<AtomBondModelConnector>() << endl;
			#endif 

			destroy();
		}

  	void AtomBondModelConnector::visit(Composite& composite)
		{
			if (!RTTI::isKindOf<Bond>(composite))
			{
				return;
			}

			Bond* bond = RTTI::castTo<Bond>(composite);
			
			Atom* first_atom = bond->getFirstAtom();
			Atom* second_atom = bond->getSecondAtom();
			Property first_model = getModel_(*first_atom);
			Property second_model = getModel_(*second_atom);
			
			// if any model is unkown => do nothing
			if (first_model == PROPERTY__UNKNOWN
					|| second_model == PROPERTY__UNKNOWN)
			{
				return;
			}

			// is any model a line model => bond will be shown as a line
			if (first_model == PROPERTY__MODEL_LINES
					|| second_model == PROPERTY__MODEL_LINES)
			{
				createLineRepresentation_(*bond);
			}
			// now is it not a line model present
			// if there is a ball and stick model present => create tube representation
			else if (first_model == PROPERTY__MODEL_BALL_AND_STICK
							 || second_model == PROPERTY__MODEL_BALL_AND_STICK)
			{
				createTubeRepresentation_(*bond);
			}		

			// neither line nor ball and stick model
			// => do nothing
		}

		Property AtomBondModelConnector::getModel_(Atom& atom)
			throw()
    {
			Property property = PROPERTY__UNKNOWN;

			// get geometric objects from first atom
			atom.applyChild(getGeometricObjectSearcher());

			List<VIEW::GeometricObject*>::Iterator it 
				= getGeometricObjectSearcher().getGeometricObjects().begin();

			for(; it != getGeometricObjectSearcher().getGeometricObjects().end(); ++it)
			{
				if ((**it).hasProperty(PROPERTY__MODEL_BALL_AND_STICK))
				{
					property = PROPERTY__MODEL_BALL_AND_STICK;
					break;
				}
				else if ((**it).hasProperty(PROPERTY__MODEL_LINES))
				{
					property = PROPERTY__MODEL_LINES;
					break;
				}
				else if ((**it).hasProperty(PROPERTY__MODEL_VDW))
				{
					property = PROPERTY__MODEL_VDW;
					break;
				}
			}

			return property;
    }

		void AtomBondModelConnector::createLineRepresentation_(Bond& bond)
			throw(Exception::OutOfMemory)
    {
			Atom* first_atom = bond.getFirstAtom();
			Atom* second_atom = bond.getSecondAtom();

			// get colors from both atoms
			first_atom->host(*getColorCalculator());
			ColorRGBA first_color = getColorCalculator()->getColor();
							
			second_atom->host(*getColorCalculator());
			ColorRGBA second_color = getColorCalculator()->getColor();
							
			// if both colors are identical
			if (first_color == second_color)
			{
				// gnerate single colored line
				Line *line = createLine_();

				if (line == 0)
				{
					throw Exception::OutOfMemory
						(__FILE__, __LINE__, sizeof(Line));
				}
								
				// carry on selected flag
				line->Selectable::set(bond);
								
				line->PropertyManager::set(*this);
				line->PropertyManager::setProperty(PROPERTY__MODEL_LINES);
				line->PropertyManager::clearProperty(GeometricObject::PROPERTY__DRAWING_MODE_SOLID);
				line->PropertyManager::setProperty(GeometricObject::PROPERTY__DRAWING_MODE_WIREFRAME);
				line->setVertex1Address(first_atom->getPosition());
				line->setVertex2Address(second_atom->getPosition());
				line->setColor(first_color);
								
				bond.Composite::appendChild(*line);
			}
			else
			{
				// generate two colored tube
				TwoColoredLine *line = createTwoColoredLine_();

				if (line == 0)
				{
					throw Exception::OutOfMemory
						(__FILE__, __LINE__, sizeof(TwoColoredLine));
				}
								
				// carry on selected flag
				line->Selectable::set(bond);
								
				line->PropertyManager::set(*this);
				line->PropertyManager::setProperty(PROPERTY__MODEL_LINES);
				line->PropertyManager::clearProperty(GeometricObject::PROPERTY__DRAWING_MODE_SOLID);
				line->PropertyManager::setProperty(GeometricObject::PROPERTY__DRAWING_MODE_WIREFRAME);
				line->setVertex1Address(first_atom->getPosition());
				line->setVertex2Address(second_atom->getPosition());
				line->setColor1(first_color);
				line->setColor2(second_color);
				
				bond.Composite::appendChild(*line);
			}
    }

		void AtomBondModelConnector::createTubeRepresentation_(Bond& bond)
			throw(Exception::OutOfMemory)
    {
			float stick_radius = 0.4;

			if (hasProperty(String("STICK_RADIUS")))
			{
				stick_radius = getProperty(String("STICK_RADIUS")).getFloat();
			}

			Atom* first_atom = bond.getFirstAtom();
			Atom* second_atom = bond.getSecondAtom();

			// get colors from both atoms
			first_atom->host(*getColorCalculator());
			ColorRGBA first_color = getColorCalculator()->getColor();
							
			second_atom->host(*getColorCalculator());
			ColorRGBA second_color = getColorCalculator()->getColor();
							
			// if both colors are identical
			if (first_color == second_color)
			{
				// generate single colored tube
				Tube *tube = createTube_();

				if (tube == 0)
				{
					throw Exception::OutOfMemory
						(__FILE__, __LINE__, sizeof(Tube));
				}
								
				// carry on selected flag
				tube->Selectable::set(bond);
								
				tube->PropertyManager::set(*this);
				tube->PropertyManager::setProperty(PROPERTY__MODEL_BALL_AND_STICK);
				tube->setRadius(stick_radius);
				tube->setVertex1Address(first_atom->getPosition());
				tube->setVertex2Address(second_atom->getPosition());
				tube->setColor(first_color);
								
				bond.Composite::appendChild(*tube);
			}
			else
			{
				// generate two colored tube
				TwoColoredTube *tube = createTwoColoredTube_();
								
				if (tube == 0)
				{
					throw Exception::OutOfMemory
						(__FILE__, __LINE__, sizeof(TwoColoredTube));
				}
								
				// carry on selected flag
				tube->Selectable::set(bond);

				tube->PropertyManager::set(*this);
				tube->PropertyManager::setProperty(PROPERTY__MODEL_BALL_AND_STICK);
				tube->setRadius(stick_radius);
				tube->setVertex1Address(first_atom->getPosition());
				tube->setVertex2Address(second_atom->getPosition());
				tube->setColor1(first_color);
				tube->setColor2(second_color);
				
				bond.Composite::appendChild(*tube);
			}
    }

		Line* AtomBondModelConnector::createLine_()
    {
			return new Line();
    }

	  TwoColoredLine* AtomBondModelConnector::createTwoColoredLine_()
    {
			return new TwoColoredLine();
    }

		Tube* AtomBondModelConnector::createTube_()
    {
			return new Tube();
    }

		TwoColoredTube* AtomBondModelConnector::createTwoColoredTube_()
    {
			return new TwoColoredTube();
    }

#		ifdef BALL_NO_INLINE_FUNCTIONS
#			include <BALL/MOLVIEW/FUNCTOR/atomBondModelConnector.iC>
#		endif

	} // namespace MOLVIEW

} // namespace BALL
