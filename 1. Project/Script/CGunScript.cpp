#include "pch.h"
#include "CGunScript.h"

#include <Engine\CLevelMgr.h>
#include <Engine\CKeyMgr.h>
#include <Engine\CRenderMgr.h>
#include "CPlayerScript.h"
#include "CBulletScript.h"
#include "CUIScript.h"
#include "CGameCameraScript.h"

CGunScript::CGunScript()
    : CScript(GUNSCRIPT)
    , m_MaximumBullet(0)
    , m_MagazineBullet(6)
    , m_fReloadtime(1.f)
    , m_fBulletSpread(8.f)
    , m_fFireInterval(0.2f)
    , m_iCurrentMagazine(6)
    , m_iCurrentBullet(0)
    , m_fAcctime(0.f)
    , m_fCharge(0.f)
    , m_fChargedtime(1.f)
    , m_eCurrentState(GUN_STATE::DEATH)
    , m_bullet(nullptr)
    , m_UI(nullptr)
    , m_act(false)
    , m_gun(GUN::rusty_sidearm)
    , m_Reload_bar(nullptr)
    , m_Reload(nullptr)
{
}

CGunScript::~CGunScript()
{
}

void CGunScript::begin()
{
}

void CGunScript::tick()
{
    if (m_act == false)
    {
        return;
    }

    if (GetOwner()->GetParent()->GetParent()->GetScript<CPlayerScript>()->IsStop())
        return;

    if (m_gun == GUN::megahand)
    {
        CGameObject* player = GetOwner()->GetParent()->GetParent();
        PLAYER_STATE player_state = player->GetScript<CPlayerScript>()->getPlayerState();
        if (player_state <= PLAYER_STATE::DODGE_BL || (PLAYER_STATE::SPIN <= player_state && player_state <= PLAYER_STATE::SLIDE_BL))
        {
            Animator2D()->Play(L"Delete", true);
            if (m_eCurrentState == GUN_STATE::RELOAD)
            {
                Reload(true);
            }
        }
        else if (PLAYER_STATE::DODGEFALL_B <= player_state && player_state <= PLAYER_STATE::FALL_L)
        {
            Animator2D()->Play(L"Delete", true);
            m_eCurrentState = GUN_STATE::IDLE;
            m_fAcctime = 0.f;
            m_fCharge = 0.f;
            if (m_Reload != nullptr)
            {
                m_Reload->Destroy();
                m_Reload = nullptr;
            }
            if (m_Reload_bar != nullptr)
            {
                m_Reload_bar->Destroy();
                m_Reload_bar = nullptr;
            }
        }
        else if (KEY_TAP(KEY::R) && m_eCurrentState == GUN_STATE::IDLE)
        {
            if (m_iCurrentBullet > 0 || m_MaximumBullet == 0)
            {
                m_eCurrentState = GUN_STATE::RELOAD;
                Animator2D()->Play(L"Reload", false);
                if (m_UI->GetScript<CUIScript>()->getCurrentGun() != nullptr)
                    m_UI->GetScript<CUIScript>()->getCurrentGun()->Animator2D()->Play(L"Reload", false);
                CGameObject* pReload = CResMgr::GetInst()->FindRes<CPrefab>(L"ReloadBarUI")->Instantiate();
                Vec3 vPlayerPos = GetOwner()->GetParent()->GetParent()->Transform()->GetRelativePos();
                vPlayerPos.z = 4.f;
                vPlayerPos.y += 45.f;
                if (m_Reload_bar != nullptr)
                    m_Reload_bar->Destroy();
                m_Reload_bar = pReload;
                Instantiate(pReload, vPlayerPos, 0);

                pReload = CResMgr::GetInst()->FindRes<CPrefab>(L"ReloadUI")->Instantiate();
                vPlayerPos.z = 3.f;
                vPlayerPos.x -= 47.f;
                if (m_Reload != nullptr)
                    m_Reload->Destroy();
                m_Reload = pReload;
                Instantiate(pReload, vPlayerPos, 0);
                Reload();
            }
        }
        else if (KEY_PRESSED(KEY::LBTN) && m_eCurrentState == GUN_STATE::IDLE)
        {
            if (m_iCurrentMagazine == 0 && m_fAcctime == 0 && (m_iCurrentBullet > 0 || m_MaximumBullet == 0))
            {
                m_eCurrentState = GUN_STATE::RELOAD;
                Animator2D()->Play(L"Reload", false);
                if (m_UI->GetScript<CUIScript>()->getCurrentGun() != nullptr)
                    m_UI->GetScript<CUIScript>()->getCurrentGun()->Animator2D()->Play(L"Reload", false);
                CGameObject* pReload = CResMgr::GetInst()->FindRes<CPrefab>(L"ReloadBarUI")->Instantiate();
                Vec3 vPlayerPos = GetOwner()->GetParent()->GetParent()->Transform()->GetRelativePos();
                vPlayerPos.z = 4.f;
                vPlayerPos.y += 45.f;
                if (m_Reload_bar != nullptr)
                    m_Reload_bar->Destroy();
                m_Reload_bar = pReload;
                Instantiate(pReload, vPlayerPos, 0);

                pReload = CResMgr::GetInst()->FindRes<CPrefab>(L"ReloadUI")->Instantiate();
                vPlayerPos.z = 3.f;
                vPlayerPos.x -= 47.f;
                if (m_Reload != nullptr)
                    m_Reload->Destroy();
                m_Reload = pReload;
                Instantiate(pReload, vPlayerPos, 0);
                Reload();
            }
            else if(m_fAcctime == 0 && m_iCurrentMagazine > 0)
            {
                if (m_fCharge >= 0.2f && m_fBulletSpread == 8.f)
                {
                    Ptr<CSound> pSound = CResMgr::GetInst()->FindRes<CSound>(L"sound\\megablaster_charge_01.wav");
                    pSound->Play(1, 0.6f, false);
                    Animator2D()->Play(L"Charge", true);
                    if (m_UI->GetScript<CUIScript>()->getCurrentGun() != nullptr)
                        m_UI->GetScript<CUIScript>()->getCurrentGun()->Animator2D()->Play(L"Charge", true);
                    m_fBulletSpread = 0.f;
                }
                
                if (m_fCharge > m_fChargedtime)
                {
                    CGameObject* impact = CResMgr::GetInst()->FindRes<CPrefab>(L"dustup_dodge")->Instantiate();
                    impact->Transform()->SetRelativeScale(Vec3(96.f, 48.f, 1.f));
                    impact->Animator2D()->Play(L"dustup_dodge", false);
                    Vec3 vPos = GetOwner()->GetParent()->GetParent()->Transform()->GetRelativePos();
                    vPos.z = 80.f;
                    vPos.y -= 45.f;
                    Instantiate(impact, vPos, 0);
                    Ptr<CSound> pSound = CResMgr::GetInst()->FindRes<CSound>(L"sound\\weapon_charged_01.wav");
                    pSound->Play(1, 0.6f, true);
                    m_fCharge = m_fChargedtime;
                }
                else if (m_fCharge < m_fChargedtime)
                {
                    Animator2D()->Play(L"Charge", true);
                    if (m_UI->GetScript<CUIScript>()->getCurrentGun() != nullptr)
                        m_UI->GetScript<CUIScript>()->getCurrentGun()->Animator2D()->Play(L"Charge", true);
                    m_fCharge += DT;
                    if (m_fCharge == m_fChargedtime)
                        m_fCharge += DT;
                }
                else
                {
                    Animator2D()->Play(L"Charge", true);
                    if (m_UI->GetScript<CUIScript>()->getCurrentGun() != nullptr)
                        m_UI->GetScript<CUIScript>()->getCurrentGun()->Animator2D()->Play(L"Charge", true);
                }
            }
        }
        else if (KEY_RELEASE(KEY::LBTN) && m_eCurrentState == GUN_STATE::IDLE && m_iCurrentMagazine > 0)
        {
            m_fBulletSpread = 8.f;

			if (m_fAcctime == 0 && (m_fCharge != m_fChargedtime))
			{
				m_iCurrentMagazine -= 1;
                m_iCurrentBullet -= 1;
                m_UI->GetScript<CUIScript>()->setMaxbullet(m_MaximumBullet, m_iCurrentBullet);
				m_UI->GetScript<CUIScript>()->setMagazine(m_iCurrentMagazine, m_MagazineBullet);
				m_eCurrentState = GUN_STATE::SHOOT;
                CRenderMgr::GetInst()->GetMainCam()->GetOwner()->GetScript<CGameCameraScript>()->camerashake();
				Animator2D()->Play(L"Shoot", false);
				if (m_UI->GetScript<CUIScript>()->getCurrentGun() != nullptr)
					m_UI->GetScript<CUIScript>()->getCurrentGun()->Animator2D()->Play(L"Shoot", false);

				Vec3 vMissilePos = Transform()->GetWorldPos();
				vMissilePos.z = 75.f;
				Vec3 vdir = Transform()->GetWorldDir(DIR::RIGHT);
				vMissilePos.x += Transform()->GetWorldScale().x / 2 * vdir.x;
				vMissilePos.y += Transform()->GetWorldScale().y / 2 * vdir.y;

				Ptr<CSound> pSound = CResMgr::GetInst()->FindRes<CSound>(L"sound\\megablaster_shotA_01.wav");
				pSound->Play(1, 0.6f, true);
				CGameObject* bullet = m_bullet->Instantiate();
				bullet->GetScript<CBulletScript>()->setDIR(getMouseDIR());
                bullet->Animator2D()->Play(L"weak", true);
				Instantiate(bullet, vMissilePos, 2);
                m_fCharge = 0.f;
			}
            else if (m_fAcctime == 0 && (m_fCharge == m_fChargedtime))
            {
                m_iCurrentMagazine -= 1;
                m_iCurrentBullet -= 1;
                m_UI->GetScript<CUIScript>()->setMaxbullet(m_MaximumBullet, m_iCurrentBullet);
                m_UI->GetScript<CUIScript>()->setMagazine(m_iCurrentMagazine, m_MagazineBullet);
                m_eCurrentState = GUN_STATE::SHOOT;
                CRenderMgr::GetInst()->GetMainCam()->GetOwner()->GetScript<CGameCameraScript>()->camerashake();
                Animator2D()->Play(L"Charge_shoot", false);
                if (m_UI->GetScript<CUIScript>()->getCurrentGun() != nullptr)
                    m_UI->GetScript<CUIScript>()->getCurrentGun()->Animator2D()->Play(L"Charge_shoot", false);

                Vec3 vMissilePos = Transform()->GetWorldPos();
                vMissilePos.z = 75.f;
                Vec3 vdir = Transform()->GetWorldDir(DIR::RIGHT);
                vMissilePos.x += Transform()->GetWorldScale().x / 2 * vdir.x;
                vMissilePos.y += Transform()->GetWorldScale().y / 2 * vdir.y;

                Ptr<CSound> pSound = CResMgr::GetInst()->FindRes<CSound>(L"sound\\megablaster_shotB_01.wav");
                pSound->Play(1, 0.6f, true);
                CGameObject* bullet = m_bullet->Instantiate();
                bullet->Transform()->SetRelativeScale(Vec3(73.f, 63.f, 1.f));
                bullet->Collider2D()->SetIgnoreObjectScale(true);
                bullet->Collider2D()->SetScale(Vec2(63.f, 38.f));
                bullet->GetScript<CBulletScript>()->setDIR(getMouseDIR());
                bullet->GetScript<CBulletScript>()->setDMG(45);
                bullet->Animator2D()->Play(L"megabuster", true);
                Instantiate(bullet, vMissilePos, 2);
                m_fCharge = 0.f;
            }
        }
        else if (m_eCurrentState == GUN_STATE::SHOOT)
        {
            m_fAcctime += DT;
            if (m_fAcctime >= m_fFireInterval)
            {
                m_fAcctime = 0.f;
                m_eCurrentState = GUN_STATE::IDLE;
            }
        }
        else if (m_eCurrentState == GUN_STATE::RELOAD)
        {
            Reload();
        }
        else
        {
            m_eCurrentState = GUN_STATE::IDLE;
            Animator2D()->Play(L"Idle", true);
            if (m_UI->GetScript<CUIScript>()->getCurrentGun() != nullptr)
                m_UI->GetScript<CUIScript>()->getCurrentGun()->Animator2D()->Play(L"Idle", true);
        }
    }
    else
    {
        CGameObject* player = GetOwner()->GetParent()->GetParent();
        PLAYER_STATE player_state = player->GetScript<CPlayerScript>()->getPlayerState();
        if (player_state <= PLAYER_STATE::DODGE_BL || (PLAYER_STATE::SPIN <= player_state && player_state <= PLAYER_STATE::SLIDE_BL))
        {
            Animator2D()->Play(L"Delete", true);
            if (m_eCurrentState == GUN_STATE::RELOAD)
            {
                Reload(true);
            }
        }
        else if (PLAYER_STATE::DODGEFALL_B <= player_state && player_state <= PLAYER_STATE::FALL_L)
        {
            Animator2D()->Play(L"Delete", true);
            m_eCurrentState = GUN_STATE::IDLE;
            m_fAcctime = 0.f;
            if (m_Reload != nullptr)
            {
                m_Reload->Destroy();
                m_Reload = nullptr;
            }
            if (m_Reload_bar != nullptr)
            {
                m_Reload_bar->Destroy();
                m_Reload_bar = nullptr;
            }
        }
        else if (KEY_TAP(KEY::R) && m_eCurrentState == GUN_STATE::IDLE)
        {
            if (m_iCurrentBullet > 0 || m_MaximumBullet == 0)
            {
                m_eCurrentState = GUN_STATE::RELOAD;
                Animator2D()->Play(L"Reload", false);
                if (m_UI->GetScript<CUIScript>()->getCurrentGun() != nullptr)
                    m_UI->GetScript<CUIScript>()->getCurrentGun()->Animator2D()->Play(L"Reload", false);
                if (m_gun == GUN::rusty_sidearm)
                {
                    Ptr<CSound> pSound = CResMgr::GetInst()->FindRes<CSound>(L"sound\\SAA_reload_01.wav");
                    pSound->Play(1, 0.6f, true);
                }
                else if (m_gun == GUN::shotgun)
                {
                    Ptr<CSound> pSound = CResMgr::GetInst()->FindRes<CSound>(L"sound\\shotgun_reload_01.wav");
                    pSound->Play(1, 0.6f, true);
                }
                CGameObject* pReload = CResMgr::GetInst()->FindRes<CPrefab>(L"ReloadBarUI")->Instantiate();
                Vec3 vPlayerPos = GetOwner()->GetParent()->GetParent()->Transform()->GetRelativePos();
                vPlayerPos.z = 4.f;
                vPlayerPos.y += 45.f;
                if (m_Reload_bar != nullptr)
                    m_Reload_bar->Destroy();
                m_Reload_bar = pReload;
                Instantiate(pReload, vPlayerPos, 0);

                pReload = CResMgr::GetInst()->FindRes<CPrefab>(L"ReloadUI")->Instantiate();
                vPlayerPos.z = 3.f;
                vPlayerPos.x -= 47.f;
                if (m_Reload != nullptr)
                    m_Reload->Destroy();
                m_Reload = pReload;
                Instantiate(pReload, vPlayerPos, 0);
                Reload();
            }
        }
        else if (KEY_PRESSED(KEY::LBTN) && m_eCurrentState == GUN_STATE::IDLE)
        {
            if (m_iCurrentMagazine == 0 && m_fAcctime == 0 && (m_iCurrentBullet > 0 || m_MaximumBullet == 0))
            {
                m_eCurrentState = GUN_STATE::RELOAD;
                Animator2D()->Play(L"Reload", false);
                if (m_UI->GetScript<CUIScript>()->getCurrentGun() != nullptr)
                    m_UI->GetScript<CUIScript>()->getCurrentGun()->Animator2D()->Play(L"Reload", false);
                if (m_gun == GUN::rusty_sidearm)
                {
                    Ptr<CSound> pSound = CResMgr::GetInst()->FindRes<CSound>(L"sound\\SAA_reload_01.wav");
                    pSound->Play(1, 0.6f, true);
                }
                else if (m_gun == GUN::shotgun)
                {
                    Ptr<CSound> pSound = CResMgr::GetInst()->FindRes<CSound>(L"sound\\shotgun_reload_01.wav");
                    pSound->Play(1, 0.6f, true);
                }
                CGameObject* pReload = CResMgr::GetInst()->FindRes<CPrefab>(L"ReloadBarUI")->Instantiate();
                Vec3 vPlayerPos = GetOwner()->GetParent()->GetParent()->Transform()->GetRelativePos();
                vPlayerPos.z = 4.f;
                vPlayerPos.y += 45.f;
                if (m_Reload_bar != nullptr)
                    m_Reload_bar->Destroy();
                m_Reload_bar = pReload;
                Instantiate(pReload, vPlayerPos, 0);

                pReload = CResMgr::GetInst()->FindRes<CPrefab>(L"ReloadUI")->Instantiate();
                vPlayerPos.z = 3.f;
                vPlayerPos.x -= 47.f;
                if (m_Reload != nullptr)
                    m_Reload->Destroy();
                m_Reload = pReload;
                Instantiate(pReload, vPlayerPos, 0);
                Reload();
            }
            else if (m_fAcctime == 0 && m_iCurrentMagazine > 0)
            {
                m_iCurrentMagazine -= 1;
                m_UI->GetScript<CUIScript>()->setMagazine(m_iCurrentMagazine, m_MagazineBullet);
                m_eCurrentState = GUN_STATE::SHOOT;
                CRenderMgr::GetInst()->GetMainCam()->GetOwner()->GetScript<CGameCameraScript>()->camerashake();
                Animator2D()->Play(L"Shoot", false);
                if (m_UI->GetScript<CUIScript>()->getCurrentGun() != nullptr)
                    m_UI->GetScript<CUIScript>()->getCurrentGun()->Animator2D()->Play(L"Shoot", false);

                Vec3 vMissilePos = Transform()->GetWorldPos();
                vMissilePos.z = 75.f;
                Vec3 vdir = Transform()->GetWorldDir(DIR::RIGHT);
                vMissilePos.x += Transform()->GetWorldScale().x / 2 * vdir.x;
                vMissilePos.y += Transform()->GetWorldScale().y / 2 * vdir.y;

                if (m_gun == GUN::rusty_sidearm)
                {
                    Ptr<CSound> pSound = CResMgr::GetInst()->FindRes<CSound>(L"sound\\colt1851_shot_0" + to_wstring((rand() % 3) + 1) + L".wav");
                    pSound->Play(1, 0.6f, true);
                    CGameObject* bullet = m_bullet->Instantiate();
                    bullet->GetScript<CBulletScript>()->setDIR(getMouseDIR() + (m_fBulletSpread * GetOwner()->GetParent()->GetParent()->GetScript<CPlayerScript>()->getSpread() / 2) * cosf((float)rand()));
                    Instantiate(bullet, vMissilePos, 2);
                }
                else if (m_gun == GUN::shotgun)
                {
                    m_iCurrentBullet -= 1;
                    m_UI->GetScript<CUIScript>()->setMaxbullet(m_MaximumBullet, m_iCurrentBullet);
                    Ptr<CSound> pSound = CResMgr::GetInst()->FindRes<CSound>(L"sound\\shotgun_shot_0" + to_wstring((rand() % 3) + 1) + L".wav");
                    pSound->Play(1, 0.6f, true);
                    for (int i = 0; i < 4; ++i)
                    {
                        CGameObject* bullet = m_bullet->Instantiate();
                        bullet->GetScript<CBulletScript>()->setDIR(getMouseDIR() + (m_fBulletSpread * GetOwner()->GetParent()->GetParent()->GetScript<CPlayerScript>()->getSpread() / 2) * cosf((float)rand()));
                        bullet->GetScript<CBulletScript>()->setSpeed(800.f + i * 40);
                        Instantiate(bullet, vMissilePos, 2);
                    }
                }
            }
        }
        else if (m_eCurrentState == GUN_STATE::SHOOT)
        {
            m_fAcctime += DT;
            if (m_fAcctime >= m_fFireInterval)
            {
                m_fAcctime = 0.f;
                m_eCurrentState = GUN_STATE::IDLE;
                Animator2D()->Play(L"Idle", true);
                if (m_UI->GetScript<CUIScript>()->getCurrentGun() != nullptr)
                    m_UI->GetScript<CUIScript>()->getCurrentGun()->Animator2D()->Play(L"Idle", true);
            }
        }
        else if (m_eCurrentState == GUN_STATE::RELOAD)
        {
            Reload();
        }
        else
        {
            m_eCurrentState = GUN_STATE::IDLE;
            Animator2D()->Play(L"Idle", true);
            if (m_UI->GetScript<CUIScript>()->getCurrentGun() != nullptr)
                m_UI->GetScript<CUIScript>()->getCurrentGun()->Animator2D()->Play(L"Idle", true);
        }
    }
}

