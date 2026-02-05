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
		SOUND_LABEL_HIT,
		SOUND_LABEL_SELECT,
		SOUND_LABEL_TITLELOGO,
		SOUND_LABEL_THROWP,
		SOUND_LABEL_THROWB,
		SOUND_LABEL_GETFLOWER,
		SOUND_LABEL_GETBOUQUWT,
		SOUND_LABEL_SETBOUQUWT,
		SOUND_LABEL_GATHER,
		SOUND_LABEL_FIRE,
		SOUND_LABEL_THUNDER,

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
		{"data/SOUND/BGM/title.wav", -1},
		{"data/SOUND/BGM/game.wav", -1},
		{"data/SOUND/BGM/result.wav", -1},

		// SE
		{"data/SOUND/SE/Damage00.wav", 0},
		{"data/SOUND/SE/select00.wav", 0},
		{"data/SOUND/SE/title00.wav", 0},
		{"data/SOUND/SE/throwP.wav", 0},
		{"data/SOUND/SE/throwB.wav", 0},
		{"data/SOUND/SE/getFlower.wav", 0},
		{"data/SOUND/SE/getBouquet.wav", 0},
		{"data/SOUND/SE/putBouquet.wav", 0},
		{"data/SOUND/SE/gather.wav", 0},
		{"data/SOUND/SE/Fire.wav", 0},
		{"data/SOUND/SE/Thunder.wav", 0},

	};

};

#endif
