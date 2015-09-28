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
	CHudElement( pElementName ), BaseClass( NULL, "HudPickups" )
{	
	vgui::Panel *pParent = g_pClientMode->GetViewport();
	SetParent( pParent );
	SetHiddenBits( HIDEHUD_MISCSTATUS );
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
void CHudPickups::Init( void )
{
	HOOK_HUD_MESSAGE( CHudPickups, ItemPickup );
	Reset();
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
void CHudPickups::Reset( void )
{
	//clear history
}

//-----------------------------------------------------------------------------
// Purpose: adds an element to the history
//-----------------------------------------------------------------------------
void CHudPickups::AddToHistory( C_BaseCombatWeapon *weapon )
{
	//Add weapon to history
}

//-----------------------------------------------------------------------------
// Purpose: Handle an item pickup event from the server
//-----------------------------------------------------------------------------
void CHudPickups::MsgFunc_ItemPickup( bf_read &msg )
{
	char szName[1024];
	msg.ReadString( szName, sizeof(szName) );
	// Add the item to the history
}

//-----------------------------------------------------------------------------
// Purpose: Draw the pickup history
//-----------------------------------------------------------------------------
void CHudPickups::Paint( void )
{
	int wide, tall;
	GetSize( wide, tall );
}
