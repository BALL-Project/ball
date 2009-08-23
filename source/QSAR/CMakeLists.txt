list(APPEND BALL_SOURCES 
	QSAR/aromaticityProcessor.C
	QSAR/connectivityBase.C
	QSAR/connectivityDescriptors.C
	QSAR/descriptor.C
	QSAR/partialChargeBase.C
	QSAR/partialChargeDescriptors.C
	QSAR/partialChargeProcessor.C
	QSAR/ringPerceptionProcessor.C
	QSAR/simpleBase.C
	QSAR/simpleDescriptors.C
	QSAR/surfaceBase.C
	QSAR/surfaceDescriptors.C
)

list(APPEND QUEASY_SOURCES 
	QSAR/allModel.C
	QSAR/bayesModel.C
	QSAR/classificationModel.C
	QSAR/classificationValidation.C
	QSAR/configIO.C
	QSAR/exception.C
	QSAR/featureSelection.C
	QSAR/fitModel.C
	QSAR/gpModel.C
	QSAR/kernel.C
	QSAR/kernelModel.C
	QSAR/knnModel.C
	QSAR/kpcrModel.C
	QSAR/kplsModel.C
	QSAR/latentVariableModel.C
	QSAR/ldaModel.C
	QSAR/linearModel.C
	QSAR/logitModel.C
	QSAR/mlrModel.C
	QSAR/Model.C
	QSAR/modelFactory.C
	QSAR/nBModel.C
	QSAR/nonlinearModel.C
	QSAR/oplsModel.C
	QSAR/pcrModel.C
	QSAR/plsModel.C
	QSAR/QSARData.C
	QSAR/registry.C
	QSAR/regressionModel.C
	QSAR/regressionValidation.C
	QSAR/rrModel.C
	QSAR/snBModel.C
	QSAR/sortedList.C
	QSAR/statistics.C
	QSAR/svmModel.C
	QSAR/svrModel.C
	QSAR/validation.C
)

if(USE_libsvm)
	list(APPEND QUEASY_SOURCES 
		QSAR/libsvmModel.C
	)
endif(USE_libsvm)

