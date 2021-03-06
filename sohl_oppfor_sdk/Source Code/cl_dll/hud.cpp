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

#define ENGINE_DEBUG

#include "hud.h"
#include "cl_util.h"
#include <string.h>
#include <stdio.h>
#include "parsemsg.h"
#include "hud_servers.h"
#include "vgui_int.h"
#include "vgui_TeamFortressViewport.h"
#include "demo.h"
#include "demo_api.h"
#include "vgui_scorepanel.h"
#include "rain.h"
#include "com_weapons.h"
#include "particle_header.h"
#include "soundengine.h"

class CHLVoiceStatusHelper : public IVoiceStatusHelper
{
public:
	virtual void GetPlayerTextColor(int entindex, int color[3])
	{
		color[0] = color[1] = color[2] = 255;

		if (entindex >= 0 && entindex < sizeof(g_PlayerExtraInfo) / sizeof(g_PlayerExtraInfo[0]))
		{
			int iTeam = g_PlayerExtraInfo[entindex].teamnumber;

			if (iTeam < 0)
			{
				iTeam = 0;
			}

			iTeam = iTeam % iNumberOfTeamColors;

			color[0] = iTeamColors[iTeam][0];
			color[1] = iTeamColors[iTeam][1];
			color[2] = iTeamColors[iTeam][2];
		}
	}

	virtual void UpdateCursorState()
	{
		gViewPort->UpdateCursorState();
	}

	virtual int	GetAckIconHeight()
	{
		return ScreenHeight - gHUD.m_iFontHeight * 3 - 6;
	}

	virtual bool			CanShowSpeakerLabels()
	{
		if (gViewPort && gViewPort->m_pScoreBoard)
			return !gViewPort->m_pScoreBoard->isVisible();
		else
			return false;
	}
};
static CHLVoiceStatusHelper g_VoiceStatusHelper;


extern client_sprite_t *GetSpriteList(client_sprite_t *pList, const char *psz, int iRes, int iCount);

extern cvar_t *sensitivity;
cvar_t *cl_lw = NULL;
cvar_t *cl_rollangle;
cvar_t *cl_rollspeed;

void ShutdownInput();

int __MsgFunc_Logo(const char *pszName, int iSize, void *pbuf)
{
	return gHUD.MsgFunc_Logo(pszName, iSize, pbuf);
}

//LRC
int __MsgFunc_HUDColor(const char *pszName, int iSize, void *pbuf)
{
	return gHUD.MsgFunc_HUDColor(pszName, iSize, pbuf);
}

//LRC
int __MsgFunc_SetFog(const char *pszName, int iSize, void *pbuf)
{
	gHUD.MsgFunc_SetFog(pszName, iSize, pbuf);
	return 1;
}

int __MsgFunc_FSound(const char *pszName, int iSize, void *pbuf) {
	return gHUD.MsgFunc_FSound(pszName, iSize, pbuf);
}

//LRC
int __MsgFunc_KeyedDLight(const char *pszName, int iSize, void *pbuf)
{
	gHUD.MsgFunc_KeyedDLight(pszName, iSize, pbuf);
	return 1;
}

//LRC
int __MsgFunc_AddShine(const char *pszName, int iSize, void *pbuf)
{
	gHUD.MsgFunc_AddShine(pszName, iSize, pbuf);
	return 1;
}

//LRC
int __MsgFunc_SetSky(const char *pszName, int iSize, void *pbuf)
{
	gHUD.MsgFunc_SetSky(pszName, iSize, pbuf);
	return 1;
}

// G-Cont. rain message
int __MsgFunc_RainData(const char *pszName, int iSize, void *pbuf)
{
	return gHUD.MsgFunc_RainData(pszName, iSize, pbuf);
}

//change body for weapon models
int __MsgFunc_SetBody(const char *pszName, int iSize, void *pbuf)
{
	gHUD.MsgFunc_SetBody(pszName, iSize, pbuf);
	return 1;
}

//change skin for weapon models
int __MsgFunc_SetSkin(const char *pszName, int iSize, void *pbuf)
{
	gHUD.MsgFunc_SetSkin(pszName, iSize, pbuf);
	return 1;
}

