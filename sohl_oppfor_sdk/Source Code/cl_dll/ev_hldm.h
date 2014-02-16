//========= Copyright � 1996-2002, Valve LLC, All rights reserved. ============
//
// Purpose: 
//
// $NoKeywords: $
//=============================================================================

#if !defined ( EV_HLDMH )
#define EV_HLDMH

enum shotgun_e {
	SHOTGUN_IDLE = 0,
	SHOTGUN_DRAW,
	SHOTGUN_HOLSTER,
	SHOTGUN_FIRE,
	SHOTGUN_FIRE2,
	SHOTGUN_START_RELOAD,
	SHOTGUN_RELOAD,
	SHOTGUN_PUMP
};

// bullet types
typedef	enum
{
	BULLET_NONE = 0,
	BULLET_PLAYER_9MM, // glock
	BULLET_PLAYER_MP5, // mp5
	BULLET_PLAYER_357, // python
	BULLET_PLAYER_BUCKSHOT, // shotgun
	BULLET_PLAYER_CROWBAR, // crowbar swipe
	BULLET_PLAYER_556, // saw

	BULLET_MONSTER_9MM,
	BULLET_MONSTER_MP5,
	BULLET_MONSTER_12MM,
	BULLET_MONSTER_556,
} Bullet;

enum glock_e {
	GLOCK_IDLE1 = 0,
	GLOCK_IDLE2,
	GLOCK_IDLE3,
	GLOCK_DRAW,
	GLOCK_HOLSTER,
	GLOCK_SHOOT,
	GLOCK_SHOOT_EMPTY,
	GLOCK_RELOAD,
	GLOCK_RELOAD_NOT_EMPTY,
	GLOCK_HOLSTER2,
	GLOCK_ADD_SILENCER,
	GLOCK_DEL_SILENCER
};

enum mp5_e
{
	MP5_IDLE = 0,
	MP5_DEPLOY,
	MP5_HOLSTER,
	MP5_FIRE1,
	MP5_FIRE2,
	MP5_FIRE3,
	MP5_LAUNCH,
	MP5_RELOAD
};

//SOHL - Opposing-Force
enum m249_e
{
	SAW_SLOWIDLE = 0,
	SAW_IDLE,
	SAW_RELOAD_START,
	SAW_RELOAD_END,
	SAW_HOLSTER,
	SAW_DRAW,
	SAW_SHOOT1,
	SAW_SHOOT2,
	SAW_SHOOT3
};

enum python_e {
	PYTHON_IDLE1 = 0,
	PYTHON_FIDGET,
	PYTHON_FIRE1,
	PYTHON_RELOAD,
	PYTHON_HOLSTER,
	PYTHON_DRAW,
	PYTHON_IDLE2,
	PYTHON_IDLE3
};

#define	GAUSS_PRIMARY_CHARGE_VOLUME	256// how loud gauss is while charging
#define GAUSS_PRIMARY_FIRE_VOLUME	450// how loud gauss is when discharged

enum gauss_e {
	GAUSS_IDLE = 0,
	GAUSS_IDLE2,
	GAUSS_FIDGET,
	GAUSS_SPINUP,
	GAUSS_SPIN,
	GAUSS_FIRE,
	GAUSS_FIRE2,
	GAUSS_HOLSTER,
	GAUSS_DRAW
};

vec3_t EV_HLDM_GetMirroredPosition(int mirror_index, vec3_t pos);
void EV_HLDM_GunshotDecalTrace( pmtrace_t *pTrace, char *decalName );
void EV_HLDM_DecalGunshot( pmtrace_t *pTrace, int iBulletType );
int EV_HLDM_CheckTracer( int idx, float *vecSrc, float *end, float *forward, float *right, int iBulletType, int iTracerFreq, int *tracerCount );
void EV_HLDM_FireBullets( int idx, float *forward, float *right, float *up, int cShots, float *vecSrc, float *vecDirShooting, float flDistance, int iBulletType, int iTracerFreq, int *tracerCount, float flSpreadX, float flSpreadY );
void EV_UpdateBeams ( void );

#endif // EV_HLDMH