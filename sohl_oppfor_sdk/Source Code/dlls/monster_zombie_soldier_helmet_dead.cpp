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
//=========================================================
// PROP: DEAD Soldier Helmet Zombie * http://half-life.wikia.com/wiki/Standard_Zombie
//=========================================================

#include	"extdll.h"
#include	"util.h"
#include	"cbase.h"
#include    "monster_zombie_soldier_helmet_dead.h"

//=========================================================
// Monster's link to Class
//=========================================================
LINK_ENTITY_TO_CLASS(monster_zombie_soldier_helmet_dead, CDeadZombieSoldierHelmet);

//=========================================================
// Spawn DEAD-Soldier Helmet Zombie
//=========================================================
void CDeadZombieSoldierHelmet::Spawn() {
	Precache();

	if (pev->model)
		SET_MODEL(ENT(pev), STRING(pev->model)); //LRC
	else
		SET_MODEL(ENT(pev), "models/zombie_soldier_helmet.mdl");

	pev->sequence = LookupSequence(m_szPoses[m_iPose]);
	if (pev->sequence == -1) {
		ALERT(at_console, "Dead zombie soldier helmet with bad pose\n");
		pev->sequence = 0;
		pev->effects = EF_BRIGHTFIELD;
	}

	CDeadZombieSoldier::Spawn();
}

//=========================================================
// Precache - precaches all resources this monster needs
//=========================================================
void CDeadZombieSoldierHelmet::Precache() {
	if (pev->model)
		PRECACHE_MODEL((char*)STRING(pev->model)); //LRC
	else
		PRECACHE_MODEL("models/zombie_soldier_helmet.mdl");

	CDeadZombieSoldier::Precache();
}