int __MsgFunc_Particles(const char *pszName, int iSize, void *pbuf)
{
	if (gViewPort)
		return gViewPort->MsgFunc_Particles(pszName, iSize, pbuf);
	return 0;
}

int __MsgFunc_SetMirror(const char *pszName, int iSize, void *pbuf)
{
	gHUD.MsgFunc_SetMirror(pszName, iSize, pbuf);
	return 1;
}

int __MsgFunc_ResetMirror(const char *pszName, int iSize, void *pbuf)
{
	gHUD.MsgFunc_ResetMirror(pszName, iSize, pbuf);
	return 1;
}

int __MsgFunc_ResetHUD(const char *pszName, int iSize, void *pbuf)
{
	return gHUD.MsgFunc_ResetHUD(pszName, iSize, pbuf);
}

int __MsgFunc_InitHUD(const char *pszName, int iSize, void *pbuf)
{
	gHUD.MsgFunc_InitHUD(pszName, iSize, pbuf);
	return 1;
}

int __MsgFunc_ViewMode(const char *pszName, int iSize, void *pbuf)
{
	gHUD.MsgFunc_ViewMode(pszName, iSize, pbuf);
	return 1;
}

int __MsgFunc_SetFOV(const char *pszName, int iSize, void *pbuf)
{
	return gHUD.MsgFunc_SetFOV(pszName, iSize, pbuf);
}

int __MsgFunc_Concuss(const char *pszName, int iSize, void *pbuf)
{
	return gHUD.MsgFunc_Concuss(pszName, iSize, pbuf);
}

int __MsgFunc_GameMode(const char *pszName, int iSize, void *pbuf)
{
	return gHUD.MsgFunc_GameMode(pszName, iSize, pbuf);
}

int __MsgFunc_CamData(const char *pszName, int iSize, void *pbuf)
{
	gHUD.MsgFunc_CamData(pszName, iSize, pbuf);
	return 1;
}

int __MsgFunc_Inventory(const char *pszName, int iSize, void *pbuf)
{
	gHUD.MsgFunc_Inventory(pszName, iSize, pbuf);
	return 1;
}

// TFFree Command Menu
void __CmdFunc_OpenCommandMenu()
{
	if (gViewPort)
	{
		gViewPort->ShowCommandMenu(gViewPort->m_StandardMenu);
	}
}

// TFC "special" command
void __CmdFunc_InputPlayerSpecial()
{
	if (gViewPort)
	{
		gViewPort->InputPlayerSpecial();
	}
}

void __CmdFunc_CloseCommandMenu()
{
	if (gViewPort)
	{
		gViewPort->InputSignalHideCommandMenu();
	}
}

void __CmdFunc_ForceCloseCommandMenu()
{
	if (gViewPort)
	{
		gViewPort->HideCommandMenu();
	}
}

void __CmdFunc_ToggleServerBrowser()
{
	if (gViewPort)
	{
		gViewPort->ToggleServerBrowser();
	}
}

// TFFree Command Menu Message Handlers
int __MsgFunc_ValClass(const char *pszName, int iSize, void *pbuf)
{
	if (gViewPort)
		return gViewPort->MsgFunc_ValClass(pszName, iSize, pbuf);
	return 0;
}

int __MsgFunc_TeamNames(const char *pszName, int iSize, void *pbuf)
{
	if (gViewPort)
		return gViewPort->MsgFunc_TeamNames(pszName, iSize, pbuf);
	return 0;
}

int __MsgFunc_Feign(const char *pszName, int iSize, void *pbuf)
{
	if (gViewPort)
		return gViewPort->MsgFunc_Feign(pszName, iSize, pbuf);
	return 0;
}

int __MsgFunc_Detpack(const char *pszName, int iSize, void *pbuf)
{
	if (gViewPort)
		return gViewPort->MsgFunc_Detpack(pszName, iSize, pbuf);
	return 0;
}

int __MsgFunc_VGUIMenu(const char *pszName, int iSize, void *pbuf)
{
	if (gViewPort)
		return gViewPort->MsgFunc_VGUIMenu(pszName, iSize, pbuf);
	return 0;
}

