#pragma once
#include "Vec2.h"

//プレイヤークラス
class Player
{

	
public:
	//コンストラクタ
	Player();
	//デストラクタ
	~Player();

	//ゲーム中に登場する物体に実装するメンバー関数の定番が存在する
	void Init(int handle); //初期化
	void End();  //終了処理

	void Update(); //毎フレームの更新処理　キャラクターの移動など
	void Draw();	//Update()での更新を踏まえて描画を行う

	//当たり判定の情報取得用関数
	float GetPosX() const { return m_pos.x; }
	float GetPosY() const { return m_pos.y; }
	float GetRadius() const { return m_radius; }
//	float GetPosX() const;	//宣言部分のみ各場合はこんな感じ

	//死亡情報の設定
	void SetDead(bool isDead) { m_isDead = isDead; }
	//死亡情報の取得
	bool IsDead() const { return m_isDead; }

private:
	//メンバー変数
	int m_handle; //グラフィックハンドル

	//生きているか死んでいるか
	bool m_isDead;

//	int m_x;	  //中心座標X
//	int m_y;	  //中心座標Y
	Vec2 m_pos;	
	//当たり判定の半径
	float m_radius;


	bool m_isTurn;//左右反転するか

	//アニメーション関連

	int m_animFrame;	//アニメーションのフレーム数
};



