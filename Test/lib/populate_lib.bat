cd /D E:\games\hs5\Hearthstone_Data\Managed
for /r %%a in (*.dll) do xcopy /y /i "%%a" "%~dp0"
for /r %%a in (*.xml) do xcopy /y /i "%%a" "%~dp0"
