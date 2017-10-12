
###################################################################
#                                                                 #
#   ENGG2800 Alarm Clock Software                                 #
#                                                                 #
#   Team01                                                        #
#                                                                 #
#   Student Number: 43948378                                      #
#   Student Name: Barney Whiteman                                 #
#                                                                 #
###################################################################

#####################################
# Imports Below                     #
#####################################

import tkinter as tk
from tkinter import *
from tkinter import messagebox
from tkinter import ttk

import serial
import serial.tools.list_ports

import math
import datetime
import time

import urllib.request
import json

#####################################
# Imports End                       #
#####################################

hour_disp = True
hour = 0
minute = 0
am_pm = "PM"
weather = ""
auto = True
draw_wthr = False
frame = 0

port = 'COM1'
serial_ports = []

alarm_set = False

drag = False

alarm = False
al_h = 12
al_m = 0
al_am_pm = "PM"

w_types = ["Sunny", "Cloudy", "Rain", "Windy", "Storm"]
#w_types_long = ["Sunny", "Cloudy", "Rain", "Windy", "Storm", "Sunny and Windy", "Cloudy and Windy", "Cloudy and Rain", "Rain and Windy", "Storm and Windy"]

sun = ["#9EEDFF", "#9EEDFF", "#9EEDFF", "#9EEDFF",
       "#9EEDFF", "#FFD900", "#FFD900", "#9EEDFF",
       "#9EEDFF", "#FFD900", "#FFD900", "#9EEDFF",
       "#9EEDFF", "#9EEDFF", "#9EEDFF", "#9EEDFF",
       
       "#FFD900", "#9EEDFF", "#9EEDFF", "#FFD900",
       "#9EEDFF", "#FFD900", "#FFD900", "#9EEDFF",
       "#9EEDFF", "#FFD900", "#FFD900", "#9EEDFF",
       "#FFD900", "#9EEDFF", "#9EEDFF", "#FFD900"]

cloud = ["#9EEDFF", "#9EEDFF", "#9EEDFF", "#9EEDFF",
       "#9EEDFF", "#FFFFFF", "#FFFFFF", "#9EEDFF",
       "#FFFFFF", "#FFFFFF", "#FFFFFF", "#FFFFFF",
       "#9EEDFF", "#9EEDFF", "#9EEDFF", "#9EEDFF",
       
       "#9EEDFF", "#9EEDFF", "#9EEDFF", "#9EEDFF",
       "#9EEDFF", "#FFFFFF", "#FFFFFF", "#9EEDFF",
       "#FFFFFF", "#FFFFFF", "#FFFFFF", "#FFFFFF",
       "#9EEDFF", "#9EEDFF", "#9EEDFF", "#9EEDFF"]

rain = ["#9EEDFF", "#82827F", "#82827F", "#9EEDFF",
       "#82827F", "#82827F", "#82827F", "#82827F",
       "#0033FF", "#9EEDFF", "#0033FF", "#9EEDFF",
       "#9EEDFF", "#0033FF", "#9EEDFF", "#0033FF",
       
       "#9EEDFF", "#82827F", "#82827F", "#9EEDFF",
       "#82827F", "#82827F", "#82827F", "#82827F",
       "#9EEDFF", "#0033FF", "#9EEDFF", "#0033FF",
       "#0033FF", "#9EEDFF", "#0033FF", "#9EEDFF"]

storm = ["#6074C4", "#82827F", "#82827F", "#6074C4",
       "#82827F", "#82827F", "#82827F", "#82827F",
       "#6074C4", "#6074C4", "#6074C4", "#6074C4",
       "#6074C4", "#6074C4", "#6074C4", "#6074C4",
       
       "#6074C4", "#82827F", "#82827F", "#6074C4",
       "#82827F", "#82827F", "#82827F", "#82827F",
       "#6074C4", "#FFD900", "#6074C4", "#6074C4",
       "#6074C4", "#6074C4", "#FFD900", "#6074C4"]

