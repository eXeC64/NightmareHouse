#include "cbase.h"
#include "hud.h"
#include "hud_macros.h"
#include "c_baseplayer.h"
#include "iclientmode.h"

#include "hudelement.h"
#include "hud_numericdisplay.h"
#include "vgui_controls/AnimationController.h"
#include "vgui/IScheme.h"
#include "vgui/ISurface.h"
#include "vgui/ILocalize.h"
#include "vgui/IImage.h"
#include "c_basehlplayer.h"

#include "tier0/memdbgon.h"

class CHudSprintBar : public CHudElement, public vgui::Panel
{
	DECLARE_CLASS_SIMPLE(CHudSprintBar, vgui::Panel);

public:
	CHudSprintBar(const char* pElementName);

	virtual void Init (void);
	virtual void Reset (void);

protected:
	virtual void Paint();

	vgui::IImage* m_pBar;
	vgui::IImage* m_pBackground;
	CPanelAnimationVar(vgui::HFont, m_hIconFont, "IconFont", "NH2HUDIcons");
	CPanelAnimationVar(int, m_iIconAlpha, "IconAlpha", "150");
	CPanelAnimationVar(float, m_flIconX, "IconX", "0.025");
	CPanelAnimationVar(float, m_flIconY, "IconY", "0.10");
	CPanelAnimationVar(float, m_flBarWidth, "BarWidth", "0.7");
	CPanelAnimationVar(float, m_flBarHeight, "BarHeight", "0.775");
	CPanelAnimationVar(float, m_flBarInsetX, "BarInsetX", "0.15");
	CPanelAnimationVar(float, m_flBarInsetY, "BarInsetY", "0.125");
};

DECLARE_HUDELEMENT (CHudSprintBar);

//------------------------------------------------------------------------
// Purpose: Constructor
//------------------------------------------------------------------------
CHudSprintBar::CHudSprintBar (const char * pElementName) :
	CHudElement(pElementName), BaseClass(NULL, "NH2SprintBar")
{
	vgui::Panel* pParent = g_pClientMode->GetViewport();
	SetParent(pParent);
	m_pBar = vgui::scheme()->GetImage("hud/healthbar_fg", false);
	m_pBackground = vgui::scheme()->GetImage("hud/healthbar_bg", false);
	SetHiddenBits (HIDEHUD_HEALTH | HIDEHUD_PLAYERDEAD | HIDEHUD_NEEDSUIT);
}

void CHudSprintBar::Init()
{
	Reset();
}

void CHudSprintBar::Reset (void)
{
	SetFgColor(Color(255,255,255,255));
	SetBgColor(Color(0,0,0,0));
}

void CHudSprintBar::Paint()
{
	C_BaseHLPlayer *pPlayer = (C_BaseHLPlayer *)C_BasePlayer::GetLocalPlayer();
	if ( !pPlayer )
		return;

	const float flPower = pPlayer->m_HL2Local.m_flSuitPower;

  // Don't draw a full bar
	if (flPower >= 100.0)
	  return;

	int panelWidth, panelHeight;
	GetSize(panelWidth, panelHeight);

	m_pBackground->SetSize(panelWidth, panelHeight);
	m_pBackground->SetPos(0,0);
	m_pBackground->Paint();


	m_pBar->SetSize(panelWidth*m_flBarWidth, panelHeight*m_flBarHeight);
	m_pBar->SetPos(panelWidth*m_flBarInsetX, panelHeight*m_flBarInsetY);
	m_pBar->SetFrame(max(0,flPower));
	m_pBar->Paint();

	//Draw sprint icon
	vgui::surface()->DrawSetTextFont(m_hIconFont);
	vgui::surface()->DrawSetTextColor(Color(255,flPower > 21 ? 255 : 0, flPower > 21 ? 255 : 0, m_iIconAlpha));
	vgui::surface()->DrawSetTextPos(panelWidth*m_flIconX, panelHeight*m_flIconY);
	vgui::surface()->DrawPrintText(L"D",1);
}