int __MsgFunc_MOTD(const char *pszName, int iSize, void *pbuf)
{
	if (gViewPort)
		return gViewPort->MsgFunc_MOTD(pszName, iSize, pbuf);
	return 0;
}

int __MsgFunc_BuildSt(const char *pszName, int iSize, void *pbuf)
{
	if (gViewPort)
		return gViewPort->MsgFunc_BuildSt(pszName, iSize, pbuf);
	return 0;
}

int __MsgFunc_RandomPC(const char *pszName, int iSize, void *pbuf)
{
	if (gViewPort)
		return gViewPort->MsgFunc_RandomPC(pszName, iSize, pbuf);
	return 0;
}

int __MsgFunc_ServerName(const char *pszName, int iSize, void *pbuf)
{
	if (gViewPort)
		return gViewPort->MsgFunc_ServerName(pszName, iSize, pbuf);
	return 0;
}

int __MsgFunc_ScoreInfo(const char *pszName, int iSize, void *pbuf)
{
	if (gViewPort)
		return gViewPort->MsgFunc_ScoreInfo(pszName, iSize, pbuf);
	return 0;
}

int __MsgFunc_TeamScore(const char *pszName, int iSize, void *pbuf)
{
	if (gViewPort)
		return gViewPort->MsgFunc_TeamScore(pszName, iSize, pbuf);
	return 0;
}

int __MsgFunc_TeamInfo(const char *pszName, int iSize, void *pbuf)
{
	if (gViewPort)
		return gViewPort->MsgFunc_TeamInfo(pszName, iSize, pbuf);
	return 0;
}

int __MsgFunc_Spectator(const char *pszName, int iSize, void *pbuf)
{
	if (gViewPort)
		return gViewPort->MsgFunc_Spectator(pszName, iSize, pbuf);
	return 0;
}

int __MsgFunc_AllowSpec(const char *pszName, int iSize, void *pbuf)
{
	if (gViewPort)
		return gViewPort->MsgFunc_AllowSpec(pszName, iSize, pbuf);
	return 0;
}

