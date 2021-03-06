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
// Weapon: Shotgun * http://half-life.wikia.com/wiki/HECU_SPAS-12
// For Spirit of Half-Life v1.9: Opposing-Force Edition
//=========================================================

#ifndef WEAPON_SHOTGUN_H
#define WEAPON_SHOTGUN_H

// special deathmatch shotgun spreads
#define VECTOR_CONE_DM_SHOTGUN			Vector( 0.08716, 0.04362, 0.00  )// 10 degrees by 5 degrees
#define VECTOR_CONE_DM_DOUBLESHOTGUN	Vector( 0.17365, 0.04362, 0.00 ) // 20 degrees by 5 degrees

//Model Animations | Sequence-ID | Frames | FPS
enum class SHOTGUN_IDLE1 { sequence = 0, frames = 21, fps = 9 };
enum class SHOTGUN_IDLE2 { sequence = 1, frames = 21, fps = 12 };
enum class SHOTGUN_IDLE3 { sequence = 2, frames = 21, fps = 9 };
enum class SHOTGUN_DRAW { sequence = 3, frames = 13, fps = 24 };
enum class SHOTGUN_HOLSTER { sequence = 4, frames = 13, fps = 30 };
enum class SHOTGUN_SHOOT { sequence = 5, frames = 31, fps = 30 };
enum class SHOTGUN_SHOOT_BIG { sequence = 6, frames = 47, fps = 30 };
enum class SHOTGUN_START_RELOAD { sequence = 7, frames = 9, fps = 12 };
enum class SHOTGUN_RELOAD { sequence = 8, frames = 10, fps = 16 };
enum class SHOTGUN_PUMP { sequence = 9, frames = 20, fps = 30 };

#ifndef CLIENT_DLL //Only in Server-DLL
//Shotgun Base-Class | Base | Attack | Animations | Vars | Events
class CShotgun : public CBasePlayerWeapon {
public:
	//Base
	void Spawn();
	void Precache();
	int GetItemInfo(ItemInfo *p);

	//Attack
	void PrimaryAttack();
	void SecondaryAttack();

	//Animations
	BOOL Deploy();
	void Holster();
	void Reload();
	void Charge(bool m_BeginAttack);
	void WeaponIdle();
private:
	//Events
	unsigned short m_usShotgunFire;
};
#endif

#endif // WEAPON_SHOTGUN_H