/*
===========================================================================
Copyright (C) 1999-2005 Id Software, Inc.
Copyright (C) 2000-2006 Tim Angus

This file is part of Tremulous.

Tremulous is free software; you can redistribute it
and/or modify it under the terms of the GNU General Public License as
published by the Free Software Foundation; either version 2 of the License,
or (at your option) any later version.

Tremulous is distributed in the hope that it will be
useful, but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with Tremulous; if not, write to the Free Software
Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
===========================================================================
*/

#include "g_local.h"

//==========================================================

/*QUAKED target_delay (1 0 0) (-8 -8 -8) (8 8 8)
"wait" seconds to pause before firing targets.
"random" delay variance, total delay = delay +/- random seconds
*/
void Think_Target_Delay(gentity_t * ent)
{
#ifdef G_LUA
	// Lua API callbacks
	if(ent->luaTrigger)
	{
		if(ent->activator)
		{
			G_LuaHook_EntityTrigger(ent->luaTrigger, ent->s.number, ent->activator->s.number);
		}
		else
		{
			G_LuaHook_EntityTrigger(ent->luaTrigger, ent->s.number, ENTITYNUM_WORLD);
		}
	}
#endif
	G_UseTargets(ent, ent->activator);
}

void Use_Target_Delay(gentity_t * ent, gentity_t * other, gentity_t * activator)
{
	ent->nextthink = level.time + (ent->wait + ent->random * crandom()) * 1000;
	ent->think = Think_Target_Delay;
	ent->activator = activator;
}

void SP_target_delay(gentity_t * ent)
{
	// check delay for backwards compatability
	if(!G_SpawnFloat("delay", "0", &ent->wait))
		G_SpawnFloat("wait", "1", &ent->wait);

	if(!ent->wait)
		ent->wait = 1;

	ent->use = Use_Target_Delay;
}


//==========================================================

/*QUAKED target_score (1 0 0) (-8 -8 -8) (8 8 8)
"count" number of points to add, default 1

The activator is given this many points.
*/
void Use_Target_Score(gentity_t * ent, gentity_t * other, gentity_t * activator)
{
	AddScore(activator, ent->count);
}

void SP_target_score(gentity_t * ent)
{
	if(!ent->count)
		ent->count = 1;

	ent->use = Use_Target_Score;
}


//==========================================================

/*QUAKED target_print (1 0 0) (-8 -8 -8) (8 8 8) humanteam alienteam private
"message" text to print
If "private", only the activator gets the message.  If no checks, all clients get the message.
*/
void Use_Target_Print(gentity_t * ent, gentity_t * other, gentity_t * activator)
{
	if(activator->client && (ent->spawnflags & 4))
	{
		G_SendCommandFromServer(activator - g_entities, va("cp \"%s\"", ent->message));
		return;
	}

	if(ent->spawnflags & 3)
	{
		if(ent->spawnflags & 1)
			G_TeamCommand(PTE_HUMANS, va("cp \"%s\"", ent->message));
		if(ent->spawnflags & 2)
			G_TeamCommand(PTE_ALIENS, va("cp \"%s\"", ent->message));

		return;
	}

	G_SendCommandFromServer(-1, va("cp \"%s\"", ent->message));
}

void SP_target_print(gentity_t * ent)
{
	ent->use = Use_Target_Print;
}


//==========================================================


/*QUAKED target_speaker (1 0 0) (-8 -8 -8) (8 8 8) looped-on looped-off global activator
"noise"   wav file to play

A global sound will play full volume throughout the level.
Activator sounds will play on the player that activated the target.
Global and activator sounds can't be combined with looping.
Normal sounds play each time the target is used.
Looped sounds will be toggled by use functions.
Multiple identical looping sounds will just increase volume without any speed cost.
"wait" : Seconds between auto triggerings, 0 = don't auto trigger
"random"  wait variance, default is 0
*/
void Use_Target_Speaker(gentity_t * ent, gentity_t * other, gentity_t * activator)
{
	if(ent->soundLooping)
	{							// looping sound toggles
		if(ent->s.loopSound)
			ent->s.loopSound = 0;	// turn it off
		else
			ent->s.loopSound = ent->soundIndex;	// start it
	}
	else
	{
		// normal sound
		if(ent->soundActivator)
			G_AddEvent(activator, EV_GENERAL_SOUND, ent->soundIndex);
		else if(ent->soundGlobal)
			G_AddEvent(ent, EV_GLOBAL_SOUND, ent->soundIndex);
		else
			G_AddEvent(ent, EV_GENERAL_SOUND, ent->soundIndex);
	}
}