wind = ["#9EEDFF", "#9EEDFF", "#9EEDFF", "#9EEDFF",
       "#6A9DA8", "#6A9DA8", "#9EEDFF", "#9EEDFF",
       "#9EEDFF", "#9EEDFF", "#9EEDFF", "#9EEDFF",
       "#9EEDFF", "#9EEDFF", "#6A9DA8", "#6A9DA8",
       
       "#9EEDFF", "#9EEDFF", "#9EEDFF", "#9EEDFF",
       "#9EEDFF", "#9EEDFF", "#6A9DA8", "#6A9DA8",
       "#9EEDFF", "#9EEDFF", "#9EEDFF", "#9EEDFF",
       "#6A9DA8", "#6A9DA8", "#9EEDFF", "#9EEDFF"]
       


class ClockView(tk.Canvas):
    """Inherits from tkinter Canvas class
    used to display graphs of station data
    """
    def __init__(self, master, parent):
        """initializes the internal data

        Constructor: ClockView(tk.Tk(), ClockApp())
        """
        super().__init__(master, bg="#999", relief=tk.SUNKEN)
        self._parent = parent
        self.bind('<Configure>', self.resize)
        self.bind('<B1-Motion>', self.click_drag)
        self.bind('<Button-1>', self.down)
        self.bind('<ButtonRelease-1>', self.up)
        
        #variables for click and drag time setting
        self._in_range = False
        self._set_hour = False
        self._tmp_hr = 0
        self._tmp_min = 0
        self._r = 0

        #variables for alarm toggle switch
        self._s = 30
        self._sx = 10
        self._sy = 10
        

    def draw_clock(self):
        """Takes the station data and uses the CoordinateTranslator class
        to convert temperatures for all the stations into coordinates

        draw_clock() -> None (clock on canvas)
        """
        self.delete(tk.ALL)
        self._x = self.winfo_width()
        self._y = self.winfo_height()

        global drag
        global alarm_set

        #Draw alarm switch
        self.create_rectangle(self._sx, self._sy, self._sx + self._s * 2, self._sy + self._s, outline="#000", fill="#fff", width = 1)
        self.create_text(self._sx + self._s * (1/2), self._sy + self._s * (1/2), text="A")
        self.create_text(self._sx + self._s * (3/2), self._sy + self._s * (1/2), text="T")
        if(alarm_set):
            x = self._sx + self._s
        else:
            x = self._sx
        self.create_rectangle(x, self._sy, x + self._s, self._sy + self._s, fill="#555")
        
        #Drawing ring
        cx = self._x/2
        cy = self._y/2

        lw = 1
        if(alarm_set):
            h = al_h
            m = al_m
            ap = al_am_pm
        else:
            h = hour
            m = minute
            ap = am_pm
        
        a = math.pi/6
        off = math.pi/2
        rad = (min(self._x, self._y) - 50)/32
        r = (min(self._x, self._y) - 50)/2
        self._r = r
        c = "#fff"
        for i in range(12):
            h = h % 12
            if(i == h and (hour_disp or drag)):
                c = "#f0f"
            elif(i <= m / 5):
                if(ap == "AM"):
                    c = "#0f0"
                else:
                    c = "#ff0"
            else:
                c = "#fff"
            self.draw_led(cx + math.cos(a*i - off)*r, cy + math.sin(a*i - off)*r, rad, (a*i - off), outline="#000", fill=c, width=lw)

        #Drawing square
        spc = (r - 25)/4
        rem = m % 5
        for i in range(4):
            for j in range(4):
                if(draw_wthr):
                    if(weather == "Sunny"):
                        f = (frame%2) + 1
                        if(f == 2):
                            x = 16
                        else:
                            x = 0
                        c = sun[(i + j * 4) + x]
                    elif(weather == "Cloudy"):
                        f = (frame%2) + 1
                        if(f == 2):
                            x = 16
                        else:
                            x = 0
                        c = cloud[(i + j * 4) + x]
                    elif(weather == "Rain"):
                        f = (frame%2) + 1
                        if(f == 2):
                            x = 16
                        else:
                            x = 0
                        c = rain[(i + j * 4) + x]
                    elif(weather == "Storm"):
                        f = (frame%2) + 1
                        if(f == 2):
                            x = 16
                        else:
                            x = 0
                        c = storm[(i + j * 4) + x]
                    elif(weather == "Windy"):
                        f = (frame%2) + 1
                        if(f == 2):
                            x = 16
                        else:
                            x = 0
                        c = wind[(i + j * 4) + x]
                    elif(weather == "Sunny and Windy"):
                        f = (frame%2) + 1
                        if(f == 2):
                            x = 16
                        else:
                            x = 0
                        if(frame < 4):
                            c = sun[(i + j * 4) + x]
                        else:
                            c = wind[(i + j * 4) + x]
                    elif(weather == "Cloudy and Windy"):
                        f = (frame%2) + 1
                        if(f == 2):
                            x = 16
                        else:
                            x = 0
                        if(frame < 4):
                            c = cloud[(i + j * 4) + x]
                        else:
                            c = wind[(i + j * 4) + x]
                    elif(weather == "Cloudy and Rain"):
                        f = (frame%2) + 1
                        if(f == 2):
                            x = 16
                        else:
                            x = 0
                        if(frame < 4):
                            c = cloud[(i + j * 4) + x]
                        else:
                            c = rain[(i + j * 4) + x]
                    elif(weather == "Rain and Windy"):
                        f = (frame%2) + 1
                        if(f == 2):
                            x = 16
                        else:
                            x = 0
                        if(frame < 4):
                            c = rain[(i + j * 4) + x]
                        else:
                            c = wind[(i + j * 4) + x]
                    elif(weather == "Storm and Windy"):
                        f = (frame%2) + 1
                        if(f == 2):
                            x = 16
                        else:
                            x = 0
                        if(frame < 4):
                            c = storm[(i + j * 4) + x]
                        else:
                            c = wind[(i + j * 4) + x]
                    
                else:
                    if(i == 0 and j == 0 and rem > 0):
                        if(ap == "AM"):
                            c = "#00ff00"
                        else:
                            c = "#ffff00"
                    elif(i == 3 and j == 0 and rem > 1):
                        if(ap == "AM"):
                            c = "#00ff00"
                        else:
                            c = "#ffff00"
                    elif(i == 0 and j == 3 and rem > 2):
                        if(ap == "AM"):
                            c = "#00ff00"
                        else:
                            c = "#ffff00"
                    elif(i == 3 and j == 3 and rem > 3):
                        if(ap == "AM"):
                            c = "#00ff00"
                        else:
                            c = "#ffff00"
                    else:
                        c = "#ffffff"

                        
                self.draw_led(cx - (1.5 * spc + 3 * rad) + (2 * rad + spc) * i, cy - (1.5 * spc + 3 * rad) + (2 * rad + spc) * j, rad, 0, outline="#000000", fill=c, width=lw)

        
    def draw_led(self, x, y, r, a, **kwargs):
        """Creates a square at position (x, y) with a side distace from centre to corner of r
        and a rotation of a radians. **kwargs holds details about fill colour, line size and colour etc

        draw_led(self, x, y, r, a, **kwargs) -> None (Draws square on canvas)
        """
        a += math.pi/4
        
        x0 = x + math.cos(a) * r
        y0 = y + math.sin(a) * r

        x1 = x + math.cos(a + (math.pi/2)) * r
        y1 = y + math.sin(a + (math.pi/2)) * r

        x2 = x + math.cos(a + (math.pi)) * r
        y2 = y + math.sin(a + (math.pi)) * r

        x3 = x + math.cos(a + (3 * math.pi/2)) * r
        y3 = y + math.sin(a + (3 * math.pi/2)) * r       

        
        return self.create_polygon(x0, y0, x1, y1, x2, y2, x3, y3, **kwargs)

        
    
    def resize(self, e):
        """Updates the graph when the window has been resized

        resize(event) -> None
        """
        try:
            self.draw_clock()
        except:
            pass

    def point_in_switch(self, x, y):
        return(x >= self._sx and x <= self._sx + self._s * 2 and y >= self._sy and y <= self._sy + self._s)

    def up(self, event):
        """When the interface is no longer being dragged on, reset the drag variable

        upevent) -> None
        """
        global drag
        global alarm_set
        drag = False
        if(self.point_in_switch(event.x, event.y)):
            alarm_set = not alarm_set
    
    def down(self, event):
        """Checks to see if the mouse pointer (when initially pressed) is
        in the correct radius and whether it is changing the hour or minute

        down(event) -> None (sets flags describing if it's in range and whether
        to change the minute or hour)
        """
        global drag

        if(alarm_set):
            global al_h
            h = al_h
        else:
            global hour
            h = hour
        
        self._x = self.winfo_width()
        self._y = self.winfo_height()
        cx = self._x/2
        cy = self._y/2

        x = event.x
        y = event.y

        dy = cy - y
        dx = cx - x
        
        r = math.sqrt((dx ** 2) + (dy ** 2))
        a = math.atan2(dy, dx) % (2 * math.pi)

        h = (h % 12) + 3
        h_a = (math.pi/6 * h) % (2 * math.pi)

        if(r < self._r + 10 and r > self._r - 10):
            self._in_range = True
        else:
            self._in_range = False

        if(a < h_a + 0.1 and a > h_a - 0.1):
            self._set_hour = True
            drag = True
        else:
            self._set_hour = False
    
    def click_drag(self, event):
        """Changes the time based on the mouses position

        click_drag(event) -> None (changes the hour/minute values and updates display)
        """
        if(self._in_range):
            self._x = self.winfo_width()
            self._y = self.winfo_height()
            cx = self._x/2
            cy = self._y/2

            x = event.x
            y = event.y

            dy = cy - y
            dx = cx - x
            a = math.atan2(dy, dx) % (2 * math.pi)
            if(alarm_set):
                global al_h
                global al_m
                global al_am_pm
                h = al_h
                m = al_m
                ap = al_am_pm
            else:
                global hour
                global minute
                global am_pm
                h = hour
                m = minute
                ap = am_pm
            if(self._set_hour):
                ph = h
                h = self.hour_from_ang(a)
                if((ph == 12 and h == 11) or (ph == 11 and h == 12)):
                    if(ap == "AM"):
                        ap = "PM"
                    else:
                        ap = "AM"
                if(alarm_set):
                    al_h = h
                    al_am_pm = ap
                else:
                    hour = h
                    am_pm = ap
                self._parent._select.reset_input()
                self.draw_clock
            else:
                m = self.minute_from_ang(a)
                if(alarm_set):
                    al_m = m
                else:
                    minute = m
                self._parent._select.reset_input()
                self.draw_clock
        
    def hour_from_ang(self, angle):
        """Gets the hour based on the angle on the clock face

        hour_from_ang(float) -> int (Hour)
        """
        pi = math.pi
        hr = math.ceil((((angle / (2 * pi)) * 12) - 3.5)) % 12
        if(hr == 0):
            hr = 12
        return hr
    
    def minute_from_ang(self, angle):
        """Gets the minute based on the angle on the clock face

        minute_from_ang(float) -> int (Hour)
        """
        pi = math.pi
        return math.ceil((((angle / (2 * pi)) * 60) - 15.5)) % 60

