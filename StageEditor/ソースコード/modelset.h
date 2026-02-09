//==============================
//
// プレイヤー[player.h]
// Author:kaiti
//
//==============================
#ifndef MODELSET_H_
#define MODELSET_H_

#include "main.h"
#include "object.h"
#include "model.h"
#include "motion.h"
#include "stage.h"
#include "loadstage.h"
#include "object3D.h"

//#define MAX_STAGE_MODEL (32)

//プレイヤークラス
class CModelSet:public CObject
{
public:
	//種類
	typedef enum
	{
		TYPE_MODEL = 0,
		TYPE_FIELD,
		TYPE_WALL,
		TYPE_GIMMICK,
		TYPE_MAX
	}TYPE;

	//メンバ関数
	//CPlayer();
	CModelSet(int nPriority = 5);
	~CModelSet();
	static CModelSet* Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot,TYPE type);
	HRESULT Init(D3DXVECTOR3 pos, D3DXVECTOR3 rot);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	//設定
	void SetType(TYPE type) { m_type = type; };
	void SetPos(D3DXVECTOR3 pos) { m_pos = pos; }
	void SetPosY(float Y) { m_pos.y = Y; }
	void SetMove(D3DXVECTOR3 move) { m_move = move; }

	//取得
	D3DXVECTOR3 GetPos(void) { return m_pos; }
	D3DXVECTOR3 GetRot(void) { return m_rot; }
	D3DXVECTOR3 GetSize(void) { return m_size; }
	D3DXVECTOR3 GetPosOld(void) { return m_posOld; }
	int GetNumModel(void) { return m_nNumModel; }
	int GetNumTex(void) { return m_nNumTex; }
	D3DXMATRIX GetMtx(void) { return m_mtxWorld; }
	const char* GetModelFilename(int index);
	const char* GetTexFilename(int index);
private:
	void Move(void);
	void Action(void);
	bool Collision(void);

	//メンバ変数
	D3DXVECTOR3 m_pos;					//位置(オフセット)
	D3DXVECTOR3 m_posOld;				//前回の位置
	D3DXVECTOR3 m_rot;					//向き
	D3DXVECTOR3 m_rotDest;				//目標の向き
	D3DXVECTOR3 m_move;					//移動量
	D3DXVECTOR3 m_size;					//サイズ
	D3DXMATRIX m_mtxWorld;				//ワールドマトリックス


	//ステータス
	int m_nLife;						//ライフ
	float m_fSpeed;						//移動速度
	TYPE m_type;
	//モデル
	CModel* m_apModel[NUM_STAGE_MODEL];	//モデルへのポインタ
	char m_pModelName[NUM_STAGE_MODEL][MAX_WORD];
	int m_nNumModel;					//モデル最大数
	int m_nDraw;
	CStage::TYPE m_ModelType;
	
	
	CObject3D* m_pField;
	CObject3D* m_pWall;

	float fX;
	float fZ;
	char m_apFieldTexture[NUM_STAGE_MODEL][MAX_WORD];  // 追加
	int m_nDrawField;

	int m_nNumTex;					//モデル最大数

};

#endif