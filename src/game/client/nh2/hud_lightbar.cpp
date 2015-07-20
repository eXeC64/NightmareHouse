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

class CHudLightBar : public CHudElement, public vgui::Panel
{
	DECLARE_CLASS_SIMPLE(CHudLightBar, vgui::Panel);

public:
	CHudLightBar(const char* pElementName);

	virtual void Init (void);
	virtual void Reset (void);

protected:
	virtual void Paint();

	vgui::IImage* m_pBar;
	vgui::IImage* m_pBackground;
	vgui::IImage* m_pIcon;
	CPanelAnimationVar(float, m_flBarWidth, "BarWidth", "0.7");
	CPanelAnimationVar(float, m_flBarHeight, "BarHeight", "0.775");
	CPanelAnimationVar(float, m_flBarInsetX, "BarInsetX", "0.15");
	CPanelAnimationVar(float, m_flBarInsetY, "BarInsetY", "0.125");
	CPanelAnimationVar(float, m_flIconSize, "IconSize", "1.0");
};

DECLARE_HUDELEMENT (CHudLightBar);

//------------------------------------------------------------------------
// Purpose: Constructor
//------------------------------------------------------------------------
CHudLightBar::CHudLightBar (const char * pElementName) :
	CHudElement(pElementName), BaseClass(NULL, "NH2LightBar")
{
	vgui::Panel* pParent = g_pClientMode->GetViewport();
	SetParent(pParent);
	m_pBar = vgui::scheme()->GetImage("hud/healthbar_fg", false);
	m_pBackground = vgui::scheme()->GetImage("hud/healthbar_bg", false);
	m_pIcon = vgui::scheme()->GetImage("hud/flashlight_icon", false);
	SetHiddenBits (HIDEHUD_HEALTH | HIDEHUD_PLAYERDEAD | HIDEHUD_NEEDSUIT);
}

void CHudLightBar::Init()
{
	Reset();
}

void CHudLightBar::Reset (void)
{
	SetFgColor(Color(255,255,255,255));
	SetBgColor(Color(0,0,0,0));
}

void CHudLightBar::Paint()
{
	C_BaseHLPlayer *pPlayer = (C_BaseHLPlayer *)C_BasePlayer::GetLocalPlayer();
	if ( !pPlayer )
		return;

	const bool bIsOn = pPlayer->IsEffectActive( EF_DIMLIGHT );
	const double flBattery = pPlayer->m_HL2Local.m_flFlashBattery;

	// Don't draw a full bar
	if (flBattery >= 100.0)
	  return;

	int panelWidth, panelHeight;
	GetSize(panelWidth, panelHeight);

	m_pBackground->SetSize(panelWidth, panelHeight);
	m_pBackground->SetPos(0,0);
	m_pBackground->Paint();

	m_pBar->SetColor(Color(255,flBattery > 21 ? 255 : 0, flBattery > 21 ? 255 : 0, 255));
	m_pBar->SetSize(panelWidth*m_flBarWidth, panelHeight*m_flBarHeight);
	m_pBar->SetPos(panelWidth*m_flBarInsetX, panelHeight*m_flBarInsetY);
	m_pBar->SetFrame(static_cast<int>(flBattery));
	m_pBar->Paint();

	m_pIcon->SetSize(panelHeight * m_flIconSize, panelHeight * m_flIconSize);
	m_pIcon->SetPos(0.5 * panelHeight * (1.0 - m_flIconSize), 0.5 * panelHeight * (1.0 - m_flIconSize));
	/* m_pBar->SetFrame(bIsOn ? 1 : 2); */
	if (bIsOn)
	m_pIcon->Paint();

	//Draw icon
}
