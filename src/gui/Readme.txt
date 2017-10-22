=============================================================================================

ENGG2800 2017 TEAM01 Software

==============================================================================================
Installing the Software
==============================================================================================
1. 

==============================================================================================
Using the GUI
To open the software, simply double click "Clock.exe"
==============================================================================================
1. Time:
	There are two text inputs and a dropdown used for setting the time.
	The first input is for the hour, the second is for the minutes.
	The dropdown allows you to choose AM/PM.
	
	The time is not saved until the "Set" button is pressed.

	Alternatively you can set the clock to be the current time according to
	your computer. To do this click the "Set to PC Time" button. There is no 
	need to "Set" the time if you have used this method

	
	You can also use the display to "Click and Drag" the time.
	To do this:
	Make sure that the toggle in the top left of the display is set to "Clock"
	and not "Alarm"
	Click and drag the hour indicator to move it
	Click and drag anywhere else in the ring to change the minutes
	Again, there is no need to "Set" the time if you used this method

	Time is only stored on the computer. 
	To set the clock time see section 4 - SENDING
==============================================================================================
2. Alarm:
	In order to set the Alarm, ensure that the Alarm is enabled using the "Alarm ON/OFF"
	toggle
	
	There are two text inputs and a dropdown used for setting the alarm time.
	The first input is for the hour, the second is for the minutes.
	The dropdown allows you to choose AM/PM.
	
	The alarm time is not saved until the "Set" button is pressed.
	
	You can also use the display to "Click and Drag" the time.
	To do this:
	Make sure that the toggle in the top left of the display is set to "Alarm"
	and not "Clock"
	Click and drag the hour indicator to move it
	Click and drag anywhere else in the ring to change the minutes
	There is no need to "Set" the time if you used this method

	Alarm Time is only stored on the computer. 
	To set the alarm time see section 4 - SENDING
==============================================================================================
3. Weather
	If "Auto-Retrieve Weather" is set to ON then the weather will be automatically
	retrieved at the start of every minute and will be displayed on the clock face display
	
	To get the weather manually, simply press the "Get Weather" button.
==============================================================================================
4. Sending
	The clocks data (Time/Alarm/Weather) will only be updated when it has been sent to 
	the clock via the IR Dongle
	
	To send the data, ensure that the dongle is plugged into your computer. The software
	should automatically detect which port the dongle is plugged into. If this is not
	the case, got to File>Choose Port to change the selected port. If the port is not
	in the list select the "Update" button to re-read the available ports.

	Once a port is selected you can press the "Send to Clock" on the main screen to update
	all of the clocks data.
==============================================================================================

Building software
==============================================================================================
1. Make sure that python 3.X is installed on your machine
2. run "install-libs.bat" to install the required libraries
3. run "build.bat" to compile the python script and libraries into an .EXE in the "dist" folder
==============================================================================================
