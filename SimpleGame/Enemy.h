#pragma once
#include "Vec2.h"

class Enemy
{
public:
	//コンストラクタ
	Enemy();
	//デストラクタ
	~Enemy();

	void Init(int handle);	//初期化
	void End();		//終了処理

	void Update();	//毎フレーム
	void Draw();	//描画

	//当たり判定の情報取得用関数
	float GetPosX() const { return m_pos.x; }
	float GetPosY() const { return m_pos.y; }
	float GetRadius() const { return m_radius; }

	
private:
	//ランダムに初期位置を設定する
	void SetupPos();

private:
	//グラフィックハンドル
	int m_handle;
	//表示位置
	Vec2 m_pos;
	//半径
	float m_radius;

	//移動情報
	float m_moveX;
};

