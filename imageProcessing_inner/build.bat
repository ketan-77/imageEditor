cls

del *.obj

del *.dll

del *.exp

del *.lib


cl.exe /c /EHsc imageProcessingInnerComponent.cpp 

link.exe imageProcessingInnerComponent.obj /DLL /DEF:imageProcessingInnerComponent.def user32.lib winmm.lib /SUBSYSTEM:WINDOWS
