#include "Item.h"
#include "Game.h"
#include "DxLib.h"

namespace
{
	//半径
	constexpr float kRadius = 16.0f;
	//配置情報
	constexpr float kSettingSpaceX = 160;
	constexpr float kSettingSpaceY = 120;
	//グラフィックサイズ
	constexpr int kWidth = 32;
	constexpr int kHeight = 32;
}

Item::Item() :
	m_isExist(true),
	m_handle(-1),
	m_radius(0.0f)
{
}

Item::~Item()
{
}

void Item::Init(int handle)
{
	//初期状態では存在する
	m_isExist = true;

	//グラフィックを受け取る
	m_handle = handle;

	//ランダムな位置に置く
	m_pos.x = GetRand(Game::kScreenWidth - kSettingSpaceX * 2) + kSettingSpaceX;
	m_pos.y = GetRand(Game::kScreenHeight - kSettingSpaceY * 2) + kSettingSpaceY;
	//半径の設定
	m_radius = kRadius;
}

void Item::End()
{

}

void Item::Update()
{
	//このアイテムが存在しない場合、何もせずに終わる
	if (!m_isExist)
	{
		//以降の処理は行わずここで関数を終了する
		return;
	}

	if (m_isExist)
	{

		//m_isExist = trueの時実行される
	}
}

void Item::Draw()
{
	//このアイテムが存在しない場合、何もせずに終わる
	if (!m_isExist)
	{
		//以降の処理は行わずここで関数を終了する
		return;
	}

	DrawRectGraph(static_cast<int>(m_pos.x - kWidth / 2), static_cast<int>(m_pos.y - kHeight / 2),
		0, 0, 32, 32,
		m_handle, true);

#ifdef _DEBUG
	DrawCircle(static_cast<int>(m_pos.x), static_cast<int>(m_pos.y),
		static_cast<int>(m_radius), GetColor(256, 0, 255), false);
#endif
}