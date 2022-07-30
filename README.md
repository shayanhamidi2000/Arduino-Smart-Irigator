# Automatic Irrigation system wih Arduino uno
The system contains two main nodes that are `Arduino Uno`, some sonsers such as `SHT-25` I2C humidity and temperature sensor, a pair of synchoronized `HC-05` UART serial bluetooth modules. Also, an LCD is installed on the central arduino to display last feedback data and decisions.
The whole system was analyzed, simulated and tested by Proteus.

## Central Node
* An Arduino Uno board as the computational core of this section, which makes decision based on transfered and sensed humidity and temperature data according to below rules:
  - Humidity higher than 50% -> No irrigation
  - Humidity lower than 20% -> 25% of irrigation power
  - Humidity between 20-50% and temperature lower than 25 Celcius -> No irrigation
  - Humidity between 20-50% and temperature higher than 25 Celcius -> 10% of irrigation power
* An LCD with `LM44` model that displays last decision and last sensed data. It uses digital pins.
* An `HC-05` bluetooth module that uses arduino TX/RX UART serial port in order to bidirectionally transfers data

![central-node](https://user-images.githubusercontent.com/41966479/181918633-6c69f2fb-4393-44e8-a8a2-24171f878d35.png)

## Edge Node
* An Arduino Uno board as the computational core of this section, which reads data from humidity and temperature sensor periodically and sends it instantly to the other node
* Another `HC-05` bluetooth module that uses arduino TX/RX UART serial port in order to bidirectionally transfers data
* The `SHT-25` humidity and temperature I2C sensor
* A DC motor with `L293D` driver that is controlled by PWM arduino-generated pulse signals

![edge-node](https://user-images.githubusercontent.com/41966479/181918841-835099fc-e214-417b-acff-9012a7e33c65.png)

## Prerequisites

- <a href="https://soft98.ir/software/engineering/3535-proteus.html">Proteus</a>
- <a href="https://www.theengineeringprojects.com/2015/12/arduino-uno-library-proteus.html">Arduino 1.8.9 windows portable</a> or use
- <a href="-https://www.yasdl.com/134583/%D8%AF%D8%A7%D9%86%D9%84%D9%88%D8%AF-arduino.html">Arduino 1.8.9 windows</a>
- Add the libraries in `library` directory in the repository to Liberaries in Proteus
