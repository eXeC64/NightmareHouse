//========= Copyright Valve Corporation, All rights reserved. ============//
//
// Purpose: 
//
//=============================================================================//

#include "cbase.h"
#include "weapon_selection.h"
#include "iclientmode.h"
#include "history_resource.h"
#include "input.h"
#include "../hud_crosshair.h"

#include "VGuiMatSurface/IMatSystemSurface.h"
#include <KeyValues.h>
#include <vgui/IScheme.h>
#include <vgui/ISurface.h>
#include <vgui/ISystem.h>
#include <vgui_controls/AnimationController.h>
#include <vgui_controls/Panel.h>

#include "vgui/ILocalize.h"

// memdbgon must be the last include file in a .cpp file!!!
#include "tier0/memdbgon.h"

#define SELECTION_TIMEOUT_THRESHOLD		0.5f	// Seconds
#define SELECTION_FADEOUT_TIME			0.75f

#define FASTSWITCH_DISPLAY_TIMEOUT		1.5f
#define FASTSWITCH_FADEOUT_TIME			1.5f

//-----------------------------------------------------------------------------
// Purpose: hl2 weapon selection hud element
//-----------------------------------------------------------------------------
class CHudWeaponSelection : public CBaseHudWeaponSelection, public vgui::Panel
{
	DECLARE_CLASS_SIMPLE( CHudWeaponSelection, vgui::Panel );

public:
	CHudWeaponSelection(const char *pElementName );

	virtual bool ShouldDraw();
	virtual void OnWeaponPickup( C_BaseCombatWeapon *pWeapon );

	virtual void CycleToNextWeapon( void );
	virtual void CycleToPrevWeapon( void );

	virtual C_BaseCombatWeapon *GetWeaponInSlot( int iSlot, int iSlotPos );
	virtual void SelectWeaponSlot( int iSlot );

	virtual C_BaseCombatWeapon	*GetSelectedWeapon( void )
	{ 
		return m_hSelectedWeapon;
	}

	virtual void OpenSelection( void );
	virtual void HideSelection( void );

	virtual void LevelInit();

protected:
	virtual void OnThink();
	virtual void Paint();
	virtual void ApplySchemeSettings(vgui::IScheme *pScheme);

	virtual bool IsWeaponSelectable()
	{ 
		if (IsInSelectionMode())
			return true;

		return false;
	}

private:
	C_BaseCombatWeapon *FindNextWeaponInWeaponSelection(int iCurrentSlot, int iCurrentPosition);
	C_BaseCombatWeapon *FindPrevWeaponInWeaponSelection(int iCurrentSlot, int iCurrentPosition);

	void DrawWeaponBox( C_BaseCombatWeapon *pWeapon, bool bSelected, int x, int y);
	void ActivateWeaponHighlight( C_BaseCombatWeapon *pWeapon );
	int GetLastPosInSlot( int iSlot ) const;
    
	void FastWeaponSwitch( int iWeaponSlot );
	void PlusTypeFastWeaponSwitch( int iWeaponSlot );

	virtual	void SetSelectedWeapon( C_BaseCombatWeapon *pWeapon ) 
	{ 
		m_hSelectedWeapon = pWeapon;
	}

	virtual	void SetSelectedSlot( int slot ) 
	{ 
		m_iSelectedSlot = slot;
	}

	void SetSelectedSlideDir( int dir )
	{
		m_iSelectedSlideDir = dir;
	}

	CPanelAnimationVarAliasType( float, m_flBoxWide, "BoxWide", "108", "proportional_float" );
	CPanelAnimationVarAliasType( float, m_flBoxTall, "BoxTall", "72", "proportional_float" );
	CPanelAnimationVarAliasType( float, m_flSelectedScale, "SelectedScale", "1.5", "float" );
	CPanelAnimationVarAliasType( float, m_flWepXPos, "WepXPos", "0.2", "float" );
	CPanelAnimationVarAliasType( float, m_flWepYPos, "WepYPos", "0.4", "float" );
	CPanelAnimationVarAliasType( float, m_flWepXSize, "WepXSize", "0.55", "float" );
	CPanelAnimationVarAliasType( float, m_flWepYSize, "WepYSize", "0.4", "float" );

