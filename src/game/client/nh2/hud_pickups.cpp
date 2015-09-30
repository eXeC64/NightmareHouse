//========= Copyright Valve Corporation, All rights reserved. ============//
//
// Purpose: Item pickup history displayed onscreen when items are picked up.
//
// $NoKeywords: $
//=============================================================================//
#include "cbase.h"
#include "hud_pickups.h"
#include "hud_macros.h"
#include <vgui_controls/Controls.h>
#include <vgui/ILocalize.h>
#include <vgui/ISurface.h>
#include "iclientmode.h"
#include "vgui_controls/AnimationController.h"

// memdbgon must be the last include file in a .cpp file!!!
#include "tier0/memdbgon.h"

using namespace vgui;

DECLARE_HUDELEMENT( CHudPickups );
DECLARE_HUD_MESSAGE( CHudPickups, ItemPickup );

//-----------------------------------------------------------------------------
// Purpose: Constructor
//-----------------------------------------------------------------------------
CHudPickups::CHudPickups( const char *pElementName ) :
	CHudElement( pElementName ), BaseClass( NULL, "NH2Pickups" )
{	
	vgui::Panel *pParent = g_pClientMode->GetViewport();
	SetParent(pParent);
	SetHiddenBits(HIDEHUD_NEEDSUIT);

	m_iWep = 0;
	m_iAmmo = 0;
	m_iAmmoQuantity = 0;

	m_pHealthKit = vgui::scheme()->GetImage("hud/medkit", false);
	m_pHealthVial = vgui::scheme()->GetImage("hud/bandage", false);

	m_pWep[0] = vgui::scheme()->GetImage("hud/slot1_icon", false);
	m_pWep[1] = vgui::scheme()->GetImage("hud/slot2_icon", false);
	m_pWep[2] = vgui::scheme()->GetImage("hud/slot3_icon", false);
	m_pWep[3] = vgui::scheme()->GetImage("hud/slot4_icon", false);
	m_pWep[4] = vgui::scheme()->GetImage("hud/slot5_icon", false);

	m_pNumbers = vgui::scheme()->GetImage("hud/numbers", false);
	m_pAmmo[0] = vgui::scheme()->GetImage("hud/slot2_ammo_icon", false);
	m_pAmmo[1] = vgui::scheme()->GetImage("hud/slot3_ammo_icon", false);
	m_pAmmo[2] = vgui::scheme()->GetImage("hud/slot4_ammo_icon", false);
	m_pAmmo[3] = vgui::scheme()->GetImage("hud/slot5_ammo_icon", false);
}

void CHudPickups::Init()
{
	HOOK_HUD_MESSAGE( CHudPickups, ItemPickup );
	Reset();
}

void CHudPickups::Reset()
{
	SetFgColor(Color(255,255,255,255));
	SetBgColor(Color(0,0,0,0));
	m_bKitVisible = false;
	m_bVialVisible = false;
	m_bWepVisible = false;
	m_bAmmoVisible = false;
}

void CHudPickups::Think()
{
	if(m_bKitVisible && m_flKitUntil < gpGlobals->curtime)
	{
		m_bKitVisible = false;
		g_pClientMode->GetViewportAnimationController()->RunAnimationCommand( this, "KitAlpha", 0.0f, 0.0f, m_flFadeTime, vgui::AnimationController::INTERPOLATOR_LINEAR);
	}
	if(m_bVialVisible && m_flVialUntil < gpGlobals->curtime)
	{
		m_bVialVisible = false;
		g_pClientMode->GetViewportAnimationController()->RunAnimationCommand( this, "VialAlpha", 0.0f, 0.0f, m_flFadeTime, vgui::AnimationController::INTERPOLATOR_LINEAR);
	}

	if(m_bWepVisible && m_flWepUntil < gpGlobals->curtime)
	{
		m_bWepVisible = false;
		g_pClientMode->GetViewportAnimationController()->RunAnimationCommand( this, "WepAlpha", 0.0f, 0.0f, m_flFadeTime, vgui::AnimationController::INTERPOLATOR_LINEAR);
	}

	if(m_bAmmoVisible && m_flAmmoUntil < gpGlobals->curtime)
	{
		m_bAmmoVisible = false;
		g_pClientMode->GetViewportAnimationController()->RunAnimationCommand( this, "AmmoAlpha", 0.0f, 0.0f, m_flFadeTime, vgui::AnimationController::INTERPOLATOR_LINEAR);
		g_pClientMode->GetViewportAnimationController()->RunAnimationCommand( this, "QtyAlpha", 0.0f, 0.0f, m_flFadeTime, vgui::AnimationController::INTERPOLATOR_LINEAR);
	}
}

