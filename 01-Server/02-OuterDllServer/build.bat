cls

del *.obj

del *.dll

del *.exp

del *.lib

cl.exe /c /EHsc PixEditTwoEffectsImpl.cpp

link.exe PixEditTwoEffectsImpl.obj /DLL /DEF:PixEditTwoEffectsImpl.def user32.lib ole32.lib /SUBSYSTEM:WINDOWS
