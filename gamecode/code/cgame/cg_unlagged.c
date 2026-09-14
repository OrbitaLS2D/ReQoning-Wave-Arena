/*
===========================================================================
Copyright (C) 2006 Neil Toronto.  Unlagged 2.01 (GPL2+).
===========================================================================
*/
#include "cg_local.h"

/* RWA-UNLAGGED: CLIENT — predicted rail/sg/mg + bbox draw. Entire file. */

void CG_ShotgunPattern( vec3_t origin, vec3_t origin2, int seed, int otherEntNum );

void CG_PredictWeaponEffects( centity_t *cent ) {
	vec3_t		muzzlePoint, forward, right, up, endPoint;
	trace_t		trace;
	entityState_t *ent = &cent->currentState;

	if ( cg.demoPlayback ) {
		return;
	}
	if ( ent->number != cg.predictedPlayerState.clientNum ) {
		return;
	}
	if ( !cgs.g_unlagged ) {
		return;
	}
	if ( !cg_delag.integer ) {
		return;
	}

	VectorCopy( cg.predictedPlayerState.origin, muzzlePoint );
	muzzlePoint[2] += cg.predictedPlayerState.viewheight;
	AngleVectors( cg.predictedPlayerState.viewangles, forward, right, up );
	VectorMA( muzzlePoint, 14, forward, muzzlePoint );

	if ( ent->weapon == WP_RAILGUN && ( cg_delag.integer & 1 || cg_delag.integer & 16 ) ) {
		VectorMA( muzzlePoint, 8192, forward, endPoint );
		CG_Trace( &trace, muzzlePoint, vec3_origin, vec3_origin, endPoint,
			cg.predictedPlayerState.clientNum, MASK_SHOT );
		VectorMA( muzzlePoint, 4, right, muzzlePoint );
		VectorMA( muzzlePoint, -1, up, muzzlePoint );
		CG_RailTrail( &cgs.clientinfo[ent->number], muzzlePoint, trace.endpos );
		if ( !( trace.surfaceFlags & SURF_NOIMPACT ) ) {
			CG_MissileHitWall( WP_RAILGUN, cg.predictedPlayerState.clientNum,
				trace.endpos, trace.plane.normal, IMPACTSOUND_DEFAULT );
		}
		return;
	}

	if ( ent->weapon == WP_SHOTGUN && ( cg_delag.integer & 1 || cg_delag.integer & 2 ) ) {
		CG_ShotgunPattern( muzzlePoint, forward, cg.predictedPlayerState.commandTime,
			cg.predictedPlayerState.clientNum );
		return;
	}

	if ( ( ent->weapon == WP_MACHINEGUN || ent->weapon == WP_CHAINGUN ) &&
		( cg_delag.integer & 1 || cg_delag.integer & 4 ) ) {
		VectorMA( muzzlePoint, 8192, forward, endPoint );
		CG_Trace( &trace, muzzlePoint, vec3_origin, vec3_origin, endPoint,
			cg.predictedPlayerState.clientNum, MASK_SHOT );
		CG_Bullet( trace.endpos, cg.predictedPlayerState.clientNum, trace.plane.normal,
			( trace.entityNum < MAX_CLIENTS ), trace.entityNum );
	}
}

