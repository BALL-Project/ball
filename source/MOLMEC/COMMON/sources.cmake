### the directory name ###
SET(DIRECTORY source/MOLMEC/COMMON)

### list all filenames of the directory here ###
SET(SOURCES_LIST
	assignTypes.C
	atomVector.C
	bendComponent.C
	chargeRuleProcessor.C
	forceField.C
	forceFieldComponent.C
	gradient.C
	periodicBoundary.C
	radiusRuleProcessor.C
	ruleEvaluator.C
	ruleProcessor.C
	snapShot.C
	snapShotManager.C
	support.C
	stretchComponent.C
	typenameRuleProcessor.C
	typeRuleProcessor.C
)	

ADD_BALL_SOURCES("MOLMEC/COMMON" "${SOURCES_LIST}")
