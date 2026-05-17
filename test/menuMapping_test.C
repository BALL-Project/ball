// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// Phase 999.46 Task 7 - minimal menuMapping_test.
//
// Scope (per phase scope-trim guidance): asserts the structural
// pieces are in place - new MainControl::PopUpID enum values for the
// 8-top-level remap exist, and CommandRegistry round-trips a
// register/search/recent sequence cleanly. Deeper coverage (every
// old shortcut maps to a command with the expected title against a
// live Mainframe) is deferred to a v1.7 RC patch since it needs a
// full QApplication + display setup that's better landed with proper
// widget-test infrastructure work.

#include <BALL/CONCEPT/classTest.h>
#include <BALLTestConfig.h>

#include <BALL/VIEW/KERNEL/mainControl.h>

#include <BALL/VIEW/KERNEL/commandRegistry.h>
#include <QtGui/QAction>

using namespace BALL;
using namespace BALL::VIEW;

START_TEST(MenuMapping)

// ----------------------------------------------------------------
// PopUpID enum surface - the 8-menu remap requires SELECT (new
// top-level for Invert/Clear/By-Expression) plus the legacy values
// (EDIT / DISPLAY / MOLECULARMECHANICS / TOOLS / WINDOWS / MACRO /
// HELP) being repurposed under BALL_UI_V2 as View / Compute / Scripts
// / Window / (Scripts > Macros submenu).
// ----------------------------------------------------------------

CHECK(PopUpID_FILE)
	int v = MainControl::FILE;
	TEST_EQUAL(v, 10001)
RESULT

CHECK(PopUpID_EDIT)
	int v = MainControl::EDIT;
	TEST_EQUAL(v, 10100)
RESULT

CHECK(PopUpID_DISPLAY_retitles_to_View)
	int v = MainControl::DISPLAY;
	TEST_EQUAL(v, 10300)
RESULT

CHECK(PopUpID_MOLECULARMECHANICS_retitles_to_Compute)
	int v = MainControl::MOLECULARMECHANICS;
	TEST_EQUAL(v, 10400)
RESULT

CHECK(PopUpID_TOOLS_retitles_to_Scripts)
	int v = MainControl::TOOLS;
	TEST_EQUAL(v, 10500)
RESULT

CHECK(PopUpID_WINDOWS_retitles_to_Window)
	int v = MainControl::WINDOWS;
	TEST_EQUAL(v, 10600)
RESULT

CHECK(PopUpID_MACRO_reroutes_to_Scripts_Macros)
	int v = MainControl::MACRO;
	TEST_EQUAL(v, 10750)
RESULT

CHECK(PopUpID_HELP)
	int v = MainControl::HELP;
	TEST_EQUAL(v, 10800)
RESULT

CHECK(PopUpID_SELECT_new_top_level)
	int v = MainControl::SELECT;
	TEST_EQUAL(v, 10900)
RESULT

CHECK(PopUpID_EDIT_STRUCTURE_submenu)
	int v = MainControl::EDIT_STRUCTURE;
	TEST_EQUAL(v, 10901)
RESULT

CHECK(PopUpID_VIEW_WORKSPACE_submenu)
	int v = MainControl::VIEW_WORKSPACE;
	TEST_EQUAL(v, 10902)
RESULT

CHECK(PopUpID_COMPUTE_ENERGY_submenu)
	int v = MainControl::COMPUTE_ENERGY;
	TEST_EQUAL(v, 10903)
RESULT

CHECK(PopUpID_SCRIPTS_MACROS_submenu)
	int v = MainControl::SCRIPTS_MACROS;
	TEST_EQUAL(v, 10904)
RESULT

// ----------------------------------------------------------------
// CommandRegistry — always available post-999.48 flag removal.
// ----------------------------------------------------------------

CHECK(CommandRegistry_round_trip_register_search_recent)
	CommandRegistry::instance().clearForTest();

	Command c;
	c.id = "view.testfullscreen";
	c.title = "Enter Fullscreen Test";
	c.category = "View";
	c.description = "Toggle BALLView's fullscreen mode (test fixture).";
	bool triggered = false;
	c.trigger = [&triggered]() { triggered = true; };
	c.isEnabled = []() { return true; };
	CommandRegistry::instance().registerCommand(c);

	QList<Command> all = CommandRegistry::instance().all();
	TEST_EQUAL(all.size(), 1)
	TEST_EQUAL(all.first().id.toStdString(), std::string("view.testfullscreen"))

	QList<Command> hits = CommandRegistry::instance().search("full");
	TEST_EQUAL(hits.size(), 1)
	TEST_EQUAL(hits.first().title.toStdString(), std::string("Enter Fullscreen Test"))

	// noteTriggered moves the id to head of MRU.
	CommandRegistry::instance().noteTriggered("view.testfullscreen");
	QList<Command> recent = CommandRegistry::instance().recent();
	TEST_EQUAL(recent.size(), 1)
	TEST_EQUAL(recent.first().id.toStdString(), std::string("view.testfullscreen"))

	// Idempotent re-register on same id replaces (doesn't dupe).
	Command c2 = c;
	c2.title = "Enter Fullscreen Test (Updated)";
	CommandRegistry::instance().registerCommand(c2);
	all = CommandRegistry::instance().all();
	TEST_EQUAL(all.size(), 1)
	TEST_EQUAL(all.first().title.toStdString(), std::string("Enter Fullscreen Test (Updated)"))

	// Fuzzy search: subsequence match.
	hits = CommandRegistry::instance().search("efs");
	TEST_NOT_EQUAL(hits.size(), 0)

	// Empty query returns recent (MRU).
	hits = CommandRegistry::instance().search("");
	TEST_EQUAL(hits.size(), 1)
	TEST_EQUAL(hits.first().id.toStdString(), std::string("view.testfullscreen"))

	// Cleanup.
	CommandRegistry::instance().clearForTest();
	TEST_EQUAL(CommandRegistry::instance().all().size(), 0)
RESULT

CHECK(CommandRegistry_search_empty_for_non_matching_query)
	CommandRegistry::instance().clearForTest();

	Command c;
	c.id = "view.fullscreen";
	c.title = "Enter Fullscreen";
	c.category = "View";
	CommandRegistry::instance().registerCommand(c);

	QList<Command> hits = CommandRegistry::instance().search("zzzqqqxxx");
	TEST_EQUAL(hits.size(), 0)

	CommandRegistry::instance().clearForTest();
RESULT

END_TEST
