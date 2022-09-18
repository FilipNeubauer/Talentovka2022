import serial
import pandas as pd
import matplotlib.pyplot as plt

ser = serial.Serial('COM4', 9600, timeout=0.50)
data_file = open("data.csv", "w")

try:
    user_command = input("Press any key to start...")
    ser.write("start".encode())
    while True:
        rawdata = (ser.readline().decode('ascii'))
        print(rawdata)
        data_file.write(rawdata)
except KeyboardInterrupt:
    data_file.close()
    print("Exiting...")


degree = []
ms = []
ir = []
full = []
visible = []
lux = []

raw_data = open("data.csv", "r")
for i in raw_data.readlines():
    if i.startswith("measure"):
        #print(i)
        list_of_el = i.split()
        #print(list_of_el)
        degree.append(float(list_of_el[1]))
        ms.append(float(list_of_el[2]))
        ir.append(float(list_of_el[3]))
        full.append(float(list_of_el[4]))
        visible.append(float(list_of_el[5]))
        lux.append(float(list_of_el[6]))

raw_data.close()

df = pd.DataFrame({"degree":degree, "ms":ms, "full":full, "visible":visible,
                   "lux":lux})

df.to_excel("destli_voda_znova.xlsx")


print(df)

plt.plot(df["degree"], df["full"], marker="o")
plt.yscale("log")
plt.show()

    
    