// This is called every time the DLL is loaded
void CHud::Init()
{
#ifdef ENGINE_DEBUG
	CONPRINT("## CHud::Init\n");
#endif
	HOOK_MESSAGE(Logo);
	HOOK_MESSAGE(ResetHUD);
	HOOK_MESSAGE(GameMode);
	HOOK_MESSAGE(InitHUD);
	HOOK_MESSAGE(ViewMode);
	HOOK_MESSAGE(SetFOV);
	HOOK_MESSAGE(Concuss);
	HOOK_MESSAGE(HUDColor); //LRC
	HOOK_MESSAGE(SetFog); //LRC
	HOOK_MESSAGE(KeyedDLight); //LRC
	HOOK_MESSAGE(AddShine); //LRC
	HOOK_MESSAGE(SetSky); //LRC
	HOOK_MESSAGE(CamData);//G-Cont. for new camera style 	
	HOOK_MESSAGE(RainData);//G-Cont. for rain control ^
	HOOK_MESSAGE(Inventory); //AJH Inventory system
	HOOK_MESSAGE(SetBody);//change body for view weapon model
	HOOK_MESSAGE(SetSkin);//change skin for view weapon model
	HOOK_MESSAGE(SetMirror);
	HOOK_MESSAGE(ResetMirror);
	HOOK_MESSAGE(Particles);
	HOOK_MESSAGE(FSound);

	// TFFree CommandMenu
	HOOK_COMMAND("+commandmenu", OpenCommandMenu);
	HOOK_COMMAND("-commandmenu", CloseCommandMenu);
	HOOK_COMMAND("ForceCloseCommandMenu", ForceCloseCommandMenu);
	HOOK_COMMAND("special", InputPlayerSpecial);
	HOOK_COMMAND("togglebrowser", ToggleServerBrowser);

	HOOK_MESSAGE(ValClass);
	HOOK_MESSAGE(TeamNames);
	HOOK_MESSAGE(Feign);
	HOOK_MESSAGE(Detpack);
	HOOK_MESSAGE(MOTD);
	HOOK_MESSAGE(BuildSt);
	HOOK_MESSAGE(RandomPC);
	HOOK_MESSAGE(ServerName);
	HOOK_MESSAGE(ScoreInfo);
	HOOK_MESSAGE(TeamScore);
	HOOK_MESSAGE(TeamInfo);

	HOOK_MESSAGE(Spectator);
	HOOK_MESSAGE(AllowSpec);

	// VGUI Menus
	HOOK_MESSAGE(VGUIMenu);

	CVAR_CREATE("hud_classautokill", "1", FCVAR_ARCHIVE | FCVAR_USERINFO);		// controls whether or not to suicide immediately on TF class switch
	CVAR_CREATE("hud_takesshots", "0", FCVAR_ARCHIVE);		// controls whether or not to automatically take screenshots at the end of a round

	//start glow effect --FragBait0
	CVAR_CREATE("r_glow", "0", FCVAR_ARCHIVE);
	//CVAR_CREATE("r_glowmode", "0", FCVAR_ARCHIVE ); //AJH this is now redundant
	CVAR_CREATE("r_glowstrength", "1", FCVAR_ARCHIVE);
	CVAR_CREATE("r_glowblur", "4", FCVAR_ARCHIVE);
	CVAR_CREATE("r_glowdark", "2", FCVAR_ARCHIVE);
	//end glow effect

	viewEntityIndex = 0; // trigger_viewset stuff
	viewFlags = 0;
	m_iLogo = 0;
	m_iFOV = 0;
	numMirrors = 0;
	m_iHUDColor = RGB_GREENISH;

	CVAR_CREATE("zoom_sensitivity_ratio", "1.2", 0);
	default_fov = CVAR_CREATE("default_fov", "90", 0);
	m_pCvarStealMouse = CVAR_CREATE("hud_capturemouse", "1", FCVAR_ARCHIVE);
	m_pCvarDraw = CVAR_CREATE("hud_draw", "1", FCVAR_ARCHIVE);
	cl_lw = gEngfuncs.pfnGetCvarPointer("cl_lw");
	RainInfo = gEngfuncs.pfnRegisterVariable("cl_raininfo", "0", 0);

	CVAR_CREATE("volume_sfx", "100", FCVAR_ARCHIVE);

	//Explosion Effects
	CVAR_CREATE("sv_grenadegib", "1", FCVAR_ARCHIVE);
	CVAR_CREATE("cl_expdetail", "1", FCVAR_ARCHIVE);

	CVAR_CREATE("cl_idleswaying", "1", FCVAR_ARCHIVE);
	CVAR_CREATE("cl_gunsmoke", "1", FCVAR_ARCHIVE);

	cl_rollangle = gEngfuncs.pfnRegisterVariable("cl_rollangle", "0.65", FCVAR_CLIENTDLL | FCVAR_ARCHIVE);
	cl_rollspeed = gEngfuncs.pfnRegisterVariable("cl_rollspeed", "300", FCVAR_CLIENTDLL | FCVAR_ARCHIVE);

	//Particles System
	CVAR_CREATE("cl_grassamount", "100", FCVAR_CLIENTDLL | FCVAR_ARCHIVE);
	CVAR_CREATE("r_particles", "1", FCVAR_ARCHIVE);
	g_ParticleCount = gEngfuncs.pfnRegisterVariable("cl_particlecount", "100", FCVAR_CLIENTDLL | FCVAR_ARCHIVE);
	g_ParticleDebug = gEngfuncs.pfnRegisterVariable("cl_particledebug", "0", FCVAR_CLIENTDLL | FCVAR_ARCHIVE);
	g_ParticleSorts = gEngfuncs.pfnRegisterVariable("cl_particlesorts", "3", FCVAR_CLIENTDLL | FCVAR_ARCHIVE);

	m_pSpriteList = NULL;
	m_pShinySurface = NULL; //LRC

	// Clear any old HUD list
	if (m_pHudList) {
		HUDLIST *pList;
		while (m_pHudList) {
			pList = m_pHudList;
			m_pHudList = m_pHudList->pNext;
			free(pList);
		}
		m_pHudList = NULL;
	}

	// In case we get messages before the first update -- time will be valid
	m_flTime = 1.0;

	m_Ammo.Init();
	m_Health.Init();
	m_SayText.Init();
	m_Spectator.Init();
	m_Geiger.Init();
	m_Train.Init();
	m_Battery.Init();
	m_Flash.Init();
	m_Message.Init();
	m_StatusBar.Init();
	m_DeathNotice.Init();
	m_AmmoSecondary.Init();
	m_TextMessage.Init();
	m_StatusIcons.Init();
	GetClientVoiceMgr()->Init(&g_VoiceStatusHelper, (vgui::Panel**)&gViewPort);
	m_Particle.Init(); // (LRC) -- 30/08/02 November235: Particles to Order
	gSoundEngine.Init();

	m_Menu.Init();
	InitRain();
	ServersInit();

	MsgFunc_ResetHUD(0, 0, NULL);
}

