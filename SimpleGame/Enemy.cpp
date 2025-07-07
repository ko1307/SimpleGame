#include "Enemy.h"
#include "DxLib.h"
#include "Game.h"


//敵キャラクターの実装
//画面買い左右のどこかから登場する
//右から出てきたら左へ、左から出てきたら右へ移動する
//画面外まで移動したら、再度左右のどこかから出てくる

namespace
{
	//移動速度
	constexpr float kSpeed = 5.0f;
	//半径
	constexpr float kDefaultRadius = 12.0f;
	//プレイヤーグラフィックのサイズ
	constexpr int kGraphWidth = 32;
	constexpr int kGraphHeight = 34;
	//アニメーション情報
	constexpr int kRunAnimNum = 14;
}

Enemy::Enemy() :
	m_handle(-1),
	m_radius(0.0f),
	m_moveX(0.0f)
{
}

Enemy::~Enemy()
{

}

void Enemy::Init(int handle)
{
	m_handle = handle;
	m_radius = kDefaultRadius;
	
	SetupPos();

}

void Enemy:: End()
{
	
}

void Enemy::Update()
{
	m_pos.x += m_moveX;
	//画面外に出た敵を再登場させる
	if (m_moveX < 0.0f)//左に移動している敵が
	{
		if (m_pos.x < 0.0f - m_radius)//画面外から外に出たら
		{
			SetupPos();
		}
	}
	if (m_moveX > 0.0f) //右に移動している敵が
	{
		if (m_pos.x > Game::kScreenWidth + m_radius) //画面右から外に出たら
		{
			SetupPos();
		}
	}
	
}

void Enemy::Draw()
{
	DrawRectGraph(static_cast<int>(m_pos.x) - kGraphWidth / 2,
				  static_cast<int>(m_pos.y) - kGraphHeight / 2,
				  0,0,kGraphWidth,kGraphHeight,
				  m_handle,true,false
				 );
//Release版では　実行してほしくない処理を
//#ifdef _DEBUG と #endif で囲む
//#ifdef _DEBUG と #endif で囲魔れた部分は
//Debug番では機能するが、Release番では機能しない
#ifdef _DEBUG

	//当たり判定のデバッグ表示
	//Release番では表示しないようにしたい
	DrawCircle(static_cast<int>(m_pos.x),
			   static_cast<int>(m_pos.y),
			   static_cast<int>(m_radius),
			   GetColor(255, 0, 0), false);
#endif

//逆にRelese版でのみ実行させたい処理は
//#ifdef NDEBUG と #endif で囲む
#ifdef NDEBUG
#endif
}

void Enemy::SetupPos()
{
	if (GetRand(1))
	{
		//右から登場
		m_pos.x = Game::kScreenWidth + m_radius;
		m_moveX = -kSpeed;
	}
	else
	{
		//左から登場
		m_pos. x = 0 - m_radius;
		m_moveX = kSpeed;
	}
	m_pos.y = GetRand(Game::kScreenHeight - m_radius * 2) + m_radius;
}
