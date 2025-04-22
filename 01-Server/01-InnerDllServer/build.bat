cls

del *.obj

del *.dll

del *.exp

del *.lib

cl.exe /c /EHsc PixEditOneEffectImpl.cpp

link.exe PixEditOneEffectImpl.obj /DLL /DEF:PixEditOneEffectImpl.def user32.lib /SUBSYSTEM:WINDOWS
