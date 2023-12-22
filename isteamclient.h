﻿//====== Copyright � 1996-2008, Valve Corporation, All rights reserved. =======
//
// Purpose: Main interface for loading and accessing Steamworks API's from the 
//			Steam client.
//			For most uses, this code is wrapped inside of SteamAPI_Init()
//=============================================================================

#ifndef ISTEAMCLIENT_H
#define ISTEAMCLIENT_H
#ifdef _WIN32
#pragma once
#endif

#include "steamtypes.h"
#include "steamclientpublic.h"

// Define compile time assert macros to let us validate the structure sizes.
#define VALVE_COMPILE_TIME_ASSERT( pred ) typedef char compile_time_assert_type[(pred) ? 1 : -1];

#ifndef REFERENCE
#define REFERENCE(arg) ((void)arg)
#endif

#if ( defined(STEAM_API_EXPORTS) || defined(STEAM_API_NODLL) ) && !defined(API_GEN)
#define STEAM_PRIVATE_API( ... ) __VA_ARGS__
#elif defined(STEAM_API_EXPORTS) && defined(API_GEN)
#define STEAM_PRIVATE_API( ... )
#else
#define STEAM_PRIVATE_API( ... ) protected: __VA_ARGS__ public:
#endif

#if defined(__linux__) || defined(__APPLE__) 
// The 32-bit version of gcc has the alignment requirement for uint64 and double set to
// 4 meaning that even with #pragma pack(8) these types will only be four-byte aligned.
// The 64-bit version of gcc has the alignment requirement for these types set to
// 8 meaning that unless we use #pragma pack(4) our structures will get bigger.
// The 64-bit structure packing has to match the 32-bit structure packing for each platform.
#define VALVE_CALLBACK_PACK_SMALL
#else
#define VALVE_CALLBACK_PACK_LARGE
#endif

#if defined( VALVE_CALLBACK_PACK_SMALL )
#pragma pack( push, 4 )
#elif defined( VALVE_CALLBACK_PACK_LARGE )
#pragma pack( push, 8 )
#else
#error ???
#endif 

typedef struct ValvePackingSentinel_t
{
    uint32 m_u32;
    uint64 m_u64;
    uint16 m_u16;
    double m_d;
} ValvePackingSentinel_t;

#pragma pack( pop )


#if defined(VALVE_CALLBACK_PACK_SMALL)
VALVE_COMPILE_TIME_ASSERT( sizeof(ValvePackingSentinel_t) == 24 )
#elif defined(VALVE_CALLBACK_PACK_LARGE)
VALVE_COMPILE_TIME_ASSERT( sizeof(ValvePackingSentinel_t) == 32 )
#else
#error ???
#endif


// handle to a communication pipe to the Steam client
typedef int32 HSteamPipe;
// handle to single instance of a steam user
typedef int32 HSteamUser;
// function prototype
#if defined( POSIX )
#define __cdecl
#endif
extern "C" typedef void (__cdecl *SteamAPIWarningMessageHook_t)(int, const char *);
extern "C" typedef uint32 ( *SteamAPI_CheckCallbackRegistered_t )( int iCallbackNum );
#if defined( __SNC__ )
	#pragma diag_suppress=1700	   // warning 1700: class "%s" has virtual functions but non-virtual destructor
#endif

// interface predec
class ISteamUser;
class ISteamGameServer;
class ISteamFriends;
class ISteamUtils;
class ISteamMatchmaking;
class ISteamContentServer;
class ISteamMatchmakingServers;
class ISteamUserStats;
class ISteamApps;
class ISteamNetworking;
class ISteamRemoteStorage;
class ISteamScreenshots;
class ISteamMusic;
class ISteamMusicRemote;
class ISteamGameServerStats;
class ISteamPS3OverlayRender;
class ISteamHTTP;
class ISteamController;
class ISteamUGC;
class ISteamAppList;
class ISteamHTMLSurface;
class ISteamInventory;
class ISteamVideo;
class ISteamParentalSettings;

