cls

del *.obj

del *.res

del *.exe

cl.exe /c /EHsc Window.cpp

rc.exe Window.rc

link.exe Window.obj Window.res user32.lib gdi32.lib ole32.lib shlwapi.lib /SUBSYSTEM:WINDOWS                                                                                                                                                                                  