# photon-vehicle

![Lang](https://badgen.net/badge/Language/Embedded-C/orange) ![Contr](https://badgen.net/badge/Controller/ATmega-328P/F96854) [![License](https://badgen.net/badge/Licence/MIT/blue)](./LICENSE)

<img src="demo.gif">

### Description
A 3-wheel vehicle that follows the path with the strongest source of light.

### Prerequisites

- An AVR micro-controller (I used ATmega328P) and a board to plug it AKA: [Arduino UNO](https://image.slidesharecdn.com/arduinounoatmega328-151028115931-lva1-app6891/95/arduino-uno-atmega328-5-638.jpg?cb=1446033702)
- AA [Batteries](https://encrypted-tbn0.gstatic.com/images?q=tbn:ANd9GcRMM5Rv62q8PDadNzyNP1a4CVt3B_raLvxWcaFju3ADlQST6xch) to power your board 
- A [Chasis](https://cdn-shop.adafruit.com/1200x900/2943-01.jpg) for your vehicle
- Two DC [motors](https://cdn-shop.adafruit.com/1200x900/2941-00.jpg) in in micro servo body and a couple of [wheels](https://encrypted-tbn0.gstatic.com/images?q=tbn:ANd9GcQ4cqBrJ4y_WUmKC7xQ7gbtW5wmvfUwopOnDSMFwQjpPdqmb_srEw)
- A dual motor [DVR8835](https://a.pololu-files.com/picture/0J4056.1200.jpg?c04dc0dba6293777436f8a523565e9e6) driver to control your motors  
- Three [photo-resistors](https://i.ebayimg.com/images/g/pSAAAOSw3v5YsYfD/s-l300.jpg) 
- Three drinking [straws](https://cdn.shopify.com/s/files/1/1329/2645/products/Drinking_Straws15_1024x1024.jpg?v=1498664680) to hold the photo-resistors :sunglasses:
- A [switch](https://media.rs-online.com/t_large/F7581922-01.jpg), a [breadboard](https://www.circuitspecialists.com/content/175324/wb-801-0.jpg) and bunch of [wires](https://i.stack.imgur.com/y7eu5.jpg) and [resistors](https://i.ebayimg.com/images/g/ZHUAAOxyqKVRizNa/s-l300.jpg)

And finally you're going to need a tool like Atmel Studio to compile and produce the .hex which you will load to the AVR with a program like XLoader.

### Vehicle Close-up

<img width="450" height="auto" src="vehicle.png">
