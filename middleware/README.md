# Instructions
#### Dependencies

* [BLE Serial](https://pypi.org/project/ble-serial/)
* [PySerial](https://pythonhosted.org/pyserial/)

#### Preparation Steps

Just in case, give permissions over arduino port
```
sudo chmod a+rw /dev/ttyUSB0
```
After installing python dependencies run this commands to connect to the BLE device (in this case, the MAC address of the module is: 50:33:8B:6D:6D:CE)
```
ble-scan
ble-scan -d 50:33:8B:6D:6D:CE
ble-serial -d 50:33:8B:6D:6D:CE
```

#### Execution

Execute
```
python3 server.py
```