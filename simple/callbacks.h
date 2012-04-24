float			deltaX = 0.4;
float			deltaY = 0.4;
float			deltaZ = 0.4;
float			updateX = 0.0;
float			updateY = 0.0;
float			updateZ = 0.0;
float			scaleAmount = 10.0;

GLfloat   ambi[]            = {0.8, 0.8, 0.8, 0.8};
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

	ambi[0] = 1;
	ambi[1] = 1;
	ambi[2] = 1;
	ambi[3] = 1;

	lightZeroColor[0] = 0.9;
	lightZeroColor[1] = 0.9;
	lightZeroColor[2] = 0.9;
	lightZeroColor[3] = 0.9;

}

//warm / halogen
void Light2ButtonCallback() {

	ambi[0] = 0.7;
	ambi[1] = 0.85;
	ambi[2] = 0.9;
	ambi[3] = 0.9;

	lightZeroColor[0] = 1;
	lightZeroColor[1] = 0.94;
	lightZeroColor[2] = 0.87;
	lightZeroColor[3] = 0.8;

}

//florescent
void Light3ButtonCallback() {

	ambi[0] = 0.75;
	ambi[1] = 0.88;
	ambi[2] = 1.0;
	ambi[3] = 0.8;

	lightZeroColor[0] = 0.75;
	lightZeroColor[1] = 0.85;
	lightZeroColor[2] = 1.0;
	lightZeroColor[3] = 0.5;

}

void ScaleIncreaseButtonCallback() {
	scaleAmount += 0.5;
}

void ScaleDecreaseButtonCallback() {
	scaleAmount -= 0.5;
}