// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: ForceField_test.C,v 1.1 2003/02/02 10:15:51 oliver Exp $
#include <BALL/CONCEPT/classTest.h>

///////////////////////////

#include <BALL/MOLMEC/COMMON/forceField.h>
#include <BALL/KERNEL/system.h>
#include <BALL/KERNEL/atom.h>
#include <BALL/KERNEL/molecule.h>

///////////////////////////

START_TEST(class_name, "$Id: ForceField_test.C,v 1.1 2003/02/02 10:15:51 oliver Exp $")

/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////

using namespace BALL;

ForceField* ff = 0;
CHECK(ForceField())
	ff = new ForceField;
	TEST_NOT_EQUAL(ff, 0)
RESULT

CHECK(~ForceField())
	delete ff;
RESULT

CHECK(bool ForceField::isValid() const)
	const ForceField* ff = new ForceField;
	TEST_NOT_EQUAL(ff, 0)
	TEST_EQUAL(ff->isValid(), false)
	delete ff;
RESULT

CHECK(ForceField(System& system))
	System S;
	ForceField ff(S);
	TEST_EQUAL(ff.isValid(), true)
RESULT


/* ??????
		ForceField(System& system, const Options& options);
		ForceField(const ForceField& force_field);
		virtual void clear()
			throw();

		ForceField&	operator = (const ForceField& force_field);

		bool setup(System& system);

		bool setup(System& system, const Options& options);

		virtual bool specificSetup();

		void setName(const String& name);
		
		String getName() const;
		Size getNumberOfAtoms() const;

		Size getNumberOfMovableAtoms() const;
		const	AtomVector& getAtoms() const;
		System* getSystem();
    const System* getSystem() const;
		bool getUseSelection();
		void setUseSelection(bool use_selection);
		ForceFieldParameters& getParameters();
		Size countComponents() const;
		const TimeStamp& getUpdateTime() const
			throw();
		const TimeStamp& getSetupTime() const
			throw();
		void insertComponent(ForceFieldComponent* force_field_component);
		void removeComponent(const ForceFieldComponent* force_field_component);
		void removeComponent(const String& name);
		ForceFieldComponent* getComponent(const Size	index) const;
		ForceFieldComponent*	getComponent(const String& name) const;
		double getEnergy() const;
		double updateEnergy();
		void updateForces();
		double getRMSGradient() const;
		virtual Size getUpdateFrequency() const;
		virtual void update();
*/											

/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////
END_TEST