void SP_target_speaker(gentity_t * ent)
{
	char            buffer[MAX_QPATH];
	char           *s;

	if(G_SpawnString("s_sound", "NOSOUND", &s))
	{
		G_SpawnBoolean("s_looping", "0", &ent->soundLooping);
		G_SpawnBoolean("s_waitfortrigger", "0", &ent->soundWaitForTrigger);
		G_SpawnBoolean("s_global", "0", &ent->soundGlobal);
		G_SpawnBoolean("s_activator", "0", &ent->soundActivator);
		G_SpawnFloat("wait", "0", &ent->wait);
		G_SpawnFloat("random", "0", &ent->random);
	}
	else if(G_SpawnString("s_shader", "NOSOUND", &s))
	{
		// Doom3 compatibility mode
		G_SpawnBoolean("s_looping", "0", &ent->soundLooping);
		G_SpawnBoolean("s_waitfortrigger", "0", &ent->soundWaitForTrigger);
		G_SpawnBoolean("s_global", "0", &ent->soundGlobal);
		G_SpawnBoolean("s_activator", "0", &ent->soundActivator);
		G_SpawnFloat("wait", "0", &ent->wait);
		G_SpawnFloat("random", "0", &ent->random);
	}
	else if(G_SpawnString("noise", "NOSOUND", &s))
	{
		// Q3A compatibility mode
		ent->soundLooping = ent->spawnflags & 1 ? qtrue : qfalse;
		ent->soundWaitForTrigger = ent->spawnflags & 2 ? qtrue : qfalse;
		ent->soundGlobal = ent->spawnflags & 4 ? qtrue : qfalse;
		ent->soundActivator = ent->spawnflags & 8 ? qtrue : qfalse;
	}
	else
	{
		//G_Error("speaker without a noise key at %s", vtos(ent->s.origin));
		G_Printf(S_COLOR_YELLOW "WARNING: speaker '%s' without a noise key at %s", ent->name, vtos(ent->s.origin));
	}

	// force all client reletive sounds to be "activator" speakers that
	// play on the entity that activates it
	if(s[0] == '*')
	{
		ent->soundActivator = qtrue;
	}

	Q_strncpyz(buffer, s, sizeof(buffer));
	Com_DefaultExtension(buffer, sizeof(buffer), ".wav");

	ent->soundIndex = G_SoundIndex(buffer);

	// a repeating speaker can be done completely client side
	ent->s.eType = ET_SPEAKER;
	ent->s.eventParm = ent->soundIndex;
	ent->s.frame = ent->wait * 10;
	ent->s.clientNum = ent->random * 10;


	// check for prestarted looping sound
	if(ent->soundLooping && !ent->soundWaitForTrigger)
	{
		ent->s.loopSound = ent->soundIndex;
	}

	ent->use = Use_Target_Speaker;

	if(ent->soundGlobal)
	{
		ent->r.svFlags |= SVF_BROADCAST;
	}

	VectorCopy(ent->s.origin, ent->s.pos.trBase);

	// must link the entity so we get areas and clusters so
	// the server can determine who to send updates to
	trap_LinkEntity(ent);
}


//==========================================================

void target_teleporter_use(gentity_t * self, gentity_t * other, gentity_t * activator)
{
	gentity_t      *dest;

	if(!activator->client)
		return;

	dest = G_PickTarget(self->target);

	if(!dest)
	{
		G_Printf("Couldn't find teleporter destination\n");
		return;
	}

	TeleportPlayer(activator, dest->s.origin, dest->s.angles);
}

/*QUAKED target_teleporter (1 0 0) (-8 -8 -8) (8 8 8)
The activator will be teleported away.
*/
void SP_target_teleporter(gentity_t * self)
{
	if(!self->name)
		G_Printf("untargeted %s at %s\n", self->classname, vtos(self->s.origin));

	self->use = target_teleporter_use;
}

//==========================================================


/*QUAKED target_relay (.5 .5 .5) (-8 -8 -8) (8 8 8) RED_ONLY BLUE_ONLY RANDOM
This doesn't perform any actions except fire its targets.
The activator can be forced to be from a certain team.
if RANDOM is checked, only one of the targets will be fired, not all of them
*/
void target_relay_use(gentity_t * self, gentity_t * other, gentity_t * activator)
{
	if((self->spawnflags & 1) && activator->client && activator->client->ps.stats[STAT_PTEAM] != PTE_HUMANS)
		return;

	if((self->spawnflags & 2) && activator->client && activator->client->ps.stats[STAT_PTEAM] != PTE_ALIENS)
		return;

	if(self->spawnflags & 4)
	{
		gentity_t      *ent;

		ent = G_PickTarget(self->target);
		if(ent && ent->use)
			ent->use(ent, self, activator);

		return;
	}

	G_UseTargets(self, activator);
}

