import serial
#from serial import Serial
#ser = serial.Serial('/dev/tty96B0', 9600)
ser = serial.Serial('COM10', 9600)
#ser.isOpen()

f = open('output.txt')
# use readline() to read the first line 
line = f.readline()
# use the read line to read further.
# If the file is not empty keep reading one line
# at a time, till the file is empty

msg = ""

while line:
    # in python 2+
    # print line
    # in python 3 print is a builtin function, so

    #parse
    #print(line[0])
    for x in range(2, len(line)):
        if line[x] == "'":
            msg += ","
            break
        msg += line[x]


    #print(msg)


    #print(line)
    #ser.write(line)
    # use realine() to read next line
    line = f.readline()

#print(msg[:(len(msg)-1)])
#print msg
ser.write(msg[:(len(msg)-1)])

f.close()