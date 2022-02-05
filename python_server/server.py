import time
import socket
import sys
import unicodedata
import numpy as np
from datetime import datetime

# datetime object containing current date and time
now = datetime.now()
# Time - H:M:S
now = now.strftime("%H:%M:%S")
now = now.encode();

def millis2Time(t):
    t = int(t)
    totalSecond = int(t / 1000)
    hour = int(totalSecond / 3600)
    minute = int((totalSecond % 3600) / 60)
    second = int(totalSecond % 3600 % 60)
    return str(hour)+"h"+str(minute)+"m"+str(second)+"s"
    

#Receiver1
UDP_IP1 = "192.168.0.110"       #MCU IP 
UDP_PORT1 = 2390                #MCU Listen Port

#Receiver2
UDP_IP2 = "192.168.0.111"       #MCU IP 
UDP_PORT2 = 2390                #MCU Listen Port

#Receiver1
sock1 = socket.socket(socket.AF_INET,        # Internet
                     socket.SOCK_DGRAM)     # UDP
sock1.sendto(now, (UDP_IP1, UDP_PORT1))

#Receiver2
sock2 = socket.socket(socket.AF_INET,        # Internet
                     socket.SOCK_DGRAM)     # UDP
sock2.sendto(now, (UDP_IP2, UDP_PORT2))
print("Setup OK")
while (True):
    # datetime object containing current date and time
    now = datetime.now()
    # Time - H:M:S
    now = now.strftime("%H:%M:%S")
    now = now.encode();
    #Receiver1:
    
    value1, server = sock1.recvfrom(4096)   #receive
    value1 = value1.decode('utf-8').rstrip()
    value1_1 = value1[3:]
    print("Receiver1: ",UDP_IP1,value1, " " , millis2Time(value1_1))
    
    value1 = value1.encode()
    
    sock1 = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
    sock2.sendto(value1, (UDP_IP1, UDP_PORT1)) #send

    
    #Receiver2:
    
    value2, server = sock2.recvfrom(4096)
    value2 = value2.decode('utf-8').rstrip()
    value2_1 = value2[3:]
    print("Receiver2: ",UDP_IP2,value2, " " , millis2Time(value2_1))
    
    value2 = value2.encode()
    
    sock2 = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
    sock1.sendto(value2, (UDP_IP2, UDP_PORT2))