void SP_target_relay(gentity_t * self)
{
	self->use = target_relay_use;
}


//==========================================================

/*QUAKED target_kill (.5 .5 .5) (-8 -8 -8) (8 8 8)
Kills the activator.
*/
void target_kill_use(gentity_t * self, gentity_t * other, gentity_t * activator)
{
	G_Damage(activator, NULL, NULL, NULL, NULL, 100000, DAMAGE_NO_PROTECTION, MOD_TELEFRAG);
}

void SP_target_kill(gentity_t * self)
{
	self->use = target_kill_use;
}

/*QUAKED target_position (0 0.5 0) (-4 -4 -4) (4 4 4)
Used as a positional target for in-game calculation, like jumppad targets.
*/
void SP_target_position(gentity_t * self)
{
	G_SetOrigin(self, self->s.origin);
}

static void target_location_linkup(gentity_t * ent)
{
	int             i;
	int             n;

	if(level.locationLinked)
		return;

	level.locationLinked = qtrue;

	level.locationHead = NULL;

	trap_SetConfigstring(CS_LOCATIONS, "unknown");

	for(i = 0, ent = g_entities, n = 1; i < level.num_entities; i++, ent++)
	{
		if(ent->classname && !Q_stricmp(ent->classname, "target_location"))
		{
			// lets overload some variables!
			ent->health = n;	// use for location marking
			trap_SetConfigstring(CS_LOCATIONS + n, ent->message);
			n++;
			ent->nextTrain = level.locationHead;
			level.locationHead = ent;
		}
	}
	// All linked together now
}

/*QUAKED target_location (0 0.5 0) (-8 -8 -8) (8 8 8)
Set "message" to the name of this location.
Set "count" to 0-7 for color.
0:white 1:red 2:green 3:yellow 4:blue 5:cyan 6:magenta 7:white

Closest target_location in sight used for the location, if none
in site, closest in distance
*/
void SP_target_location(gentity_t * self)
{
	self->think = target_location_linkup;
	self->nextthink = level.time + 200;	// Let them all spawn first

	G_SetOrigin(self, self->s.origin);
}


/*
===============
target_rumble_think
===============
*/
void target_rumble_think(gentity_t * self)
{
	int             i;
	gentity_t      *ent;

	if(self->last_move_time < level.time)
		self->last_move_time = level.time + 0.5;

	for(i = 0, ent = g_entities + i; i < level.num_entities; i++, ent++)
	{
		if(!ent->inuse)
			continue;

		if(!ent->client)
			continue;

		if(ent->client->ps.groundEntityNum == ENTITYNUM_NONE)
			continue;

		ent->client->ps.groundEntityNum = ENTITYNUM_NONE;
		ent->client->ps.velocity[0] += crandom() * 150;
		ent->client->ps.velocity[1] += crandom() * 150;
		ent->client->ps.velocity[2] = self->speed;
	}

	if(level.time < self->timestamp)
		self->nextthink = level.time + FRAMETIME;
}

/*
===============
target_rumble_use
===============
*/
void target_rumble_use(gentity_t * self, gentity_t * other, gentity_t * activator)
{
	self->timestamp = level.time + (self->count * FRAMETIME);
	self->nextthink = level.time + FRAMETIME;
	self->activator = activator;
	self->last_move_time = 0;
}

/*
===============
SP_target_rumble
===============
*/
void SP_target_rumble(gentity_t * self)
{
	if(!self->name)
	{
		G_Printf(S_COLOR_YELLOW "WARNING: untargeted %s at %s\n", self->classname, vtos(self->s.origin));
	}

	if(!self->count)
		self->count = 10;

	if(!self->speed)
		self->speed = 100;

	self->think = target_rumble_think;
	self->use = target_rumble_use;
}

/*
===============
target_alien_win_use
===============
*/
void target_alien_win_use(gentity_t * self, gentity_t * other, gentity_t * activator)
{
	level.uncondAlienWin = qtrue;
}

/*
===============
SP_target_alien_win
===============
*/
void SP_target_alien_win(gentity_t * self)
{
	self->use = target_alien_win_use;
}

/*
===============
target_human_win_use
===============
*/
void target_human_win_use(gentity_t * self, gentity_t * other, gentity_t * activator)
{
	level.uncondHumanWin = qtrue;
}

/*
===============
SP_target_human_win
===============
*/
void SP_target_human_win(gentity_t * self)
{
	self->use = target_human_win_use;
}
