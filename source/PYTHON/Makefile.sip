SIP=sip

default:
	$(SIP) -c . BALL.sip
	for i in `ls ./sipBALL*cpp`; do mv $$i ../`basename $$i .cpp`.C ; done
	for i in `ls ./sipBALL*h`; do mv $$i .. ; done
	mv ./BALLcmodule.cpp ../BALLcmodule.C
