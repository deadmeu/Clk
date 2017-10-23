@echo off

del "C:\Program Files\Clock"
mkdir "C:\Program Files\Clock"

xcopy /y "%~dp0dist\Clock.exe" "C:\Program Files\Clock"
xcopy /y "%~dp0Readme.txt" "C:\Program Files\Clock"
xcopy /y "%~dp0icon.ico" "C:\Program Files\Clock"


set SCRIPT="%TEMP%\%RANDOM%-%RANDOM%-%RANDOM%-%RANDOM%.vbs"

echo Set oWS = WScript.CreateObject("WScript.Shell") >> %SCRIPT%
echo sLinkFile = "%PUBLIC%\Desktop\Clock.lnk" >> %SCRIPT%
echo Set oLink = oWS.CreateShortcut(sLinkFile) >> %SCRIPT%
echo oLink.TargetPath = "C:\Program Files\Clock\Clock.exe" >> %SCRIPT%
echo oLink.IconLocation = "C:\Program Files\Clock\icon.ico" >> %SCRIPT%
echo oLink.Save >> %SCRIPT%

cscript /nologo %SCRIPT%
del %SCRIPT%

xcopy /y "%PUBLIC%\Desktop\Clock.lnk" "%USERPROFILE%\AppData\Roaming\Microsoft\Windows\Start Menu\Programs\Clock"

pause