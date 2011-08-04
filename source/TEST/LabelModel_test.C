// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//

#include <BALL/CONCEPT/classTest.h>
#include <BALLTestConfig.h>
#include <BALL/FORMAT/PDBFile.h>
#include <BALL/KERNEL/forEach.h>
#include <BALL/KERNEL/bond.h>
#include <BALL/VIEW/PRIMITIVES/label.h>
#include <BALL/KERNEL/extractors.h>

///////////////////////////
#include <BALL/VIEW/MODELS/labelModel.h>
///////////////////////////

using namespace BALL;
using namespace BALL::VIEW;

START_TEST(LabelModel)

CHECK(CSTR)
	LabelModel();
RESULT

CHECK(LabelModel::BALL_CREATE(LabelModel))
  //BAUSTELLE
RESULT


PDBFile pdb(BALL_TEST_DATA_PATH(1BNA.pdb));
System system;
pdb >> system;

CHECK(LabelModel::Processor::Result operator() (Composite& composite))
	LabelModel bs;
	bool result = bs.operator() (system);
	TEST_EQUAL(result, true)
RESULT

Size nr_atoms = system.countAtoms();

CHECK(void setFont(const QFont& font))
	LabelModel bs;
	bs.setFont(QFont("Courier"));
RESULT

CHECK(const QFont& getFont() const throw())
	LabelModel bs;
	bs.setFont(QFont("Courier"));
	TEST_EQUAL(bs.getFont() == QFont("Courier"), true)
RESULT

CHECK(void setText(const String& text))
	LabelModel bs;
	bs.setText("asd");
RESULT

CHECK(const String& getText() const)
	LabelModel bs;
	bs.setText("asd");
	TEST_EQUAL(bs.getText(), "asd")
RESULT

CHECK(void setMode(Mode mode) throw())
	LabelModel bs;
	bs.setMode(LabelModel::ALL_RESIDUES);
RESULT

CHECK(void setColor(const ColorRGBA& color))
	LabelModel bs;
	bs.setColor(ColorRGBA(1.0,0,0));
RESULT

CHECK(const ColorRGBA& getColor() const throw())
	LabelModel bs;
	bs.setColor(ColorRGBA(1.0,0,0));
	TEST_EQUAL(bs.getColor(), ColorRGBA(1., 0, 0));
RESULT

CHECK(LabelModel::createGeometricObjects() throw())
	LabelModel bs;
	bs.setText("%Y");
	bs.setMode(LabelModel::ALL_ATOMS);
	system.apply(bs);
	bs.createGeometricObjects();
	TEST_EQUAL(bs.getGeometricObjects().size(), nr_atoms)
	Label* label = dynamic_cast<Label*>(*bs.getGeometricObjects().begin());
	const Atom* atom = dynamic_cast<const Atom*>(label->getComposite());
	TEST_EQUAL(label->getExpandedText(), atom->getTypeName())
RESULT


/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////
END_TEST
