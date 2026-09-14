/*
 = *==========================================================================
 Copyright (C) 2006 Neil Toronto.  Unlagged 2.01 (GPL2+).
 ===========================================================================
 */
#include "cg_local.h"

void CG_ShotgunPattern( vec3_t origin, vec3_t origin2, int seed, int otherEntNum );
void CG_Bullet( vec3_t end, int sourceEntityNum, vec3_t normal, qboolean flesh, int fleshEntityNum );

#define MACHINEGUN_SPREAD	200
#define CHAINGUN_SPREAD		600

void CG_PredictWeaponEffects( centity_t *cent ) {
    vec3_t		muzzlePoint, forward, right, up, endPoint;
    trace_t		trace;
    entityState_t *ent = &cent->currentState;

    if ( cg.demoPlayback )
        return;
    if ( ent->number != cg.predictedPlayerState.clientNum )
        return;
    if ( !cgs.g_unlagged )
        return;
    if ( !cg_delag.integer )
        return;

    VectorCopy( cg.predictedPlayerState.origin, muzzlePoint );
    muzzlePoint[2] += cg.predictedPlayerState.viewheight;
    AngleVectors( cg.predictedPlayerState.viewangles, forward, right, up );
    VectorMA( muzzlePoint, 14, forward, muzzlePoint );

    /* rail */
    if ( ent->weapon == WP_RAILGUN && ( cg_delag.integer & 1 || cg_delag.integer & 16 ) ) {
        VectorMA( muzzlePoint, 8192, forward, endPoint );
        CG_Trace( &trace, muzzlePoint, vec3_origin, vec3_origin, endPoint,
                  cg.predictedPlayerState.clientNum, MASK_SHOT );
        VectorMA( muzzlePoint, 4, right, muzzlePoint );
        VectorMA( muzzlePoint, -1, up, muzzlePoint );
        CG_RailTrail( &cgs.clientinfo[ent->number], muzzlePoint, trace.endpos );
        if ( !( trace.surfaceFlags & SURF_NOIMPACT ) && trace.entityNum != ENTITYNUM_NONE )
            CG_MissileHitWall( WP_RAILGUN, cg.predictedPlayerState.clientNum,
                               trace.endpos, trace.plane.normal, IMPACTSOUND_DEFAULT );
            return;
    }

    /* shotgun */
    if ( ent->weapon == WP_SHOTGUN && ( cg_delag.integer & 1 || cg_delag.integer & 2 ) ) {
        CG_ShotgunPattern( muzzlePoint, forward, cg.predictedPlayerState.commandTime,
                           cg.predictedPlayerState.clientNum );
        return;
    }

    /* MG / chaingun */
    if ( ( ent->weapon == WP_MACHINEGUN || ent->weapon == WP_CHAINGUN ) &&
        ( cg_delag.integer & 1 || cg_delag.integer & 4 ) ) {
        VectorMA( muzzlePoint, 8192, forward, endPoint );
    CG_Trace( &trace, muzzlePoint, vec3_origin, vec3_origin, endPoint,
              cg.predictedPlayerState.clientNum, MASK_SHOT );
    CG_Bullet( trace.endpos, cg.predictedPlayerState.clientNum, trace.plane.normal,
               (trace.entityNum < MAX_CLIENTS), trace.entityNum );
        }
}
