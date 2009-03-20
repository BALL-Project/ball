// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: label.C,v 1.10.16.1 2007-03-25 22:02:34 oliver Exp $
//

#include <BALL/VIEW/PRIMITIVES/label.h>
#include <BALL/KERNEL/atom.h>
#include <BALL/KERNEL/atomContainer.h>
#include <BALL/KERNEL/residue.h>
#include <BALL/KERNEL/PTE.h>

using namespace std;

namespace BALL
{
	namespace VIEW
	{

		Label::Label()
			:	GeometricObject(),
				Vertex()
		{
		}

		Label::Label(const Label& label)
			:	GeometricObject(label),
				Vertex(label),
				text_(label.text_)
		{
		}

		Label::~Label()
		{
			#ifdef BALL_VIEW_DEBUG
				Log.info() << "Destructing object " << (void *)this 
						 			 << " of class " << RTTI::getName<Label>() << std::endl;
			#endif 
		}

		void Label::clear()
		{
			GeometricObject::clear();
			Vertex::clear();
			text_.clear();
		}

		void Label::set(const Label& label)
		{
			GeometricObject::set(label);
			Vertex::set(label);
			text_ = label.text_;
		}

		const Label& Label::operator = (const Label& label)
		{
			set(label);
			return *this;
		}

		void Label::swap(Label& label)
		{
			GeometricObject::swap(label);
			Vertex::swap(label);
		}

		bool Label::isValid() const
		{
			return (GeometricObject::isValid() &&
							Vertex::isValid());
		}

		void Label::dump(ostream& s, Size depth) const
		{
			BALL_DUMP_STREAM_PREFIX(s);
			
			BALL_DUMP_DEPTH(s, depth);
			BALL_DUMP_HEADER(s, this, this);

			BALL_DUMP_DEPTH(s, depth);
			s << "Label Text: " << text_ << endl;

			GeometricObject::dump(s, depth + 1);
			Vertex::dump(s, depth + 1);

			BALL_DUMP_STREAM_SUFFIX(s);
		}


		String Label::getExpandedText() const
		{
			if (!text_.has('%') || getComposite() == 0)
			{
				return text_;
			}
			
			String result;

			const Atom* atom = dynamic_cast<const Atom*> (getComposite());
			const AtomContainer* ac = dynamic_cast<const AtomContainer*> (getComposite());
			const Residue* residue = dynamic_cast<const Residue*> (getComposite());

			for (Position pos = 0; pos < text_.size(); pos++)
			{
				// normal text
				if (text_[pos] != '%')
				{
					result += text_[pos];
					continue;
				}

				pos++;

				// dont step over the size of the string
				if (pos == text_.size()) break;

				// make it possible to escape %
				if (text_[pos] == '%')
				{
					result += '%';
				}

				// name
				else if (text_[pos] == 'N')
				{
					if (atom) result += atom->getName();
					else 		  result += ac->getName();
				}

				// resiude ID
				else if (text_[pos] == 'I')
				{
					if (residue) result += residue->getID();
					if (atom) 	 result += ((Residue*)atom->getParent())->getID();
				}

				// atom type
				else if (text_[pos] == 'T')
				{
					if (atom) result += String(atom->getType());
				}

				// atom charge
				else if (text_[pos] == 'C')
				{
					if (atom)
					{
						String charge(atom->getCharge());
						charge.trimRight("0");
						if (charge.hasSuffix(".")) charge += '0';
						result += charge;
					}
				}

				// element
				else if (text_[pos] == 'E')
				{
					if (atom)
					{
						result += atom->getElement().getSymbol();
					}
				}
				
				// type name
				else if (text_[pos] == 'Y')
				{
					if (atom)
					{
						result += atom->getTypeName();
					}
				}
			} // for

			return result;
		} // getExpandedText

		void Label::getVertices(vector<Vector3>& vertices) const
		{
			vertices.push_back(getVertex());
		}
			
	} // namespace VIEW
} // namespace BALL
