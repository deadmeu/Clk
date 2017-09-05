
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

class ClockView(tk.Canvas):
    """Inherits from tkinter Canvas class
    used to display graphs of station data
    """
    def __init__(self, master, parent):
        """initializes the internal data

        Constructor: ClockView(tk.Tk(), ClockApp())
        """
        super().__init__(master,bg="white",relief=tk.SUNKEN)
        self._parent = parent
        self.bind('<Configure>', self.resize)
        self.bind('<B1-Motion>', self.click_drag)
        self.bind('<Button-1>', self.down)
        
        #variables for click and drag time setting
        self._in_range = False
        self._set_hour = False
        self._tmp_hr = 0
        self._tmp_min = 0
        self._r = 0
        

    def draw_clock(self):
        """Takes the station data and uses the CoordinateTranslator class
        to convert temperatures for all the stations into coordinates

        draw_clock() -> None (clock on canvas)
        """
        self.delete(tk.ALL)
        self._x = self.winfo_width()
        self._y = self.winfo_height()

        #Drawing ring
        cx = self._x/2
        cy = self._y/2

        a = math.pi/6
        off = math.pi/2
        rad = 10
        r = (min(self._x, self._y) - 50)/2
        self._r = r
        c = "#fff"
        for i in range(12):
            h = hour % 12
            if(i == h and hour_disp):
                c = "#f0f"
            elif(i <= minute / 5):
                if(am_pm == "AM"):
                    c = "#0f0"
                else:
                    c = "#ff0"
            else:
                c = "#fff"
            self.draw_led(cx + math.cos(a*i - off)*r, cy + math.sin(a*i - off)*r, rad, (a*i - off), outline="#000", fill=c, width=2)

        #Drawing square
        spc = (r - 25)/4
        rem = minute % 5
        for i in range(4):
            for j in range(4):
                if(i == 0 and j == 0 and rem > 0):
                    if(am_pm == "AM"):
                        c = "#0f0"
                    else:
                        c = "#ff0"
                elif(i == 3 and j == 0 and rem > 1):
                    if(am_pm == "AM"):
                        c = "#0f0"
                    else:
                        c = "#ff0"
                elif(i == 0 and j == 3 and rem > 2):
                    if(am_pm == "AM"):
                        c = "#0f0"
                    else:
                        c = "#ff0"
                elif(i == 3 and j == 3 and rem > 3):
                    if(am_pm == "AM"):
                        c = "#0f0"
                    else:
                        c = "#ff0"
                else:
                    c = "#fff"
                self.draw_led(cx - (1.5 * spc + 3 * rad) + (2 * rad + spc) * i, cy - (1.5 * spc + 3 * rad) + (2 * rad + spc) * j, rad, 0, outline="#000", fill=c, width=2)

        
    def draw_led(self, x, y, r, a, **kwargs):

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

    def down(self, event):
        """Checks to see if the mouse pointer (when initially pressed) is
        in the correct radius and whether it is changing the hour or minute

        down(event) -> None (sets flags describing if it's in range and whether
        to change the minute or hour)
        """
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

        h = (hour % 12) + 3
        h_a = (math.pi/6 * h) % (2 * math.pi)
        print(str(h_a) + ":" + str(a))

        if(r < self._r + 10 and r > self._r - 10):
            self._in_range = True
        else:
            self._in_range = False

        if(a < h_a + 0.1 and a > h_a - 0.1):
            self._set_hour = True
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

            if(self._set_hour):
                global hour
                global am_pm
                ph = hour
                hour = self.hour_from_ang(a)
                if((ph == 12 and hour == 11) or (ph == 11 and hour == 12)):
                    if(am_pm == "AM"):
                        am_pm = "PM"
                    else:
                        am_pm = "AM"
                self._parent._select.reset_input()
                self.draw_clock
            else:
                global minute
                minute = self.minute_from_ang(a)
                self._parent._select.reset_input()
                self.draw_clock
            #print(str(x) + ":" + str(y))
        
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
        self._mn = StringVar(self, value = str(minute))
        
        self._hour = ttk.Entry(self, width = 2, textvariable = self._hr)
        self._min =  ttk.Entry(self, width = 2, textvariable = self._mn)
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
        self._request_weather = ttk.Button(self, text = "Get Weather", command = self.update_weather);
        self._weather.grid(row = 4, column = 0, columnspan = 5, pady = 5)
        self._request_weather.grid(row = 5, column = 0, columnspan = 5, sticky = 'ew', pady = 10)

    def reset_input(self):
        """Resets the value in the input

        reset_input() -> None (Changes the display to reflect the stored hr/min vals)
        """
        if(not self._hour.get() == str(hour) or not self._min.get() == str(minute) ):
            self._hr = StringVar(self, value = str(hour))
            self._mn = StringVar(self, value = str(minute))
            self._hour.config(textvariable = self._hr)
            self._min.config(textvariable = self._mn)
            self._am_pm.set(am_pm);

            print(str(hour) + ":" +  str(minute) + ":" + am_pm)

    def update_weather(self):
        """Uses the get weather function and updates the gui to display current weather

        update_weather() -> None (Updates the gui)
        """
        global weather
        weather = get_weather();
        print(weather);
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

        get_time() -> None (Update time and display)
        """
        time = [self._hour.get(), self._min.get(), self._am_pm.get()]
        
        global hour
        global minute
        global am_pm
        ap = time[2]

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
                messagebox.showinfo("Time Set", "Time set to " + str(h) + ":" + str(m) + " " + ap)
                print("Time set to " + str(h) + ":" + str(m) + " " + ap)
            else:
                messagebox.showerror("Invalid Input", "Invalid input: " + str(h) + ":" + str(m) + " " + ap + "\n Please ensure you enter numbers\n"
                                     "Between 1 and 12 for the hour and\n Between 0 and 59 for the minute")
                print("Invalid input: " + str(h) + ":" + str(m) + " " + ap)
                self.reset_input()
        else:
                messagebox.showerror("Invalid Input", "Invalid input: " + str(time[0]) + ":" + str(time[1]) + " " + ap + "\n Please ensure you enter numbers\n"
                                     "Between 1 and 12 for the hour and\nBetween 0 and 59 for the minute")
                print("Invalid input: " + str(time[0]) + ":" + str(time[1]) + " " + ap)
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
        master.title("Clk")
        master.minsize(500, 375)

        self._select = SelectionFrame(master, self)
        self._select.pack(side=tk.LEFT, anchor=tk.N)

        self._clock = ClockView(master, self)
        self._clock.pack(side=tk.RIGHT,expand=1,fill=tk.BOTH)


def getTime():
    """
    gets current time from computer

    getTime() -> array [hour, minute, seconds, milliseconds]
    """
    return datetime.datetime.now().time()

def millis():
    return int(round(time.time() * 1000))

def get_weather():
    """Sends an HTTP request to the engg2800-weather.uqcloud.net on port 80
    and asks for 'weather.json'.

    getWeather() -> array describing weather (e.g. ["windy", "storm"])
    
    """
    request = urllib.request.urlopen("http://engg2800-weather.uqcloud.net/weather.json", data=None)
    output = request.read()
    
    data = str(output)[2:-1] #Strip the quotes it comes in to allow for JSON parsing
    weather_list = json.loads(data)
    weather = weather_list['weather']#['main']
    request.close;

    return(weather)

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
    weather = get_weather()
    root = tk.Tk()
    app = ClockApp(root)
    root.geometry("640x480")

    old = millis()
    s = datetime.datetime.now().time().second
    app._select.get_time()
    s_set = False
    
    while 1:
        cur = millis()
        try:
            root.update_idletasks()
            root.update()
        except:
            break
        if(cur - old > 500):
            hour_disp = not hour_disp
            if(hour_disp):
                s = datetime.datetime.now().time().second
                if(s != 0):
                    s_set = False
            if(s == 0 and not s_set):
                s_set = True
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
            app._clock.draw_clock()
            old = cur

if __name__ == '__main__':
    main()
