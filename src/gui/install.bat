@echo off

mkdir "C:\Program Files\Clock"

xcopy "%~dp0dist\Clock.exe" "C:\Program Files\Clock"
xcopy "%~dp0Readme.txt" "C:\Program Files\Clock"
xcopy "%~dp0icon.ico" "C:\Program Files\Clock"


set SCRIPT="%TEMP%\%RANDOM%-%RANDOM%-%RANDOM%-%RANDOM%.vbs"

echo Set oWS = WScript.CreateObject("WScript.Shell") >> %SCRIPT%
echo sLinkFile = "%USERPROFILE%\Desktop\Clock.lnk" >> %SCRIPT%
echo Set oLink = oWS.CreateShortcut(sLinkFile) >> %SCRIPT%
echo oLink.TargetPath = "C:\Program Files\Clock\Clock.exe" >> %SCRIPT%
echo oLink.IconLocation = "C:\Program Files\Clock\icon.ico" >> %SCRIPT%
echo oLink.Save >> %SCRIPT%

cscript /nologo %SCRIPT%
del %SCRIPT%
pause