import time,collections, random
from tkinter import *
from tkinter import messagebox
from PIL import Image
from PIL import ImageTk
import cv2, imutils, serial
import numpy as np
import matplotlib.pyplot as plt
import matplotlib.animation as animacion
from matplotlib.backends.backend_tkagg import FigureCanvasTkAgg

arduino1 = serial.Serial('/dev/ttyACM0', 115200)

def ConvertStringToBytes(src):
    converted = []
    for b in src:
        converted.append(ord(b))        
    return converted

datos = 0.0
muestraD = 700
data = collections.deque([0]*muestraD, maxlen=muestraD)
xmin = 0
xmax = muestraD
ymin = 35
ymax = 75

def Iniciar():
    global datos
 
    anim = animacion.FuncAnimation(fig, plotData,  fargs=(muestraD,lines),interval = 100, blit = False )
    lienzo.draw()

def plotData(self,muestraD,lines):
    datos = Yp
    data.append(datos)
    lines.set_data(range(muestraD), data)

def visualizar():
    global cap
    global kernel
    global cont
    global Xp
    global Yp
    global spv
    global Banx
    global Bany
    if cap is not None:
        ret, frame = cap.read()
        xp = 0.0
        yp = 0.0
        meassureX = "0"
        meassureY = "0"
        err_e = "0"
        s = select.get()
        if ret == True:
            frame = imutils.resize(frame, width=570)
            frame = cv2.cvtColor(frame, cv2.COLOR_BGR2RGB)
            cv2.circle(frame, (67, 16), 5, (255,0,0), 2)
            cv2.circle(frame, (497, 16), 5, (255,0,0), 2)
            cv2.circle(frame, (67, 425), 5, (255,0,0), 2)
            cv2.circle(frame, (497, 425), 5, (255,0,0), 2)
            M1 = np.float32([[67,16], [497,16], [67,425],[497,425]])
            M2 = np.float32([[0,0], [440,0], [0,420],[440,420]])
            M = cv2.getPerspectiveTransform(M1, M2)
            dst = cv2.warpPerspective(frame, M, (440,420))
            
            frameHSV = cv2.cvtColor(dst, cv2.COLOR_RGB2HSV)
            rangomax = np.array([95, 250, 250], np.uint8)
            rangomin = np.array([60, 21, 33], np.uint8)
            mascara = cv2.inRange(frameHSV, rangomin, rangomax)
            opening = cv2.morphologyEx(mascara, cv2.MORPH_OPEN, kernel)
            x, y, w, h = cv2.boundingRect(opening)
            cv2.rectangle(dst,(x,y),(x+w,y+h),(250,0,0),1)
            cv2.circle(dst,(int(x+w/2),int(y+h/2)),3,(0, 0, 100),-1)   
            contours, hierarchy = cv2.findContours(opening, cv2.RETR_TREE, cv2.CHAIN_APPROX_NONE)
            
            if len(contours) != 0:
                cnt = contours[0]
                M = cv2.moments(cnt)
                Cx = int(M['m10']/M['m00'])
                Cy = int(M['m01']/M['m00'])                
                x = ((Cx*110)/440)
                y = ((Cy*-105)/420)+55               
                xp = ((Cx*110)/440)
                yp = ((Cy*105)/420)
        
                meassureX = (str(float('{:.2f}'.format(xp))))
                meassureY = (str(float('{:.2f}'.format(yp))))
                
                if cont >= 38:
                    try:
                        if Banx == True:
                            err = (spv - xp)
                            err_e = (str(err))
                            envio = ConvertStringToBytes(err_e)
                            arduino1.write(envio)
                            cont = 0
                            print(err)
                            
                        elif Bany == True:
                            err = (spv - yp)
                            err_e = (str(err))
                            envio = ConvertStringToBytes(err_e)
                            arduino1.write(envio)
                            cont = 0
                            print(err)
                        
                    except IOError as e:
                        print (e)
            
            lblPX.configure(text=meassureX)
            lblPY.configure(text=meassureY)
            lblerror.configure(text=err_e)
            
            if s==1:
                global lblVideo
                frame_grafica.place_forget()
                lblVideo.place(x=350, y=5)
                im = Image.fromarray(dst)
                img = ImageTk.PhotoImage(image=im)           
                lblVideo.configure(image=img)
                lblVideo.image = img
            
            if s==2:
                lblVideo.place_forget()
                Xp = xp
                Yp = yp
                
            cont = cont+1
            root.after(1, visualizar)
        else:
            lblVideo.image = ""
            cap.release()
    
