 
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
//-----------------------------------------------------------------------------
// Purpose: Shows the veins
//-----------------------------------------------------------------------------
 
void cb_veinsoverlay( IConVar *pConVar, const char *pOldString, float flOldValue );
ConVar r_veinsoverlay("r_veinsoverlay","1", FCVAR_ARCHIVE ,"Toggle visibility of veins overlay.", false, 0, true, 1, cb_veinsoverlay );

static int veins_lastactive;
void cb_veinsoverlay( IConVar *pConVar, const char *pOldString, float flOldValue )
{
	
	veins_lastactive = gpGlobals->curtime + 2.0; //the time at which point we stop flickering
}

//extern ConVar r_flashlightforceflicker( "r_flashlightforceflicker","0", FCVAR_NONE, "Force flashlight flicker");
class CHudVeins : public CHudElement, public vgui::Panel
{
 
	DECLARE_CLASS_SIMPLE(CHudVeins, vgui::Panel);
 
public:
	CHudVeins(const char * pElementName);
 
	virtual void Init (void);
	virtual void Reset (void);
	virtual void OnThink (void);
 
protected:
	virtual void ApplySchemeSettings(vgui::IScheme *scheme);
	virtual void Paint();
	virtual void PaintBackground();

private:
	int m_nTexture_FG;

	int lasthealth;
	//CHudTexture*	m_pVeins;
 
 
};
 
DECLARE_HUDELEMENT_DEPTH( CHudVeins, 100 );
 
 
//------------------------------------------------------------------------
// Purpose: Constructor
//------------------------------------------------------------------------
 
CHudVeins:: CHudVeins (const char * pElementName) : 
	CHudElement (pElementName), BaseClass (NULL, "HudVeins")
{
	vgui::Panel * pParent = g_pClientMode->GetViewport();
	//vgui::VPANEL pParent = enginevgui->GetPanel( PANEL_CLIENTDLL );
	SetParent (pParent);

	
	m_nTexture_FG = surface()->CreateNewTextureID();
	surface()->DrawSetTextureFile( m_nTexture_FG, "effects/veins", true, false );

	int screenWide, screenTall;
	GetHudSize(screenWide, screenTall);
	SetBounds(0, 0, screenWide, screenTall);

	r_veinsoverlay.SetValue( 1 );
	ConVar* flicker = cvar->FindVar("r_flashlightforceflicker");
	if (flicker)
		flicker->SetValue( 0 );
	

 
}

void CHudVeins::ApplySchemeSettings( vgui::IScheme *scheme )
{
	BaseClass::ApplySchemeSettings(scheme);
 
	SetPaintBackgroundEnabled(false);
	SetPaintBorderEnabled(false);
 
	//if (!m_pVeins)
	//{
	//	m_pVeins = gHUD.GetIcon("veins");
	//}
}

//------------------------------------------------------------------------
// Purpose:
//------------------------------------------------------------------------
 
void CHudVeins:: Init()
{
	Reset();
}
 
//------------------------------------------------------------------------
// Purpose:
//-----------------------------------------------------------------------
 
void CHudVeins:: Reset (void)
{
	SetBgColor (Color (255,255,255,255));
	SetFgColor (Color (255,255,255,255));
	SetAlpha( 0 );
}
 
 
//------------------------------------------------------------------------
// Purpose:
//------------------------------------------------------------------------


void CHudVeins:: OnThink (void)
{


}
 
 
//------------------------------------------------------------------------
// Purpose: draws the power bar
//------------------------------------------------------------------------
 
void CHudVeins::Paint()
{

	C_BaseHLPlayer *pPlayer = (C_BaseHLPlayer *)C_BasePlayer::GetLocalPlayer();
	if ( !pPlayer )
		return;

	

	
	

	//if ( r_veinstime.GetFloat() < gpGlobals->curtime )
		//r_veinsoverlay.SetValue( 1 );

	if ( veins_lastactive < gpGlobals->curtime )
		r_veinsoverlay.SetValue( 1 );

	if ( pPlayer->GetHealth() <= 25  && r_veinsoverlay.GetBool() )
	{
		
		float a = pPlayer->GetHealth();
		// 15-25 /-15/ 0-10 /*25.5/ 0-255 /*-1/ 0-(-255) /+255/ 255-0
		a -= 15;
		a *= 25.5;
		a *= -1;
		a += 255;
		

		g_pClientMode->GetViewportAnimationController()->RunAnimationCommand( this, "alpha", a, 0.0f, 0.4f, AnimationController::INTERPOLATOR_LINEAR);
	}
	else
	{
		
		g_pClientMode->GetViewportAnimationController()->RunAnimationCommand( this, "alpha", 0.0f, 0.0f, 0.4f, AnimationController::INTERPOLATOR_LINEAR);
	}
	
	
	SetBgColor(Color(0,0,0,0));
	vgui::surface()->DrawSetColor(GetFgColor());
	surface()->DrawSetTexture( m_nTexture_FG );
	//surface()->DrawTexturedRect( 0, 0, 640, 480 );
	//surface()->DrawTexturedRect( 0, 0, GetWide(), GetTall() );
	int w,h;
	GetHudSize( w, h);
	if (GetWide() != w)
		SetWide(w);

	if (GetTall() != h)
	SetTall(h);
	surface()->DrawTexturedRect( 0, 0, w, h );

	//EmitSound();
	

	//m_pVeins->DrawSelf(0,0, GetWide(), GetTall(), Color(255,255,255,255));
		

}



void CHudVeins::PaintBackground()
{
	SetBgColor(Color(0,0,0,0));
	SetPaintBorderEnabled(false);

	BaseClass::PaintBackground();

}

