#pragma once

struct BoxCollision
{
	float posX;
	float posY;
	float width;
	float height;
};

// ‹éŒ`‚Æ“_‚Ì“–‚½‚è”»’è
bool CheckSquarePoint(float squarePosX, float squarePosY, float squareWidth, float squareHeight, float pointX, float pointY);

// ‹éŒ`‚Æ‹éŒ`‚Ì“–‚½‚è”»’è
bool CheckSquareSquare(float squareA_PosX, float squareA_PosY, float squareA_Width, float squareA_Height, float squareB_PosX, float squareB_PosY, float squareB_Width, float squareB_Height);

// ‰~‚Æ“_‚Ì“–‚½‚è”»’è
bool CheckCirclePoint(float circlePosX, float circlePosY, float circleRadius, float pointX, float pointY);

// ‰~‚Æ‰~‚Ì“–‚½‚è”»’è
bool CheckCircleCircle(float circleA_PosX, float circleA_PosY, float circleA_Radius, float circleB_PosX, float circleB_PosY, float circleB_Radius);

// Še“–‚½‚è”»’è
void CheckCollision();

