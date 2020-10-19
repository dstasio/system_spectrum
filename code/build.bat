@echo off
set CommonCompilerFlags=-diagnostics:column -MTd -nologo -Gm- -GR- -EHa- -Od -Oi -WX -W4 -wd4201 -wd4100 -wd4189 -wd4456 -wd4505 -DSYSPEC_INTERNAL=1 -DSYSPEC_DEBUG=1 -FAsc -Z7
set CommonLinkerFlags=-incremental:no -opt:ref user32.lib d3d11.lib
set RendererExports=-EXPORT:win32_load_d3d11 -EXPORT:d3d11_reload_shader -EXPORT:d3d11_load_wexp
REM user32.lib gdi32.lib winmm.lib

IF NOT EXIST ..\build mkdir ..\build
IF NOT EXIST ..\rundir mkdir ..\rundir
IF NOT EXIST ..\rundir\assets mkdir ..\rundir\assets
pushd ..\build

REM 64-bit build
del *.pdb > NUL 2> NUL
cl %CommonCompilerFlags% ..\code\syspec.cpp -Fmsyspec.map -LD /link -incremental:no /PDB:windy_%random%.pdb -EXPORT:UpdateAndRender
cl %CommonCompilerFlags% ..\code\win32_layer.cpp -Fmwin32_syspec.map /link %CommonLinkerFlags% %RendererExports%
popd

pushd ..\rundir\assets
set name=phong
fxc -nologo -Tvs_5_0 -DVERTEX_HLSL=1 -DPIXEL_HLSL=0 ..\..\code\phong.hlsl -Fo%name%v.tmp
fxc -nologo -Tps_5_0 -DVERTEX_HLSL=0 -DPIXEL_HLSL=1 ..\..\code\phong.hlsl -Fo%name%p.tmp
move /Y %name%v.tmp %name%.vsh
move /Y %name%p.tmp %name%.psh

set name=fonts
fxc -nologo -Tvs_5_0 -DVERTEX_HLSL=1 -DPIXEL_HLSL=0 ..\..\code\fonts.hlsl -Fo%name%v.tmp
fxc -nologo -Tps_5_0 -DVERTEX_HLSL=0 -DPIXEL_HLSL=1 ..\..\code\fonts.hlsl -Fo%name%p.tmp
move /Y %name%v.tmp %name%.vsh
move /Y %name%p.tmp %name%.psh
popd