	bool m_bFadingOut;

	// fastswitch weapon display
	struct WeaponBox_t
	{
		int m_iSlot;
		int m_iSlotPos;
   	};
	CUtlVector<WeaponBox_t>	m_WeaponBoxes;
	int						m_iSelectedWeaponBox;
	int						m_iSelectedSlideDir;
	int						m_iSelectedBoxPosition;
	int						m_iSelectedSlot;
	C_BaseCombatWeapon		*m_pLastWeapon;
	CPanelAnimationVar( float, m_flHorizWeaponSelectOffsetPoint, "WeaponBoxOffset", "0" );

	//NH2
	int	m_nBoxTextureId;
	int	m_nBoxTextureId2;
	int	m_nSlot1TextureId;
	int	m_nSlot2TextureId;
	int	m_nSlot3TextureId;
	int	m_nSlot4TextureId;
	int	m_nSlot5TextureId;
};

DECLARE_HUDELEMENT( CHudWeaponSelection );

using namespace vgui;

//-----------------------------------------------------------------------------
// Purpose: Constructor
//-----------------------------------------------------------------------------
CHudWeaponSelection::CHudWeaponSelection( const char *pElementName ) : CBaseHudWeaponSelection(pElementName), BaseClass(NULL, "NH2WeaponSelection")
{
	vgui::Panel *pParent = g_pClientMode->GetViewport();
	SetParent( pParent );
	m_bFadingOut = false;
}

//-----------------------------------------------------------------------------
// Purpose: sets up display for showing weapon pickup
//-----------------------------------------------------------------------------
void CHudWeaponSelection::OnWeaponPickup( C_BaseCombatWeapon *pWeapon )
{
	// add to pickup history
	CHudHistoryResource *pHudHR = GET_HUDELEMENT( CHudHistoryResource );
	if ( pHudHR )
	{
		pHudHR->AddToHistory( pWeapon );
	}
}

//-----------------------------------------------------------------------------
// Purpose: updates animation status
//-----------------------------------------------------------------------------
void CHudWeaponSelection::OnThink( void )
{
	float flSelectionTimeout = SELECTION_TIMEOUT_THRESHOLD;
	float flSelectionFadeoutTime = SELECTION_FADEOUT_TIME;
	if ( hud_fastswitch.GetBool() )
	{
		flSelectionTimeout = FASTSWITCH_DISPLAY_TIMEOUT;
		flSelectionFadeoutTime = FASTSWITCH_FADEOUT_TIME;
	}

	// Time out after awhile of inactivity
	if ( ( gpGlobals->curtime - m_flSelectionTime ) > flSelectionTimeout )
	{
		if (!m_bFadingOut)
		{
			// start fading out
			g_pClientMode->GetViewportAnimationController()->StartAnimationSequence( "FadeOutWeaponSelectionMenu" );
			m_bFadingOut = true;
		}
		else if ( gpGlobals->curtime - m_flSelectionTime > flSelectionTimeout + flSelectionFadeoutTime )
		{
			// finished fade, close
			HideSelection();
		}
	}
	else if (m_bFadingOut)
	{
		// stop us fading out, show the animation again
		g_pClientMode->GetViewportAnimationController()->StartAnimationSequence( "OpenWeaponSelectionMenu" );
		m_bFadingOut = false;
	}
}