class SelectionFrame(tk.Frame):
    """A class that inherits from the tkinter Frame class

    creates and displays the inputs for the clock. (Set time/get time, weather query
    and display, set alarm, etc)
    """
    def __init__(self,master,parent):
        """initializes the internal data

        Constructor: SelectionFrame(tk.Tk(), TemperaturePlotApp())
        """
        super().__init__(master)
        self._parent = parent
        #Time selection
        self._select = ttk.Label(self, text = "Time:")
        self._timeLabel = ttk.Label(self, text = " : ")

        self._hr = StringVar(self, value = str(hour))
        if(minute < 10):
            self._mn = StringVar(self, value = "0" + str(minute))
        else:
            self._mn = StringVar(self, value = str(minute))
        
        self._hour = ttk.Entry(self, width = 3, textvariable = self._hr)
        self._min =  ttk.Entry(self, width = 3, textvariable = self._mn)
        self._set = ttk.Button(self, text = "Set", command = self.set_time)

        self._choices = {"", "PM", "AM"}
        self._am_pm = StringVar(self, value = am_pm)
        self._am_pm_option = ttk.OptionMenu(self, self._am_pm, *self._choices)
        self._am_pm_option.config(width = 3)

        self._select.grid(row = 0, column = 0, pady = 10)
        self._set.grid(row = 1, column = 0, sticky = 'ew', pady = 10)
        self._hour.grid(row = 1, column = 1)
        self._timeLabel.grid(row = 1, column = 2)
        self._min.grid(row = 1, column = 3)
        self._am_pm_option.grid(row = 1, column = 4, sticky = 'ew')
        

        #Get current time button
        self._get_time = ttk.Button(self, text = "Set to PC Time", command = self.get_time);
        self._get_time.grid(row = 3, column = 0, columnspan = 5, sticky = 'ew', pady = 10)

        #Weather display
        self._weather = ttk.Label(self, text = weather, width = 20, justify = LEFT)
        self._request_weather = ttk.Button(self, text = "Get Weather", command = self.update_weather)        
        self._auto_weather = ttk.Button(self, text = "Auto-Retrieve Weather: ON", command = self.toggle_auto)
        self._weather.grid(row = 4, column = 0, columnspan = 5, pady = 5)
        self._request_weather.grid(row = 5, column = 0, columnspan = 5, sticky = 'ew', pady = 10)
        self._auto_weather.grid(row = 6, column = 0, columnspan = 5, sticky = 'ew', pady = 10)

        #Alarm setting
        self._alarm = ttk.Label(self, text = "Alarm:")
        self._al_label = ttk.Label(self, text = " : ")

        self._al_hr = StringVar(self, value = str(al_h))
        self._al_mn = StringVar(self, value = str(al_m))
        
        self._al_hour = ttk.Entry(self, width = 3, textvariable = self._al_hr, state = 'disabled')
        self._al_min =  ttk.Entry(self, width = 3, textvariable = self._al_mn, state = 'disabled')
        self._al_set = ttk.Button(self, text = "Set", command = self.set_alarm, state = 'disabled')

        self._al_am_pm = StringVar(self, value = al_am_pm)
        self._al_am_pm_option = ttk.OptionMenu(self, self._al_am_pm, *self._choices)
        self._al_am_pm_option.config(state = 'disabled')
        self._al_am_pm_option.config(width = 3)
        self._alarm_switch = ttk.Button(self, text = "Alarm: OFF", command = self.toggle_alarm)

        self._alarm.grid(row = 7, column = 0, pady = 10)
        self._al_set.grid(row = 8, column = 0, sticky = 'ew', pady = 10)
        self._al_hour.grid(row = 8, column = 1)
        self._al_label.grid(row = 8, column = 2)
        self._al_min.grid(row = 8, column = 3)
        self._al_am_pm_option.grid(row = 8, column = 4, sticky = 'ew')
        self._alarm_switch.grid(row = 9, column = 0, columnspan = 5, sticky = 'ew', pady = 10)

        self._send = ttk.Button(self, text = "Send to Clock", command = self.send)
        self._send.grid(row = 10, column = 0, rowspan = 2, columnspan = 5, sticky = 'ewns', pady = 10)
        
    def send(self):
        """Sends packet serial -> FTDI -> IR -> Clock

        send() -> None (Sends data to usb port to send time, alarm time, etc to clock)
        """
        print("send data")
        sendToClock()

    def toggle_auto(self):
        """Toggles whether weather retrieval is automatic or not

    
        toggle_auto() -> None (Changes settings)
        """
        global auto
        auto = not auto
        print(auto)
        if(auto):
            self._auto_weather.config(text = "Auto-Retrieve Weather: ON")
            self.update_weather()
        else:
            self._auto_weather.config(text = "Auto-Retrieve Weather: OFF")

    def toggle_alarm(self):
        """Toggles whether the alarm is on or off

    
        toggle_alarm() -> None (Changes settings)
        """
        global alarm
        alarm = not alarm
        if(alarm):
            self._alarm_switch.config(text = "Alarm: ON")

            self._al_hour.config(state = 'enabled')
            self._al_min.config(state = 'enabled')
            self._al_set.config(state = 'enabled')
            self._al_am_pm_option.config(state = 'enabled')
        else:
            self._alarm_switch.config(text = "Alarm: OFF")

            self._al_hour.config(state = 'disabled')
            self._al_min.config(state = 'disabled')
            self._al_set.config(state = 'disabled')
            self._al_am_pm_option.config(state = 'disabled')
    

    def reset_input(self):
        """Resets the value in the input

        reset_input() -> None (Changes the display to reflect the stored hr/min vals)
        """
        if(minute < 10):
            self._mn = StringVar(self, value = "0" + str(minute))
        else:
            self._mn = StringVar(self, value = str(minute))
        self._hr = StringVar(self, value = str(hour))
        self._hour.config(textvariable = self._hr)
        self._min.config(textvariable = self._mn)
        self._am_pm.set(am_pm);
        
        if(al_m < 10):
            self._al_mn = StringVar(self, value = "0" + str(al_m))
        else:
            self._al_mn = StringVar(self, value = str(al_m))
        self._al_hr = StringVar(self, value = str(al_h))
        self._al_hour.config(textvariable = self._al_hr)
        self._al_min.config(textvariable = self._al_mn)
        self._al_am_pm.set(al_am_pm);

    def update_weather(self):
        """Uses the get weather function and updates the gui to display current weather

        update_weather() -> None (Updates the gui)
        """
        global weather
        global draw_wthr
        weather = getWeather();
        draw_wthr = True;
        self._weather.config(text = weather)

    def get_time(self):
        """Uses the date_time library to get the current computer time and updates the clock

        get_time() -> None (Updates display and time variables)
        """
        global hour
        global minute
        global am_pm
        time = datetime.datetime.now().time()
        if(time.hour >= 12):
            hour = time.hour - 12;
            am_pm = "PM"
        else:
            hour = time.hour
            am_pm = "AM"
        if(hour == 0):
            hour = 12
        minute = time.minute
        self._parent._clock.draw_clock()
        self.reset_input()
        
        
    def set_time(self):
        """Gets values from the inputs in the menu

        set_time() -> None (Update time and display)
        """
        time = [self._hour.get(), self._min.get(), self._am_pm.get()]
        
        global hour
        global minute
        global am_pm
        ap = time[2]
        if(time[0] == ''): time[0] = '0'
        if(time[1] == ''): time[1] = '0'
        if(is_int(time[0]) and is_int(time[1])):
            h = int(time[0])
            m = int(time[1])
            if((h < 13 and h >= 0) and (m < 60 and m >= 0)):
                hour = h
                minute = m
                am_pm = ap
                if(hour == 0):
                    hour = 12
                    h = 12
                self._parent._clock.draw_clock()
                self.reset_input()
            else:
                messagebox.showerror("Invalid Input", "Invalid input: " + str(h) + ":" + str(m) + " " + ap + "\n Please ensure you enter numbers\n"
                                     "Between 1 and 12 for the hour and\n Between 0 and 59 for the minute")
                self.reset_input()
        else:
                messagebox.showerror("Invalid Input", "Invalid input: " + str(time[0]) + ":" + str(time[1]) + " " + ap + "\n Please ensure you enter numbers\n"
                                     "Between 1 and 12 for the hour and\nBetween 0 and 59 for the minute")
                self.reset_input()


    def set_alarm(self):
        """Gets values from the inputs in the menu

        set_alarm() -> None (Update time and display)
        """
        time = [self._al_hour.get(), self._al_min.get(), self._al_am_pm.get()]
        
        global al_h
        global al_m
        global al_am_pm
        ap = time[2]

        
        if(time[0] == ''): time[0] = '0'
        if(time[1] == ''): time[1] = '0'

        if(is_int(time[0]) and is_int(time[1])):
            h = int(time[0])
            m = int(time[1])
            if((h < 13 and h >= 0) and (m < 60 and m >= 0)):
                al_h = h
                al_m = m
                al_am_pm = ap
                if(hour == 0):
                    al_h = 12
                    h = 12
                self._parent._clock.draw_clock()
                self.reset_input()
            else:
                messagebox.showerror("Invalid Input", "Invalid input: " + str(h) + ":" + str(m) + " " + ap + "\n Please ensure you enter numbers\n"
                                     "Between 1 and 12 for the hour and\n Between 0 and 59 for the minute")
                self.reset_input()
        else:
                messagebox.showerror("Invalid Input", "Invalid input: " + str(time[0]) + ":" + str(time[1]) + " " + ap + "\n Please ensure you enter numbers\n"
                                     "Between 1 and 12 for the hour and\nBetween 0 and 59 for the minute")
                self.reset_input()


