#include "pch.h"
#include "CRoomScript.h"

#include "CBooklletScript.h"
#include "CBulletKingScript.h"
#include "CBulletKinScript.h"
#include "CGunnutScript.h"
#include "CShotgunBulletKinScript.h"
#include "CWizardScript.h"
#include "CBulletKingScript.h"
#include "CSpawnScript.h"

#include <Engine/CLevelMgr.h>

CRoomScript::CRoomScript()
	: CScript(ROOMSCRIPT)
	, arrNode{}
	, m_pPlayer(nullptr)
	, m_iDestPosX(0)
	, m_iDestPosY(0)
	, m_iCurPosX(0)
	, m_iCurPosY(0)
	, m_iXCount(0)
	, m_iYCount(0)
	, m_pDarkroom(nullptr)
	, m_iMonsterwave(0)
	, m_iMonster(0)
	, m_wavesize(0)
	, m_bSpawn(false)
	, m_fAcctime(0.f)
	, m_eType(RoomType::None)
{

}

CRoomScript::~CRoomScript()
{
}

void CRoomScript::begin()
{
}

void CRoomScript::tick()
{
	/*m_iXCount = TileMap()->GetTileCount().x;
	m_iYCount = TileMap()->GetTileCount().y;
	for (int i = 0; i < m_iYCount; ++i)
	{
		for (int j = 0; j < m_iXCount; ++j)
		{
			arrNode[i][j].iIdxX = j;
			arrNode[i][j].iIdxY = i;
		}
	}*/

	if (m_iMonsterwave > 0 && vec_monster.size() == m_wavesize && m_iMonster == 0)
	{
		if (m_Wall != nullptr)
		{
			m_Wall->Destroy();
			m_Wall = nullptr;
		}
			
		if (m_Wall2 != nullptr)
		{
			m_Wall2->Destroy();
			m_Wall2 = nullptr;
		}
		Ptr<CSound> pSound = CResMgr::GetInst()->FindRes<CSound>(L"sound\\opendoor_finished.mp3");
		pSound->Play(1, 0.6f, true);
		m_iMonsterwave = -1;
	}

	if (m_iMonsterwave > 0 && vec_monster.size() != m_wavesize && m_iMonster == 0)
	{
		m_bSpawn = true;
	}

	
	if (m_bSpawn)
	{
		if (m_fAcctime == 0.f)
		{
			vector<MonsterInfo>::iterator iter = vec_monster.begin();
			iter += m_wavesize;
			if (iter == vec_monster.end() || iter->m_iWave > m_iMonsterwave)
			{
				m_bSpawn = false;
				m_fAcctime = 0.f;
				++m_iMonsterwave;
			}
			else
			{
				++m_iMonster;
				++m_wavesize;
				CGameObject* impact = CResMgr::GetInst()->FindRes<CPrefab>(L"monsterspawn")->Instantiate();
				impact->Animator2D()->Play(L"monsterspawn", false);
				impact->AddComponent(new CSpawnScript);
				impact->GetScript<CSpawnScript>()->setAll(m_pPlayer, GetOwner(), *iter);
				Instantiate(impact, iter->m_vPos, 0);
				Ptr<CSound> pSound = CResMgr::GetInst()->FindRes<CSound>(L"sound\\spawn_reticle_01.wav");
				pSound->Play(1, 0.6f, true);
				m_fAcctime += DT;
			}
		}
		else
		{
			m_fAcctime += DT;
			if (m_fAcctime >= 0.2f)
			{
				m_fAcctime = 0;
			}
		}
	}
}

