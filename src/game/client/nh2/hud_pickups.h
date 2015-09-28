//========= Copyright Valve Corporation, All rights reserved. ============//
//
// Purpose: Item pickup history displayed onscreen when items are picked up.
//
// $NoKeywords: $
//=============================================================================//

#ifndef PICKUPS_H
#define PICKUPS_H
#pragma once

#include "hudelement.h"
#include "ehandle.h"

#include <vgui_controls/Panel.h>

class C_BaseCombatWeapon;

//-----------------------------------------------------------------------------
// Purpose: Used to draw the history of ammo / weapon / item pickups by the player
//-----------------------------------------------------------------------------
class CHudPickups : public CHudElement, public vgui::Panel
{
	DECLARE_CLASS_SIMPLE( CHudPickups, vgui::Panel );

public:
	CHudPickups( const char *pElementName );

	// CHudElement overrides
	virtual void Init( void );
	virtual void Reset( void );
	virtual void Paint( void );

	void AddToHistory( C_BaseCombatWeapon *weapon );
	void MsgFunc_ItemPickup( bf_read &msg );
};

#endif // PICKUPS_H
