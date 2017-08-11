
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

import urllib.request
import json

#####################################
# Imports End                       #
#####################################


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
        #self.bind('<B1-Motion>', self.draw_line)
        #self.bind('<Button-1>', self.draw_line)
        self.bind('<Configure>', self.resize)

    def draw_clock(self):
        """Takes the station data and uses the CoordinateTranslator class
        to convert temperatures for all the stations into coordinates

        draw_clock() -> None (clock on canvas)
        """
        self.delete(tk.ALL)
        self._x = self.winfo_width()
        self._y = self.winfo_height()

        self._ct = CoordinateTranslator(self._x, self._y, self._ranges[0],
                        self._ranges[1], self._ranges[2],self._ranges[3])
        i=0
        for station in self._data.get_stations():
            if self._data.is_selected(i):
                data = self._data.get_data()[station]
                years = range(data.get_year_range()[0],
                              data.get_year_range()[1])
                old = self._ct.temperature_coords(data.get_year_range()[0],
                                    data.get_temp(data.get_year_range()[0]))
                for j in years:
                    c=self._ct.temperature_coords(j+1,data.get_temp(j+1))
                    self.create_line(old[0],old[1],c[0],c[1],fill=COLOURS[i])
                    old=c                    
            i+=1

    def draw_line(self, e):
        """Draws a vertical line where the mouse has been pressed

        draw_line(event) -> None (Vertical line on canvas)
        """
        

    def resize(self, e):
        """Updates the graph when the window has been resized

        resize(event) -> None
        """
        try:
            self.draw_graph()
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
        self._select = tk.Label(self, text = "Time Selection: ")
        self._select.pack(side = tk.LEFT, fill = tk.X, expand = 1)
        self._timeLabel = tk.Label(self, text = " : ")
        self._hour = Entry(self)
        self._minutes =  Entry(self)
        self._hour.pack(side = tk.LEFT)
        self._timeLabel.pack(side = tk.LEFT)
        self._minutes.pack(side = tk.LEFT)

    def update_data(self, data):
        """Updates the classes data variable

        update_data(temperatureData()) -> None
        """
        self._tempdata=data
    
    def add_station(self,station):
        """
        adds a checkbox in the frame for the supplied station

        add_station(str) -> None (Checkbox for the station)
        """
        i=self._tempdata.get_stations().index(station)
        command_button=lambda j=i:self.checkbutton_press(j)
        check_button=tk.Checkbutton(self,text=station,command=command_button,
                                      fg=COLOURS[i])
        check_button.select()
        check_button.pack(side=tk.LEFT)

    def checkbutton_press(self,num):
        """
        Toggles the state of the toggle variable when the check box is selected
        Redraws the graphs with regards to the new list of selected stations
        displays relevant data

        checkbutton_press(int) -> None (Updates graphs and toggle variables)
        """
        self._parent._tempdata.toggle_selected(num)
        self._parent._plotter.draw_graph()
        self._parent._plotter.draw_line(self._parent._xmouse)
        self._parent._data.display()


class DataFrame(tk.Frame):
    """A class that inherits for the tkinter Frame class

    creates and displays the labels for showing the temperature
    at the selected date (where the mouse is clicked)
    """
    def __init__(self,master,parent):
        """initializes the internal data

        Constructor: DataFrame(tk.Tk(), TemperaturePlotApp())
        """
        super().__init__(master)
        self._parent=parent
        self._data=tk.Label(self,text="" )
        self._data.pack(side=tk.LEFT,fill=tk.X,expand=1)

    def display(self):
        """Displays the temperature for all selected stations at the
        selected date (where the canvas has been clicked)

        display() -> None (Displays dates)
        """
        for widget in self.winfo_children():
            widget.destroy()
        year=self._parent._plotter._ct.get_year(self._parent._xmouse.x)
        self._data=tk.Label(self,text="Data for "+str(year)+":" )
        self._data.pack(side=tk.LEFT,fill=tk.X,expand=1)
        for num in range(len(self._parent._tempdata._toggle)):
            if self._parent._tempdata.is_selected(num):
                station=self._parent._tempdata._data[
                    self._parent._tempdata._names[num]]
                temp=station.get_temp(year)
                self.stations=tk.Label(self, text=temp,fg=COLOURS[num])
                self.stations.pack(side=tk.LEFT,fill=tk.X,expand=1, ipadx=10)

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
        
        self._data = DataFrame(master, self)
        self._data.pack(side=tk.LEFT, anchor=tk.N)

        self._clock = ClockView(master,self)
        self._clock.pack(side=tk.RIGHT,expand=1,fill=tk.BOTH)
        
        menubar = tk.Menu(master)
        master.config(menu=menubar)

        filemenu = tk.Menu(menubar)
        menubar.add_cascade(label="File", menu=filemenu)
        filemenu.add_command(label="Open", command=self.openFile)

    def openFile(self):
        """Opens files that are selected through the menu bar
        it will check if it is a valid file or is already open in the program

        Creates an error message box if the check is failed

        openFile() -> None (creates a new Station
        class for the selected station)
        """
        file = tk.filedialog.askopenfilename(title="Open data file").rsplit(
            "/",1)[1]
        if file[:-4] not in self._tempdata._names:
            try:
                self._tempdata.load_data(file)
                self._plotter.update_data(self._tempdata)
                self._select.update_data(self._tempdata)
                self._select.add_station(file[:-4])
                self._plotter.draw_graph()
            except:
                messagebox.showerror("Incorrect File Type",
                        "The file:  '"+file+"' is not recognized")
        else:
            messagebox.showerror("File already open",
                        "The file: '"+file+"' is already open")


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