class ISteamClient
{
public:
	virtual HSteamPipe CreateSteamPipe() = 0;
	virtual bool BReleaseSteamPipe( HSteamPipe hSteamPipe ) = 0;
	virtual HSteamUser ConnectToGlobalUser( HSteamPipe hSteamPipe ) = 0;
	virtual HSteamUser CreateLocalUser( HSteamPipe *phSteamPipe, EAccountType eAccountType ) = 0;
	virtual void ReleaseUser( HSteamPipe hSteamPipe, HSteamUser hUser ) = 0;
	virtual ISteamUser *GetISteamUser( HSteamUser hSteamUser, HSteamPipe hSteamPipe, const char *pchVersion ) = 0;
	virtual ISteamGameServer *GetISteamGameServer( HSteamUser hSteamUser, HSteamPipe hSteamPipe, const char *pchVersion ) = 0;
	virtual void SetLocalIPBinding( uint32 unIP, uint16 usPort ) = 0; 
	virtual ISteamFriends *GetISteamFriends( HSteamUser hSteamUser, HSteamPipe hSteamPipe, const char *pchVersion ) = 0;
	virtual ISteamUtils *GetISteamUtils( HSteamPipe hSteamPipe, const char *pchVersion ) = 0;
	virtual ISteamMatchmaking *GetISteamMatchmaking( HSteamUser hSteamUser, HSteamPipe hSteamPipe, const char *pchVersion ) = 0;
	virtual ISteamMatchmakingServers *GetISteamMatchmakingServers( HSteamUser hSteamUser, HSteamPipe hSteamPipe, const char *pchVersion ) = 0;
	virtual void *GetISteamGenericInterface( HSteamUser hSteamUser, HSteamPipe hSteamPipe, const char *pchVersion ) = 0;
	virtual ISteamUserStats *GetISteamUserStats( HSteamUser hSteamUser, HSteamPipe hSteamPipe, const char *pchVersion ) = 0;
	virtual ISteamGameServerStats *GetISteamGameServerStats( HSteamUser hSteamuser, HSteamPipe hSteamPipe, const char *pchVersion ) = 0;
	virtual ISteamApps *GetISteamApps( HSteamUser hSteamUser, HSteamPipe hSteamPipe, const char *pchVersion ) = 0;
	virtual ISteamNetworking *GetISteamNetworking( HSteamUser hSteamUser, HSteamPipe hSteamPipe, const char *pchVersion ) = 0;
	virtual ISteamRemoteStorage *GetISteamRemoteStorage( HSteamUser hSteamuser, HSteamPipe hSteamPipe, const char *pchVersion ) = 0;
	virtual ISteamScreenshots *GetISteamScreenshots( HSteamUser hSteamuser, HSteamPipe hSteamPipe, const char *pchVersion ) = 0;

	STEAM_PRIVATE_API( virtual void RunFrame() = 0; )

	virtual uint32 GetIPCCallCount() = 0;
	virtual void SetWarningMessageHook( SteamAPIWarningMessageHook_t pFunction ) = 0;
	virtual bool BShutdownIfAllPipesClosed() = 0;
	virtual ISteamHTTP *GetISteamHTTP( HSteamUser hSteamuser, HSteamPipe hSteamPipe, const char *pchVersion ) = 0;

	STEAM_PRIVATE_API( virtual void *DEPRECATED_GetISteamUnifiedMessages( HSteamUser hSteamuser, HSteamPipe hSteamPipe, const char *pchVersion ) = 0 ; )

	virtual ISteamController *GetISteamController( HSteamUser hSteamUser, HSteamPipe hSteamPipe, const char *pchVersion ) = 0;
	virtual ISteamUGC *GetISteamUGC( HSteamUser hSteamUser, HSteamPipe hSteamPipe, const char *pchVersion ) = 0;
	virtual ISteamAppList *GetISteamAppList( HSteamUser hSteamUser, HSteamPipe hSteamPipe, const char *pchVersion ) = 0;
	virtual ISteamMusic *GetISteamMusic( HSteamUser hSteamuser, HSteamPipe hSteamPipe, const char *pchVersion ) = 0;
	virtual ISteamMusicRemote *GetISteamMusicRemote(HSteamUser hSteamuser, HSteamPipe hSteamPipe, const char *pchVersion) = 0;
	virtual ISteamHTMLSurface *GetISteamHTMLSurface(HSteamUser hSteamuser, HSteamPipe hSteamPipe, const char *pchVersion) = 0;

	STEAM_PRIVATE_API( virtual void DEPRECATED_Set_SteamAPI_CPostAPIResultInProcess( void (*)() ) = 0; )
	STEAM_PRIVATE_API( virtual void DEPRECATED_Remove_SteamAPI_CPostAPIResultInProcess( void (*)() ) = 0; )
	STEAM_PRIVATE_API( virtual void Set_SteamAPI_CCheckCallbackRegisteredInProcess( SteamAPI_CheckCallbackRegistered_t func ) = 0; )

