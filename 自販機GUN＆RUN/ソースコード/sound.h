//=============================================================================
//
// サウンド処理 [sound.h]
// Author : 
//
//=============================================================================

// 二重インクルード防止
#ifndef _SOUND_H_
#define _SOUND_H_

// インクルード
#include "main.h"
#include "xaudio2.h"

// サウンドクラス
class CSound
{
public:
	//*****************************************************************************
	// サウンド一覧
	//*****************************************************************************
	typedef enum
	{
		// BGM
		SOUND_LABEL_TITLE = 0,
		SOUND_LABEL_GAME,
		SOUND_LABEL_RESULT,
		// SE
		SOUND_LABEL_BREAK,
		SOUND_LABEL_CAR,
		SOUND_LABEL_CRITICAL,
		SOUND_LABEL_DAMAGE,
		SOUND_LABEL_HIT,
		SOUND_LABEL_NODAMAGE,
		SOUND_LABEL_RESIST,
		SOUND_LABEL_RESTOCK,
		SOUND_LABEL_SHOT,
		SOUND_LABEL_CONFIRM,
		SOUND_LABEL_TITLELOGO,
		SOUND_LABEL_KILL,
		SOUND_LABEL_ITEM,
		SOUND_LABEL_MISS,
		SOUND_LABEL_START,
		SOUND_LABEL_FINISH,
		SOUND_LABEL_SELECT,
		SOUND_LABEL_CHANGE,
		SOUND_LABEL_CONVENI,
		SOUND_LABEL_ADD,

		SOUND_LABEL_MAX,
	} SOUND_LABEL;

	//*****************************************************************************
	// サウンド情報の構造体定義
	//*****************************************************************************
	typedef struct
	{
		const char* pFilename;	// ファイル名
		int nCntLoop;			// ループカウント
	} SOUNDINFO;

	CSound();
	~CSound();

	HRESULT Init(HWND hWnd);
	void Uninit(void);
	HRESULT PlaySound(SOUND_LABEL label);
	void Stop(SOUND_LABEL label);
	void Stop(void);
	HRESULT CheckChunk(HANDLE hFile, DWORD format, DWORD* pChunkSize, DWORD* pChunkDataPosition);
	HRESULT ReadChunkData(HANDLE hFile, void* pBuffer, DWORD dwBuffersize, DWORD dwBufferoffset);

private:
	IXAudio2* m_pXAudio2 = NULL;								// XAudio2オブジェクトへのインターフェイス
	IXAudio2MasteringVoice* m_pMasteringVoice = NULL;			// マスターボイス
	IXAudio2SourceVoice* m_apSourceVoice[SOUND_LABEL_MAX] = {};	// ソースボイス
	BYTE* m_apDataAudio[SOUND_LABEL_MAX] = {};					// オーディオデータ
	DWORD m_aSizeAudio[SOUND_LABEL_MAX] = {};					// オーディオデータサイズ

	// サウンドの情報
	SOUNDINFO m_aSoundInfo[SOUND_LABEL_MAX] =
	{
		// BGM
		{"data/SOUND/BGM/title00.wav", -1},
		{"data/SOUND/BGM/game00.wav", -1},
		{"data/SOUND/BGM/result00.wav", -1},

		// SE
		{"data/SOUND/SE/break00.wav", 0},
		{"data/SOUND/SE/car00.wav", 0},
		{"data/SOUND/SE/critical00.wav", 0},
		{"data/SOUND/SE/Damage00.wav", 0},
		{"data/SOUND/SE/hit00.wav", 0},
		{"data/SOUND/SE/NoDamage00.wav", 0},
		{"data/SOUND/SE/Resist00 .wav", 0},
		{"data/SOUND/SE/restock00.wav", 0},
		{"data/SOUND/SE/shot00.wav", 0},
		{"data/SOUND/SE/switch00.wav", 0},
		{"data/SOUND/SE/title00.wav", 0},
		{"data/SOUND/SE/kill00.wav", 0},
		{"data/SOUND/SE/item00.wav", 0},
		{"data/SOUND/SE/miss00.wav", 0},
		{"data/SOUND/SE/start00.wav", 0},
		{"data/SOUND/SE/finish00.wav", 0},
		{"data/SOUND/SE/Laser.wav", 0},
		{"data/SOUND/SE/change.wav", 0},
		{"data/SOUND/SE/conveni.wav", 0},
		{"data/SOUND/SE/add.wav", 0},

	};

};

#endif
