//===============================================
//
// インタラクトオブジェクト処理[interact.h]
// Author : Kaiti Aoto
//
//===============================================

// 二重インクルード防止
#ifndef _INTERACT_H_
#define _INTERACT_H_

// インクルード
#include "objectPhysics.h"
#include "meshcircle.h"

// インタラクトオブジェクトクラス
class CInteract :public CObjectPhysics
{
public:
	// 種類
	typedef enum
	{
		TYPE_DUST = 0,
		TYPE_GOODS,
		TYPE_FLOWER,
		TYPE_MAX
	}TYPE;

	// メンバ関数
	CInteract(int nPriority = static_cast<int>(Priority::StageModel));
	virtual ~CInteract();

	HRESULT Init(const char* Filename, D3DXVECTOR3 pos, D3DXVECTOR3 rot, float fWeight, TYPE type);
	virtual void Uninit(void);
	virtual void Update(void);
	void Draw(void);

	void ChangeAsh(void);

	// 設定
	void SetType(TYPE type) { m_type = type; }
	void SetGather(bool bGather) { m_bGather = bGather; }
	void SetDrop(bool bDrop, D3DXVECTOR3 AreaPos) { m_bDrop = bDrop; m_AreaPos = AreaPos; }
	void SetMyRising(void) { m_bRising = true; m_bDrop = false; }
	// 取得
	TYPE GetType(void) { return m_type; }
	bool GetbAsh(void) { return m_bAsh; }
	bool GetGather(void) { return m_bGather; }
	bool GetRising(void) { return m_bRising; }

	// 静的メンバ関数
	static CInteract* Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, TYPE type);
	static CInteract* Create(const char* Filename, D3DXVECTOR3 pos, D3DXVECTOR3 rot, float fWeight, TYPE type);
	static int GetNumDust(void) { return m_nNumDust; }
	static int GetNumGoods(void) { return m_nNumGoods; }
	static int GetNumFlower(void) { return m_nNumFlower; }
	static void RandAdd(TYPE type, D3DXVECTOR3 pos, int nNumCreate, float fSpace);
	static void SetRising(void);
private:
	bool CollRadius(void);
	void RisingMove(void);
	void ShakeDustBox(void);
	void Reaction(void);

	static constexpr float RISING_SPEED = 11.0f;
	static constexpr float MAX_RISING = 800.0f;
	// 灰モデル名
	static constexpr const char* AshFilename = "data\\MODEL\\ash00.x";

	// メンバ変数
	CMeshCircle* m_pCircle;		// 円へのポインタ
	D3DXVECTOR3 m_size;			// サイズ
	D3DXVECTOR3 m_AreaPos;		// 範囲の中心位置
	TYPE m_type;				// 種類
	float m_fRadius;			// 半径
	bool m_bAsh;				// 灰かどうか
	bool m_bGather;				// 収集エリアにあるか
	bool m_bRising;				// 上昇するかどうか
	bool m_bDrop;				// 零れ落ちるか
	bool m_bRLShake;

	// 静的メンバ変数
	static int m_nNumDust;				// ゴミの最大数
	static int m_nNumGoods;				// 雑貨の最大数
	static int m_nNumFlower;			// 花の最大数
};

// インタラクトゴミクラス
class CInteractDust :public CInteract
{
public:
	// 種類
	typedef enum
	{
		TYPE_BAG = 0,
		TYPE_BUCKET,
		TYPE_MAX
	}TYPE;

	CInteractDust();
	~CInteractDust();

	// 静的メンバ関数
	static CInteract* Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, CInteract::TYPE type);
	static CInteract* Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, CInteract::TYPE type, CInteractDust::TYPE DustType);
private:
	// モデル名
	static constexpr const char* Filename[TYPE_MAX] =
	{
		"data\\MODEL\\dust00.x",
		"data\\MODEL\\plasticBucket00.x",
	};
	// 重さ
	static constexpr float Weight[TYPE_MAX] =
	{
		1.0f,
		2.0f
		
	};

};

// インタラクト雑貨クラス
class CInteractGoods :public CInteract
{
public:
	// 種類
	typedef enum
	{
		TYPE_TISSUE = 0,
		TYPE_BOOKS,
		TYPE_CAR,
		TYPE_MAX
	}TYPE;

	CInteractGoods();
	~CInteractGoods();

	// 静的メンバ関数
	static CInteract* Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, CInteract::TYPE type);
	static CInteract* Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, CInteract::TYPE type, CInteractGoods::TYPE GoodsType);

private:
	// モデル名
	static constexpr const char* Filename[TYPE_MAX] =
	{
		"data\\MODEL\\tissue00.x",
		"data\\MODEL\\someBooks01.x",
		"data\\MODEL\\car00.x",

	};
	// 重さ
	static constexpr float Weight[TYPE_MAX] =
	{
		1.0f,
		2.0f,
		2.0f,
	};
};

// インタラクト花クラス
class CInteractFlower :public CInteract
{
public:
	// 種類
	typedef enum
	{
		TYPE_NONE = 0,
		TYPE_MAX
	}TYPE;

	CInteractFlower();
	~CInteractFlower();

	// 静的メンバ関数
	static CInteract* Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, CInteract::TYPE type);

private:
	// モデル名
	static constexpr const char* Filename[TYPE_MAX] =
	{
		"data\\MODEL\\flower00.x",
	};
	// 重さ
	static constexpr float Weight[TYPE_MAX] =
	{
		1.0f,
	};
};

#endif