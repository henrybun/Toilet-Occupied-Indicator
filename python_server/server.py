import time
import socket
import sys
import unicodedata
import numpy as np
from datetime import datetime
import socket  
from time import sleep 
import select
import cv2

def toilet():
    #initialize
    waiting = 0
    occupy = 0
    queue = 0
    status = 0
    full_cubicle = False
    full_queue = False
    full = False
    quit_ =  False

    #Status of toilet 3
    print("How is the status of Toilet 3? ")
    print("0: Normal; 1:Maintenance; 2:Unknown; Other: Quit")    
    status = int(input())
    
    if (status == 0):
        #Number of occupied cubicle
        print("How many occupied cubicles in Toilet 3? ")
        occupy = int(input())
        if (occupy == 3):
            full_cubicle = True
        print(occupy)
        
        #Length of the queue
        print("How long is the queue outside Toilet 3? ")
        queue = int(input())
        if (queue == 3):
            full_queue = True
        print(queue)
        
        if ((full_queue == True) & (full_cubicle == True)):
            print("Toilet 3 is full. ")
        
        if (full_cubicle == True):
            waiting = int((queue+1)*7.5*60*1000/3)
    elif ((status != 1)&(status != 2)):
        quit_ = True
    result = '112/'+str(waiting)+"/"+str(status)+"/"
    return result, quit_

# Time - H:M:S
toilet3 = "112/0/2/"
toilet3 = toilet3.encode();
    
#Receiver1
UDP_IP1 = "192.168.0.110"       #MCU IP 
UDP_PORT1 = 2390                #MCU Listen Port

#Receiver2
UDP_IP2 = "192.168.0.111"       #MCU IP 
UDP_PORT2 = 2390                #MCU Listen Port

#Receiver2
UDP_IP3 = "192.168.0.112"       #MCU IP 
UDP_PORT3 = 2390                #MCU Listen Port

#Receiver1
sock1 = socket.socket(socket.AF_INET,        # Internet IPV4
                     socket.SOCK_DGRAM)     # UDP
sock1.sendto(toilet3, (UDP_IP1, UDP_PORT1))



#Receiver2
sock2 = socket.socket(socket.AF_INET,        # Internet
                     socket.SOCK_DGRAM)     # UDP
sock2.sendto(toilet3, (UDP_IP2, UDP_PORT2))

sock3 = socket.socket(socket.AF_INET,        # Internet
                     socket.SOCK_DGRAM)     # UDP
sock3.sendto(toilet3, (UDP_IP3, UDP_PORT3))

print("Setup OK")
while (True):
    #Receiver1:
    try:
        connected1 = True
        sock1.setblocking(0)

        ready = select.select([sock1], [], [], 0.5)
        if ready[0]:
            value1, server= sock1.recvfrom(4096)   #receive  
        else:
            value1 = '111/0/2/'
            
        if (value1 != '111/0/2/'):
            value1 = value1.decode('utf-8').rstrip()

        print("Receiver1: ",UDP_IP1,value1)
        
        value1 = value1.encode()
        sock1 = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
        sock2.sendto(value1, (UDP_IP1, UDP_PORT1)) #send
        sock2.sendto(toilet3, (UDP_IP1, UDP_PORT1)) #send
    except KeyboardInterrupt:
        toilet3, quit_ = toilet()
        toilet3 = toilet3.encode();
        if (quit_ == True):
            break

    #Receiver2:
    try:
        sock2.setblocking(0)

        ready = select.select([sock2], [], [], 0.5)
        if ready[0]:
            value2, server = sock2.recvfrom(4096)
        else:
            value2 = '110/0/2/'
        if (value2 != '110/0/2/'):
            value2 = value2.decode('utf-8').rstrip()
        print("Receiver2: ",UDP_IP2,value2)
        
        value2 = value2.encode()
        sock2 = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
        sock1.sendto(value2, (UDP_IP2, UDP_PORT2))
        sock1.sendto(toilet3, (UDP_IP2, UDP_PORT2)) #send
    except KeyboardInterrupt:
        toilet3, quit_ = toilet()
        toilet3 = toilet3.encode();
        if (quit_ == True):
            break
