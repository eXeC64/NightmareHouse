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

#include "tier0/memdbgon.h"

class CHudHealthBar : public CHudElement, public vgui::Panel
{
	DECLARE_CLASS_SIMPLE(CHudHealthBar, vgui::Panel);

public:
	CHudHealthBar(const char* pElementName);

	virtual void Init (void);
	virtual void Reset (void);
	virtual void OnThink (void);

protected:
	virtual void Paint();

	vgui::IImage* m_pBar;
	vgui::IImage* m_pBackground;
	float m_flHealth;
	CPanelAnimationVar(vgui::HFont, m_hTextFont, "TextFont", "NH2HUDText");
	CPanelAnimationVar(int, m_iTextAlpha, "TextAlpha", "150");
	CPanelAnimationVar(float, m_flTextX, "TextX", "0.025");
	CPanelAnimationVar(float, m_flTextY, "TextY", "0.10");
	CPanelAnimationVar(float, m_flBarWidth, "BarWidth", "0.7");
	CPanelAnimationVar(float, m_flBarHeight, "BarHeight", "0.775");
	CPanelAnimationVar(float, m_flBarInsetX, "BarInsetX", "0.15");
	CPanelAnimationVar(float, m_flBarInsetY, "BarInsetY", "0.125");
};

DECLARE_HUDELEMENT (CHudHealthBar);

//------------------------------------------------------------------------
// Purpose: Constructor
//------------------------------------------------------------------------
CHudHealthBar::CHudHealthBar (const char * pElementName) :
	CHudElement(pElementName), BaseClass(NULL, "NH2HealthBar")
{
	vgui::Panel* pParent = g_pClientMode->GetViewport();
	SetParent(pParent);
	m_pBar = vgui::scheme()->GetImage("hud/bar_fg", false);
	m_pBackground = vgui::scheme()->GetImage("hud/bar_bg", false);
	SetHiddenBits (HIDEHUD_HEALTH | HIDEHUD_PLAYERDEAD | HIDEHUD_NEEDSUIT);
}

void CHudHealthBar::Init()
{
	Reset();
}

void CHudHealthBar::Reset (void)
{
	m_flHealth = 100;
	SetFgColor(Color(255,255,255,255));
	SetBgColor(Color(0,0,0,0));
}

void CHudHealthBar::OnThink (void)
{
	C_BasePlayer* local = C_BasePlayer::GetLocalPlayer();
	if (local)
		m_flHealth = max(local->GetHealth(), 0);
	if (m_flHealth > 100)
		m_flHealth = 100;
}

void CHudHealthBar::Paint()
{
	int panelWidth, panelHeight;
	GetSize(panelWidth, panelHeight);

	m_pBackground->SetSize(panelWidth, panelHeight);
	m_pBackground->SetPos(0,0);
	m_pBackground->Paint();

	m_pBar->SetSize(panelWidth*m_flBarWidth, panelHeight*m_flBarHeight);
	m_pBar->SetPos(panelWidth*m_flBarInsetX, panelHeight*m_flBarInsetY);
	m_pBar->SetFrame(static_cast<int>(m_flHealth-1));
	m_pBar->Paint();

	//Draw text
	char sz[16];
	wchar_t wsz[16];
	Q_snprintf(sz, sizeof( sz ), "%d", static_cast<int>(m_flHealth));
	g_pVGuiLocalize->ConvertANSIToUnicode(sz, wsz, sizeof(wsz));

	vgui::surface()->DrawSetTextFont(m_hTextFont);
	vgui::surface()->DrawSetTextColor(Color(255,m_flHealth > 21 ? 255 : 0, m_flHealth > 21 ? 255 : 0, m_iTextAlpha));
	vgui::surface()->DrawSetTextPos(panelWidth*m_flTextX, panelHeight*m_flTextY);
	vgui::surface()->DrawPrintText(wsz,wcslen(wsz));
}
