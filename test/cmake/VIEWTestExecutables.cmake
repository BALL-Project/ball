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
	# Phase 999.48 §8.1 — a11y smoke test for the 4 named custom widgets.
	# Asserts accessibleName + focusPolicy invariants. Deeper coverage
	# (Inspector dock tab-chain walk via focusNextChild() against a
	# per-section manifest) deferred to v1.7 RC patch.
	tabOrder_test
	# v1.7.x-25 — controller command-contract safety net for the v1.7.x-24
	# cut-over. Asserts set->apply->revert round-trips through the Stage for
	# the four Stage-attached controllers (Stage/Camera/Stereo/Light).
	ControllerContract_test
)

SET(VIEW_TESTS
	${BALL_VIEW_TESTS}
)