	virtual ISteamInventory *GetISteamInventory( HSteamUser hSteamuser, HSteamPipe hSteamPipe, const char *pchVersion ) = 0;
	virtual ISteamVideo *GetISteamVideo( HSteamUser hSteamuser, HSteamPipe hSteamPipe, const char *pchVersion ) = 0;
	virtual ISteamParentalSettings *GetISteamParentalSettings( HSteamUser hSteamuser, HSteamPipe hSteamPipe, const char *pchVersion ) = 0;
};
inline ISteamClient* SteamClient;

#define STEAMCLIENT_INTERFACE_VERSION "SteamClient017"

enum { k_iSteamUserCallbacks = 100 };
enum { k_iSteamGameServerCallbacks = 200 };
enum { k_iSteamFriendsCallbacks = 300 };
enum { k_iSteamBillingCallbacks = 400 };
enum { k_iSteamMatchmakingCallbacks = 500 };
enum { k_iSteamContentServerCallbacks = 600 };
enum { k_iSteamUtilsCallbacks = 700 };
enum { k_iClientFriendsCallbacks = 800 };
enum { k_iClientUserCallbacks = 900 };
enum { k_iSteamAppsCallbacks = 1000 };
enum { k_iSteamUserStatsCallbacks = 1100 };
enum { k_iSteamNetworkingCallbacks = 1200 };
enum { k_iClientRemoteStorageCallbacks = 1300 };
enum { k_iClientDepotBuilderCallbacks = 1400 };
enum { k_iSteamGameServerItemsCallbacks = 1500 };
enum { k_iClientUtilsCallbacks = 1600 };
enum { k_iSteamGameCoordinatorCallbacks = 1700 };
enum { k_iSteamGameServerStatsCallbacks = 1800 };
enum { k_iSteam2AsyncCallbacks = 1900 };
enum { k_iSteamGameStatsCallbacks = 2000 };
enum { k_iClientHTTPCallbacks = 2100 };
enum { k_iClientScreenshotsCallbacks = 2200 };
enum { k_iSteamScreenshotsCallbacks = 2300 };
enum { k_iClientAudioCallbacks = 2400 };
enum { k_iClientUnifiedMessagesCallbacks = 2500 };
enum { k_iSteamStreamLauncherCallbacks = 2600 };
enum { k_iClientControllerCallbacks = 2700 };
enum { k_iSteamControllerCallbacks = 2800 };
enum { k_iClientParentalSettingsCallbacks = 2900 };
enum { k_iClientDeviceAuthCallbacks = 3000 };
enum { k_iClientNetworkDeviceManagerCallbacks = 3100 };
enum { k_iClientMusicCallbacks = 3200 };
enum { k_iClientRemoteClientManagerCallbacks = 3300 };
enum { k_iClientUGCCallbacks = 3400 };
enum { k_iSteamStreamClientCallbacks = 3500 };
enum { k_IClientProductBuilderCallbacks = 3600 };
enum { k_iClientShortcutsCallbacks = 3700 };
enum { k_iClientRemoteControlManagerCallbacks = 3800 };
enum { k_iSteamAppListCallbacks = 3900 };
enum { k_iSteamMusicCallbacks = 4000 };
enum { k_iSteamMusicRemoteCallbacks = 4100 };
enum { k_iClientVRCallbacks = 4200 };
enum { k_iClientGameNotificationCallbacks = 4300 }; 
enum { k_iSteamGameNotificationCallbacks = 4400 }; 
enum { k_iSteamHTMLSurfaceCallbacks = 4500 };
enum { k_iClientVideoCallbacks = 4600 };
enum { k_iClientInventoryCallbacks = 4700 };
enum { k_iClientBluetoothManagerCallbacks = 4800 };
enum { k_iClientSharedConnectionCallbacks = 4900 };
enum { k_ISteamParentalSettingsCallbacks = 5000 };
enum { k_iClientShaderCallbacks = 5100 };

#ifdef STEAM_CALLBACK_INSPECTION_ENABLED

