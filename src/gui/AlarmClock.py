
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

import math
import datetime
import time

import urllib.request
import json

#####################################
# Imports End                       #
#####################################

hour_disp = True
hour = 2
minute = 20
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
        c = "#fff"
        for i in range(12):
            if(i == hour and hour_disp):
                c = "#f0f"
            elif(i <= minute / 5):
                if(am_pm == "AM"):
                    c = "#0f0"
                else:
                    c = "#ff0"
            else:
                c = "#fff"
            self.create_circle(cx + math.cos(a*i - off)*r, cy + math.sin(a*i - off)*r, rad, outline="#000", fill=c)

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
                self.create_circle(cx - (1.5 * spc + 3 * rad) + (2 * rad + spc) * i, cy - (1.5 * spc + 3 * rad) + (2 * rad + spc) * j, rad, outline="#000", fill=c)

        
    def create_circle(self, x, y, r, **kwargs):
        return self.create_oval(x-r, y-r, x+r, y+r, **kwargs)
        
    
    def resize(self, e):
        """Updates the graph when the window has been resized

        resize(event) -> None
        """
        try:
            self.draw_clock()
        except:
            pass        

class SelectionFrame(tk.Frame):
    """A class that inherits from the tkinter Frame class

    creates and displays the checkboxes used for toggeling
    whether to display a stations data or not
    """
    def __init__(self,master,parent):
        """initializes the internal data

        Constructor: SelectionFrame(tk.Tk(), TemperaturePlotApp())
        """
        super().__init__(master)
        self._parent = parent
        #Time selection
        self._select = tk.Label(self, text = "Time:")
        self._timeLabel = tk.Label(self, text = " : ")

        self._hr = StringVar(self, value = str(hour))
        self._mn = StringVar(self, value = str(minute))
        
        self._hour = Entry(self, width = 2, textvariable = self._hr)
        self._min =  Entry(self, width = 2, textvariable = self._mn)
        self._set = Button(self, text="set", command = self.set_time)

        self._choices = {"PM", "AM"}
        self._am_pm = StringVar(self, value = am_pm)
        self._am_pm_option = OptionMenu(self, self._am_pm, *self._choices)

        self._select.grid(row = 0, column = 0)
        self._hour.grid(row = 0, column = 1)
        self._timeLabel.grid(row = 0, column = 2)
        self._min.grid(row = 0, column = 3)
        self._am_pm_option.grid(row = 0, column = 4)
        self._set.grid(row = 0, column = 5)

        #Get current time button
        self._get_time = Button(self, text = "Get Current Time", command = self.get_time);
        self._get_time.grid(row = 1, column = 0)

        #Weather display
        self._weather = tk.Label(self, text = weather)
        self._request_weather = Button(self, text = "Get Weather", command = self.update_weather);
        self._weather.grid(row = 2, column = 0)
        self._request_weather.grid(row = 3, column = 0)

    def reset_input(self):
        """Resets the value in the input

        reset_input() -> None
        """
        self._hr = StringVar(self, value = str(hour))
        self._mn = StringVar(self, value = str(minute))
        self._hour.config(textvariable = self._hr)
        self._min.config(textvariable = self._mn)
        self._am_pm = StringVar(self, value = am_pm);

        print(str(hour) + ":" +  str(minute) + ":" + am_pm + ":" + self._am_pm.get())

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
        if(time.hour > 12):
            hour = time.hour - 12;
            am_pm = "PM"
        else:
            hour = time.hour
            am_pm = "AM"
        minute = time.minute
        print(str(hour) + ":" +  str(minute) + ":" + am_pm)
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
               self._parent._clock.draw_clock()
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

        self._clock = ClockView(master,self)
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
    weather = weather_list['weather']['main']
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
    weather = get_weather()
    root = tk.Tk()
    app = ClockApp(root)
    root.geometry("640x480")

    old = millis()
    
    
    while 1:
        cur = millis()
        try:
            root.update_idletasks()
            root.update()
        except:
            break
        if(cur - old > 500):
            hour_disp = not hour_disp
            app._clock.draw_clock()
            old = cur

if __name__ == '__main__':
    main()