def iniciar():
    global cap
    global kernel
    global cont
    cap = cv2.VideoCapture(0)
    kernel = np.ones((3,3),np.uint8)
    cont = 0
    visualizar()

def panelgrafica():
    #plt.title()
    ax.set_ylabel("Señal")
    lienzo.get_tk_widget().pack(padx=5, pady=5 , expand=1, fill='both') 
    Button(frame_grafica, text='Grafica Datos', width = 15, bg='purple4',fg='white', command=Iniciar).pack(pady =5,side='left',expand=1)
    
def estado():
    frame_grafica.place(x = 290, y = 5)
    panelgrafica()
    
def setpoint():
    global spv
    global Banx
    global Bany
    val = setp.get()
    try:
        spv = (float(val))
        eje = axis.get()
        if eje == 1:
            Banx = True
            Bany = False
        elif eje == 2:
            Bany = True
            Banx = False
        else:
            messagebox.showinfo("Alerta", "Debe escoger un eje")
        
    except:
        messagebox.showinfo("Alerta", "Solo se permiten valores numericos")

global spv
global Banx
global Bany
global lblVideo
Banx = False
Bany = False
cap = None
root = Tk()
root.title("Ultrasonic Levitator")
width= root.winfo_screenwidth()  
height= root.winfo_screenheight()
root.geometry("%dx%d" % (width, height))
root.resizable(0,0)

cont1=LabelFrame(root, text="Particle Location", bg="gray86", fg="blue", height = 130, width = 180)
cont1.place(x=5, y=80)
cont2=LabelFrame(root, text="Settings", bg="gray86", fg="blue", height = 180, width = 280)
cont2.place(x=5, y=210)

btnIniciar = Button(root, text="Start", command=iniciar, bg="lime", fg="#000000", activebackground="cyan", activeforeground="#000000")
btnIniciar.place(x=5, y=5)

btnFinalizar = Button(root, text="Stop", bg="red", fg="#ffffff", activebackground="#fa6dfd", activeforeground="#ffffff")
btnFinalizar.place(x=75, y=5)

setp=StringVar()
lblname = Label(cont2, text="Set Point: ", bg="gray86").place(x=2, y=12)
entry = Entry(cont2, textvariable=setp)
entry.place(x=80, y=11)
lblaxis = Label(cont2, text="Axis ", bg="gray86").place(x=130, y=50)
axis = IntVar()
rbx = Radiobutton(cont2, text="X axis", value=1, variable=axis).place(x = 47, y = 75)
rby = Radiobutton(cont2, text="Y axis", value=2, variable=axis).place(x = 165, y = 75)
btnsend = Button(cont2, text="Send", command=setpoint)
btnsend.place(x=110, y=120)

lblVideo = Label(root)

lblPX = Label(cont1, bg="gray86")
lblPX.place(x=81, y=10)
lblX = Label(cont1, text="X position: ", bg="gray86").place(x=2, y=10)

lblPY = Label(cont1, bg="gray86")
lblPY.place(x=81, y=50)
lblY = Label(cont1, text="Y position: ", bg="gray86").place(x=2, y=50)

lblerror = Label(cont1, bg="gray86")
lblerror.place(x=81, y=75)
lble = Label(cont1, text="Error: ", bg="gray86").place(x=2, y=75)

select = IntVar()
rb_video = Radiobutton(root, text="Video", value=1, variable=select).place(x = 190, y = 10)
rb_grafica = Radiobutton(root, text="Plote", value=2, variable=select, command=estado).place(x = 190, y = 40)
frame_grafica = Frame(root, bg = "blue", height = 400, width = 400, bd = 0, relief = "ridge")
fig = plt.figure(facecolor="0.55",figsize=(5, 4), clear=True)
ax = plt.axes(xlim=(xmin,xmax),ylim=(ymin,ymax))
lines = ax.plot([] ,[], 'r')[0]
lienzo = FigureCanvasTkAgg(fig, master = frame_grafica) 

root.mainloop()