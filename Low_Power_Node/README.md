
course-project-TimChien112
course-project-TimChien112 created by GitHub Classroom

                        Mesh based Smart Home (Home Automation)
This project is developed as final project for the course ECEN 5823.

 Authors: 	santhosh Thummanapalli 
			Tim chien

 This Project aims at developing a secure mesh based smart home. This project tries to implement a low power smart fire alert system, smart lighting control system and app for updating these values using Bluetooth mesh.

 Board used: silicon labs Blue gecko starter kit - BRD4001A and Bluetooth Mesh App

 This project develops	1. A low power Node.
							Low power node will send the state changing request to friend node.
							Low power node will receive the changing request (presense state)from friend node, and enable smoke sensor and light sensor.
						2. Fiend node. 
							with IR beam (presense detecter) sensor and LED (bulb).
							Friend node will receive the state changing request (brightness) from LPN, and change the local state. According to the current local state, Friend will change the brightness of the bulb.
							Friend node will sent the presense state to Low power node.
						3. Proxy Node.
							

 Sensors used 	1. MAX3010 Particle sensor for smoke detection 
				2. APDS 9301 Ambient light sensor for light intensity detection 
				3. 5mm IR break out board.
			 
4-20 process update:
	Complete the friendship between LPN and Friend.
	Can publish the request from LPN to friend for button state changing.
	Enable the GPIO interrupt for IR Beam sensor interrupt.
	Start working on the I2C communication for light ambient sensor.