//-----------------------------------------------------------------------------
// Purpose: returns true if the panel should draw
//-----------------------------------------------------------------------------
bool CHudWeaponSelection::ShouldDraw()
{
	C_BasePlayer *pPlayer = C_BasePlayer::GetLocalPlayer();
	if ( !pPlayer )
	{
		if ( IsInSelectionMode() )
		{
			HideSelection();
		}
		return false;
	}

	bool bret = CBaseHudWeaponSelection::ShouldDraw();
	if ( !bret )
		return false;

	// draw weapon selection a little longer if in fastswitch so we can see what we've selected
	if ( hud_fastswitch.GetBool() && ( gpGlobals->curtime - m_flSelectionTime ) < (FASTSWITCH_DISPLAY_TIMEOUT + FASTSWITCH_FADEOUT_TIME) )
		return true;

	return ( m_bSelectionVisible ) ? true : false;
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
void CHudWeaponSelection::LevelInit()
{
	CHudElement::LevelInit();

	m_iSelectedWeaponBox = -1;
	m_iSelectedSlideDir  = 0;
	m_pLastWeapon        = NULL;

	m_nBoxTextureId = surface()->CreateNewTextureID();
	surface()->DrawSetTextureFile( m_nBoxTextureId, "vgui/hud/weaponboxbig", true, false );

	m_nBoxTextureId2 = surface()->CreateNewTextureID();
	surface()->DrawSetTextureFile( m_nBoxTextureId2, "vgui/hud/weaponboxsmall", true, false );

	m_nSlot1TextureId = surface()->CreateNewTextureID();
	surface()->DrawSetTextureFile( m_nSlot1TextureId, "vgui/hud/slot1_icon", true, false );

	m_nSlot2TextureId = surface()->CreateNewTextureID();
	surface()->DrawSetTextureFile( m_nSlot2TextureId, "vgui/hud/slot2_icon", true, false );
	
	m_nSlot3TextureId = surface()->CreateNewTextureID();
	surface()->DrawSetTextureFile( m_nSlot3TextureId, "vgui/hud/slot3_icon", true, false );
	
	m_nSlot4TextureId = surface()->CreateNewTextureID();
	surface()->DrawSetTextureFile( m_nSlot4TextureId, "vgui/hud/slot4_icon", true, false );
	
	m_nSlot5TextureId = surface()->CreateNewTextureID();
	surface()->DrawSetTextureFile( m_nSlot5TextureId, "vgui/hud/slot5_icon", true, false );
}


//-----------------------------------------------------------------------------
// Purpose: starts animating the highlight for the selected weapon
//-----------------------------------------------------------------------------
void CHudWeaponSelection::ActivateWeaponHighlight( C_BaseCombatWeapon *pSelectedWeapon )
{
	C_BasePlayer *pPlayer = C_BasePlayer::GetLocalPlayer();
	if ( !pPlayer )
		return;

	// make sure all our configuration data is read
	MakeReadyForUse();

	C_BaseCombatWeapon *pWeapon = GetWeaponInSlot( m_iSelectedSlot, m_iSelectedBoxPosition );
	if ( !pWeapon )
		return;

	// start the highlight after the scroll completes
	g_pClientMode->GetViewportAnimationController()->StartAnimationSequence( "WeaponHighlight" );
}

//-------------------------------------------------------------------------
// Purpose: draws the selection area
//-------------------------------------------------------------------------
void CHudWeaponSelection::Paint()
{
	if (!ShouldDraw())
		return;

	C_BasePlayer *pPlayer = C_BasePlayer::GetLocalPlayer();
	if ( !pPlayer )
		return;

	// find and display our current selection
	C_BaseCombatWeapon *pSelectedWeapon = NULL;
	if (hud_fastswitch.GetInt())
		pSelectedWeapon = pPlayer->GetActiveWeapon();
	else
		pSelectedWeapon = GetSelectedWeapon();

	if ( !pSelectedWeapon )
		return;

	bool bPushedViewport = false;
	if(hud_fastswitch.GetInt())
	{
		CMatRenderContextPtr pRenderContext( materials );
		if( pRenderContext->GetRenderTarget() )
		{
			surface()->PushFullscreenViewport();
			bPushedViewport = true;
		}
	}

	if (hud_fastswitch.GetInt() == 0)
	{
		int xpos = 0;
		int ypos = 0;

		// draw the weapons list
		for ( int i = 0; i < MAX_WEAPON_SLOTS; i++ )
		for (int slotpos = 0, lastPos = GetLastPosInSlot(i); slotpos <= lastPos; slotpos++)
		{
			C_BaseCombatWeapon *pWeapon = GetWeaponInSlot( i, slotpos );
			if ( pWeapon )
			{
				bool bSelected = (pWeapon == pSelectedWeapon);
				DrawWeaponBox(pWeapon, bSelected, xpos, ypos);

				if (bSelected)
					ypos += m_flBoxTall * m_flSelectedScale;
				else
					ypos += m_flBoxTall;
			}
		}
	}

	if( bPushedViewport )
	{
		surface()->PopFullscreenViewport();
	}
}


//-----------------------------------------------------------------------------
// Purpose: draws a single weapon selection box
//-----------------------------------------------------------------------------
void CHudWeaponSelection::DrawWeaponBox( C_BaseCombatWeapon *pWeapon, bool bSelected, int xpos, int ypos)
{
	float boxWide = m_flBoxWide;
	float boxTall = m_flBoxTall;

	if (bSelected)
	{
		boxWide *= m_flSelectedScale;
		boxTall *= m_flSelectedScale;
	}

	int iconAlpha = bSelected ? 255 : 128;
	float iconX = xpos + m_flWepXPos * boxWide;
	float iconY = ypos + m_flWepYPos * boxTall;
	float iconWide = m_flWepXSize * boxWide;
	float iconTall = m_flWepYSize * boxTall;

	//Draw background
	surface()->DrawSetTexture( m_nBoxTextureId );
	surface()->DrawSetColor( 255, 255, 255, 255 );
	surface()->DrawTexturedRect(xpos, ypos, xpos + boxWide, ypos + boxTall);

	// draw icon
	// draw an active version over the top
	if ( pWeapon->GetSlot() + 1 == 1 )
		surface()->DrawSetTexture( m_nSlot1TextureId );

	if ( pWeapon->GetSlot() + 1 == 2 )
		surface()->DrawSetTexture( m_nSlot2TextureId );

	if ( pWeapon->GetSlot() + 1 == 3 )
		surface()->DrawSetTexture( m_nSlot3TextureId );

	if ( pWeapon->GetSlot() + 1 == 4 )
		surface()->DrawSetTexture( m_nSlot4TextureId );

	if ( pWeapon->GetSlot() + 1 == 5 )
		surface()->DrawSetTexture( m_nSlot5TextureId );

	surface()->DrawSetColor( 255, 255, 255, iconAlpha);
	surface()->DrawTexturedRect(iconX, iconY, iconX + iconWide, iconY + iconTall);
}


//-----------------------------------------------------------------------------
// Purpose: hud scheme settings
//-----------------------------------------------------------------------------
void CHudWeaponSelection::ApplySchemeSettings(vgui::IScheme *pScheme)
{
	BaseClass::ApplySchemeSettings(pScheme);
	SetPaintBackgroundEnabled(false);

	// set our size
	int screenWide, screenTall;
	int x, y;
	GetPos(x, y);
	GetHudSize(screenWide, screenTall);
	SetBounds( x, y, screenWide - x, screenTall - y );
}

//-----------------------------------------------------------------------------
// Purpose: Opens weapon selection control
//-----------------------------------------------------------------------------
void CHudWeaponSelection::OpenSelection( void )
{
	Assert(!IsInSelectionMode());

	CBaseHudWeaponSelection::OpenSelection();
	g_pClientMode->GetViewportAnimationController()->StartAnimationSequence("OpenWeaponSelectionMenu");
	m_iSelectedBoxPosition = 0;
	m_iSelectedSlot = -1;
}

//-----------------------------------------------------------------------------
// Purpose: Closes weapon selection control immediately
//-----------------------------------------------------------------------------
void CHudWeaponSelection::HideSelection( void )
{
	CBaseHudWeaponSelection::HideSelection();
	g_pClientMode->GetViewportAnimationController()->StartAnimationSequence("CloseWeaponSelectionMenu");
	m_bFadingOut = false;
}

//-----------------------------------------------------------------------------
// Purpose: Returns the next available weapon item in the weapon selection
//-----------------------------------------------------------------------------
C_BaseCombatWeapon *CHudWeaponSelection::FindNextWeaponInWeaponSelection(int iCurrentSlot, int iCurrentPosition)
{
	C_BasePlayer *pPlayer = C_BasePlayer::GetLocalPlayer();
	if ( !pPlayer )
		return NULL;

	C_BaseCombatWeapon *pNextWeapon = NULL;

	// search all the weapons looking for the closest next
	int iLowestNextSlot = MAX_WEAPON_SLOTS;
	int iLowestNextPosition = MAX_WEAPON_POSITIONS;
	for ( int i = 0; i < MAX_WEAPONS; i++ )
	{
		C_BaseCombatWeapon *pWeapon = pPlayer->GetWeapon(i);
		if ( !pWeapon )
			continue;

		if ( CanBeSelectedInHUD( pWeapon ) )
		{
			int weaponSlot = pWeapon->GetSlot(), weaponPosition = pWeapon->GetPosition();

			// see if this weapon is further ahead in the selection list
			if ( weaponSlot > iCurrentSlot || (weaponSlot == iCurrentSlot && weaponPosition > iCurrentPosition) )
			{
				// see if this weapon is closer than the current lowest
				if ( weaponSlot < iLowestNextSlot || (weaponSlot == iLowestNextSlot && weaponPosition < iLowestNextPosition) )
				{
					iLowestNextSlot = weaponSlot;
					iLowestNextPosition = weaponPosition;
					pNextWeapon = pWeapon;
				}
			}
		}
	}

	return pNextWeapon;
}

//-----------------------------------------------------------------------------
// Purpose: Returns the prior available weapon item in the weapon selection
//-----------------------------------------------------------------------------
C_BaseCombatWeapon *CHudWeaponSelection::FindPrevWeaponInWeaponSelection(int iCurrentSlot, int iCurrentPosition)
{
	C_BasePlayer *pPlayer = C_BasePlayer::GetLocalPlayer();
	if ( !pPlayer )
		return NULL;

	C_BaseCombatWeapon *pPrevWeapon = NULL;

	// search all the weapons looking for the closest next
	int iLowestPrevSlot = -1;
	int iLowestPrevPosition = -1;
	for ( int i = 0; i < MAX_WEAPONS; i++ )
	{
		C_BaseCombatWeapon *pWeapon = pPlayer->GetWeapon(i);
		if ( !pWeapon )
			continue;

		if ( CanBeSelectedInHUD( pWeapon ) )
		{
			int weaponSlot = pWeapon->GetSlot(), weaponPosition = pWeapon->GetPosition();

			// see if this weapon is further ahead in the selection list
			if ( weaponSlot < iCurrentSlot || (weaponSlot == iCurrentSlot && weaponPosition < iCurrentPosition) )
			{
				// see if this weapon is closer than the current lowest
				if ( weaponSlot > iLowestPrevSlot || (weaponSlot == iLowestPrevSlot && weaponPosition > iLowestPrevPosition) )
				{
					iLowestPrevSlot = weaponSlot;
					iLowestPrevPosition = weaponPosition;
					pPrevWeapon = pWeapon;
				}
			}
		}
	}

	return pPrevWeapon;
}

//-----------------------------------------------------------------------------
// Purpose: Moves the selection to the next item in the menu
//-----------------------------------------------------------------------------
void CHudWeaponSelection::CycleToNextWeapon( void )
{
	// Get the local player.
	C_BasePlayer *pPlayer = C_BasePlayer::GetLocalPlayer();
	if ( !pPlayer )
		return;

	m_pLastWeapon = pPlayer->GetActiveWeapon();

	C_BaseCombatWeapon *pNextWeapon = NULL;
	if ( IsInSelectionMode() )
	{
		// find the next selection spot
		C_BaseCombatWeapon *pWeapon = GetSelectedWeapon();
		if ( !pWeapon )
			return;

		pNextWeapon = FindNextWeaponInWeaponSelection( pWeapon->GetSlot(), pWeapon->GetPosition() );
	}
	else
	{
		// open selection at the current place
		pNextWeapon = pPlayer->GetActiveWeapon();
		if ( pNextWeapon )
		{
			pNextWeapon = FindNextWeaponInWeaponSelection( pNextWeapon->GetSlot(), pNextWeapon->GetPosition() );
		}
	}

	if ( !pNextWeapon )
	{
		// wrap around back to start
		pNextWeapon = FindNextWeaponInWeaponSelection(-1, -1);
	}

	if ( pNextWeapon )
	{
		SetSelectedWeapon( pNextWeapon );
		SetSelectedSlideDir( 1 );

		if ( !IsInSelectionMode() )
		{
			OpenSelection();
		}

		// Play the "cycle to next weapon" sound
		pPlayer->EmitSound( "Player.WeaponSelectionMoveSlot" );
	}
}

//-----------------------------------------------------------------------------
// Purpose: Moves the selection to the previous item in the menu
//-----------------------------------------------------------------------------
void CHudWeaponSelection::CycleToPrevWeapon( void )
{
	// Get the local player.
	C_BasePlayer *pPlayer = C_BasePlayer::GetLocalPlayer();
	if ( !pPlayer )
		return;

	m_pLastWeapon = pPlayer->GetActiveWeapon();

	C_BaseCombatWeapon *pNextWeapon = NULL;
	if ( IsInSelectionMode() )
	{
		// find the next selection spot
		C_BaseCombatWeapon *pWeapon = GetSelectedWeapon();
		if ( !pWeapon )
			return;

		pNextWeapon = FindPrevWeaponInWeaponSelection( pWeapon->GetSlot(), pWeapon->GetPosition() );
	}
	else
	{
		// open selection at the current place
		pNextWeapon = pPlayer->GetActiveWeapon();
		if ( pNextWeapon )
		{
			pNextWeapon = FindPrevWeaponInWeaponSelection( pNextWeapon->GetSlot(), pNextWeapon->GetPosition() );
		}
	}

	if ( !pNextWeapon )
	{
		// wrap around back to end of weapon list
		pNextWeapon = FindPrevWeaponInWeaponSelection(MAX_WEAPON_SLOTS, MAX_WEAPON_POSITIONS);
	}

	if ( pNextWeapon )
	{
		SetSelectedWeapon( pNextWeapon );
		SetSelectedSlideDir( -1 );

		if ( !IsInSelectionMode() )
		{
			OpenSelection();
		}

		// Play the "cycle to next weapon" sound
		pPlayer->EmitSound( "Player.WeaponSelectionMoveSlot" );
	}
}

//-----------------------------------------------------------------------------
// Purpose: returns the # of the last weapon in the specified slot
//-----------------------------------------------------------------------------
int CHudWeaponSelection::GetLastPosInSlot( int iSlot ) const
{
	C_BasePlayer *player = C_BasePlayer::GetLocalPlayer();
	int iMaxSlotPos;

	if ( !player )
		return -1;

	iMaxSlotPos = -1;
	for ( int i = 0; i < MAX_WEAPONS; i++ )
	{
		C_BaseCombatWeapon *pWeapon = player->GetWeapon(i);
		
		if ( pWeapon == NULL )
			continue;

		if ( pWeapon->GetSlot() == iSlot && pWeapon->GetPosition() > iMaxSlotPos )
			iMaxSlotPos = pWeapon->GetPosition();
	}

	return iMaxSlotPos;
}

//-----------------------------------------------------------------------------
// Purpose: returns the weapon in the specified slot
//-----------------------------------------------------------------------------
C_BaseCombatWeapon *CHudWeaponSelection::GetWeaponInSlot( int iSlot, int iSlotPos )
{
	C_BasePlayer *player = C_BasePlayer::GetLocalPlayer();
	if ( !player )
		return NULL;

	for ( int i = 0; i < MAX_WEAPONS; i++ )
	{
		C_BaseCombatWeapon *pWeapon = player->GetWeapon(i);

		if ( pWeapon == NULL )
			continue;

		if ( pWeapon->GetSlot() == iSlot && pWeapon->GetPosition() == iSlotPos )
			return pWeapon;
	}

	return NULL;
}

//-----------------------------------------------------------------------------
// Purpose: Opens the next weapon in the slot
//-----------------------------------------------------------------------------
void CHudWeaponSelection::FastWeaponSwitch( int iWeaponSlot )
{
	// get the slot the player's weapon is in
	C_BasePlayer *pPlayer = C_BasePlayer::GetLocalPlayer();
	if ( !pPlayer )
		return;

	m_pLastWeapon = NULL;

	// see where we should start selection
	int iPosition = -1;
	C_BaseCombatWeapon *pActiveWeapon = pPlayer->GetActiveWeapon();
	if ( pActiveWeapon && pActiveWeapon->GetSlot() == iWeaponSlot )
	{
		// start after this weapon
		iPosition = pActiveWeapon->GetPosition();
	}

	C_BaseCombatWeapon *pNextWeapon = NULL;

	// search for the weapon after the current one
	pNextWeapon = FindNextWeaponInWeaponSelection(iWeaponSlot, iPosition);
	// make sure it's in the same bucket
	if ( !pNextWeapon || pNextWeapon->GetSlot() != iWeaponSlot )
	{
		// just look for any weapon in this slot
		pNextWeapon = FindNextWeaponInWeaponSelection(iWeaponSlot, -1);
	}

	// see if we found a weapon that's different from the current and in the selected slot
	if ( pNextWeapon && pNextWeapon != pActiveWeapon && pNextWeapon->GetSlot() == iWeaponSlot )
	{
		// select the new weapon
		::input->MakeWeaponSelection( pNextWeapon );
	}
	else if ( pNextWeapon != pActiveWeapon )
	{
		// error sound
		pPlayer->EmitSound( "Player.DenyWeaponSelection" );
	}

	// kill any fastswitch display
	m_flSelectionTime = 0.0f;
}

//-----------------------------------------------------------------------------
// Purpose: Opens the next weapon in the slot
//-----------------------------------------------------------------------------
void CHudWeaponSelection::PlusTypeFastWeaponSwitch( int iWeaponSlot )
{
	// get the slot the player's weapon is in
	C_BasePlayer *pPlayer = C_BasePlayer::GetLocalPlayer();
	if ( !pPlayer )
		return;

	m_pLastWeapon = NULL;
	int newSlot = m_iSelectedSlot;

	// Changing slot number does not necessarily mean we need to change the slot - the player could be
	// scrolling through the same slot but in the opposite direction. Slot pairs are 0,2 and 1,3 - so
	// compare the 0 bits to see if we're within a pair. Otherwise, reset the box to the zero position.
	if ( -1 == m_iSelectedSlot || ( ( m_iSelectedSlot ^ iWeaponSlot ) & 1 ) )
	{
		// Changing vertical/horizontal direction. Reset the selected box position to zero.
		m_iSelectedBoxPosition = 0;
		m_iSelectedSlot = iWeaponSlot;
	}
	else
	{
		// Still in the same horizontal/vertical direction. Determine which way we're moving in the slot.
		int increment = 1;
		if ( m_iSelectedSlot != iWeaponSlot )
		{
			// Decrementing within the slot. If we're at the zero position in this slot, 
			// jump to the zero position of the opposite slot. This also counts as our increment.
			increment = -1;
			if ( 0 == m_iSelectedBoxPosition )
			{
				newSlot = ( m_iSelectedSlot + 2 ) % 4;
				increment = 0;
			}
		}

		// Find out of the box position is at the end of the slot
		int lastSlotPos = -1;
		for ( int slotPos = 0; slotPos < MAX_WEAPON_POSITIONS; ++slotPos )
		{
			C_BaseCombatWeapon *pWeapon = GetWeaponInSlot( newSlot, slotPos );
			if ( pWeapon )
			{
				lastSlotPos = slotPos;
			}
		}

		// Increment/Decrement the selected box position
		if ( m_iSelectedBoxPosition + increment <= lastSlotPos )
		{
			m_iSelectedBoxPosition += increment;
			m_iSelectedSlot = newSlot;
		}
		else
		{
			// error sound
			pPlayer->EmitSound( "Player.DenyWeaponSelection" );
			return;
		}
	}

	// Select the weapon in this position
	bool bWeaponSelected = false;
	C_BaseCombatWeapon *pActiveWeapon = pPlayer->GetActiveWeapon();
	C_BaseCombatWeapon *pWeapon = GetWeaponInSlot( m_iSelectedSlot, m_iSelectedBoxPosition );
	if ( pWeapon )
	{
		if ( pWeapon != pActiveWeapon )
		{
			// Select the new weapon
			::input->MakeWeaponSelection( pWeapon );
			SetSelectedWeapon( pWeapon );
			bWeaponSelected = true;
		}
	}

	if ( !bWeaponSelected )
	{
		// Still need to set this to make hud display appear
		SetSelectedWeapon( pPlayer->GetActiveWeapon() );
	}
}

//-----------------------------------------------------------------------------
// Purpose: Moves selection to the specified slot
//-----------------------------------------------------------------------------
void CHudWeaponSelection::SelectWeaponSlot( int iSlot )
{
	// iSlot is one higher than it should be, since it's the number key, not the 0-based index into the weapons
	--iSlot;

	// Get the local player.
	C_BasePlayer *pPlayer = C_BasePlayer::GetLocalPlayer();
	if ( !pPlayer )
		return;

	// Don't try and read past our possible number of slots
	if ( iSlot >= MAX_WEAPON_SLOTS )
		return;
	
	// Make sure the player's allowed to switch weapons
	if ( pPlayer->IsAllowedToSwitchWeapons() == false )
		return;

	if (hud_fastswitch.GetInt())
	{
		FastWeaponSwitch( iSlot );
		return;
	}
	else
	{
		int slotPos = 0;
		C_BaseCombatWeapon *pActiveWeapon = GetSelectedWeapon();

		// start later in the list
		if ( IsInSelectionMode() && pActiveWeapon && pActiveWeapon->GetSlot() == iSlot )
		{
			slotPos = pActiveWeapon->GetPosition() + 1;
		}

		// find the weapon in this slot
		pActiveWeapon = GetNextActivePos( iSlot, slotPos );
		if ( !pActiveWeapon )
		{
			pActiveWeapon = GetNextActivePos( iSlot, 0 );
		}
		
		if ( pActiveWeapon != NULL )
		{
			if ( !IsInSelectionMode() )
			{
				// open the weapon selection
				OpenSelection();
			}

			// Mark the change
			SetSelectedWeapon( pActiveWeapon );
			SetSelectedSlideDir( 0 );
		}
	}

	pPlayer->EmitSound( "Player.WeaponSelectionMoveSlot" );
}
