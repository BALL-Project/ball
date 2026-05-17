// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// Phase 999.48 §8.1 - minimal tabOrder_test.
//
// Scope (per phase scope-trim guidance): asserts the 4 a11y-named
// custom widgets construct cleanly + advertise their accessibleName
// after Phase 999.48 Task 1. Deeper coverage (walking the Inspector
// dock's tab chain via focusNextChild() and asserting a per-section
// manifest order) is deferred to a v1.7 RC patch since it requires
// a fully-constructed Mainframe + Inspector + QApplication setup
// behind a display.

#include <BALL/CONCEPT/classTest.h>
#include <BALLTestConfig.h>

#include <BALL/VIEW/WIDGETS/swatchButton.h>
#include <BALL/VIEW/WIDGETS/labeledSlider.h>
#include <BALL/VIEW/WIDGETS/sectionHeader.h>
#include <BALL/VIEW/WIDGETS/inspector/inspectorSection.h>

#include <QtCore/QCoreApplication>
#include <QtGui/QGuiApplication>
#include <QtWidgets/QApplication>
#include <QtWidgets/QSlider>
#include <QtWidgets/QToolButton>

using namespace BALL;
using namespace BALL::VIEW;

START_TEST(TabOrder)
// QApplication is required for any QWidget construction.
int argc = 1;
char argv0[] = "tabOrder_test";
char* argv[] = { argv0, nullptr };
QApplication app(argc, argv);

// ----------------------------------------------------------------
// SwatchButton: accessibleName "Color: #ffffff" after default ctor;
// updates on setColor().
// ----------------------------------------------------------------

CHECK(SwatchButton_accessibleName_default)
	SwatchButton b;
	bool hasColorPrefix = b.accessibleName().contains(QStringLiteral("Color:"));
	TEST_EQUAL(hasColorPrefix, true)
RESULT

CHECK(SwatchButton_accessibleName_updates_on_setColor)
	SwatchButton b;
	b.setColor(QColor(Qt::red));
	// QColor(Qt::red).name(HexRgb) == "#ff0000"
	bool hasRedHex = b.accessibleName().contains(QStringLiteral("#ff0000"));
	TEST_EQUAL(hasRedHex, true)
RESULT

// ----------------------------------------------------------------
// LabeledSlider: units string mirrored into slider+spinbox names.
// ----------------------------------------------------------------

CHECK(LabeledSlider_units_propagates_to_slider_a11y)
	LabeledSlider ls(0, 100, 50, QStringLiteral("%"));
	// Locate the slider child by objectName.
	QSlider* s = ls.findChild<QSlider*>(QStringLiteral("labeledSliderSlider"));
	TEST_NOT_EQUAL(s, nullptr)
	if (s != nullptr)
	{
		std::string name = s->accessibleName().toStdString();
		TEST_EQUAL(name, std::string("%"))
	}
RESULT

// ----------------------------------------------------------------
// SectionHeader: accessibleName matches title; setTitle keeps in sync;
// chevron is StrongFocus (tab-reachable).
// ----------------------------------------------------------------

CHECK(SectionHeader_accessibleName_matches_title)
	SectionHeader h(QStringLiteral("Background"));
	std::string name = h.accessibleName().toStdString();
	TEST_EQUAL(name, std::string("Background"))
RESULT

CHECK(SectionHeader_setTitle_updates_a11y_name)
	SectionHeader h(QStringLiteral("Background"));
	h.setTitle(QStringLiteral("Lights"));
	std::string name = h.accessibleName().toStdString();
	TEST_EQUAL(name, std::string("Lights"))
RESULT

CHECK(SectionHeader_chevron_is_tab_reachable)
	SectionHeader h(QStringLiteral("Stage"));
	QToolButton* chev = h.findChild<QToolButton*>(QStringLiteral("sectionHeaderChevron"));
	TEST_NOT_EQUAL(chev, nullptr)
	if (chev != nullptr)
	{
		TEST_EQUAL(int(chev->focusPolicy()), int(Qt::StrongFocus))
	}
RESULT

// ----------------------------------------------------------------
// InspectorSection: accessibleName matches title.
// ----------------------------------------------------------------

CHECK(InspectorSection_accessibleName_matches_title)
	InspectorSection s(QStringLiteral("Properties"), QStringLiteral("Scene/Properties"));
	std::string name = s.accessibleName().toStdString();
	TEST_EQUAL(name, std::string("Properties"))
RESULT

END_TEST
