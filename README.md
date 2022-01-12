# RemoteHeadlamps

Description
- Remote headlamp system for car that tracks steering wheel

Purpose
- Learning platform for coding, design, and fabrication for real world applications

Physical Setup
- Transmitter inside car w/ arduino, joysitck, swtich, and steering wheel potentiometer
- Reciever in each lamp system w/ arduino, 2 servo motors, and 12v aux lamp

Funcitonality
- System disarmed and in stowed position
- Long button press to arm system
- Short button click changes mode
- Lamps initialize position when modes or arm state changes
- Spotlight mode: Lamps follow eachother and read joystick data
- Mirror mode: Lamps mirror eachother and read joystick data
- Auto mode: Lamps follow eachother and read wheel potentiometer data

Goals
- System should be clean and modular whith the ability to add more lights
- Add birghtness adjustment to code
- Needs to be weatherproof
- Proper use of .h files and libraries

Checklist
- CamelCase
- Watch PID stuff
- Joystick not a class
- Gimbal class
- Initial positioning class Spotlight, Mirror, Auto

System in Action
- @matteblackrobotics