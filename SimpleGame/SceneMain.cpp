#include "SceneMain.h"
#include "DxLib.h"
#include "Game.h"

//#include <math.h>
#include <cmath>

namespace
{
	//フェードにかけるフレーム数
	constexpr int kFadeFrame = 30;

}

SceneMain::SceneMain() :
	m_itemGetSeHandle(-1),
	m_scoreFontHandle(-1),
	m_playerGraphHandle(-1),
	m_enemyGraphHandle(-1),
	m_itemGraphHandle(-1),
	m_bgGraphHandle(-1),
	m_mainBgmHandle(-1),
	m_gameoverBgmHandle(-1),
	m_resultFontHandle(-1),
	m_mainVolume(0),
	m_gameoverVolume(0),
	m_gameSeq(SeqFadeIn),
	m_frameCount(0),
	m_fadeFrame(0)
{
}

SceneMain::~SceneMain()
{
}

void SceneMain::Init()	//初期化
{
	//ゲームで使用するフォントを作成
	m_scoreFontHandle = CreateFontToHandle("HGP創英角ﾎﾟｯﾌﾟ体", 32, -1, DX_FONTTYPE_ANTIALIASING_EDGE_8X8);
	m_resultFontHandle = CreateFontToHandle("HGP行書体", 96, -1, -1);

	//初期シーケンスの決定
	m_gameSeq = SeqFadeIn;
	m_frameCount = 0;

	//フェード状態の初期化
	m_fadeFrame = 0;

	//グラフィックのロード
	m_playerGraphHandle = LoadGraph("data/Idle.png");
	m_enemyGraphHandle = LoadGraph("data/Enemy.png");
	m_itemGraphHandle = LoadGraph("data/Melon.png");
	m_bgGraphHandle = LoadGraph("data/Mapchip.png");
	//サウンドのロード
	m_itemGetSeHandle = LoadSoundMem("data/ItemGet.mp3");
	m_mainBgmHandle = LoadSoundMem("data/MainBgm.mp3");
	m_gameoverBgmHandle = LoadSoundMem("data/GameoverBgm.mp3");

	//BGMの再生開始
	m_mainVolume = 128;
	m_gameoverVolume = 0;

	PlaySoundMem(m_mainBgmHandle, DX_PLAYTYPE_LOOP);
	ChangeVolumeSoundMem(170, m_mainBgmHandle);

	//各オブジェクトの初期化
	m_player.Init(m_playerGraphHandle);
	for (auto& enemy : m_enemyTbl)
	{
		enemy.Init(m_enemyGraphHandle);
	}

	for (auto& item : m_itemTbl)
	{
		item.Init(m_itemGraphHandle);
	}

	m_bg.Init(m_bgGraphHandle);
}

void SceneMain::End()			//終了処理
{
	//BGMを停止する
	StopSoundMem(m_mainBgmHandle);
	StopSoundMem(m_gameoverBgmHandle);

	m_player.End();
	for (auto& enemy : m_enemyTbl)
	{
		enemy.End();
	}

	for (auto& item : m_itemTbl)
	{
		item.End();
	}
	m_bg.End();
	//フォントの削除
	DeleteFontToHandle(m_scoreFontHandle);
	DeleteFontToHandle(m_resultFontHandle);
	//グラフィックを解放
	DeleteGraph(m_playerGraphHandle);
	DeleteGraph(m_enemyGraphHandle);
	DeleteGraph(m_itemGraphHandle);
	DeleteGraph(m_bgGraphHandle);
	//サウンドを削除
	DeleteSoundMem(m_itemGetSeHandle);
	DeleteSoundMem(m_mainBgmHandle);
	DeleteSoundMem(m_gameoverBgmHandle);
}

void SceneMain::Update()	//毎フレームの更新
{
	//現在のシーケンスの経過フレーム数をカウント
	m_frameCount++;
	switch (m_gameSeq)
	{
	case SeqFadeIn:
		UpdateFadeIn();
		break;
	case SeqGame:
		UpdateGame();
		break;
	case SeqClear:
		UpdateClear();
		break;
	case SeqGameover:
		UpdateGameover();
		break;
	}
	//UpdateGame();
}