void CG_AddBoundingBox( centity_t *cent ) {
	polyVert_t verts[4];
	clientInfo_t *ci;
	int i;
	vec3_t mins = { -15, -15, -24 };
	vec3_t maxs = { 15, 15, 32 };
	float extx, exty, extz;
	vec3_t corners[8];
	qhandle_t bboxShader, bboxShader_nocull;

	if ( !cg_drawBBox.integer ) {
		return;
	}
	if ( cent->currentState.eFlags & EF_DEAD ) {
		return;
	}
	if ( cent->currentState.number == cg.predictedPlayerState.clientNum &&
		!cg.renderingThirdPerson ) {
		return;
	}

	bboxShader = trap_R_RegisterShader( "bbox" );
	bboxShader_nocull = trap_R_RegisterShader( "bbox_nocull" );
	if ( !bboxShader || !bboxShader_nocull ) {
		return;
	}

	ci = &cgs.clientinfo[cent->currentState.clientNum];

	if ( cent->currentState.number == cg.predictedPlayerState.clientNum ) {
		maxs[2] = cg.predictedPlayerState.viewheight + 6;
	} else {
		int x, zd, zu;
		x = (cent->currentState.solid & 255);
		zd = ((cent->currentState.solid >> 8) & 255);
		zu = ((cent->currentState.solid >> 16) & 255) - 32;
		mins[0] = mins[1] = -x;
		maxs[0] = maxs[1] = x;
		mins[2] = -zd;
		maxs[2] = zu;
	}

	extx = maxs[0] - mins[0];
	exty = maxs[1] - mins[1];
	extz = maxs[2] - mins[2];

	verts[0].st[0] = 0; verts[0].st[1] = 0;
	verts[1].st[0] = 0; verts[1].st[1] = 1;
	verts[2].st[0] = 1; verts[2].st[1] = 1;
	verts[3].st[0] = 1; verts[3].st[1] = 0;

	for ( i = 0; i < 4; i++ ) {
		if ( ci->team == TEAM_RED ) {
			verts[i].modulate[0] = 160; verts[i].modulate[1] = 0; verts[i].modulate[2] = 0;
		} else if ( ci->team == TEAM_BLUE ) {
			verts[i].modulate[0] = 0; verts[i].modulate[1] = 0; verts[i].modulate[2] = 192;
		} else {
			verts[i].modulate[0] = 0; verts[i].modulate[1] = 200; verts[i].modulate[2] = 0;
		}
		verts[i].modulate[3] = 255;
	}

	VectorAdd( cent->lerpOrigin, maxs, corners[3] );
	VectorCopy( corners[3], corners[2] ); corners[2][0] -= extx;
	VectorCopy( corners[2], corners[1] ); corners[1][1] -= exty;
	VectorCopy( corners[1], corners[0] ); corners[0][0] += extx;
	for ( i = 0; i < 4; i++ ) {
		VectorCopy( corners[i], corners[i + 4] );
		corners[i + 4][2] -= extz;
	}

	VectorCopy( corners[0], verts[0].xyz );
	VectorCopy( corners[1], verts[1].xyz );
	VectorCopy( corners[2], verts[2].xyz );
	VectorCopy( corners[3], verts[3].xyz );
	trap_R_AddPolyToScene( bboxShader, 4, verts );

	VectorCopy( corners[7], verts[0].xyz );
	VectorCopy( corners[6], verts[1].xyz );
	VectorCopy( corners[5], verts[2].xyz );
	VectorCopy( corners[4], verts[3].xyz );
	trap_R_AddPolyToScene( bboxShader, 4, verts );

	VectorCopy( corners[3], verts[0].xyz );
	VectorCopy( corners[2], verts[1].xyz );
	VectorCopy( corners[6], verts[2].xyz );
	VectorCopy( corners[7], verts[3].xyz );
	trap_R_AddPolyToScene( bboxShader_nocull, 4, verts );

	VectorCopy( corners[2], verts[0].xyz );
	VectorCopy( corners[1], verts[1].xyz );
	VectorCopy( corners[5], verts[2].xyz );
	VectorCopy( corners[6], verts[3].xyz );
	trap_R_AddPolyToScene( bboxShader_nocull, 4, verts );

	VectorCopy( corners[0], verts[0].xyz );
	VectorCopy( corners[3], verts[1].xyz );
	VectorCopy( corners[7], verts[2].xyz );
	VectorCopy( corners[4], verts[3].xyz );
	trap_R_AddPolyToScene( bboxShader_nocull, 4, verts );

	VectorCopy( corners[1], verts[0].xyz );
	VectorCopy( corners[0], verts[1].xyz );
	VectorCopy( corners[4], verts[2].xyz );
	VectorCopy( corners[5], verts[3].xyz );
	trap_R_AddPolyToScene( bboxShader_nocull, 4, verts );
}
