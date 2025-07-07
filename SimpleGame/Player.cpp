#include "Player.h"
#include "Dxlib.h"
#include "Enemy.h"
#include "SceneMain.h"

//定数定義
namespace
{
	//プレイヤーの開始位置
	constexpr int kPlayerStartX = 256;
	constexpr int kPlayerStartY = 128;
	//プレイヤーグラフィックのサイズ
	constexpr int kGraphWidth = 32;
	constexpr int kGraphHeight = 32;
	//アニメーション情報
	constexpr int kIdleAnimNum = 11;
	constexpr int kAnimWaitFrame = 5;	//アニメ1コマ当たりのフレーム数

	//当たり判定の半径
	constexpr float kDefaultRadius = 16.0f;

	//移動速度
	constexpr int kSpeed = 4;
}

//コンストラクタ
// コンストラクタはPlayerのメモリが確保されたとき、
// 自動的に、最初に 必ず呼び出される
// 
//コンストラクタですべてのメンバー変数の初期化を行っておく

//コンストラクタでの変数の初期化を行うときは【コンストラクタ初期化子】
//を使って初期化を行う
Player::Player() :
	m_handle(-1),
	m_isDead(false),
	m_radius(0.0f),
	m_isTurn(false),
	m_animFrame(0)
{	
}

//デストラクタ
//メモリが解放されるとき、
//自動的に、 最後に 呼ばれる
Player::~Player()
{
	
}



void Player::Init(int handle)
{
	//各変数の初期化
	m_handle = handle;
	m_isDead = false;
	m_pos.x = kPlayerStartX;
	m_pos.y = kPlayerStartY;
	m_radius = kDefaultRadius;
	m_isTurn = false;

	m_animFrame = 0;
}

void Player::End()
{
}

void Player::Update()
{
	m_animFrame++;
	if (m_animFrame >= kIdleAnimNum * kAnimWaitFrame)
	{
		m_animFrame = 0;
	}
	

	//コントローラーのボタンの押された状態を取得する
	int pad = GetJoypadInputState(DX_INPUT_KEY_PAD1);
	if ((pad & PAD_INPUT_UP) != 0) //&演算:ビット単位の演算
	{
		m_pos.y -= kSpeed;
	}
	if ((pad & PAD_INPUT_DOWN) != 0)
	{
		m_pos.y += kSpeed;
	}
	if ((pad & PAD_INPUT_LEFT) != 0)
	{
		m_pos.x -= kSpeed;
		m_isTurn = true;
	}
	if ((pad & PAD_INPUT_RIGHT) != 0)
	{
		m_pos.x += kSpeed;
		m_isTurn = false;
	}
}



void Player::Draw()
{
	//アニメーションのフレーム数から表示したいコマ番号を計算で求める
	int animNo = m_animFrame / kAnimWaitFrame;

	//アニメーションの進行に合わせてグラフィックの切り取り位置を変更する
	int srcX = kGraphWidth * animNo;
	int srcY = 0;
	
	DrawRectGraph(static_cast<int>(m_pos.x) - kGraphWidth / 2,
		static_cast<int>(m_pos.y) - kGraphHeight / 2,
		srcX,srcY,
		kGraphWidth,kGraphHeight,
		m_handle, true, m_isTurn
	);
#ifdef _DEBUG
	DrawCircle(static_cast<int>(m_pos.x),
			   static_cast<int>(m_pos.y),
			   static_cast<int>(m_radius),
			   GetColor(255, 255, 0), false
	          );
#endif
}