void SceneMain::Draw()	//描画
{
	m_bg.Draw();
	m_player.Draw();
	for (auto& enemy : m_enemyTbl)
	{
		enemy.Draw();
	}

	for (auto& item : m_itemTbl)
	{
		item.Draw();
	}
	//残りアイテム数を表示する
	int itemNum = GetItemNum();

	//文字を画面右上に表示する
	int strWidth = GetDrawFormatStringWidthToHandle(m_scoreFontHandle, "のこり%d個", itemNum);

	DrawFormatStringToHandle(Game::kScreenWidth - strWidth, 16, //座標
		GetColor(255, 255, 255), m_scoreFontHandle, "のこり%d個", itemNum);

	//ゲームクリア表示
	if (itemNum == 0)
	{
		strWidth = GetDrawFormatStringWidthToHandle(m_resultFontHandle, "赤崎バゼル襲来赤崎バゼル襲来");
		//int x = Game::kScreenWidth / 2 - strWidth / 2;
		int x = -3; //遊び　//文字を左から出すため
		int y = Game::kScreenHeight / 2 - 96 / 2;
		DrawStringToHandle(x, y, "赤崎バゼル襲来 赤崎バゼル襲来", GetColor(240, 32, 32), m_resultFontHandle);
	}
	else if (m_player.IsDead()) //死んでいたら
	{
		strWidth = GetDrawFormatStringWidthToHandle(m_resultFontHandle, "赤崎バルス");
		int x = Game::kScreenWidth / 2 - strWidth / 2;
		int y = Game::kScreenHeight / 2 - 96 / 2;
		DrawStringToHandle(x, y, "赤崎バルス", GetColor(240, 32, 32), m_resultFontHandle);

		if ((m_frameCount / 30) % 2)
		{
			strWidth = GetDrawFormatStringWidthToHandle(m_scoreFontHandle, "A:リトライ");
			x = Game::kScreenWidth / 2 - strWidth / 2;
			y = Game::kScreenHeight / 2 + 64;
			DrawStringToHandle(x, y, "A:リトライ", GetColor(240, 32, 255), m_scoreFontHandle);
		}

#ifdef _DEBUG
		//現在実行中のシーンのデバッグ表示
		DrawString(0, 0, "SceneMain", GetColor(255, 255, 255));
		DrawString(0, 16, "Xボタンでゲームクリア", GetColor(255, 255, 255));
#endif
		//フェードの描画
		int fadeAlpha = 0;

		//フレームカウントをフェードの進行度(割合)に変換する
		float fadeProgress = static_cast<float>(m_fadeFrame) / kFadeFrame;
		//255 -> 0に変化させたいので割合を逆転させる 0.0 -> 1.0  1.0 -> 0.0
		fadeProgress = 1.0f - fadeProgress;
		//割合をかけることで現在のフェード値を決定する
		fadeAlpha = 255 * fadeProgress;

		SetDrawBlendMode(DX_BLENDMODE_ALPHA, fadeAlpha);
		DrawBox(0, 0, Game::kScreenWidth, Game::kScreenHeight, GetColor(0, 0, 0), true);
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
	}
}

int SceneMain::GetItemNum()
{
	int itemNum = 0;
	for (auto& item : m_itemTbl)
	{
		if (item.IsExist())
		{
			itemNum++;
		}
	}
	return itemNum;
}

void SceneMain::UpdateFadeIn()
{
	m_fadeFrame++;
	if (m_fadeFrame > kFadeFrame)
	{
		m_fadeFrame = kFadeFrame;
		//フェード因果完了したのでゲーム本体に遷移する
		m_gameSeq = SeqGame;
		m_frameCount = 0;
		//printfDx("abc");
	}
	//キャラクターは動かさないので以降呼ばない
}

