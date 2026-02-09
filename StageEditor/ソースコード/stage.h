//==============================
//
// 自販機[vender.h]
// Author:kaiti
//
//==============================
#ifndef STAGE_H_
#define STAGE_H_

#include "main.h"
#include "object.h"
#include "model.h"

//クラス
class CStage:public CObject
{
public:
	//種類
	typedef enum
	{
		TYPE_MODEL = 0,
		TYPE_GIMMICK,
		TYPE_MAX
	}TYPE;

	//メンバ関数
	CStage(int nPriority = 4);
	~CStage();
	HRESULT Init(const char* pFilename, const D3DXVECTOR3 pos, const D3DXVECTOR3 rot, TYPE type, int nIndx);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	static CStage* Create(const char* pFilename, const D3DXVECTOR3 pos, const D3DXVECTOR3 rot, TYPE type, int nIndx);
	D3DXVECTOR3 GetSize(void) { return m_size; }
	D3DXVECTOR3 GetPos(void) { return m_pos; }
	D3DXVECTOR3 GetRot(void) { return m_rot; }
	TYPE GetType(void) { return m_type; }
	int GetIndx(void) { return m_nIndx; }
	const char* GetFilename() const { return m_pFileName; }
	static int GetCntModel(void) { return m_nCntModel; }
	static int GetMaxIndx(void) { return m_Max_Indx; }
private:
	//メンバ変数
	D3DXVECTOR3 m_pos;			//位置(オフセット)
	D3DXVECTOR3 m_rot;			//向き
	D3DXVECTOR3 m_size;			//サイズ

	D3DXMATRIX m_mtxWorld;		//ワールドマトリックス

	CModel* m_pModel;			//モデルへのポインタ
	TYPE m_type;
	int m_nIndx;
	char m_pFileName[MAX_WORD];

	static int m_nCntModel;
	static int m_Max_Indx;
};

#endif