// CHud destructor
// cleans up memory allocated for m_rg* arrays
CHud :: ~CHud()
{
	delete pParticleManager;
	pParticleManager = NULL;
	delete[] m_rghSprites;
	delete[] m_rgrcRects;
	delete[] m_rgszSpriteNames;

	ResetRain();
	gSoundEngine.ResetEngine();

	//LRC - clear all shiny surfaces
	if (m_pShinySurface)
	{
		delete m_pShinySurface;
		m_pShinySurface = NULL;
	}

	if (m_pHudList)
	{
		HUDLIST *pList;
		while (m_pHudList)
		{
			pList = m_pHudList;
			m_pHudList = m_pHudList->pNext;
			free(pList);
		}
		m_pHudList = NULL;
	}

	ServersShutdown();
	gSoundEngine.Shutdown();
}

// GetSpriteIndex()
// searches through the sprite list loaded from hud.txt for a name matching SpriteName
// returns an index into the gHUD.m_rghSprites[] array
// returns 0 if sprite not found
int CHud::GetSpriteIndex(const char *SpriteName)
{
	// look through the loaded sprite name list for SpriteName
	for (int i = 0; i < m_iSpriteCount; i++)
	{
		if (strncmp(SpriteName, m_rgszSpriteNames + (i * MAX_SPRITE_NAME_LENGTH), MAX_SPRITE_NAME_LENGTH) == 0)
			return i;
	}

	return -1; // invalid sprite
}

