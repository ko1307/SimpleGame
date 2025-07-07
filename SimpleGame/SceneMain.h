#pragma once
#include "Player.h"
#include "Enemy.h"
#include "Item.h"
#include "Bg.h"

class SceneMain
{
public:
	SceneMain();  //コンストラクタ
	~SceneMain(); //デストラクタ

	void Init();	//初期化
	void End();		//終了処理
	void Update();	//毎フレームの更新
	void Draw();	//描画

private:
	//シーケンス
	enum Seq
	{
		SeqFadeIn,
		SeqGame,
		SeqClear,
		SeqGameover,

		SeqNum
	};

private:
	//残りアイテム数の取得
	int GetItemNum();

	//シーケンス毎にUpdate処理を切り替える
	void UpdateFadeIn();
	void UpdateGame();

	void UpdateClear();
	void UpdateGameover();

private:
	//フォントのハンドル
	int m_scoreFontHandle;		//残り？個を表示するフォント
	int m_resultFontHandle;		//ゲームオーバー、クリアを表示するフォント

	//使用するグラフィック
	int m_playerGraphHandle;	//プレイヤーのグラフィック
	int m_enemyGraphHandle;		//敵のグラフィック
	int m_itemGraphHandle;		//アイテムのグラフィック
	int m_bgGraphHandle;		//背景のグラフィック

	int m_mainBgmHandle;		//BGM
	int m_gameoverBgmHandle;		//ゲームオーバー時のBGM
	int m_itemGetSeHandle;		//アイテム取得時のサウンド

	//サウンドの音量
	int m_mainVolume;
	int m_gameoverVolume;

	//現在のシーケンス
	Seq m_gameSeq;
	int m_frameCount;

	//フェードのフレーム数 0:真っ暗
	int m_fadeFrame;

	//プレイヤー
	Player m_player;
	//敵
	Enemy m_enemyTbl[16];
	//集めるアイテム
	Item m_itemTbl[16];

	//背景
	Bg m_bg;
};

