	DEVICE ZXSPECTRUM128

	ORG 0x6000
START       
	incbin "code.raw"
ENDPROG
	SAVEHOB  "zxwiztst.$C","zxwiztst.C",START,ENDPROG-START
