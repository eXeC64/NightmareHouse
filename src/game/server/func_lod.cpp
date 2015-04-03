//========= Copyright Valve Corporation, All rights reserved. ============//
//
// Purpose: 
//
// $NoKeywords: $
//
//=============================================================================//

#include "cbase.h"

// memdbgon must be the last include file in a .cpp file!!!
#include "tier0/memdbgon.h"

class CFunc_LOD : public CBaseEntity
{
	DECLARE_DATADESC();
	DECLARE_CLASS( CFunc_LOD, CBaseEntity );
public:
	DECLARE_SERVERCLASS();

					CFunc_LOD();
	virtual 		~CFunc_LOD();


	// When the viewer is between:
	// (0 and m_fNonintrusiveDist):					the bmodel is forced to be visible
	// (m_fNonintrusiveDist and m_fDisappearDist):	the bmodel is trying to appear or disappear nonintrusively
	//												(waits until it's out of the view frustrum or until there's a lot of motion)
	// (m_fDisappearDist+):							the bmodel is forced to be invisible
	CNetworkVar( float, m_fDisappearDist );
	CNetworkVar( bool, m_bInvertAlpha );

// CBaseEntity overrides.
public:

	virtual void	Spawn();
	bool			CreateVPhysics();
	virtual void	Activate();
	virtual bool	KeyValue( const char *szKeyName, const char *szValue );
};


IMPLEMENT_SERVERCLASS_ST(CFunc_LOD, DT_Func_LOD)
	SendPropFloat(SENDINFO(m_fDisappearDist), 0, SPROP_NOSCALE),
	SendPropBool(SENDINFO(m_bInvertAlpha)),
END_SEND_TABLE()


LINK_ENTITY_TO_CLASS(func_lod, CFunc_LOD);


//---------------------------------------------------------
// Save/Restore
//---------------------------------------------------------
BEGIN_DATADESC( CFunc_LOD )

	DEFINE_FIELD( m_fDisappearDist,	FIELD_FLOAT ),
	DEFINE_FIELD( m_bInvertAlpha,	FIELD_BOOLEAN ),

END_DATADESC()


// ------------------------------------------------------------------------------------- //
// CFunc_LOD implementation.
// ------------------------------------------------------------------------------------- //
CFunc_LOD::CFunc_LOD()
{
}


CFunc_LOD::~CFunc_LOD()
{
}


void CFunc_LOD::Spawn()
{
	// Bind to our bmodel.
	SetModel( STRING( GetModelName() ) );
	SetSolid( SOLID_BSP );
	BaseClass::Spawn();

	CreateVPhysics();
}

bool CFunc_LOD::CreateVPhysics()
{
	VPhysicsInitStatic();
	return true;
}

void CFunc_LOD::Activate()
{
	BaseClass::Activate();
}


bool CFunc_LOD::KeyValue( const char *szKeyName, const char *szValue )
{
	if (FStrEq(szKeyName, "DisappearDist"))
	{
		m_fDisappearDist = (float)atof(szValue);
	}
	else if (FStrEq(szKeyName, "Solid"))
	{
		if (atoi(szValue) != 0)
		{
			AddSolidFlags( FSOLID_NOT_SOLID );
		}
	}
	else if (FStrEq(szKeyName, "InvertAlpha"))
	{
		if (atoi(szValue) != 0)
			m_bInvertAlpha = true;
		else
			m_bInvertAlpha = false;
	}
	else
	{
		return BaseClass::KeyValue(szKeyName, szValue);
	}

	return true;
}
			  
