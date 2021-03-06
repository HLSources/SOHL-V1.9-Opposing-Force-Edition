/***
*
*   SPIRIT OF HALF-LIFE 1.9: OPPOSING-FORCE EDITION
*
*   Half-Life and their logos are the property of their respective owners.
*   Copyright (c) 1996-2002, Valve LLC. All rights reserved.
*
*   This product contains software technology licensed from Id
*   Software, Inc. ("Id Technology").  Id Technology (c) 1996 Id Software, Inc.
*
*   Use, distribution, and modification of this source code and/or resulting
*   object code is restricted to non-commercial enhancements to products from
*   Valve LLC.  All other use, distribution, or modification is prohibited
*   without written permission from Valve LLC.
*
*	Spirit of Half-Life, by Laurie R. Cheers. (LRC)
*   Modified by Lucas Brucksch (Code merge & Effects)
*   Modified by Andrew J Hamilton (AJH)
*   Modified by XashXT Group (g-cont...)
*
*   Code used from Battle Grounds Team and Contributors.
*   Code used from SamVanheer (Opposing Force code)
*   Code used from FWGS Team (Fixes for SOHL)
*   Code used from LevShisterov (Bugfixed and improved HLSDK)
*	Code used from Fograin (Half-Life: Update MOD)
*
***/

// CBaseSpectator

// YWB:  UNDONE

// Spectator functions
// 
#include	"extdll.h"
#include	"util.h"
#include	"cbase.h"
#include	"spectator.h"

/*
===========
SpectatorConnect

called when a spectator connects to a server
============
*/
void CBaseSpectator::SpectatorConnect()
{
	pev->flags = FL_SPECTATOR;
	pev->solid = SOLID_NOT;
	pev->movetype = MOVETYPE_NOCLIP;

	m_pGoalEnt = NULL;
}

/*
===========
SpectatorDisconnect

called when a spectator disconnects from a server
============
*/
void CBaseSpectator::SpectatorDisconnect()
{
}

/*
================
SpectatorImpulseCommand

Called by SpectatorThink if the spectator entered an impulse
================
*/
void CBaseSpectator::SpectatorImpulseCommand()
{
	static edict_t	*pGoal = NULL;
	CBaseEntity		*pPreviousGoal;
	CBaseEntity		*pCurrentGoal;
	BOOL			bFound;

	switch (pev->impulse)
	{
	case 1:
		// teleport the spectator to the next spawn point; note that if the spectator is
		// tracking, this doesn't do much
		pPreviousGoal = (CBaseEntity*)GET_PRIVATE(pGoal);
		pCurrentGoal = (CBaseEntity*)GET_PRIVATE(pGoal);
		// Start at the current goal, skip the world, and stop if we looped back around

		bFound = FALSE;
		while (1)
		{
			pCurrentGoal = UTIL_FindEntityByClassname(pCurrentGoal, "info_player_deathmatch");
			// Looped around, failure
			if (pCurrentGoal == pPreviousGoal)
			{
				ALERT(at_debug, "Could not find a spawn spot.\n");
				break;
			}
			// Found a non-world entity, set success, otherwise, look for the next one.
			if (pCurrentGoal)
			{
				bFound = TRUE;
				break;
			}
		}

		if (!bFound)  // Didn't find a good spot.
			break;

		pGoal = ENT(pCurrentGoal->pev);
		UTIL_SetOrigin(this, pGoal->v.origin);
		pev->angles = pGoal->v.angles;
		pev->fixangle = FALSE;
		break;
	default:
		ALERT(at_debug, "Unknown spectator impulse\n");
		break;
	}

	pev->impulse = 0;
}

/*
================
SpectatorThink

Called every frame after physics are run
================
*/
void  CBaseSpectator::SpectatorThink()
{
	if (!(pev->flags & FL_SPECTATOR))
	{
		pev->flags = FL_SPECTATOR;
	}

	pev->solid = SOLID_NOT;
	pev->movetype = MOVETYPE_NOCLIP;

	if (pev->impulse)
		SpectatorImpulseCommand();
}

/*
===========
Spawn

  Called when spectator is initialized:
  UNDONE:  Is this actually being called because spectators are not allocated in normal fashion?
============
*/
void CBaseSpectator::Spawn()
{
	pev->flags = FL_SPECTATOR;
	pev->solid = SOLID_NOT;
	pev->movetype = MOVETYPE_NOCLIP;

	m_pGoalEnt = NULL;
}
