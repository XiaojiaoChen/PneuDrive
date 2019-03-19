# PneuDrive

[![N|Solid](https://cldup.com/dTxpPi9lDf.thumb.png)](https://nodesource.com/products/nsolid)


Pneudrive is a realtime control platform for soft pneumatic robotics (SPRs).

It offers an easy way to access advanced embedded control features, like real-time control, high performance computation and real-time communication, for multiple-chambered SPR projects, ideal for quickly setting up pneuamtic projects with ease.


### Features

  - Real-time control
  --  1 KHz control loop
  - Fast communication
  -- serial transmission with up to 1KHz frequency
  -- EtherCAT for real-time remote control
  - Large input and output extensibility
  -- built-in 16-channel Analog-in ports, 26 PWM-out ports
  -- extensible to any number of Analog-in or PWM-out with ease
  - Hardware accelerated computation
  -- built-in PID, kalman filters
  


Data Transmission:
 * EtherCAT (expansion board I-NUCLEO-NETX)
 * serial transmission
 * Non-blocking variable size serial receving (using DMA and input idleline Detection) 

control:
 * Simple Kalman filter (Using CMSIS DSP library, who exploits fast hardware computation)
 * Advanced PID controller
 * simple Sliding mode controller



## Getting Started

### Prerequisites

Hardware:
 * STM32F767ZI-Nucleo board
 * I-NUCLEO-NETX
 * ADS8588 board
 * LTC2668 board
 * other sensors
 
Software:
 * STM32CubeMX
 * SW4STM32
 * cifX toolkit form Hilscher
 
 
### Installing

## Deployment

## Authors

* **Xiaojiao Chen** 

## License

This project is licensed under the MIT License - see the [LICENSE.md](LICENSE.md) file for details

## Acknowledgments
