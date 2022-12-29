@ECHO OFF

REM change console screen
COLOR F0

REM source folder location
set Source_Folder="C:\Program Files\Graphviz\bin"
REM destination folder location
set Dest_Folder="C:\Users\WIN\Documents\Qt programs\QPetriNets\Executable file"
REM Ask file name from user
set FileName=gvc.dll cdt.dll cgraph.dll gvplugin_dot_layout.dll config6 expat.dll Pathplan.dll xdot.dll
REM copy the files
for %%a in (%FileName%) do (
	copy %Source_Folder%\*%%a* %Dest_Folder%\.
)