void CRoomScript::BeginOverlap(CCollider2D* _pOther)
{
	if (_pOther->GetOwner()->GetName() == L"Player")
	{
		m_pPlayer = _pOther->GetOwner();
		if (m_eType == RoomType::Shop)
		{
			CLevelMgr::GetInst()->playBGM(L"sound\\shop.mp3");
		}
		if (m_pDarkroom != nullptr)
		{
			m_pDarkroom->Destroy();
			m_pDarkroom = nullptr;
			if (m_eType == RoomType::Monster)
			{
				CLevelMgr::GetInst()->playBGM(L"sound\\battle.mp3");
			}
			else if (m_eType == RoomType::Boss)
			{
				CLevelMgr::GetInst()->playBGM(L"sound\\bossbgm.mp3");
			}
		}
		if (vec_monster.size() != 0)
		{
			if (m_iMonsterwave == 0)
			{
				for (vector<MonsterInfo>::iterator iter = vec_monster.begin(); iter != vec_monster.end(); ++iter)
				{
					if (iter->m_iWave != 0)
						break;
					++m_iMonster;
					++m_wavesize;
					switch (iter->m_eMonster)
					{
					case Monster::Bookllet:
					{
						Ptr<CPrefab> prefab = CResMgr::GetInst()->FindRes<CPrefab>(L"Bookllet");
						CGameObject* monster = prefab->Instantiate();
						monster->GetScript<CBooklletScript>()->setPlayer(m_pPlayer);
						monster->GetScript<CBooklletScript>()->setRoom(GetOwner());
						Instantiate(monster, iter->m_vPos, 3);
					}
					break;
					case Monster::BulletKin:
					{
						Ptr<CPrefab> prefab = CResMgr::GetInst()->FindRes<CPrefab>(L"BulletKin");
						CGameObject* monster = prefab->Instantiate();
						monster->GetScript<CBulletKinScript>()->setPlayer(m_pPlayer);
						monster->GetScript<CBulletKinScript>()->setRoom(GetOwner());
						Instantiate(monster, iter->m_vPos, 3);
					}
					break;
					case Monster::Gunnut:
					{
						Ptr<CPrefab> prefab = CResMgr::GetInst()->FindRes<CPrefab>(L"Gunnut");
						CGameObject* monster = prefab->Instantiate();
						monster->GetScript<CGunnutScript>()->setPlayer(m_pPlayer);
						monster->GetScript<CGunnutScript>()->setRoom(GetOwner());
						Instantiate(monster, iter->m_vPos, 3);
					}
					break;
					case Monster::ShotgunKinRed:
					{
						Ptr<CPrefab> prefab = CResMgr::GetInst()->FindRes<CPrefab>(L"ShotgunKinRed");
						CGameObject* monster = prefab->Instantiate();
						monster->GetScript<CShotgunBulletKinScript>()->setPlayer(m_pPlayer);
						monster->GetScript<CShotgunBulletKinScript>()->setRoom(GetOwner());
						Instantiate(monster, iter->m_vPos, 3);
					}
					break;
					case Monster::ShotgunKinBlue:
					{
						Ptr<CPrefab> prefab = CResMgr::GetInst()->FindRes<CPrefab>(L"ShotgunKinBlue");
						CGameObject* monster = prefab->Instantiate();
						monster->GetScript<CShotgunBulletKinScript>()->setPlayer(m_pPlayer);
						monster->GetScript<CShotgunBulletKinScript>()->setRoom(GetOwner());
						Instantiate(monster, iter->m_vPos, 3);
					}
					break;
					case Monster::Wizard:
					{
						Ptr<CPrefab> prefab = CResMgr::GetInst()->FindRes<CPrefab>(L"Wizard");
						CGameObject* monster = prefab->Instantiate();
						monster->GetScript<CWizardScript>()->setPlayer(m_pPlayer);
						monster->GetScript<CWizardScript>()->setRoom(GetOwner());
						Instantiate(monster, iter->m_vPos, 3);
					}
					break;
					case Monster::Wall_h:
					{
						Ptr<CPrefab> prefab = CResMgr::GetInst()->FindRes<CPrefab>(L"walldoor_h");
						CGameObject* monster = prefab->Instantiate();
						Instantiate(monster, iter->m_vPos, 3);
						Ptr<CSound> pSound = CResMgr::GetInst()->FindRes<CSound>(L"sound\\shutdoor.mp3");
						pSound->Play(1, 0.15f, true);
						--m_iMonster;
						if (m_Wall == nullptr)
							m_Wall = monster;
						else
							m_Wall2 = monster;
					}
					break;
					case Monster::Wall_v:
					{
						Ptr<CPrefab> prefab = CResMgr::GetInst()->FindRes<CPrefab>(L"walldoor_v");
						CGameObject* monster = prefab->Instantiate();
						Instantiate(monster, iter->m_vPos, 3);
						Ptr<CSound> pSound = CResMgr::GetInst()->FindRes<CSound>(L"sound\\shutdoor.mp3");
						pSound->Play(1, 0.15f, true);
						--m_iMonster;
						if (m_Wall == nullptr)
							m_Wall = monster;
						else
							m_Wall2 = monster;
					}
					break;
					case Monster::BulletKing:
					{
						Ptr<CPrefab> prefab = CResMgr::GetInst()->FindRes<CPrefab>(L"BulletKing");
						CGameObject* monster = prefab->Instantiate();
						monster->GetScript<CBulletKingScript>()->setPlayer(m_pPlayer);
						Instantiate(monster, iter->m_vPos, 3);
					}
					break;
					}
				}

				++m_iMonsterwave;
			}
		}
	}
}