void SceneMain::UpdateGame()
{
#ifdef _DEBUG
	//ボタン一発でクリアできるデバッグ機能

	//Xボタンを押したらクリアになるようにする
	int pad = GetJoypadInputState(DX_INPUT_KEY_PAD1);
	//padのXボタンもしくはキーボードのCを押したとき
	if ((pad & PAD_INPUT_3) != 0) //&演算:ビット単位の演算
	{
		//m_gameSeq = SeqCliar;
			//勝利条件を満たすような処理を書く
		for (auto& item : m_itemTbl)
		{
			//すべてのアイテムを取得済みの状態にする
			item.SetExist(false);
		}
	}
#endif

	m_player.Update();
	for (auto& enemy : m_enemyTbl)
	{
		enemy.Update();
	}

	for (auto& item : m_itemTbl)
	{
		item.Update();
	}
	m_bg.Update();

	//プレイヤーと敵の当たり判定をとる
	const float playerX = m_player.GetPosX();
	const float playerY = m_player.GetPosY();
	const float playerRadius = m_player.GetRadius();
	for (auto& enemy : m_enemyTbl)
	{
		const float enemyX = enemy.GetPosX();
		const float enemyY = enemy.GetPosY();
		const float enemyRadius = enemy.GetRadius();

		//三平方の定理を使って円同士の距離を求める
		float distX = playerX - enemyX;
		float distY = playerY - enemyY;

		//距離の2乗を求める
		float dist = (distX * distX) + (distY * distY);
		//距離の2乗の平方根が距離
		dist = sqrtf(dist);

		if (dist < (playerRadius + enemyRadius))
		{
			/*printfDx("あたっちゃってるyo!\n");*/
			m_player.SetDead(true);
			m_gameSeq = SeqGameover;
			m_frameCount = 0;
			//BGMをゲームオーバーの時のものに切り替え
		//	StopSoundMem(m_mainBgmHandle);
			m_gameoverVolume = 0;
			PlaySoundMem(m_gameoverBgmHandle, DX_PLAYTYPE_LOOP);
			ChangeVolumeSoundMem(m_gameoverVolume, m_gameoverBgmHandle);
		}
	}


	//プレイヤーとアイテムの当たり判定を取得する
	for (auto& item : m_itemTbl)
	{
		//存在しないアイテムは取得できない
		if (!item.IsExist())
		{
			continue;
		}
		const float itemX = item.GetPos().x;
		const float itemY = item.GetPos().y;
		const float itemRadius = item.GetRadius();

		float distX = playerX - itemX;
		float distY = playerY - itemY;

		float dist = (distX * distX) + (distY * distY);
		dist = sqrtf(dist);
		if (dist < (playerRadius + itemRadius))
		{
			//	printfDx("アイテムに当たってる!\n");
			item.SetExist(false);
			//	PlaySoundFile("data/ItemGet.mp3", DX_PLAYTYPE_BACK);
			PlaySoundMem(m_itemGetSeHandle, DX_PLAYTYPE_BACK);
		}
	}
	//アイテムの数が0個になったらゲームクリア
	if (GetItemNum() == 0)
	{
		m_gameSeq = SeqClear;
		m_frameCount = 0;
	}
}

void SceneMain::UpdateClear()
{
	m_player.Update();
}

void SceneMain::UpdateGameover()
{
	int pad = GetJoypadInputState(DX_INPUT_KEY_PAD1);
	if ((pad & PAD_INPUT_1) != 0)
	{
		m_player.Init(m_playerGraphHandle);
		for (auto& enemy : m_enemyTbl)
		{
			enemy.Init(m_enemyGraphHandle);
		}
		for (auto& item : m_itemTbl)
		{
			item.Init(m_itemGraphHandle);
		}
		m_bg.Init(m_bgGraphHandle);
		//BGMボリュームの再設定
		m_mainVolume = 128;
		m_gameoverVolume = 0;
		ChangeVolumeSoundMem(m_mainVolume, m_mainBgmHandle);
		StopSoundMem(m_gameoverBgmHandle);

		m_gameSeq = SeqGame;
		m_frameCount = 0;
	}


	m_mainVolume--;
	if (m_mainVolume < 0)
	{
		m_mainVolume = 0;
	}
	//gameoverのボリューム上げる
	m_gameoverVolume++;
	if (m_gameoverVolume > 128)
	{
		m_gameoverVolume = 128;
	}
	ChangeVolumeSoundMem(m_mainVolume, m_mainBgmHandle);
	ChangeVolumeSoundMem(m_gameoverVolume, m_gameoverBgmHandle);
}