class ClockApp(object):
    """Main class that keeps all the other classes together (Base class)
    Creates all instances of the other classes
    """
    def __init__(self, master):
        """initializes the internal data
        Creates instances of the following classes:
        SelectionFrame, DataFrame, temperatureData, Plotter

        constructs the gui (packs all the frames & canvases)

        Constructor: ClockApp(tk.Tk())
        """
        self._master = master
        master.minsize(854, 480)

        self._select = SelectionFrame(master, self)
        self._select.pack(side=tk.LEFT, anchor=tk.N)

        self._clock = ClockView(master, self)
        self._clock.pack(side=tk.RIGHT,expand=1,fill=tk.BOTH)

        

        self.port_menu()
        
        """for p in serial_ports:
            if(p == port):
                p = ">" + p
            self.portlist.add_command(label = p, command = lambda:self.set_port(p))
        self.portlist.add_command(label = "Update", command = self.update)"""
        

    def set_port(self, port_name):
        
        global port
        port = port_name
        print("port set to: " + port)
        self.port_menu()

    def update(self):
        print("updated port list")
        getPorts()
        self.port_menu()

    def port_menu(self):
        global serial_ports
        global port
        self.menubar = Menu(self._master, tearoff=False)
        self._master.config(menu = self.menubar)

        self.ports = Menu(self.menubar, tearoff=False)
        self.portlist = Menu(self.ports, tearoff=False)
        
        self.menubar.add_cascade(label="File", menu = self.ports)
        self.ports.add_cascade(label = "Choose port", menu = self.portlist)
        i = 0
        for p in serial_ports:
            if(p == port):
                self.portlist.add_command(label = p, command = lambda p = p: self.set_port(p))
                self.portlist.entryconfig(i, foreground = 'red')
            else:
                self.portlist.add_command(label = p, command = lambda p = p: self.set_port(p))
            i += 1
        
        self.portlist.add_command(label = "Update", command = self.update)
    
