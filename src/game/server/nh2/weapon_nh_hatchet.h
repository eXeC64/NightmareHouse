//========= Copyright Valve Corporation, All rights reserved. ============//
//
// Purpose:
//
//=============================================================================//

#ifndef WEAPON_NH_HATCHET_H
#define WEAPON_NH_HATCHET_H

#include "basebludgeonweapon.h"

#if defined( _WIN32 )
#pragma once
#endif

#define	HATCHET_RANGE	75.0f
#define	HATCHET_REFIRE	0.4f

//-----------------------------------------------------------------------------
// CWeaponNHHatchet
//-----------------------------------------------------------------------------

class CWeaponNHHatchet : public CBaseHLBludgeonWeapon
{
public:
	DECLARE_CLASS( CWeaponNHHatchet, CBaseHLBludgeonWeapon );

	DECLARE_SERVERCLASS();
	DECLARE_ACTTABLE();

	CWeaponNHHatchet();

	float		GetRange( void )		{	return	HATCHET_RANGE;	}
	float		GetFireRate( void )		{	return	HATCHET_REFIRE;	}

	void		AddViewKick( void );
	float		GetDamageForActivity( Activity hitActivity );

	// Animation event
	virtual void Operator_HandleAnimEvent( animevent_t *pEvent, CBaseCombatCharacter *pOperator );

private:
	// Animation event handlers
	void HandleAnimEventMeleeHit( animevent_t *pEvent, CBaseCombatCharacter *pOperator );
};

#endif // WEAPON_NH_HATCHET_H
