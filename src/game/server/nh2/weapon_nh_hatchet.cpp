//========= Copyright Valve Corporation, All rights reserved. ============//
//
// Purpose: NH Hatchet
//
// $NoKeywords: $
//=============================================================================//

#include "cbase.h"
#include "basehlcombatweapon.h"
#include "player.h"
#include "gamerules.h"
#include "ammodef.h"
#include "mathlib/mathlib.h"
#include "in_buttons.h"
#include "soundent.h"
#include "basebludgeonweapon.h"
#include "vstdlib/random.h"
#include "npcevent.h"
#include "ai_basenpc.h"
#include "weapon_nh_hatchet.h"

// memdbgon must be the last include file in a .cpp file!!!
#include "tier0/memdbgon.h"

ConVar    sk_plr_dmg_hatchet		( "sk_plr_dmg_hatchet","0");
ConVar    sk_npc_dmg_hatchet		( "sk_npc_dmg_hatchet","0");
ConVar sk_weapon_hatchet_primary_delay("sk_weapon_hatchet_primary_delay","0.2");
ConVar sk_weapon_hatchet_secondary_delay("sk_weapon_hatchet_secondary_delay","0.95");

//-----------------------------------------------------------------------------
// CWeaponNHHatchet
//-----------------------------------------------------------------------------
IMPLEMENT_SERVERCLASS_ST(CWeaponNHHatchet, DT_WeaponNHHatchet)
END_SEND_TABLE()

LINK_ENTITY_TO_CLASS( weapon_nh_hatchet, CWeaponNHHatchet );
PRECACHE_WEAPON_REGISTER( weapon_nh_hatchet );

acttable_t CWeaponNHHatchet::m_acttable[] = 
{
	{ ACT_MELEE_ATTACK1,	ACT_MELEE_ATTACK_SWING, true },
	{ ACT_IDLE,				ACT_IDLE_ANGRY_MELEE,	false },
	{ ACT_IDLE_ANGRY,		ACT_IDLE_ANGRY_MELEE,	false },
};

IMPLEMENT_ACTTABLE(CWeaponNHHatchet);

//-----------------------------------------------------------------------------
// Constructor
//-----------------------------------------------------------------------------
CWeaponNHHatchet::CWeaponNHHatchet( void )
{
	m_bIsSwinging = false;
}

//-----------------------------------------------------------------------------
// Purpose: Get the damage amount for the animation we're doing
// Input  : hitActivity - currently played activity
// Output : Damage amount
//-----------------------------------------------------------------------------
float CWeaponNHHatchet::GetDamageForActivity( Activity hitActivity )
{
	float dmg;
	if ( ( GetOwner() != NULL ) && ( GetOwner()->IsPlayer() ) )
		dmg = sk_plr_dmg_hatchet.GetFloat();
	else
		dmg = sk_npc_dmg_hatchet.GetFloat();

	if(hitActivity == ACT_VM_HITCENTER2 || hitActivity == ACT_VM_MISSCENTER)
		dmg *= 3.0;

	return dmg;
}

//-----------------------------------------------------------------------------
// Purpose: Add in a view kick for this weapon
//-----------------------------------------------------------------------------
void CWeaponNHHatchet::AddViewKick( void )
{
	CBasePlayer *pPlayer  = ToBasePlayer( GetOwner() );
	
	if ( pPlayer == NULL )
		return;

	QAngle punchAng;

	punchAng.x = random->RandomFloat( 1.0f, 2.0f );
	punchAng.y = random->RandomFloat( -2.0f, -1.0f );
	punchAng.z = 0.0f;
	
	pPlayer->ViewPunch( punchAng ); 
}

void CWeaponNHHatchet::PrimaryAttack()
{
	if(m_bIsSwinging)
		return;
	m_flSwingTime = gpGlobals->curtime + sk_weapon_hatchet_primary_delay.GetFloat();
	m_bIsSwinging = true;
	m_iSwingType = 1;
	SendWeaponAnim(random->RandomInt(1,2) == 1 ? ACT_VM_HITCENTER : ACT_VM_MISSCENTER);
}

void CWeaponNHHatchet::SecondaryAttack()
{
	if(m_bIsSwinging)
		return;
	m_flSwingTime = gpGlobals->curtime + sk_weapon_hatchet_secondary_delay.GetFloat();
	m_bIsSwinging = true;
	m_iSwingType = 2;
	SendWeaponAnim(ACT_VM_MISSCENTER2);
}

void CWeaponNHHatchet::ItemPostFrame()
{
	if(m_bIsSwinging && gpGlobals->curtime > m_flSwingTime)
	{
		m_bIsSwinging = false;
		//Do the actual attack
		if(m_iSwingType == 1)
			BaseClass::PrimaryAttack();
		else if (m_iSwingType == 2)
			BaseClass::SecondaryAttack();
	}

	BaseClass::ItemPostFrame();
}