void CRoomScript::Overlap(CCollider2D* _pOther)
{
}

void CRoomScript::EndOverlap(CCollider2D* _pOther)
{
	if (_pOther->GetOwner()->GetName() == L"Player")
	{
		m_pPlayer = nullptr;
		if (m_eType == RoomType::Shop)
		{
			CLevelMgr::GetInst()->playBGM(L"sound\\stage1bgm.mp3");
		}
	}
}

void CRoomScript::SaveToFile(FILE* _File)
{
	CScript::SaveToFile(_File);
	fwrite(&m_iXCount, sizeof(int), 1, _File);
	fwrite(&m_iYCount, sizeof(int), 1, _File);
	fwrite(&arrNode, sizeof(tNode) * 10000, 1, _File);
}

void CRoomScript::LoadFromFile(FILE* _File)
{
	CScript::LoadFromFile(_File);
	fread(&m_iXCount, sizeof(int), 1, _File);
	fread(&m_iYCount, sizeof(int), 1, _File);
	fread(&arrNode, sizeof(tNode) * 10000, 1, _File);
}

void CRoomScript::Rebuild(priority_queue<tNode*, vector<tNode*>, Compare>& _queue)
{
	priority_queue<tNode*, vector<tNode*>, Compare> tempQueue;

	while (!_queue.empty())
	{
		tempQueue.push(_queue.top());
		_queue.pop();
	}

	_queue.swap(tempQueue);
}

void CRoomScript::CalculateCost(tNode* _pCurNode, tNode* _pOrigin)
{
	_pCurNode->pPrevNode = _pOrigin;
	_pCurNode->fFromParent = 10.f + _pOrigin->fFromParent;
	_pCurNode->fToDest = abs(m_iDestPosX - _pCurNode->iIdxX) * 10.f + abs(m_iDestPosY - _pCurNode->iIdxY) * 10;
	_pCurNode->fFinal = _pCurNode->fFromParent + _pCurNode->fToDest;
}

