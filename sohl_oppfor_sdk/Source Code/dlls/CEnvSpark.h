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

#ifndef CENVSPARK_H
#define CENVSPARK_H

class CEnvSpark : public CBaseEntity
{
public:
	void	Spawn();
	void	Precache();
	void	EXPORT SparkThink();
	void	EXPORT SparkWait();
	void	EXPORT SparkCyclic(CBaseEntity *pActivator, CBaseEntity *pCaller, USE_TYPE useType, float value);
	void	EXPORT SparkStart(CBaseEntity *pActivator, CBaseEntity *pCaller, USE_TYPE useType, float value);
	void	EXPORT SparkStop(CBaseEntity *pActivator, CBaseEntity *pCaller, USE_TYPE useType, float value);
	void	KeyValue(KeyValueData *pkvd);
	void	DoSpark(entvars_t *pev, const Vector &location);

	virtual int		Save(CSave &save);
	virtual int		Restore(CRestore &restore);

	static	TYPEDESCRIPTION m_SaveData[];

	float	m_flDelay;
	STATE	m_iState; //LRC
	virtual STATE	GetState() { return m_iState; };
};
#endif // CENVSPARK_H