def getPorts():
    """
    Detects all serial ports on the computer and creates a list that the user can select from

    getPorts() -> None(updates list)
    """
    global serial_ports
    ports = serial.tools.list_ports.comports()
    connected = []
    for element in ports:
        connected.append(element.device)
    print("Connected COM ports: " + str(connected))

    serial_ports = connected

def sendToClock():
    """
    Uses the pyserial library to send data to the clock via FTDI chip
    over IR

    sendToClock(str[] args) -> None (Data sent to the clock)
    """

    #Weather
    start = 0
    end = 0
    i = 0
   
    for w in w_types:
        if(weather.startswith(w)):
            start = i
        if(weather.endswith(w)):
            end = i
        i += 1
    
    print(weather + " is -> " + str(start) + " : " + str(end))

    #Time
    h = hour
    m = minute
    s = datetime.datetime.now().time().second 
    if(am_pm == "PM"):
        if(h != 12):
            h += 12
            h = h % 24
    elif(h == 12):
        h = 0

    #Alarm
    alh = al_h
    alm = al_m
    als = 0
    if(al_am_pm == "PM"):
        if(alh != 12):
            alh += 12
            alh = alh % 24
    elif(alh == 12):
        alh = 0
    
    
    val = bytearray([start, end, alh, alm, als, h, m, s])
    
    for v in val:
        print(str(v))
        
    try:
        ser = serial.Serial(port, 300, serial.EIGHTBITS, serial.PARITY_NONE, serial.STOPBITS_TWO)
        for i in range(5):
            ser.write(val)
        print("sent")
    except:
        print("error sending, please check you have selected the correct port")