void CRoomScript::AddOpenList(int _iXIdx, int _iYIdx, tNode* _pOrigin)
{
	// 현재 지점에서 갈 수 있는 곳을 OpenList 에 넣는다.
	// 노드 범위를 벗어난 경우
	if (_iXIdx < 0 || _iXIdx >= m_iXCount || _iYIdx < 0 || _iYIdx >= m_iYCount
		|| !arrNode[_iYIdx][_iXIdx].bMove)
		return;

	// 해당 길이 Clost List 에 있는 경우, Open List 에 넣지 않는다.
	if (arrNode[_iYIdx][_iXIdx].bClosed)
		return;

	// Open List 에 비용을 계산해서 넣는다.
	if (false == arrNode[_iYIdx][_iXIdx].bOpen)
	{
		CalculateCost(&arrNode[_iYIdx][_iXIdx], _pOrigin);
		// Open List 에 넣는다.
		arrNode[_iYIdx][_iXIdx].bOpen = true;
		m_OpenList.push(&arrNode[_iYIdx][_iXIdx]);
	}
	else // 이미 OpenList 에 있는 경우,
	{
		//비용을 계산해서 더 효율이 좋은 것으로 대체한다.
		tNode node = arrNode[_iYIdx][_iXIdx];
		CalculateCost(&node, _pOrigin);

		if (arrNode[_iYIdx][_iXIdx].fFinal > node.fFinal)
		{
			arrNode[_iYIdx][_iXIdx] = node;

			// 오픈리스트(우선순위큐) 재설정
			Rebuild(m_OpenList);
		}
	}
}

Vec2 CRoomScript::FindPath(CGameObject* _monster)
{
	while (!m_OpenList.empty())
	{
		priority_queue<tNode*, vector<tNode*>, Compare> a;
		m_OpenList = a;
	}

	for (int i = 0; i < m_iYCount; ++i)
	{
		for (int j = 0; j < m_iXCount; ++j)
		{
			arrNode[i][j].bClosed = false;
			arrNode[i][j].bOpen = false;
			arrNode[i][j].pPrevNode = nullptr;
		}
	}
	// 목적지
	Vec2 o = {};
	o.x = Transform()->GetRelativePos().x - (Transform()->GetRelativeScale().x / 2);
	o.y = Transform()->GetRelativePos().y + (Transform()->GetRelativeScale().y / 2);
	Vec2 pos;
	pos.x = m_pPlayer->Transform()->GetRelativePos().x;
	pos.y = m_pPlayer->Transform()->GetRelativePos().y - 30.f;

	Vec2 Idx = pos - o;

	if (Idx.x < 0 || Idx.y > 0)
		return Vec2(0.f, 0.f);

	m_iDestPosX = (int)Idx.x / 48;
	m_iDestPosY = (int)-Idx.y / 48;

	int i = 1;
	while (!arrNode[m_iDestPosY][m_iDestPosX].bMove)
	{
		if (arrNode[m_iDestPosY - i][m_iDestPosX].bMove == true)
		{
			m_iDestPosY -= i;
			break;
		}
		else if (arrNode[m_iDestPosY + i][m_iDestPosX].bMove == true)
		{
			m_iDestPosY += i;
			break;
		}
		else if (arrNode[m_iDestPosY][m_iDestPosX - i].bMove == true)
		{
			m_iDestPosX -= i;
			break;
		}
		else if (arrNode[m_iDestPosY][m_iDestPosX + i].bMove == true)
		{
			m_iDestPosX += i;
			break;
		}
		++i;
	}

	// 출발지
	pos.x = _monster->Transform()->GetRelativePos().x;
	pos.y = _monster->Transform()->GetRelativePos().y - 30.f;

	Idx = pos - o;

	if (Idx.x < 0 || Idx.y > 0)
		return Vec2(0.f, 0.f);

	m_iCurPosX = (int)Idx.x / 48;
	m_iCurPosY = (int)-Idx.y / 48;

	// 계산
	int iCurX = m_iCurPosX;
	int iCurY = m_iCurPosY;

	tNode* pCurNode = &arrNode[iCurY][iCurX];
	pCurNode->bClosed = true;

	while (true)
	{
		// 현재 지점에서 4방향의 노드를 OpenList 에 넣는다.
		// UP
		AddOpenList(pCurNode->iIdxX
			, pCurNode->iIdxY - 1, pCurNode);

		// RIGHT		
		AddOpenList(pCurNode->iIdxX + 1
			, pCurNode->iIdxY, pCurNode);

		// DOWN		
		AddOpenList(pCurNode->iIdxX
			, pCurNode->iIdxY + 1, pCurNode);

		// LEFT		
		AddOpenList(pCurNode->iIdxX - 1
			, pCurNode->iIdxY, pCurNode);

		// 2. Open List 에서 가장 효율이 좋은 노드를 꺼낸다.
		//  - 해당 노드는 CloseList 에 넣는다.
		pCurNode = m_OpenList.top();
		pCurNode->bClosed = true;

		m_OpenList.pop();

		// 목적지에 도착한 경우 Astar 종료
		if (pCurNode->iIdxX == m_iDestPosX && pCurNode->iIdxY == m_iDestPosY)
		{
			break;
		}
		if (m_OpenList.size() > 10000)
		{
			return Vec2(0.f, 0.f);
		}
	}

	while (true)
	{
		if (pCurNode->pPrevNode->iIdxX == m_iCurPosX && pCurNode->pPrevNode->iIdxY == m_iCurPosY)
		{
			break;
		}
		pCurNode = pCurNode->pPrevNode;
	}

	// 다음 노드 방향 알리기
	pos.x = o.x + 48 * pCurNode->iIdxX;
	pos.y = o.y - 48 * pCurNode->iIdxY;
	return(pos);
}

