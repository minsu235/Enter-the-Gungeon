#include "pch.h"
#include "CEventMgr.h"

#include "CLevelMgr.h"
#include "CLevel.h"
#include "CLayer.h"
#include "CGameObject.h"

#include "CResMgr.h"
#include "CMeshRender.h"


CEventMgr::CEventMgr()
{
}

CEventMgr::~CEventMgr()
{
}

void CEventMgr::tick()
{
	m_bLevelChanged = false;

	// 메모리 정리
	for (size_t i = 0; i < m_vecGarbage.size(); ++i)
	{		
		delete m_vecGarbage[i];
		m_bLevelChanged = true;
	}
	m_vecGarbage.clear();

	// Pool 정리
	for (size_t i = 0; i < m_vecReturnObject.size(); ++i)
	{
		m_vecReturnObject[i]->m_bDead = false;
		m_vecReturnObject[i]->m_pParent = nullptr;
		m_vecReturnObject[i]->m_iLayerIdx = -1;
		m_vecReturnObject[i]->MeshRender()->SetSharedMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"MonsterBulletMtrl"));
		m_vecReturnObject[i]->ScriptReset();
		g_vecMonsterBullet.push_back(m_vecReturnObject[i]);
	}
	m_vecReturnObject.clear();


	// 이벤트 처리
	for (size_t i = 0; i < m_vecEvent.size(); ++i)
	{
		switch (m_vecEvent[i].eType)
		{
		case EVENT_TYPE::CREATE_OBJECT:
		{
			// wParam : GameObject Adress
			// lParam : Layer Index
			CGameObject* pNewObj = (CGameObject*)m_vecEvent[i].wParam;
			int iLayerIdx = (int)m_vecEvent[i].lParam;

			CLevel* pLevel = CLevelMgr::GetInst()->GetCurLevel();
			pLevel->AddGameObject(pNewObj, iLayerIdx);		
			m_bLevelChanged = true;
		}
			break;
		case EVENT_TYPE::DELETE_OBJECT:
		{
			// wParam : GameObject Adress
			CGameObject* pObj = (CGameObject*)m_vecEvent[i].wParam;

			if (!pObj->IsDead())
			{				
				// 삭제처리할 최상위 부모만 가비지에 넣는다.
				//m_vecGarbage.push_back(pObj);

				// 삭제할 오브젝트 포함, 모든 자식오브젝트를 Dead 체크한다.
				static list<CGameObject*> queue;				
				queue.push_back(pObj);
				while (!queue.empty())
				{
					CGameObject* pObj = queue.front();
					queue.pop_front();

					const vector<CGameObject*>& vecChild = pObj->GetChildObject();
					for (size_t i = 0; i < vecChild.size(); ++i)
					{
						queue.push_back(vecChild[i]);
					}

					pObj->m_bDead = true;
					m_vecGarbage.push_back(pObj);
				}				
			}
		}
			break;
		case EVENT_TYPE::RETURN_POOL:
		{
			// wParam : GameObject Adress
			CGameObject* pObj = (CGameObject*)m_vecEvent[i].wParam;

			if (!pObj->IsDead())
			{
				pObj->m_bDead = true;
				m_vecReturnObject.push_back(pObj);
			}
		}
		break;
		case EVENT_TYPE::ADD_CHILD:
		{
			// wParam : Child Adress, lParam : Parent Adress
			CGameObject* pParent = (CGameObject*)m_vecEvent[i].lParam;
			CGameObject* pChild = (CGameObject*)m_vecEvent[i].wParam;

			pParent->AddChild(pChild);
			m_bLevelChanged = true;
			
		}
			break;
		case EVENT_TYPE::DELETE_RES:
		{
			// wParam : RES_TYPE, lParam : Resource Adress
			if (!CResMgr::GetInst()->DeleteRes((RES_TYPE)m_vecEvent[i].wParam, ((CRes*)m_vecEvent[i].lParam)->GetKey()))
			{
				MessageBox(nullptr, L"리소스 삭제 실패", L"에러", MB_OK);
			}
		}

			break;
		case EVENT_TYPE::CHANGE_LEVEL_STATE:
		{
			// wParam : Level State
			m_bLevelChanged = true;			
			CLevelMgr::GetInst()->ChangeLevelState((LEVEL_STATE)m_vecEvent[i].wParam);
		}
		break;		

		case EVENT_TYPE::CHANGE_LEVEL:
		{
			m_bLevelChanged = true;
		}
			break;
		case EVENT_TYPE::END:
			break;
		default:
			break;
		}
	}

	m_vecEvent.clear();
}