#define DEFINE_CALLBACK( callbackname, callbackid ) \
struct callbackname { \
	typedef callbackname SteamCallback_t; \
	enum { k_iCallback = callbackid }; \
	static callbackname *GetNullPointer() { return 0; } \
	static const char *GetCallbackName() { return #callbackname; } \
	static uint32  GetCallbackID() { return callbackname::k_iCallback; }

#define CALLBACK_MEMBER( varidx, vartype, varname ) \
	public: vartype varname ; \
	static void GetMemberVar_##varidx( unsigned int &varOffset, unsigned int &varSize, uint32 &varCount, const char **pszName, const char **pszType ) { \
			varOffset = (unsigned int)(size_t)&GetNullPointer()->varname; \
			varSize = sizeof( vartype ); \
			varCount = 1; \
			*pszName = #varname; *pszType = #vartype; }

#define CALLBACK_ARRAY( varidx, vartype, varname, varcount ) \
	public: vartype varname [ varcount ]; \
	static void GetMemberVar_##varidx( unsigned int &varOffset, unsigned int &varSize, uint32 &varCount, const char **pszName, const char **pszType ) { \
	varOffset = (unsigned int)(size_t)&GetNullPointer()->varname[0]; \
	varSize = sizeof( vartype ); \
	varCount = varcount; \
	*pszName = #varname; *pszType = #vartype; }


#define END_CALLBACK_INTERNAL_BEGIN( numvars )  \
	static uint32  GetNumMemberVariables() { return numvars; } \
	static bool    GetMemberVariable( uint32 index, uint32 &varOffset, uint32 &varSize,  uint32 &varCount, const char **pszName, const char **pszType ) { \
	switch ( index ) { default : return false;


#define END_CALLBACK_INTERNAL_SWITCH( varidx ) case varidx : GetMemberVar_##varidx( varOffset, varSize, varCount, pszName, pszType ); return true;

#define END_CALLBACK_INTERNAL_END() }; } };

#define END_DEFINE_CALLBACK_0() \
	static uint32  GetNumMemberVariables() { return 0; } \
	static bool    GetMemberVariable( uint32 index, uint32 &varOffset, uint32 &varSize,  uint32 &varCount, const char **pszName, const char **pszType ) { REFERENCE( pszType ); REFERENCE( pszName ); REFERENCE( varCount ); REFERENCE( varSize ); REFERENCE( varOffset ); REFERENCE( index ); return false; } \
	};
	
#else

#define DEFINE_CALLBACK( callbackname, callbackid )	struct callbackname { typedef callbackname SteamCallback_t; enum { k_iCallback = callbackid };
#define CALLBACK_MEMBER( varidx, vartype, varname )	public: vartype varname ; 
#define CALLBACK_ARRAY( varidx, vartype, varname, varcount ) public: vartype varname [ varcount ];
#define END_CALLBACK_INTERNAL_BEGIN( numvars )  
#define END_CALLBACK_INTERNAL_SWITCH( varidx )
#define END_CALLBACK_INTERNAL_END()					};
#define END_DEFINE_CALLBACK_0()						};

#endif

#define END_DEFINE_CALLBACK_1() \
	END_CALLBACK_INTERNAL_BEGIN( 1 ) \
	END_CALLBACK_INTERNAL_SWITCH( 0 ) \
	END_CALLBACK_INTERNAL_END()

#define END_DEFINE_CALLBACK_2() \
	END_CALLBACK_INTERNAL_BEGIN( 2 ) \
	END_CALLBACK_INTERNAL_SWITCH( 0 ) \
	END_CALLBACK_INTERNAL_SWITCH( 1 ) \
	END_CALLBACK_INTERNAL_END()

#define END_DEFINE_CALLBACK_3() \
	END_CALLBACK_INTERNAL_BEGIN( 3 ) \
	END_CALLBACK_INTERNAL_SWITCH( 0 ) \
	END_CALLBACK_INTERNAL_SWITCH( 1 ) \
	END_CALLBACK_INTERNAL_SWITCH( 2 ) \
	END_CALLBACK_INTERNAL_END()

#define END_DEFINE_CALLBACK_4() \
	END_CALLBACK_INTERNAL_BEGIN( 4 ) \
	END_CALLBACK_INTERNAL_SWITCH( 0 ) \
	END_CALLBACK_INTERNAL_SWITCH( 1 ) \
	END_CALLBACK_INTERNAL_SWITCH( 2 ) \
	END_CALLBACK_INTERNAL_SWITCH( 3 ) \
	END_CALLBACK_INTERNAL_END()

#define END_DEFINE_CALLBACK_5() \
	END_CALLBACK_INTERNAL_BEGIN( 4 ) \
	END_CALLBACK_INTERNAL_SWITCH( 0 ) \
	END_CALLBACK_INTERNAL_SWITCH( 1 ) \
	END_CALLBACK_INTERNAL_SWITCH( 2 ) \
	END_CALLBACK_INTERNAL_SWITCH( 3 ) \
	END_CALLBACK_INTERNAL_SWITCH( 4 ) \
	END_CALLBACK_INTERNAL_END()


#define END_DEFINE_CALLBACK_6() \
	END_CALLBACK_INTERNAL_BEGIN( 6 ) \
	END_CALLBACK_INTERNAL_SWITCH( 0 ) \
	END_CALLBACK_INTERNAL_SWITCH( 1 ) \
	END_CALLBACK_INTERNAL_SWITCH( 2 ) \
	END_CALLBACK_INTERNAL_SWITCH( 3 ) \
	END_CALLBACK_INTERNAL_SWITCH( 4 ) \
	END_CALLBACK_INTERNAL_SWITCH( 5 ) \
	END_CALLBACK_INTERNAL_END()

#define END_DEFINE_CALLBACK_7() \
	END_CALLBACK_INTERNAL_BEGIN( 7 ) \
	END_CALLBACK_INTERNAL_SWITCH( 0 ) \
	END_CALLBACK_INTERNAL_SWITCH( 1 ) \
	END_CALLBACK_INTERNAL_SWITCH( 2 ) \
	END_CALLBACK_INTERNAL_SWITCH( 3 ) \
	END_CALLBACK_INTERNAL_SWITCH( 4 ) \
	END_CALLBACK_INTERNAL_SWITCH( 5 ) \
	END_CALLBACK_INTERNAL_SWITCH( 6 ) \
	END_CALLBACK_INTERNAL_END()

#define END_DEFINE_CALLBACK_8() \
	END_CALLBACK_INTERNAL_BEGIN( 8 ) \
	END_CALLBACK_INTERNAL_SWITCH( 0 ) \
	END_CALLBACK_INTERNAL_SWITCH( 1 ) \
	END_CALLBACK_INTERNAL_SWITCH( 2 ) \
	END_CALLBACK_INTERNAL_SWITCH( 3 ) \
	END_CALLBACK_INTERNAL_SWITCH( 4 ) \
	END_CALLBACK_INTERNAL_SWITCH( 5 ) \
	END_CALLBACK_INTERNAL_SWITCH( 6 ) \
	END_CALLBACK_INTERNAL_SWITCH( 7 ) \
	END_CALLBACK_INTERNAL_END()

#define END_DEFINE_CALLBACK_9() \
	END_CALLBACK_INTERNAL_BEGIN( 9 ) \
	END_CALLBACK_INTERNAL_SWITCH( 0 ) \
	END_CALLBACK_INTERNAL_SWITCH( 1 ) \
	END_CALLBACK_INTERNAL_SWITCH( 2 ) \
	END_CALLBACK_INTERNAL_SWITCH( 3 ) \
	END_CALLBACK_INTERNAL_SWITCH( 4 ) \
	END_CALLBACK_INTERNAL_SWITCH( 5 ) \
	END_CALLBACK_INTERNAL_SWITCH( 6 ) \
	END_CALLBACK_INTERNAL_SWITCH( 7 ) \
	END_CALLBACK_INTERNAL_SWITCH( 8 ) \
	END_CALLBACK_INTERNAL_END()

#define END_DEFINE_CALLBACK_10() \
	END_CALLBACK_INTERNAL_BEGIN( 10 ) \
	END_CALLBACK_INTERNAL_SWITCH( 0 ) \
	END_CALLBACK_INTERNAL_SWITCH( 1 ) \
	END_CALLBACK_INTERNAL_SWITCH( 2 ) \
	END_CALLBACK_INTERNAL_SWITCH( 3 ) \
	END_CALLBACK_INTERNAL_SWITCH( 4 ) \
	END_CALLBACK_INTERNAL_SWITCH( 5 ) \
	END_CALLBACK_INTERNAL_SWITCH( 6 ) \
	END_CALLBACK_INTERNAL_SWITCH( 7 ) \
	END_CALLBACK_INTERNAL_SWITCH( 8 ) \
	END_CALLBACK_INTERNAL_SWITCH( 9 ) \
	END_CALLBACK_INTERNAL_END()

#define END_DEFINE_CALLBACK_11() \
	END_CALLBACK_INTERNAL_BEGIN( 11 ) \
	END_CALLBACK_INTERNAL_SWITCH( 0 ) \
	END_CALLBACK_INTERNAL_SWITCH( 1 ) \
	END_CALLBACK_INTERNAL_SWITCH( 2 ) \
	END_CALLBACK_INTERNAL_SWITCH( 3 ) \
	END_CALLBACK_INTERNAL_SWITCH( 4 ) \
	END_CALLBACK_INTERNAL_SWITCH( 5 ) \
	END_CALLBACK_INTERNAL_SWITCH( 6 ) \
	END_CALLBACK_INTERNAL_SWITCH( 7 ) \
	END_CALLBACK_INTERNAL_SWITCH( 8 ) \
	END_CALLBACK_INTERNAL_SWITCH( 9 ) \
	END_CALLBACK_INTERNAL_SWITCH( 10 ) \
	END_CALLBACK_INTERNAL_END()

#define END_DEFINE_CALLBACK_12() \
	END_CALLBACK_INTERNAL_BEGIN( 12 ) \
	END_CALLBACK_INTERNAL_SWITCH( 0 ) \
	END_CALLBACK_INTERNAL_SWITCH( 1 ) \
	END_CALLBACK_INTERNAL_SWITCH( 2 ) \
	END_CALLBACK_INTERNAL_SWITCH( 3 ) \
	END_CALLBACK_INTERNAL_SWITCH( 4 ) \
	END_CALLBACK_INTERNAL_SWITCH( 5 ) \
	END_CALLBACK_INTERNAL_SWITCH( 6 ) \
	END_CALLBACK_INTERNAL_SWITCH( 7 ) \
	END_CALLBACK_INTERNAL_SWITCH( 8 ) \
	END_CALLBACK_INTERNAL_SWITCH( 9 ) \
	END_CALLBACK_INTERNAL_SWITCH( 10 ) \
	END_CALLBACK_INTERNAL_SWITCH( 11 ) \
	END_CALLBACK_INTERNAL_END()

#define END_DEFINE_CALLBACK_13() \
	END_CALLBACK_INTERNAL_BEGIN( 13 ) \
	END_CALLBACK_INTERNAL_SWITCH( 0 ) \
	END_CALLBACK_INTERNAL_SWITCH( 1 ) \
	END_CALLBACK_INTERNAL_SWITCH( 2 ) \
	END_CALLBACK_INTERNAL_SWITCH( 3 ) \
	END_CALLBACK_INTERNAL_SWITCH( 4 ) \
	END_CALLBACK_INTERNAL_SWITCH( 5 ) \
	END_CALLBACK_INTERNAL_SWITCH( 6 ) \
	END_CALLBACK_INTERNAL_SWITCH( 7 ) \
	END_CALLBACK_INTERNAL_SWITCH( 8 ) \
	END_CALLBACK_INTERNAL_SWITCH( 9 ) \
	END_CALLBACK_INTERNAL_SWITCH( 10 ) \
	END_CALLBACK_INTERNAL_SWITCH( 11 ) \
	END_CALLBACK_INTERNAL_SWITCH( 12 ) \
	END_CALLBACK_INTERNAL_END()

#define END_DEFINE_CALLBACK_14() \
	END_CALLBACK_INTERNAL_BEGIN( 14 ) \
	END_CALLBACK_INTERNAL_SWITCH( 0 ) \
	END_CALLBACK_INTERNAL_SWITCH( 1 ) \
	END_CALLBACK_INTERNAL_SWITCH( 2 ) \
	END_CALLBACK_INTERNAL_SWITCH( 3 ) \
	END_CALLBACK_INTERNAL_SWITCH( 4 ) \
	END_CALLBACK_INTERNAL_SWITCH( 5 ) \
	END_CALLBACK_INTERNAL_SWITCH( 6 ) \
	END_CALLBACK_INTERNAL_SWITCH( 7 ) \
	END_CALLBACK_INTERNAL_SWITCH( 8 ) \
	END_CALLBACK_INTERNAL_SWITCH( 9 ) \
	END_CALLBACK_INTERNAL_SWITCH( 10 ) \
	END_CALLBACK_INTERNAL_SWITCH( 11 ) \
	END_CALLBACK_INTERNAL_SWITCH( 12 ) \
	END_CALLBACK_INTERNAL_SWITCH( 13 ) \
	END_CALLBACK_INTERNAL_END()

#endif