void CHud::VidInit()
{
	m_scrinfo.iSize = sizeof(m_scrinfo);
	GetScreenInfo(&m_scrinfo);

	// ----------
	// Load Sprites
	// ---------
//	m_hsprFont = LoadSprite("sprites/%d_font.spr");

	m_hsprLogo = 0;
	m_hsprCursor = 0;
	numMirrors = 0;
	ResetRain();
	gSoundEngine.ResetEngine();

	//LRC - clear all shiny surfaces
	if (m_pShinySurface)
	{
		delete m_pShinySurface;
		m_pShinySurface = NULL;
	}

	if (ScreenWidth < 640)
		m_iRes = 320;
	else
		m_iRes = 640;

	// Only load this once
	if (!m_pSpriteList)
	{
		// we need to load the hud.txt, and all sprites within
		m_pSpriteList = SPR_GetList("sprites/hud.txt", &m_iSpriteCountAllRes);
		int j = 0;

		if (m_pSpriteList)
		{
			// count the number of sprites of the appropriate res
			m_iSpriteCount = 0;
			client_sprite_t *p = m_pSpriteList;
			for (j = 0; j < m_iSpriteCountAllRes; j++)
			{
				if (p->iRes == m_iRes)
					m_iSpriteCount++;
				p++;
			}

			// allocated memory for sprite handle arrays
			m_rghSprites = new HL_HSPRITE[m_iSpriteCount];
			m_rgrcRects = new wrect_t[m_iSpriteCount];
			m_rgszSpriteNames = new char[m_iSpriteCount * MAX_SPRITE_NAME_LENGTH];

			p = m_pSpriteList;
			int index = 0;
			for (j = 0; j < m_iSpriteCountAllRes; j++)
			{
				if (p->iRes == m_iRes)
				{
					char sz[256];
					snprintf(sz, 256, "sprites/%s.spr", p->szSprite);
					m_rghSprites[index] = SPR_Load(sz);
					m_rgrcRects[index] = p->rc;
					strncpy(&m_rgszSpriteNames[index * MAX_SPRITE_NAME_LENGTH], p->szName, MAX_SPRITE_NAME_LENGTH);

					index++;
				}

				p++;
			}
		}
	}
	else
	{
		// we have already have loaded the sprite reference from hud.txt, but
		// we need to make sure all the sprites have been loaded (we've gone through a transition, or loaded a save game)
		client_sprite_t *p = m_pSpriteList;
		int index = 0;
		for (int j = 0; j < m_iSpriteCountAllRes; j++)
		{
			if (p->iRes == m_iRes)
			{
				char sz[256];
				snprintf(sz, 256, "sprites/%s.spr", p->szSprite);
				m_rghSprites[index] = SPR_Load(sz);
				index++;
			}

			p++;
		}
	}

	// assumption: number_1, number_2, etc, are all listed and loaded sequentially
	m_HUD_number_0 = GetSpriteIndex("number_0");

	m_iFontHeight = m_rgrcRects[m_HUD_number_0].bottom - m_rgrcRects[m_HUD_number_0].top;

	m_Ammo.VidInit();
	m_Health.VidInit();
	m_Spectator.VidInit();
	m_Geiger.VidInit();
	m_Train.VidInit();
	m_Battery.VidInit();
	m_Flash.VidInit();
	m_Message.VidInit();
	m_StatusBar.VidInit();
	m_DeathNotice.VidInit();
	m_SayText.VidInit();
	m_Menu.VidInit();
	m_AmmoSecondary.VidInit();
	m_TextMessage.VidInit();
	m_StatusIcons.VidInit();
	GetClientVoiceMgr()->VidInit();
	m_Particle.VidInit(); // (LRC) -- 30/08/02 November235: Particles to Order
	gSoundEngine.VidInit();

	if (!pParticleManager) {
		pParticleManager = new CParticleSystemManager;
		pParticleManager->PrecacheTextures();
	}

	pParticleManager->RemoveParticles();
	pParticleManager->RemoveSystems();

	ENGINEPRINT("==========================================\n");
	ENGINEPRINT("Client-DLL for Spirit of Half-Life: Revision Build 1.9\n");
	ENGINEPRINT("https://github.com/Hammermaps-DEV/SOHL-V1.9-Opposing-Force-Edition\n");
	ENGINEPRINT("==========================================\n");
}

int CHud::MsgFunc_Logo(const char *pszName, int iSize, void *pbuf)
{
	BEGIN_READ(pbuf, iSize);

	// update Train data
	m_iLogo = READ_BYTE();

	return 1;
}

//LRC
int CHud::MsgFunc_HUDColor(const char *pszName, int iSize, void *pbuf)
{
	BEGIN_READ(pbuf, iSize);

	m_iHUDColor = READ_LONG();

	return 1;
}

float g_lastFOV = 0.0;

//==================================================
// FMOD Stream
//==================================================
int CHud::MsgFunc_FSound(const char *pszName, int iSize, void *pbuf) {
	static char songname[256];
	BEGIN_READ(pbuf, iSize);
	char readname[256];
	strncpy(readname, READ_STRING(), sizeof(readname));
	readname[sizeof(readname) - 1] = '\0';
	gSoundEngine.PlayMusic(readname, READ_BYTE());
	return 1;
}