void CHudPickups::ShowPickup(C_BaseCombatWeapon *weapon)
{
	//Add weapon to history
	m_iWep = weapon->GetSlot();
	g_pClientMode->GetViewportAnimationController()->RunAnimationCommand( this, "WepAlpha", 1.0f, 0.0f, m_flFadeTime, vgui::AnimationController::INTERPOLATOR_LINEAR);
	m_bWepVisible = true;
	m_flWepUntil = gpGlobals->curtime + m_flDisplayTime;
}

void CHudPickups::ShowAmmo(int type, int quantity)
{
	switch(type)
	{
		case 3:
			m_iAmmo = 0; break;
		case 5:
			m_iAmmo = 1; break;
		case 4:
			m_iAmmo = 2; break;
		case 7:
			m_iAmmo = 3; break;
		default:
			return;
	}
	m_iAmmoQuantity = quantity;
	m_bAmmoVisible = true;
	m_flAmmoUntil = gpGlobals->curtime + m_flDisplayTime;
	g_pClientMode->GetViewportAnimationController()->RunAnimationCommand( this, "AmmoAlpha", 1.0f, 0.0f, m_flFadeTime, vgui::AnimationController::INTERPOLATOR_LINEAR);
	g_pClientMode->GetViewportAnimationController()->RunAnimationCommand( this, "QtyAlpha", 1.0f, 0.0f, m_flFadeTime, vgui::AnimationController::INTERPOLATOR_LINEAR);
}

void CHudPickups::MsgFunc_ItemPickup( bf_read &msg )
{
	char szName[1024];
	msg.ReadString(szName, sizeof(szName));
	// Add the item to the history
	if(Q_strcmp("item_healthkit", szName) == 0)
	{
		g_pClientMode->GetViewportAnimationController()->RunAnimationCommand( this, "KitAlpha", 1.0f, 0.0f, m_flFadeTime, vgui::AnimationController::INTERPOLATOR_LINEAR);
		m_bKitVisible = true;
		m_flKitUntil = gpGlobals->curtime + m_flDisplayTime;
	}

	if(Q_strcmp("item_healthvial", szName) == 0)
	{
		g_pClientMode->GetViewportAnimationController()->RunAnimationCommand( this, "VialAlpha", 1.0f, 0.0f, m_flFadeTime, vgui::AnimationController::INTERPOLATOR_LINEAR);
		m_bVialVisible = true;
		m_flVialUntil = gpGlobals->curtime + m_flDisplayTime;
	}
}

void CHudPickups::Paint()
{
	int panelWidth, panelHeight;
	GetSize(panelWidth, panelHeight);

	m_pHealthKit->SetSize(panelHeight * m_flKitScale, panelHeight * m_flKitScale);
	m_pHealthKit->SetPos(panelWidth*m_flKitX, panelHeight*m_flKitY);
	m_pHealthKit->SetColor(Color(255,255,255,255 * m_flKitAlpha));
	m_pHealthKit->Paint();

	m_pHealthVial->SetSize(panelHeight * m_flVialScale, panelHeight * m_flVialScale);
	m_pHealthVial->SetPos(panelWidth*m_flVialX, panelHeight*m_flVialY);
	m_pHealthVial->SetColor(Color(255,255,255,255 * m_flVialAlpha));
	m_pHealthVial->Paint();

	if(m_iWep >= 0 && m_iWep < 5)
	{
		m_pWep[m_iWep]->SetSize(2*panelHeight*m_flWepScale, panelHeight*m_flWepScale);
		m_pWep[m_iWep]->SetPos(panelWidth*m_flWepX, panelHeight*m_flWepY);
		m_pWep[m_iWep]->SetColor(Color(255,255,255,255 * m_flWepAlpha));
		m_pWep[m_iWep]->Paint();
	}

	if(m_iAmmo >= 0 && m_iAmmo < 4)
	{
		m_pAmmo[m_iAmmo]->SetSize(panelHeight*m_flAmmoScale, panelHeight*m_flAmmoScale);
		m_pAmmo[m_iAmmo]->SetPos(panelWidth*m_flAmmoX, panelHeight*m_flAmmoY);
		m_pAmmo[m_iAmmo]->SetColor(Color(255,255,255,255 * m_flAmmoAlpha));
		m_pAmmo[m_iAmmo]->Paint();

		m_pNumbers->SetSize(panelHeight*m_flAmmoQuantityScale, panelHeight*m_flAmmoQuantityScale);
		m_pNumbers->SetPos(panelWidth*m_flAmmoQuantityX, panelHeight*m_flAmmoQuantityY);
		m_pNumbers->SetColor(Color(255,255,255,255 * m_flAmmoAlpha));
		m_pNumbers->SetFrame(m_iAmmoQuantity);
		m_pNumbers->Paint();
	}
}
