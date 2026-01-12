# Arduino_Archive
Okayyy soooo I've decided to delve a little bit into Electronics / Embedded Systems /Robotics. Well, specifically into Arduino Electronics. At first, it was solely just for me to get on a level head for my Thesis (it still is to some extent, lmao) but then I realized...wait, this is kinda fun! I've been studying CpE for the longest time and I haven't had the chance to actually handle an Arduino microcontroller. Welp, that changes today. These aren't going to be big projects just yet, but it's an initial learning step. This is going to be a collective archive of all the Arduino sketches and programs that I've made so far in order to get a grasp on the basics of Arduino. The limitations of this learning segment are up to the components provided within the learning kit (please refer to the given image below): 

<img width="1000" height="1000" alt="image" src="https://github.com/user-attachments/assets/cd7ac987-c4cd-42a3-8f17-fcf31fa1af04" />

Regardless, that's not going to stop me from trying to get a hands-on experience with the Arduino Uno microcontroller, as well as various component interfacing methods. Whether it's small systems or big modules, any type of entry is welcome.I have a BIG feeling that this'll be useful later on. Eitherway, there's only one way to find out. Let's do this!

## Exercise 1: Temp-Alarm
<img width="1234" height="682" alt="Screenshot 2026-01-12 at 9 32 49 PM" src="https://github.com/user-attachments/assets/be8411e7-f0bc-4d22-933e-6955b85cdb9e" />
<p>This exercise is designed to monitor the current state of the room and flag LED signals according to the temperature. If it gets too hot, an alarm triggers. This exercise made me learn the following</p>
<ul>
  <li>Basic Arduino Interfacing of LEDs and Buzzers</li>
  <li>Usage of the DHT11 Temperature Sensor</li>
  <li>The Serial Monitor</li>
</ul>

## Exercise 2: Tri-State Environ-Monitor
<img width="1248" height="930" alt="Screenshot 2026-01-12 at 9 34 33 PM" src="https://github.com/user-attachments/assets/eed35de7-38b8-4d3f-85c3-d05debc6e387" />
<p>This exercise is designed to monitor different parameters of a given environment. Specifically, light, temperature and humidity, and water levels. This exercise made me learn the following</p>
<ul>
  <li>Tactile Switch Interfacing</li>
  <li>Usage and Intefacing of the 16x2 LCD Display module</li>
  <li>Usage of the Water Level Sensor Module</li>
  <li>State Design and Configuration</li>
</ul>

## Exercise 3: Overstimulation
<img width="1067" height="673" alt="Screenshot 2026-01-12 at 9 34 08 PM" src="https://github.com/user-attachments/assets/6362e887-ce83-4c1b-8a1b-6f1b635033e0" />
<p>This exercise is designed to serve as a "Fidget Toy" by offering LED control with joystick manipulation, servo motor swerving, and buzzer sounds. This exercise made me learn the following</p>
<ul>
  <li>Usage of the Servo Motor and Analog Joystick</li>
  <li>Analog Reading to Digital Output</li>
  <li>Asynchronous Event Handling</li>
</ul>

## Exercise 4: Distance Reader
<img width="1225" height="827" alt="Screenshot 2026-01-12 at 9 35 00 PM" src="https://github.com/user-attachments/assets/8334436f-8073-44c8-84b3-56d90375f364" />
<p>This exercise is designed to sense objects using a Ultrasonic Sensor and have it react based on certain distances read. This exercise made me learn the following</p>
<ul>
  <li>Usage of the HC-SR04 Ultrasonic Sensor Module </li>
  <li>Threshold Indicating with LEDs</li>
  <li>Real-Time Data Retrieval</li>
</ul>

## Exercise 5: Mini-Rapiscan
<img width="645" height="397" alt="image" src="https://github.com/user-attachments/assets/dcc5783f-f933-4f6d-8428-69b176a4f37a" />
<p>This exercise is designed to act as an X-Ray machine by activating based on an ML Algorithm that detects certain objects during an X-Ray Scan. This exercise made me learn the following</p>
<ul>
  <li>Arduino and Python Integration</li>
  <li>Object Detection and Image Classification with the YOLO-V8 ML Model</li>
  <li>An idea of what the field of Embedded Systems is like (I think)</li>
</ul>

<a href = https://github.com/SuillyCoder/Mini-RapiScan>You can actually view the project here!</a>

## Exercise 6: Volume Visualizer
<img width="1346" height="817" alt="Screenshot 2026-01-12 at 9 48 48 PM" src="https://github.com/user-attachments/assets/f296b1ea-41fc-4c17-b3d8-0f22a4ff8e55" />
<p>This exercise is designed to receive analog input and visualize its thresholds on an LED Array (like an audio visualizer). This exercise made me learn the following</p>
<ul>
  <li>Usage of the KY-037 Sound Sensor Module</li>
  <li>Understanding and Manipulation of Analog and Digital Sound Outputs</li>
  <li>Visualization of Analog and Digital Output</li>
</ul>

## Exercise 7: Digi-Math Quiz
<img width="723" height="646" alt="Screenshot 2026-01-12 at 9 49 05 PM" src="https://github.com/user-attachments/assets/b25cde55-5668-4d0e-88c4-9c97b7078317" />
<p>This exercise is designed to act as a mini-math quiz device that breaks your streak on an incorrect answer. This exercise made me learn the following</p>
<ul>
  <li>Usage and interfacing of the membrane switch / numeric keypad</li>
  <li>Displaying output of the keypad both on the LCD and the Monitor</li>
  <li>C Library Inclusions in Sketches</li>
</ul>

## Exercise 8: Motor Control
<img width="1174" height="750" alt="Screenshot 2026-01-12 at 9 49 28 PM" src="https://github.com/user-attachments/assets/6a3307a5-d3c0-4aaf-823b-c07340ebe153" />
<p>This exercise is designed to control motor speed with a potentiometer and visualize its speed with an LED Array. This exercise made me learn the following</p>
<ul>
  <li>Usage of the 74HC595 8-Bit Serial In / Parallel Out Shift Register</li>
  <li>Usage and interfacing of motors with a Power Supply module</li>
  <li>Manipulation of output with analog control (potentiometer)</li>
</ul>

<p>So far, this run through has given me a roguh overview of the basics of Arduino, Embedded Systems Electronics, and Hardware Control. Of course, these are still baby steps. While they're not that big of a deal yet, they're a stepping stone to get a little bit closer to building actual projects. As for what I'll be building, looks like I'll have to see. </p>

