cls

del *.obj

del *.dll

del *.exp

del *.lib


cl.exe /c /EHsc imageProcessingOuterComponent.cpp 

link.exe imageProcessingOuterComponent.obj /DLL /DEF:imageProcessingOuterComponent.def user32.lib ole32.lib winmm.lib /SUBSYSTEM:WINDOWS

