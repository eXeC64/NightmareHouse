 
#include "cbase.h"
#include "hud.h"
#include "hudelement.h"
#include "hud_macros.h"
#include "iclientmode.h"
#include "c_basehlplayer.h"
#include "vgui_controls/Panel.h"
#include "vgui_controls/AnimationController.h"
#include "vgui/ISurface.h"
#include <vgui/ILocalize.h>
#include "ienginevgui.h"
 
using namespace vgui;
 
#include "tier0/memdbgon.h" 

class CHudVeins : public CHudElement, public vgui::Panel
{
	DECLARE_CLASS_SIMPLE(CHudVeins, vgui::Panel);
 
public:
	CHudVeins(const char * pElementName);
 
	virtual void Init (void);
	virtual void Reset (void);
	virtual void Paint();

private:
	vgui::IImage* m_pVeins;
};
 
DECLARE_HUDELEMENT_DEPTH( CHudVeins, 100 );
 
CHudVeins:: CHudVeins (const char * pElementName) : 
	CHudElement (pElementName), BaseClass (NULL, "NH2Veins")
{
	vgui::Panel * pParent = g_pClientMode->GetViewport();
	SetParent (pParent);
	m_pVeins = vgui::scheme()->GetImage("hud/veins", false);
}

void CHudVeins::Init()
{
	Reset();
}
 
void CHudVeins::Reset (void)
{
	SetFgColor(Color(255,255,255,255));
	SetBgColor(Color(255,255,255,0));
}
 
void CHudVeins::Paint()
{
	C_BaseHLPlayer *pPlayer = (C_BaseHLPlayer *)C_BasePlayer::GetLocalPlayer();
	if ( !pPlayer )
		return;

	if(pPlayer->GetHealth() > 25)
		return;

	int wide, tall;
	GetHudSize(wide, tall);
	SetBounds(0, 0, wide, tall);
	m_pVeins->SetSize(wide, tall);
	m_pVeins->SetPos(0,0);
	m_pVeins->SetColor(Color(255,255,255,255 - pPlayer->GetHealth() * 10));
	m_pVeins->Paint();
}
