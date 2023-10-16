#include "pch.h"
#include "CGameObject.h"

#include "CComponent.h"
#include "CRenderComponent.h"
#include "CMeshRender.h"
#include "CScript.h"

#include "CLevelMgr.h"
#include "CLevel.h"

CGameObject::CGameObject()
	: m_pParent(nullptr)
	, m_arrCom{}
	, m_pRenderComponent(nullptr)
	, m_iLayerIdx(-1)
{
}

CGameObject::CGameObject(const CGameObject& _origin)
	: CEntity(_origin)
	, m_pParent(nullptr)
	, m_arrCom{}
	, m_pRenderComponent(nullptr)
	, m_iLayerIdx(-1)
{
	for (UINT i = 0; i < (UINT)COMPONENT_TYPE::END; ++i)
	{
		if (nullptr == _origin.m_arrCom[i])
			continue;

		AddComponent(_origin.m_arrCom[i]->Clone());		
	}
	
	for (size_t i = 0; i < _origin.m_vecScripts.size(); ++i)
	{
		AddComponent(_origin.m_vecScripts[i]->Clone());
	}

	for (size_t i = 0; i < _origin.m_vecChild.size(); ++i)
	{
		AddChild(_origin.m_vecChild[i]->Clone());
	}	
}

CGameObject::~CGameObject()
{
	Safe_Del_Array(m_arrCom);
	Safe_Del_Vec(m_vecScripts);
	Safe_Del_Vec(m_vecChild);
}

void CGameObject::begin()
{
	// Components
	for (UINT i = 0; i < (UINT)COMPONENT_TYPE::END; ++i)
	{
		if (nullptr != m_arrCom[i])
			m_arrCom[i]->begin();
	}

	// Scripts
	for (size_t i = 0; i < m_vecScripts.size(); ++i)
	{
		m_vecScripts[i]->begin();
	}

	// Child Object
	for (size_t i = 0; i < m_vecChild.size(); ++i)
	{
		m_vecChild[i]->begin();
	}
}

void CGameObject::tick()
{
	// Comopnent
	for (UINT i = 0; i < (UINT)COMPONENT_TYPE::END; ++i)
	{
		if(nullptr != m_arrCom[i])
			m_arrCom[i]->tick();
	}

	// Scripts
	for (size_t i = 0; i < m_vecScripts.size(); ++i)
	{
		m_vecScripts[i]->tick();
	}	

	// Child Object
	for (size_t i = 0; i < m_vecChild.size(); ++i)
	{
		m_vecChild[i]->tick();
	}
}

void CGameObject::finaltick()
{
	// Component
	for (UINT i = 0; i < (UINT)COMPONENT_TYPE::END; ++i)
	{
		if (nullptr != m_arrCom[i])
			m_arrCom[i]->finaltick();
	}

	// Child Object
	vector<CGameObject*>::iterator iter = m_vecChild.begin();
	for (; iter != m_vecChild.end();)
	{
		(*iter)->finaltick();

		if ((*iter)->IsDead())
		{
			iter = m_vecChild.erase(iter);
		}
		else
		{
			++iter;
		}
	}

	// Register Layer
	CLevel* pCurLevel = CLevelMgr::GetInst()->GetCurLevel();
	CLayer* pLayer = pCurLevel->GetLayer(m_iLayerIdx);
	pLayer->RegisterObject(this);
}

void CGameObject::render()
{
	if (nullptr == m_pRenderComponent)
		return;

	m_pRenderComponent->render();
}

void CGameObject::AddComponent(CComponent* _pComponent)
{
	COMPONENT_TYPE eComType = _pComponent->GetType();

	// 스크립트가 아닌 경우
	if (COMPONENT_TYPE::SCRIPT != eComType)
	{
		assert(!m_arrCom[(UINT)eComType]);

		// 입력된 Component 가 RenderComponent 라면
		CRenderComponent* pRenderCom = dynamic_cast<CRenderComponent*>(_pComponent);
		if (nullptr != pRenderCom )
		{
			assert(!m_pRenderComponent); // render 기능 컴포넌트는 한개만 가질 수 있다.
			m_pRenderComponent = pRenderCom;
		}

		// GameObject 와 Component 가 서로를 가리킴
		_pComponent->m_pOwnerObject = this;
		m_arrCom[(UINT)eComType] = _pComponent;
	}
	
	// Script 인 경우
	else
	{
		_pComponent->m_pOwnerObject = this;
		m_vecScripts.push_back((CScript*)_pComponent);		
	}
}

void CGameObject::DelComponent(COMPONENT_TYPE _type)
{
	if (COMPONENT_TYPE::SCRIPT != _type)
	{
		delete m_arrCom[(UINT)_type];
		m_arrCom[(UINT)_type] = nullptr;
	}
}

void CGameObject::AddChild(CGameObject* _pChild)
{
	// 자식으로 들어오는 오브젝트가 최상위 부모 오브젝트다
	if (nullptr == _pChild->GetParent())
	{
		if (-1 == _pChild->m_iLayerIdx)
		{
			_pChild->m_iLayerIdx = m_iLayerIdx;
		}
		else
		{
			CLayer* pLayer = CLevelMgr::GetInst()->GetCurLevel()->GetLayer(_pChild->m_iLayerIdx);
			pLayer->DeregisterObject(_pChild);
		}
	}

	// 자식으로 들어오는 오브젝트가 다른 오브젝트의 자식 오브젝트였다면
	else
	{
		_pChild->DisconnectFromParent();
	}


	_pChild->m_pParent = this;
	m_vecChild.push_back(_pChild);
}

void CGameObject::DisconnectFromParent()
{
	vector<CGameObject*>::iterator iter = m_pParent->m_vecChild.begin();

	for (; iter != m_pParent->m_vecChild.end(); ++iter)
	{
		if (*iter == this)
		{
			m_pParent->m_vecChild.erase(iter);
			m_pParent = nullptr;

			return;
		}
	}

	assert(nullptr);
}

void CGameObject::DisconnectChild()
{
	m_vecChild.clear();
}

void CGameObject::ScriptReset()
{
	for (size_t i = 0; i < m_vecScripts.size(); ++i)
	{
		m_vecScripts[i]->reset();
	}
}

#include "CEventMgr.h"
void CGameObject::Destroy()
{
	tEvent eve = {};
	eve.eType = EVENT_TYPE::DELETE_OBJECT;
	eve.wParam = (DWORD_PTR)this;

	CEventMgr::GetInst()->AddEvent(eve);
}

void CGameObject::ReturnPool()
{
	tEvent eve = {};
	eve.eType = EVENT_TYPE::RETURN_POOL;
	eve.wParam = (DWORD_PTR)this;

	CEventMgr::GetInst()->AddEvent(eve);
}