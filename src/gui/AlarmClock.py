
###################################################################
#                                                                 #
#   ENGG2800 Alarm Clock Software                                 #
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

import math

import urllib.request
import json

#####################################
# Imports End                       #
#####################################


hour = 10
minute = 59
am_pm = "AM"

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

        cx = self._x/2
        cy = self._y/2

        a = math.pi/6
        r = 50
        c = "#fff"
        for i in range(12):
            if(i%h == 0):
                c = "#f0f"
            else:
                c = "#fff"
            create_circle(cx+cos(a*i), cy+sin(a*i), outline="#000", fill=c)
            
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
        self._select = tk.Label(self, text = "Time:")
        self._select.pack(side = tk.LEFT, fill = tk.X, expand = 1)
        self._timeLabel = tk.Label(self, text = " : ")

        self._hr = StringVar(self, value = str(hour))
        self._mn = StringVar(self, value = str(minute))
        
        self._hour = Entry(self, width = 2, textvariable = self._hr)
        self._min =  Entry(self, width = 2, textvariable = self._mn)
        self._set = Button(self, text="set", command = self.set_time)

        self._choices = {"PM", "AM"}
        self._am_pm = StringVar(self, value = am_pm)
        self._am_pm_option = OptionMenu(self, self._am_pm, *self._choices)

        self._hour.pack(side = tk.LEFT)
        self._timeLabel.pack(side = tk.LEFT)
        self._min.pack(side = tk.LEFT)
        self._am_pm_option.pack(side = tk.LEFT)
        self._set.pack(side = tk.LEFT)

    def set_time(self):
        """Gets values from the inputs in the menu

        get_time() -> None (Update time and display)
        """
        time = [self._hour.get(), self._min.get(), self._am_pm.get()]
        hour = int(time[0])
        minute = int(time[1])
        am_pm = time[2]
        print(time)
        

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


def getWeather():
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
    print(getWeather())

    root = tk.Tk()
    app = ClockApp(root)
    root.geometry("640x480")
    root.mainloop()

if __name__ == '__main__':
    main()
