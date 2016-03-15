//--------------------------------------------------------------------------------------
// File: FirewallInstallHelper.h
//
// Copyright (c) Microsoft Corporation. All rights reserved.
//--------------------------------------------------------------------------------------
#include <windows.h>
#include <netfw.h>
#include <msi.h>
#include <msiquery.h>

//--------------------------------------------------------------------------------------
// UNICODE/ANSI define mappings
//--------------------------------------------------------------------------------------
#ifdef UNICODE
#define AddApplicationToExceptionList AddApplicationToExceptionListW
#define RemoveApplicationFromExceptionList RemoveApplicationFromExceptionListW
#define CanLaunchMultiplayerGame CanLaunchMultiplayerGameW
#else
#define AddApplicationToExceptionList AddApplicationToExceptionListA
#define RemoveApplicationFromExceptionList RemoveApplicationFromExceptionListA
#define CanLaunchMultiplayerGame CanLaunchMultiplayerGameW
#endif


//--------------------------------------------------------------------------------------
// Adds application from firewall exception list
//--------------------------------------------------------------------------------------
STDAPI AddApplicationToExceptionListW(const WCHAR * strGameExeFullPath, const WCHAR * strFriendlyAppName);
STDAPI AddApplicationToExceptionListA(const CHAR * strGameExeFullPath, const CHAR * strFriendlyAppName);

//--------------------------------------------------------------------------------------
// Removes application from firewall exception list
//--------------------------------------------------------------------------------------
STDAPI RemoveApplicationFromExceptionListW(const WCHAR * strGameExeFullPath);
STDAPI RemoveApplicationFromExceptionListA(const CHAR * strGameExeFullPath);

//--------------------------------------------------------------------------------------
// Returns false if the game is not allowed through the firewall
//--------------------------------------------------------------------------------------
BOOL WINAPI CanLaunchMultiplayerGameW(const WCHAR * strGameExeFullPath);
BOOL WINAPI CanLaunchMultiplayerGameA(const CHAR * strGameExeFullPath);

//--------------------------------------------------------------------------------------
// For use during an MSI custom action install. 
// This sets up the CustomActionData properties for the deferred custom actions. 
//--------------------------------------------------------------------------------------
UINT WINAPI SetMSIFirewallProperties(MSIHANDLE hModule);

//--------------------------------------------------------------------------------------
// For use during an MSI custom action install. 
// This adds the game to the Game Explorer
//--------------------------------------------------------------------------------------
UINT WINAPI AddToExceptionListUsingMSI(MSIHANDLE hModule);

//--------------------------------------------------------------------------------------
// For use during an MSI custom action install. 
// This adds the game to the Game Explorer
//--------------------------------------------------------------------------------------
UINT WINAPI RemoveFromExceptionListUsingMSI(MSIHANDLE hModule);
