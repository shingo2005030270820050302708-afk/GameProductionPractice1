#pragma once
#include "DxLib.h"


// アニメーションデータ構造体
struct AnimationData
{
	bool active;        // アニメーション中か
	bool isLoop;        // ループ再生するか
	int handle;         // 読み込んだ画像ハンドル
	int nowFrame;       // 現在のフレーム番号
	int frameNum;       // 全フレーム数
	int frameWidth;     // 1コマの幅
	int frameHeight;    // 1コマの高さ
	int timer;          // コマ切り替えタイマー
	int interval;       // コマ切り替え間隔（フレーム単位）
	float posX;         // 描画X座標
	float posY;         // 描画Y座標
	int frameIndex;
	int frameWait;
};


// 関数プロトタイプ宣言
void InitAnimation(AnimationData* animData);
void StartAnimation(AnimationData* animData, float posX, float posY, int interval, int frameNum, int width, int height, bool isLoop);
void UpdateAnimation(AnimationData* animData);
void DrawAnimation(AnimationData* animData, bool isTurnX = false, bool isTurnY = false);
void DrawAnimation(AnimationData* animData, float posX, float posY, bool isTurnX = false, bool isTurnY = false);