void CGunScript::setAct()
{
    m_act = true;
    m_eCurrentState = GUN_STATE::IDLE;
    Animator2D()->Play(L"Idle", true);
    Vec3 GUNUIPos = Vec3(615.f, -350.f, 1.f);
    CGameObject* UIGUN = nullptr;
    if (m_gun == GUN::rusty_sidearm)
        UIGUN = CResMgr::GetInst()->FindRes<CPrefab>(L"rusty_sidearm_UI")->Instantiate();
    else if (m_gun == GUN::shotgun)
        UIGUN = CResMgr::GetInst()->FindRes<CPrefab>(L"shotgun_UI")->Instantiate();
    else if (m_gun == GUN::megahand)
        UIGUN = CResMgr::GetInst()->FindRes<CPrefab>(L"megahand_UI")->Instantiate();
    Instantiate(UIGUN, GUNUIPos, 31);

    m_UI->GetScript<CUIScript>()->setCurrentGun(UIGUN);
    m_UI->GetScript<CUIScript>()->getCurrentGun()->Animator2D()->Play(L"Idle", true);
    m_UI->GetScript<CUIScript>()->setMagazine(m_iCurrentMagazine, m_MagazineBullet);
    m_UI->GetScript<CUIScript>()->setMaxbullet(m_MaximumBullet, m_iCurrentBullet);
}

