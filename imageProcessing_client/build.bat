cls

del *.exe

del *.res

del *.obj

cl.exe /c /EHsc /D_UNICODE /DUNICODE window.cpp ./src/dialog_edit.cpp ./src/dialog_register.cpp ./src/dialog_about.cpp ./src/helpers.cpp 

rc.exe Window.rc 

link.exe window.obj dialog_edit.obj dialog_register.obj dialog_about.obj helpers.obj Window.res user32.lib gdi32.lib comdlg32.lib ole32.lib winmm.lib /SUBSYSTEM:WINDOWS
