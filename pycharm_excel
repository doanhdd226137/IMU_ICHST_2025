import serial
import csv

ser = serial.Serial('COM4', 115200)  # Thay đổi 'COM8' thành cổng COM bạn đang sử dụng


with open('sensor_data1.csv', mode='w', newline='') as file:
   writer = csv.writer(file)
   # Ghi tiêu đề cột
   writer.writerow(['id', 'timestamp', 'roll'])

   while True:
       line = ser.readline().decode('utf-8').strip()
       if line:
           data = line.split(',')
           if len(data) >= 5:
               id_value = data[0]
               timestamp_value = data[1]
               roll_value = data[4]
               writer.writerow([id_value, timestamp_value, roll_value])
               print(f'{id_value}, {timestamp_value}, {roll_value}')

