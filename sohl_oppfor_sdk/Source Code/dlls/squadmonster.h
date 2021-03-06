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
// CSquadMonster - all the extra data for monsters that
// form squads.
//=========================================================

#define bits_NO_SLOT		0

// HUMAN GRUNT SLOTS
#define bits_SLOT_HGRUNT_ENGAGE1	( 1 << 0 )
#define bits_SLOT_HGRUNT_ENGAGE2	( 1 << 1 )
#define bits_SLOTS_HGRUNT_ENGAGE	( bits_SLOT_HGRUNT_ENGAGE1 | bits_SLOT_HGRUNT_ENGAGE2 )

#define bits_SLOT_HGRUNT_GRENADE1	( 1 << 2 ) 
#define bits_SLOT_HGRUNT_GRENADE2	( 1 << 3 ) 
#define bits_SLOTS_HGRUNT_GRENADE	( bits_SLOT_HGRUNT_GRENADE1 | bits_SLOT_HGRUNT_GRENADE2 )

// ALIEN GRUNT SLOTS
#define bits_SLOT_AGRUNT_HORNET1	( 1 << 4 )
#define bits_SLOT_AGRUNT_HORNET2	( 1 << 5 )
#define bits_SLOT_AGRUNT_CHASE		( 1 << 6 )
#define bits_SLOTS_AGRUNT_HORNET	( bits_SLOT_AGRUNT_HORNET1 | bits_SLOT_AGRUNT_HORNET2 )

// HOUNDEYE SLOTS
#define bits_SLOT_HOUND_ATTACK1		( 1 << 7 )
#define bits_SLOT_HOUND_ATTACK2		( 1 << 8 )
#define bits_SLOT_HOUND_ATTACK3		( 1 << 9 )
#define bits_SLOTS_HOUND_ATTACK		( bits_SLOT_HOUND_ATTACK1 | bits_SLOT_HOUND_ATTACK2 | bits_SLOT_HOUND_ATTACK3 )

// global slots
#define bits_SLOT_SQUAD_SPLIT		( 1 << 10 )// squad members don't all have the same enemy

#define NUM_SLOTS			11// update this every time you add/remove a slot.

#define	MAX_SQUAD_MEMBERS	5

//=========================================================
// CSquadMonster - for any monster that forms squads.
//=========================================================
class CSquadMonster : public CBaseMonster
{
public:
	// squad leader info
	EHANDLE	m_hSquadLeader;		// who is my leader
	EHANDLE	m_hSquadMember[MAX_SQUAD_MEMBERS - 1];	// valid only for leader
	int		m_afSquadSlots;
	float	m_flLastEnemySightTime; // last time anyone in the squad saw the enemy
	BOOL	m_fEnemyEluded;

	// squad member info
	int		m_iMySlot;// this is the behaviour slot that the monster currently holds in the squad. 

	int  CheckEnemy(CBaseEntity *pEnemy);
	void StartMonster();
	void VacateSlot();
	void ScheduleChange();
	void Killed(entvars_t *pevAttacker, int iGib);
	BOOL OccupySlot(int iDesiredSlot);
	BOOL NoFriendlyFire();
	BOOL NoFriendlyFire(BOOL playerAlly);

	// squad functions still left in base class
	CSquadMonster *MySquadLeader()
	{
		CSquadMonster *pSquadLeader = (CSquadMonster *)((CBaseEntity *)m_hSquadLeader);
		if (pSquadLeader != NULL)
			return pSquadLeader;
		return this;
	}
	CSquadMonster *MySquadMember(int i)
	{
		if (i >= MAX_SQUAD_MEMBERS - 1)
			return this;
		else
			return (CSquadMonster *)((CBaseEntity *)m_hSquadMember[i]);
	}
	int	InSquad() { return m_hSquadLeader != NULL; }
	int IsLeader() { return m_hSquadLeader == this; }
	int SquadRecruit(int searchRadius, int maxMembers);
	int	SquadCount();
	void SquadRemove(CSquadMonster *pRemove);
	BOOL SquadAdd(CSquadMonster *pAdd);
	void SquadMakeEnemy(CBaseEntity *pEnemy);
	void SquadPasteEnemyInfo();
	void SquadCopyEnemyInfo();
	BOOL SquadEnemySplit();
	BOOL SquadMemberInRange(const Vector &vecLocation, float flDist);

	virtual CSquadMonster *MySquadMonsterPointer() { return this; }

	static TYPEDESCRIPTION m_SaveData[];

	int	Save(CSave &save);
	int Restore(CRestore &restore);

	BOOL FValidateCover(const Vector &vecCoverLocation);

	MONSTERSTATE GetIdealState();
	Schedule_t	*GetScheduleOfType(int iType);
};

