float			deltaX = 0.4;
float			deltaY = 0.4;
float			deltaZ = 0.4;
float			updateX = 0.0;
float			updateY = 0.0;
float			updateZ = 0.0;

GLfloat   ambi[]            = {0.1, 0.1, 0.1, 0.1};
GLfloat   lightZeroColor[]  = {0.9, 0.9, 0.9, 0.1};

void UpButtonCallback() {
	updateY += deltaY;
}

void DownButtonCallback() {
	updateY-= deltaY;
}

void LeftButtonCallback() {
	updateX += deltaX;
}

void RightButtonCallback() {
	updateX -= deltaX;
}

void RaiseButtonCallback() {
	updateZ += deltaZ;
}

void LowerButtonCallback() {
	updateZ -= deltaZ;
}

void Light1ButtonCallback() {

	ambi[0] = 0.1;
	ambi[1] = 0.1;
	ambi[2] = 0.1;
	ambi[3] = 0.1;

	lightZeroColor[0] = 0.9;
	lightZeroColor[1] = 0.9;
	lightZeroColor[2] = 0.9;
	lightZeroColor[3] = 0.9;

}

void Light2ButtonCallback() {

	ambi[0] = 0.1;
	ambi[1] = 0.1;
	ambi[2] = 0.1;
	ambi[3] = 0.1;

	lightZeroColor[0] = 0.4;
	lightZeroColor[1] = 0.4;
	lightZeroColor[2] = 0.4;
	lightZeroColor[3] = 0.4;

}

void Light3ButtonCallback() {

	ambi[0] = 0.5;
	ambi[1] = 0.0;
	ambi[2] = 0.0;
	ambi[3] = 0.1;

	lightZeroColor[0] = 0.9;
	lightZeroColor[1] = 0.3;
	lightZeroColor[2] = 0.3;
	lightZeroColor[3] = 0.3;

}