def getTime():
    """
    gets current time from computer

    getTime() -> array [hour, minute, seconds, milliseconds]
    """
    return datetime.datetime.now().time()

def millis():
    return int(round(time.time() * 1000))

def getWeather():
    """Sends an HTTP request to the engg2800-weather.uqcloud.net on port 80
    and asks for 'weather.json'.

    getWeather() -> array describing weather (e.g. ["windy", "storm"])
    
    """
    try:
        request = urllib.request.urlopen("http://engg2800-weather.uqcloud.net/weather.json", data=None)
        output = request.read()
        
        data = str(output)[2:-1] #Strip the quotes it comes in to allow for JSON parsing
        weather_list = json.loads(data)
        weather = weather_list['weather']#['main']
        request.close;
        return(weather)
    except:
        print("No Connection")

def is_int(x):
    """Determine if input into time fields are numbers"""
    try:
        x = int(x)
        return True
    except:
        return False

def main():
    global weather
    global hour_disp
    global hour
    global minute
    global am_pm
    global auto
    global draw_wthr
    global frame
    global serial_ports
    global port
    weather = getWeather()
    getPorts()
    try:
        port = serial_ports[0]
    except:
        print("no devices found")
    root = tk.Tk()
    app = ClockApp(root)
    root.geometry("640x480")

    old = millis()
    f = millis()
    s = datetime.datetime.now().time().second
    app._select.get_time()
    s_set = False
    x = 0
    while 1:
        cur = millis()
        try:
            root.update_idletasks()
            root.update()
        except:
            break
        if(cur - old > 500):
            root.title("Clk | " + str(x/(0.5)) + " fps")
            x = 0
            hour_disp = not hour_disp
            if(draw_wthr):
                frame += 1
            if(hour_disp):
                s = datetime.datetime.now().time().second
                if(s != 0):
                    s_set = False
            if(s == 0 and not s_set):
                s_set = True
                if(auto):
                    app._select.update_weather()
                minute += 1
                if(minute == 60):
                    minute = 0
                    hour += 1
                if(hour == 12):
                    if(am_pm == "AM"):
                        am_pm = "PM"
                    else:
                        am_pm = "AM"
                if(hour == 13):
                    hour = 1
                app._select.reset_input()
            if(s < 4):
                draw_wthr = True
            if(draw_wthr == True and frame > 7):
                draw_wthr = False
                frame = 0
            old = cur
        if(cur - f > (500/30)):
            x += 1
            f = cur
            app._clock.draw_clock()

if __name__ == '__main__':
    main()