void CGunScript::setDeact()
{
    m_act = false;
    m_eCurrentState = GUN_STATE::IDLE;
    Animator2D()->Play(L"Delete", true);
    m_fAcctime = 0.f;
    m_fCharge = 0.f;
    if (m_Reload != nullptr)
    {
        m_Reload->Destroy();
        m_Reload = nullptr;
    }
    if (m_Reload_bar != nullptr)
    {
        m_Reload_bar->Destroy();
        m_Reload_bar = nullptr;
    }
}

float CGunScript::getMouseDIR()
{
    Vec3 vPlayerPos = Transform()->GetWorldPos();

    Vec3 vMousePos = CLevelMgr::GetInst()->getMousepos();

    float degree = atan2((vMousePos.y - vPlayerPos.y), (vMousePos.x - vPlayerPos.x)) * 180 / 3.141592f;

    return degree;
}

void CGunScript::Reload(bool _delete)
{
    if (m_fAcctime >= m_fReloadtime)
    {
        UINT num = m_MagazineBullet;
        if (m_iCurrentBullet != 0 && m_iCurrentBullet < m_MagazineBullet)
        {
            num = m_iCurrentBullet;
        }
        m_iCurrentMagazine = num;

        m_eCurrentState = GUN_STATE::IDLE;
        Animator2D()->Play(L"Idle", true);
        if (m_UI->GetScript<CUIScript>()->getCurrentGun() != nullptr)
            m_UI->GetScript<CUIScript>()->getCurrentGun()->Animator2D()->Play(L"Idle", true);
        m_fAcctime = 0.f;
        m_UI->GetScript<CUIScript>()->setMagazine(m_iCurrentMagazine, m_MagazineBullet);
        if (m_Reload != nullptr)
        {
            m_Reload->Destroy();
            m_Reload = nullptr;
        }
        if (m_Reload_bar != nullptr)
        {
            m_Reload_bar->Destroy();
            m_Reload_bar = nullptr;
        }
        return;
    }

    if (Animator2D()->GetCurrentAnimation() != Animator2D()->FindAnimation(L"Reload") && !_delete)
    {
        Animator2D()->Play(L"Idle", true);
    }
    m_fAcctime += DT;

    {
        Vec3 vPos = GetOwner()->GetParent()->GetParent()->Transform()->GetRelativePos();
        vPos.z = 4.f;
        vPos.y += 45.f;
        m_Reload_bar->Transform()->SetRelativePos(vPos);
        vPos.z = 3.f;
        vPos.x += (m_fAcctime / m_fReloadtime * 94.f) - 47.f;
        m_Reload->Transform()->SetRelativePos(vPos);
    }
}

void CGunScript::SaveToFile(FILE* _File)
{
    CScript::SaveToFile(_File);
    fwrite(&m_MaximumBullet, sizeof(UINT), 1, _File);
    fwrite(&m_MagazineBullet, sizeof(UINT), 1, _File);
    fwrite(&m_fReloadtime, sizeof(float), 1, _File);
    fwrite(&m_fBulletSpread, sizeof(float), 1, _File);
    //fwrite(&m_bullet, sizeof(Ptr<CPrefab>), 1, _File);
}

void CGunScript::LoadFromFile(FILE* _File)
{
    CScript::LoadFromFile(_File);
    fread(&m_MaximumBullet, sizeof(UINT), 1, _File);
    fread(&m_MagazineBullet, sizeof(UINT), 1, _File);
    fread(&m_fReloadtime, sizeof(float), 1, _File);
    fread(&m_fBulletSpread, sizeof(float), 1, _File);
    //fread(&m_bullet, sizeof(Ptr<CPrefab>), 1, _File);
}


