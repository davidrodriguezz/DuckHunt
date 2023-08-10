#pragma once


class Controller
{
public:
	float L2 = 0;
	float R2 = 0;

	bool circle=false;
	bool cross=false;
	bool square = false;

	//Joystick o acelerometros
	float leftStickX = 0;
	float leftStickY = 0;

	float gyroscopeAngularVelocityX = 0;
	float gyroscopeAngularVelocityY = 0;
};