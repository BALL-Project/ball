SET(BALL_VIEW_TESTS
	BallAndStickModel_test
	AddCartoonModel_test
	LabelModel_test
	LineModel_test
	SurfaceModel_test
	HBondModel_test
	# Phase 999.46 — menu remap + CommandRegistry round-trip test.
	# Minimal scope (PopUpID enum surface + registry CRUD); deeper
	# every-shortcut-to-command coverage deferred to v1.7 RC patch.
	menuMapping_test
)

SET(VIEW_TESTS 
	${BALL_VIEW_TESTS}
)
