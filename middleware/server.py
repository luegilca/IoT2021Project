import requests
import serial
import time

ser = serial.Serial('/tmp/ttyBLE', 9600, rtscts=True, dsrdtr=True)

def readFromSerial():
    time.sleep(0.05)
    data = ser.readline()
    return data.decode('UTF-8')

def extractDeviceId(line):
  line_stripped = line[:-1]
  device_id = line_stripped.split(':')
  return device_id[1]

print('Server is ready and listening for serial port!')
backend_url = 'https://blooming-shelf-43966.herokuapp.com'

while True:
  data = readFromSerial()
  device_id = extractDeviceId(data)
  print('Sending data to server from device ' + device_id)
  jsonData = {
    'device': {
      'device_id': device_id
    }
  }
  res = requests.post(backend_url + '/usages', json=jsonData)
  json_response = res.json()
  print('Response from server:', res.text)