/*
============
COM_FileBase
============
*/
// Extracts the base name of a file (no path, no extension, assumes '/' as path separator)
void COM_FileBase(const char *in, char *out)
{
	int len, start, end;

	len = strlen(in);

	// scan backward for '.'
	end = len - 1;
	while (end && in[end] != '.' && in[end] != '/' && in[end] != '\\')
		end--;

	if (in[end] != '.')		// no '.', copy to end
		end = len - 1;
	else
		end--;					// Found ',', copy to left of '.'


	// Scan backward for '/'
	start = len - 1;
	while (start >= 0 && in[start] != '/' && in[start] != '\\')
		start--;

	if (in[start] != '/' && in[start] != '\\')
		start = 0;
	else
		start++;

	// Length of new sting
	len = end - start + 1;

	// Copy partial string
	strncpy(out, &in[start], len);
	// Terminate it
	out[len] = 0;
}

/*
=================
HUD_IsGame

=================
*/
int HUD_IsGame(const char *game)
{
	const char *gamedir;
	char gd[1024];

	gamedir = gEngfuncs.pfnGetGameDirectory();
	if (gamedir && gamedir[0])
	{
		COM_FileBase(gamedir, gd);
		if (!_stricmp(gd, game))
			return 1;
	}
	return 0;
}

/*
=====================
HUD_GetFOV

Returns last FOV
=====================
*/
float HUD_GetFOV()
{
	if (gEngfuncs.pDemoAPI->IsRecording())
	{
		// Write it
		int i = 0;
		unsigned char buf[100];

		// Active
		*(float *)&buf[i] = g_lastFOV;
		i += sizeof(float);

		Demo_WriteBuffer(TYPE_ZOOM, i, buf);
	}

	if (gEngfuncs.pDemoAPI->IsPlayingback())
	{
		g_lastFOV = g_demozoom;
	}
	return g_lastFOV;
}

int CHud::MsgFunc_SetFOV(const char *pszName, int iSize, void *pbuf)
{
	BEGIN_READ(pbuf, iSize);

	int newfov = READ_BYTE();
	int def_fov = CVAR_GET_FLOAT("default_fov");

	//Weapon prediction already takes care of changing the fog. ( g_lastFOV ).
	//if ( cl_lw && cl_lw->value )
	//	return 1;

	g_lastFOV = newfov;

	if (newfov == 0)
	{
		m_iFOV = def_fov;
	}
	else
	{
		m_iFOV = newfov;
	}

	// the clients fov is actually set in the client data update section of the hud

	// Set a new sensitivity
	if (m_iFOV == def_fov)
	{
		// reset to saved sensitivity
		m_flMouseSensitivity = 0;
	}
	else
	{
		// set a new sensitivity that is proportional to the change from the FOV default
		m_flMouseSensitivity = sensitivity->value * ((float)newfov / (float)def_fov) * CVAR_GET_FLOAT("zoom_sensitivity_ratio");
	}

	return 1;
}


void CHud::AddHudElem(CHudBase *phudelem)
{
	HUDLIST *pdl, *ptemp;

	//phudelem->Think();

	if (!phudelem)
		return;

	pdl = (HUDLIST *)malloc(sizeof(HUDLIST));
	if (!pdl)
		return;

	memset(pdl, 0, sizeof(HUDLIST));
	pdl->p = phudelem;

	if (!m_pHudList)
	{
		m_pHudList = pdl;
		return;
	}

	ptemp = m_pHudList;

	while (ptemp->pNext)
		ptemp = ptemp->pNext;

	ptemp->pNext = pdl;
}

float CHud::GetSensitivity()
{
	return m_flMouseSensitivity;
}

// fixes the buffer overrun in hl
char* CHud::HandleServerString(char *szServerMsg)
{
	char szString[2048];
	szString[sizeof(szString) - 1] = '\0';

	if (szServerMsg == NULL)
	{
		gEngfuncs.pfnConsolePrint("Null string being sent\n");
		return NULL;
	}

	else
	{
		int iResult = sizeof(szString) - 1;
		iResult = _snprintf(szString, iResult, "%s\0", szServerMsg);

		if (iResult < 0)
		{
			gEngfuncs.pfnConsolePrint("Bigger than allowed string being sent\n");
			return NULL;
		}
		else
		{
			szString[sizeof(szString) - 1] = '\0';
			_snprintf(m_szString, sizeof(m_szString) - 1, "%s", szString);
			return (char*)m_szString;
		}
	}
}

