//==============================
//
// テクスチャ[texture.cpp]
// Author:kaiti
//
//==============================
#include "savestage.h"
#include "manager.h"
#include "renderer.h"
#include "model.h"
#include "player.h"
#include "modelset.h"
#include "texture.h"
//==================
// コンストラクタ
//==================
CSaveStage::CSaveStage()
{
	m_nCntStage = 0;
	m_nNumModel = 0;
    m_nCntField = 0;
    m_nCntWall = 0;
	for (int nCnt = 0; nCnt < MAX_STAGE_MODEL; nCnt++)
	{
		m_pStage[nCnt] = nullptr;
        m_pField[nCnt] = nullptr;
	}
}
//================
// デストラクタ
//================
CSaveStage::~CSaveStage()
{
}
//=======================
// ステージ読み込み
//=======================
void CSaveStage::Save(const char* pFilename)
{
    CTexture* pTex = CManager::GetTex();

    CModelSet* pModelSet = CManager::GetModelSet();
    //モデルの種類数
    int nMaxIndx  = pModelSet->GetNumModel();
    int nMaxTex = pModelSet->GetNumTex();

    //CModelSet* pModelSet = CManager::GetModelSet();
    //int nMaxModel=pModelSet->GetNumModel();

    //char* ModelFile[nMaxIndx];
    // 
    //設置モデル数
    m_nNumModel = CStage::GetCntModel();

    FILE* pFile = fopen(pFilename, "w");

    if (!pFile)
        return;

    // ヘッダー部分
    fprintf(pFile, "#==============================================================================\n");
    fprintf(pFile, "# 『モデル配置情報』ステージファイル名 [%s]\n", pFilename);
    fprintf(pFile, "#==============================================================================\n");
    fprintf(pFile, "SCRIPT\n\n");

    // モデル数
    fprintf(pFile, "# テクスチャ数\n");
    fprintf(pFile, "NUM_TEXTURE = %d\n\n", nMaxTex);

    // モデルファイル名
    for (int i = 0; i < nMaxTex; ++i)
    {
        const char* filename = pModelSet->GetTexFilename(i);
        if (filename && filename[0] != '\0')
        {
            fprintf(pFile, "TEXTURE_FILENAME = %s\n", filename);
        }
    }
    fprintf(pFile, "\n");


    // モデル数
    fprintf(pFile, "# モデル数\n");
    fprintf(pFile, "NUM_MODEL = %d\n\n", nMaxIndx);

    // モデルファイル名
    for (int i = 0; i < nMaxIndx; ++i)
    {
        const char* filename = pModelSet->GetModelFilename(i);
        if (filename && filename[0] != '\0')
        {
            fprintf(pFile, "MODEL_FILENAME = %s\n", filename);
        }
    }  
    fprintf(pFile, "\n");

    // モデル配置情報
    fprintf(pFile, "#==============================================================================\n");
    fprintf(pFile, "# フィールド配置情報\n");
    fprintf(pFile, "#==============================================================================\n");
    for (int i = 0; i < m_nCntField; ++i)
    {
        CObject3D::TYPE type = m_pField[i]->GetType();       // OBJECTの種別（0,1,2...） ※実装必要

        if (type == CObject3D::TYPE_FIELD)
        {
            if (!m_pField[i])
            {
                continue;
            }
            const D3DXVECTOR3& pos = m_pField[i]->GetPos();
            const D3DXVECTOR3& rot = m_pField[i]->GetRot();
            const float x = m_pField[i]->GetWidth();
            const float z = m_pField[i]->GetHeight();

            // テクスチャインデックスを取得（Texture.cpp側のインデックス）
            const int texIdxInTexture = m_pField[i]->GetTexIdx();

            // Texture からファイル名を取得
            const char* texFilename = pTex->GetFilename(texIdxInTexture);

            // CModelSet 側のインデックスを検索してスクリプト用の番号に変換
            int texIdxForScript = -1;
            for (int j = 0; j < nMaxTex; ++j)
            {
                const char* modelTexFilename = pModelSet->GetTexFilename(j);
                if (modelTexFilename && texFilename && strcmp(modelTexFilename, texFilename) == 0)
                {
                    texIdxForScript = j;
                    break;
                }
            }

            if (texIdxForScript == -1)
            {
                OutputDebugStringA("警告: FIELD のテクスチャが modelset に見つかりません\n");
                texIdxForScript = 0;
            }

            fprintf(pFile, "FIELDSET\n");
            fprintf(pFile, "\tTEXTYPE = %d\n", texIdxForScript);
            fprintf(pFile, "\tPOS = %.0f %.0f %.0f\n", pos.x, pos.y, pos.z);
            fprintf(pFile, "\tROT = %.0f %.0f %.0f\n",
                rot.x * 180.0f / D3DX_PI,
                rot.y * 180.0f / D3DX_PI,
                rot.z * 180.0f / D3DX_PI);
            fprintf(pFile, "\tSIZE = %.0f %.0f\n", x, z);
            fprintf(pFile, "END_FIELDSET\n\n");
        }
    }

    fprintf(pFile, "#==============================================================================\n");
    fprintf(pFile, "# 壁配置情報\n");
    fprintf(pFile, "#==============================================================================\n");
    for (int i = 0; i < m_nCntWall; ++i)
    {
        CObject3D::TYPE type = m_pWall[i]->GetType();       // OBJECTの種別（0,1,2...） ※実装必要

        if (type == CObject3D::TYPE_WALL)
        {
            if (!m_pWall[i])
            {
                continue;
            }
            const D3DXVECTOR3& pos = m_pWall[i]->GetPos();
            const D3DXVECTOR3& rot = m_pWall[i]->GetRot();
            const float x = m_pWall[i]->GetWidth();
            const float z = m_pWall[i]->GetHeight();

            // テクスチャインデックスを取得（Texture.cpp側のインデックス）
            const int texIdxInTexture = m_pWall[i]->GetTexIdx();

            // Texture からファイル名を取得
            const char* texFilename = pTex->GetFilename(texIdxInTexture);

            // CModelSet 側のインデックスを検索してスクリプト用の番号に変換
            int texIdxForScript = -1;
            for (int j = 0; j < nMaxTex; ++j)
            {
                const char* modelTexFilename = pModelSet->GetTexFilename(j);
                if (modelTexFilename && texFilename && strcmp(modelTexFilename, texFilename) == 0)
                {
                    texIdxForScript = j;
                    break;
                }
            }

            if (texIdxForScript == -1)
            {
                OutputDebugStringA("警告: FIELD のテクスチャが modelset に見つかりません\n");
                texIdxForScript = 0;
            }

            fprintf(pFile, "WALLSET\n");
            fprintf(pFile, "\tTEXTYPE = %d\n", texIdxForScript);
            fprintf(pFile, "\tPOS = %.0f %.0f %.0f\n", pos.x, pos.y, pos.z);
            fprintf(pFile, "\tROT = %.0f %.0f %.0f\n",
                rot.x * 180.0f / D3DX_PI,
                rot.y * 180.0f / D3DX_PI,
                rot.z * 180.0f / D3DX_PI);
            fprintf(pFile, "\tSIZE = %.0f %.0f\n", x, z);
            fprintf(pFile, "END_WALLSET\n\n");
        }
    }

    // モデル配置情報
    fprintf(pFile, "#==============================================================================\n");
    fprintf(pFile, "# モデル配置情報\n");
    fprintf(pFile, "#==============================================================================\n");
    for (int i = 0; i < m_nNumModel; ++i)
    {
        if (!m_pStage[i])
        {
            continue;
        }
        const D3DXVECTOR3& pos = m_pStage[i]->GetPos();  // ※必要に応じて実装
        const D3DXVECTOR3& rot = m_pStage[i]->GetRot();  // ※必要に応じて実装
        int type = m_pStage[i]->GetIndx();               // ※必要に応じて実装
        int object = m_pStage[i]->GetType();       // OBJECTの種別（0,1,2...） ※実装必要

        fprintf(pFile, "MODELSET\n");
        fprintf(pFile, "\tTYPE = %d\n", type);  // または m_pStage[i]->GetModelIndex();
        fprintf(pFile, "\tPOS = %.0f %.0f %.0f\n", pos.x, pos.y, pos.z);
        fprintf(pFile, "\tROT = %.0f %.0f %.0f\n",
                                                   rot.x * 180.0f / D3DX_PI,
                                                   rot.y * 180.0f / D3DX_PI,
                                                   rot.z * 180.0f / D3DX_PI);
        fprintf(pFile, "\tSHADOW = 0\n");    // 固定値
        fprintf(pFile, "\tOBJECT = %d\n", object);
        fprintf(pFile, "END_MODELSET\n\n");
    }

    // プレイヤー出力（仮にプレイヤー座標が固定 or 別途渡される場合）
    fprintf(pFile, "#==============================================================================\n");
    fprintf(pFile, "# プレイヤー情報\n");
    fprintf(pFile, "#==============================================================================\n");

    fprintf(pFile, "PLAYERSET\n");
    fprintf(pFile, "\tMOTION_FILENAME = data/MOTION/motion_vending.txt\n");
    fprintf(pFile, "\tPOS = -1500 0 -880\n");  // ※プレイヤー座標取得に変更してもOK
    fprintf(pFile, "\tROT = 0 0 0\n");
    fprintf(pFile, "END_PLAYERSET\n\n");

    // 終了
    fprintf(pFile, "END_SCRIPT\n");

    fclose(pFile);
}
//=======================
// 全テクスチャ破棄
//=======================
void CSaveStage::Unload(void)
{

}
//==============
// モデル生成
//==============
void CSaveStage::Create(const char* pFilename, D3DXVECTOR3 pos, D3DXVECTOR3 rot, CStage::TYPE type,int nIndx)
{
	m_pStage[m_nCntStage] = CStage::Create(pFilename, pos, rot, type, nIndx);
}
