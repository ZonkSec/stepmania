/*
-----------------------------------------------------------------------------
 Class: ScreenUnlock

 Desc: See header.

 Copyright (c) 2003 by the person(s) listed below.  All rights reserved.
	Andrew Wong
-----------------------------------------------------------------------------
*/

#include "global.h"
#include "PrefsManager.h"
#include "ScreenUnlock.h"
#include "ThemeManager.h"
#include "GameState.h"
#include "RageLog.h"
#include "UnlockSystem.h"
#include "SongManager.h"

ScreenUnlock::ScreenUnlock() : ScreenAttract("ScreenUnlock")
{
	LOG->Trace("ScreenUnlock::ScreenUnlock()");
	PointsUntilNextUnlock.LoadFromFont( THEME->GetPathToF("Common normal") );
	PointsUntilNextUnlock.SetHorizAlign( Actor::align_left );

	CString sDP = ssprintf( "%d", (int)GAMESTATE->m_pUnlockingSys->DancePointsUntilNextUnlock() );
	CString sAP = ssprintf( "%d", (int)GAMESTATE->m_pUnlockingSys->ArcadePointsUntilNextUnlock() );
	CString sSP = ssprintf( "%d", (int)GAMESTATE->m_pUnlockingSys->SongPointsUntilNextUnlock() );

	CString PointDisplay = THEME->GetMetric("ScreenUnlock", "TypeOfPointsToDisplay");
	
	CString IconCommand = 
		THEME->GetMetric("ScreenUnlock", "UnlockIconCommand");

	for(int i=1; i <= THEME->GetMetricI("ScreenUnlock", "NumUnlocks"); i++)
	{
		// new unlock graphic
		Unlocks[i].Load( THEME->GetPathToG(ssprintf("ScreenUnlock icon %d", i)) );

		Unlocks[i].SetName( ssprintf("Unlock%d",i) );
		SET_XY( Unlocks[i] );

		Song *pSong = SONGMAN->FindSong("", THEME->GetMetric("ScreenUnlock", ssprintf("Unlock%dSong", i)) );
		if( pSong == NULL )
			continue;

		Unlocks[i].Command(IconCommand);

		const bool SongIsLocked = GAMESTATE->m_pUnlockingSys->SongIsLocked( pSong );
		if ( !SongIsLocked )
			this->AddChild(&Unlocks[i]);
	}

	// No negative numbers
	if( sDP.Left(1) == "-" ) 
		sDP = "*";

	PointsUntilNextUnlock.SetName( "PointsDisplay" );
	
	if (PointDisplay == "DP" || PointDisplay == "Dance")
		PointsUntilNextUnlock.SetText( sDP );

	if (PointDisplay == "AP" || PointDisplay == "Arcade")
		PointsUntilNextUnlock.SetText( sAP );

	if (PointDisplay == "SP" || PointDisplay == "Song")
		PointsUntilNextUnlock.SetText( sSP );

	PointsUntilNextUnlock.SetZoom( THEME->GetMetricF("ScreenUnlock","PointsZoom") );
	SET_XY( PointsUntilNextUnlock );
	this->AddChild( &PointsUntilNextUnlock );

	this->ClearMessageQueue( SM_BeginFadingOut );	// ignore ScreenAttract's SecsToShow

	this->PostScreenMessage( SM_BeginFadingOut, 
		THEME->GetMetricI("ScreenUnlock", "TimeToDisplay") );

}