Vec2 CRoomScript::FindTele()
{
	// 목적지
	Vec2 o = {};
	o.x = Transform()->GetWorldPos().x - (Transform()->GetWorldScale().x / 2);
	o.y = Transform()->GetWorldPos().y + (Transform()->GetWorldScale().y / 2);
	Vec2 pos;
	pos.x = m_pPlayer->Transform()->GetWorldPos().x;
	pos.y = m_pPlayer->Transform()->GetWorldPos().y - 30.f;

	Vec2 Idx = pos - o;

	if (Idx.x < 0 || Idx.y > 0)
		return Vec2(0.f, 0.f);

	int iDestPosX = (int)Idx.x / 48;
	int iDestPosY = (int)-Idx.y / 48;

	int j = rand() % 8;

	int iminX = iDestPosX - 4;
	int iminY = iDestPosY - 4;
	int imaxX = iDestPosX + 4;
	int imaxY = iDestPosY + 4;
	if (imaxX >= m_iXCount)
		imaxX = m_iXCount - 1;
	if (imaxY >= m_iYCount)
		imaxY = m_iYCount - 1;
	if (iminX < 0)
		iminX = 0;
	if (iminY < 0)
		iminY = 0;

	for (int r = 0; r < 8; ++r)
	{
		if (j == 0)
		{
			if (arrNode[imaxY][iminX].bMove == true)
			{
				iDestPosX = iminX;
				iDestPosY = imaxY;
				break;
			}
		}
		else if (j == 1)
		{
			if (arrNode[imaxY][iDestPosX].bMove == true)
			{
				iDestPosY = imaxY;
				break;
			}
		}
		else if (j == 2)
		{
			if (arrNode[imaxY][imaxX].bMove == true)
			{
				iDestPosX = imaxX;
				iDestPosY = imaxY;
				break;
			}
		}
		else if (j == 3)
		{
			if (arrNode[iDestPosY][imaxX].bMove == true)
			{
				iDestPosX = imaxX;
				break;
			}
		}
		else if (j == 4)
		{
			if (arrNode[iminY][imaxX].bMove == true)
			{
				iDestPosX = imaxX;
				iDestPosY = iminY;
				break;
			}
		}
		else if (j == 5)
		{
			if (arrNode[iminY][iDestPosX].bMove == true)
			{
				iDestPosY = iminY;
				break;
			}
		}
		else if (j == 6)
		{
			if (arrNode[iminY][iminX].bMove == true)
			{
				iDestPosX = iminX;
				iDestPosY = iminY;
				break;
			}
		}
		else if (j == 7)
		{
			if (arrNode[iDestPosY][iminX].bMove == true)
			{
				iDestPosX = iminX;
				break;
			}
		}

		++j;
		if (j >= 8)
			j = 0;
	}

	// 다음 노드 방향 알리기
	pos.x = o.x + 48 * iDestPosX;
	pos.y = o.y - 48 * iDestPosY;
	return(pos);
}
