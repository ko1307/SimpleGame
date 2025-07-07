#pragma once
class Bg
{
public:
	Bg();
	~Bg();

	void Init(int handle);
	void End();
	void Update();
	void Draw();

private:

	//グラフィックハンドル
	int m_handle;
	//グラフィックが横に何個チップがあるかの情報
	int m_graphChipNumX;
};

