#pragma once
#include "Vec2.h"

class Item
{
public:
	Item();
	~Item();

	void Init(int handle);
	void End();
	void Update();
	void Draw();

	Vec2 GetPos() const { return m_pos; }
	float GetRadius() const { return m_radius; }

	//アイテム取得時にm_isExistをfalseにするための関数
	void SetExist(bool isExist) {  m_isExist = isExist; }

	//このアイテムがゲーム中に存在するか
	bool IsExist() const { return m_isExist; }

	//グラフィックハンドル
	int m_handle;

private:

	//このアイテムがゲーム中に存在するか
	bool m_isExist;

	Vec2 m_pos;
	float m_radius;
};

