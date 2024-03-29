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

qboolean G_SpawnString(const char *key, const char *defaultString, char **out)
{
	int             i;

	if(!level.spawning)
	{
		*out = (char *)defaultString;
//    G_Error( "G_SpawnString() called while not spawning" );
	}

	for(i = 0; i < level.numSpawnVars; i++)
	{
		if(!Q_stricmp(key, level.spawnVars[i][0]))
		{
			*out = level.spawnVars[i][1];
			return qtrue;
		}
	}

	*out = (char *)defaultString;
	return qfalse;
}

qboolean G_SpawnFloat(const char *key, const char *defaultString, float *out)
{
	char           *s;
	qboolean        present;

	present = G_SpawnString(key, defaultString, &s);
	*out = atof(s);
	return present;
}

qboolean G_SpawnInt(const char *key, const char *defaultString, int *out)
{
	char           *s;
	qboolean        present;

	present = G_SpawnString(key, defaultString, &s);
	*out = atoi(s);
	return present;
}

qboolean G_SpawnBoolean(const char *key, const char *defaultString, qboolean * out)
{
	char           *s;
	qboolean        present;

	present = G_SpawnString(key, defaultString, &s);

	if(!Q_stricmp(s, "qfalse") || !Q_stricmp(s, "false") || !Q_stricmp(s, "0"))
	{
		*out = qfalse;
	}
	else if(!Q_stricmp(s, "qtrue") || !Q_stricmp(s, "true") || !Q_stricmp(s, "1"))
	{
		*out = qtrue;
	}
	else
	{
		*out = qfalse;
	}
	return present;
}

qboolean G_SpawnVector(const char *key, const char *defaultString, float *out)
{
	char           *s;
	qboolean        present;

	present = G_SpawnString(key, defaultString, &s);
	sscanf(s, "%f %f %f", &out[0], &out[1], &out[2]);
	return present;
}

qboolean G_SpawnVector4(const char *key, const char *defaultString, float *out)
{
	char           *s;
	qboolean        present;

	present = G_SpawnString(key, defaultString, &s);
	sscanf(s, "%f %f %f %f", &out[0], &out[1], &out[2], &out[3]);
	return present;
}



//
// fields are needed for spawning from the entity string
//
typedef enum
{
	F_INT,
	F_FLOAT,
	F_LSTRING,					// string on disk, pointer in memory, TAG_LEVEL
	F_GSTRING,					// string on disk, pointer in memory, TAG_GAME
	F_VECTOR,
	F_VECTOR4,					//TA
	F_ANGLEHACK,
	F_MOVEDIRHACK,
	F_ROTATIONHACK,
	F_ENTITY,					// index on disk, pointer in memory
	F_ITEM,						// index on disk, pointer in memory
	F_CLIENT,					// index on disk, pointer in memory
	F_IGNORE
} fieldtype_t;

typedef struct
{
	char           *name;
	int             ofs;
	fieldtype_t     type;
	int             flags;
} field_t;

field_t         fields[] = {
	{"classname", FOFS(classname), F_LSTRING},
	{"origin", FOFS(s.origin), F_VECTOR},
	{"model", FOFS(model), F_LSTRING},
	{"model2", FOFS(model2), F_LSTRING},
	{"spawnflags", FOFS(spawnflags), F_INT},
	{"speed", FOFS(speed), F_FLOAT},
	{"target", FOFS(target), F_LSTRING},
	{"target0", FOFS(target), F_LSTRING},
	{"name", FOFS(name), F_LSTRING},
	{"targetname", FOFS(name), F_LSTRING},
	{"message", FOFS(message), F_LSTRING},
	{"team", FOFS(team), F_LSTRING},
	{"wait", FOFS(wait), F_FLOAT},
	{"random", FOFS(random), F_FLOAT},
	{"count", FOFS(count), F_INT},
	{"health", FOFS(health), F_INT},
	{"light", 0, F_IGNORE},
	{"dmg", FOFS(damage), F_INT},
	{"angles", FOFS(s.angles), F_VECTOR},
	{"angle", FOFS(s.angles), F_ANGLEHACK},
	{"movedir", FOFS(movedir), F_MOVEDIRHACK},
	{"rotation", FOFS(s.angles), F_ROTATIONHACK},
	//TA
	{"bounce", FOFS(physicsBounce), F_FLOAT},
	{"alpha", FOFS(pos1), F_VECTOR},
	{"radius", FOFS(pos2), F_VECTOR},
	{"acceleration", FOFS(acceleration), F_VECTOR},
	{"animation", FOFS(animation), F_VECTOR4},
	{"rotatorAngle", FOFS(rotatorAngle), F_FLOAT},
	//TA
	{"targetShaderName", FOFS(targetShaderName), F_LSTRING},
	{"targetShaderNewName", FOFS(targetShaderNewName), F_LSTRING},

#ifdef G_LUA
	{"luaThink", FOFS(luaThink), F_LSTRING},
	{"luaTouch", FOFS(luaTouch), F_LSTRING},
	{"luaUse", FOFS(luaUse), F_LSTRING},
	{"luaHurt", FOFS(luaHurt), F_LSTRING},
	{"luaDie", FOFS(luaDie), F_LSTRING},
	{"luaFree", FOFS(luaFree), F_LSTRING},
	{"luaTrigger", FOFS(luaTrigger), F_LSTRING},
	{"luaSpawn", FOFS(luaSpawn), F_LSTRING},

	{"luaParam1", FOFS(luaParam1), F_LSTRING},
	{"luaParam2", FOFS(luaParam2), F_LSTRING},
	{"luaParam3", FOFS(luaParam3), F_LSTRING},
	{"luaParam4", FOFS(luaParam4), F_LSTRING},
#endif

	{NULL}
};


typedef struct
{
	char           *name;
	void            (*spawn) (gentity_t * ent);
} spawn_t;

void            SP_info_player_start(gentity_t * ent);
void            SP_info_player_deathmatch(gentity_t * ent);
void            SP_info_player_intermission(gentity_t * ent);

//TA: extra bits
void            SP_info_alien_intermission(gentity_t * ent);
void            SP_info_human_intermission(gentity_t * ent);

void            SP_info_firstplace(gentity_t * ent);
void            SP_info_secondplace(gentity_t * ent);
void            SP_info_thirdplace(gentity_t * ent);
void            SP_info_podium(gentity_t * ent);

void            SP_func_plat(gentity_t * ent);
void            SP_func_static(gentity_t * ent);
void            SP_func_rotating(gentity_t * ent);
void            SP_func_bobbing(gentity_t * ent);
void            SP_func_pendulum(gentity_t * ent);
void            SP_func_button(gentity_t * ent);
void            SP_func_door(gentity_t * ent);
void            SP_func_door_rotating(gentity_t * ent);	//TA
void            SP_func_door_model(gentity_t * ent);	//TA
void            SP_func_train(gentity_t * ent);
void            SP_func_timer(gentity_t * self);

void            SP_trigger_always(gentity_t * ent);
void            SP_trigger_multiple(gentity_t * ent);
void            SP_trigger_push(gentity_t * ent);
void            SP_trigger_teleport(gentity_t * ent);
void            SP_trigger_hurt(gentity_t * ent);
void            SP_trigger_stage(gentity_t * ent);
void            SP_trigger_win(gentity_t * ent);
void            SP_trigger_buildable(gentity_t * ent);
void            SP_trigger_class(gentity_t * ent);
void            SP_trigger_equipment(gentity_t * ent);
void            SP_trigger_gravity(gentity_t * ent);
void            SP_trigger_heal(gentity_t * ent);
void            SP_trigger_ammo(gentity_t * ent);

void            SP_target_delay(gentity_t * ent);
void            SP_target_speaker(gentity_t * ent);
void            SP_target_print(gentity_t * ent);
void            SP_target_character(gentity_t * ent);
void            SP_target_score(gentity_t * ent);
void            SP_target_teleporter(gentity_t * ent);
void            SP_target_relay(gentity_t * ent);
void            SP_target_kill(gentity_t * ent);
void            SP_target_position(gentity_t * ent);
void            SP_target_location(gentity_t * ent);
void            SP_target_push(gentity_t * ent);
void            SP_target_rumble(gentity_t * ent);
void            SP_target_alien_win(gentity_t * ent);
void            SP_target_human_win(gentity_t * ent);

void            SP_light(gentity_t * self);
void            SP_info_null(gentity_t * self);
void            SP_info_notnull(gentity_t * self);
void            SP_info_camp(gentity_t * self);
void            SP_path_corner(gentity_t * self);

void            SP_misc_teleporter_dest(gentity_t * self);
void            SP_misc_model(gentity_t * ent);
void            SP_misc_portal_camera(gentity_t * ent);
void            SP_misc_portal_surface(gentity_t * ent);

void            SP_shooter_rocket(gentity_t * ent);
void            SP_shooter_plasma(gentity_t * ent);
void            SP_shooter_grenade(gentity_t * ent);

//TA:
void            SP_misc_particle_system(gentity_t * ent);
void            SP_misc_anim_model(gentity_t * ent);
void            SP_misc_light_flare(gentity_t * ent);

spawn_t         spawns[] = {
	// info entities don't do anything at all, but provide positional
	// information for things controlled by other processes
	{"info_player_start", SP_info_player_start},
	{"info_player_deathmatch", SP_info_player_deathmatch},
	{"info_player_intermission", SP_info_player_intermission},

	//TA: extra bits
	{"info_alien_intermission", SP_info_alien_intermission},
	{"info_human_intermission", SP_info_human_intermission},

	{"info_null", SP_info_null},
	{"info_notnull", SP_info_notnull},	// use target_position instead
	{"info_camp", SP_info_camp},

	{"func_plat", SP_func_plat},
	{"func_button", SP_func_button},
	{"func_door", SP_func_door},
	{"func_door_rotating", SP_func_door_rotating},	//TA
	{"func_door_model", SP_func_door_model},	//TA
	{"func_static", SP_func_static},
	{"func_rotating", SP_func_rotating},
	{"func_bobbing", SP_func_bobbing},
	{"func_pendulum", SP_func_pendulum},
	{"func_train", SP_func_train},
	{"func_group", SP_info_null},
	{"func_timer", SP_func_timer},	// rename trigger_timer?

	// Triggers are brush objects that cause an effect when contacted
	// by a living player, usually involving firing targets.
	// While almost everything could be done with
	// a single trigger class and different targets, triggered effects
	// could not be client side predicted (push and teleport).
	{"trigger_always", SP_trigger_always},
	{"trigger_multiple", SP_trigger_multiple},
	{"trigger_push", SP_trigger_push},
	{"trigger_teleport", SP_trigger_teleport},
	{"trigger_hurt", SP_trigger_hurt},
	{"trigger_stage", SP_trigger_stage},
	{"trigger_win", SP_trigger_win},
	{"trigger_buildable", SP_trigger_buildable},
	{"trigger_class", SP_trigger_class},
	{"trigger_equipment", SP_trigger_equipment},
	{"trigger_gravity", SP_trigger_gravity},
	{"trigger_heal", SP_trigger_heal},
	{"trigger_ammo", SP_trigger_ammo},

	// targets perform no action by themselves, but must be triggered
	// by another entity
	{"target_delay", SP_target_delay},
	{"target_speaker", SP_target_speaker},
	{"target_print", SP_target_print},
	{"target_score", SP_target_score},
	{"target_teleporter", SP_target_teleporter},
	{"target_relay", SP_target_relay},
	{"target_kill", SP_target_kill},
	{"target_position", SP_target_position},
	{"target_location", SP_target_location},
	{"target_push", SP_target_push},
	{"target_rumble", SP_target_rumble},
	{"target_alien_win", SP_target_alien_win},
	{"target_human_win", SP_target_human_win},

	{"speaker", SP_target_speaker},

	{"light", SP_light},
	{"path_corner", SP_path_corner},

	{"misc_teleporter_dest", SP_misc_teleporter_dest},
	{"misc_model", SP_misc_model},
	{"misc_portal_surface", SP_misc_portal_surface},
	{"misc_portal_camera", SP_misc_portal_camera},

	{"misc_particle_system", SP_misc_particle_system},
	{"misc_anim_model", SP_misc_anim_model},
	{"misc_light_flare", SP_misc_light_flare},

	{NULL, 0}
};

/*
===============
G_CallSpawn

Finds the spawn function for the entity and calls it,
returning qfalse if not found
===============
*/
qboolean G_CallSpawn(gentity_t * ent)
{
	spawn_t        *s;
	buildable_t     buildable;

	if(!ent->classname)
	{
		G_Printf("G_CallSpawn: NULL classname\n");
		return qfalse;
	}

	//check buildable spawn functions
	if((buildable = BG_FindBuildNumForEntityName(ent->classname)) != BA_NONE)
	{
		if(buildable == BA_A_SPAWN || buildable == BA_H_SPAWN)
		{
			ent->s.angles[YAW] += 180.0f;
			AngleNormalize360(ent->s.angles[YAW]);
		}

		G_SpawnBuildable(ent, buildable);

#ifdef G_LUA
			// Lua API callbacks
		if(ent->luaSpawn)
		{
			G_LuaHook_EntitySpawn(ent->luaSpawn, ent->s.number);
		}
#endif
		return qtrue;
	}

	// check normal spawn functions
	for(s = spawns; s->name; s++)
	{
		if(!Q_stricmp(s->name, ent->classname))
		{
			// found it
			/*
			if(ent->name)
			{
				G_Printf("...spawning %s\n", ent->name);
			}
			else
			{
				G_Printf("...spawning %s\n", ent->classname);
			}
			*/
			s->spawn(ent);

#ifdef G_LUA
			// Lua API callbacks
			if(ent->luaSpawn)
			{
				G_LuaHook_EntitySpawn(ent->luaSpawn, ent->s.number);
			}
#endif
			return qtrue;
		}
	}

	G_Printf("%s doesn't have a spawn function\n", ent->classname);
	return qfalse;
}

/*
=============
G_NewString

Builds a copy of the string, translating \n to real linefeeds
so message texts can be multi-line
=============
*/
char           *G_NewString(const char *string)
{
	char           *newb, *new_p;
	int             i, l;

	l = strlen(string) + 1;

	newb = G_Alloc(l);

	new_p = newb;

	// turn \n into a real linefeed
	for(i = 0; i < l; i++)
	{
		if(string[i] == '\\' && i < l - 1)
		{
			i++;
			if(string[i] == 'n')
				*new_p++ = '\n';
			else
				*new_p++ = '\\';
		}
		else
			*new_p++ = string[i];
	}

	return newb;
}




/*
===============
G_ParseField

Takes a key/value pair and sets the binary values
in a gentity
===============
*/
void G_ParseField(const char *key, const char *value, gentity_t * ent)
{
	field_t        *f;
	byte           *b;
	float           v;
	vec3_t          vec;
	vec3_t          angles;
	vec4_t          vec4;
	matrix_t        rotation;
	int             i;
	char           *p;
	char           *token;

	for(f = fields; f->name; f++)
	{
		if(!Q_stricmp(f->name, key))
		{
			// found it
			b = (byte *) ent;

			switch (f->type)
			{
				case F_LSTRING:
					*(char **)(b + f->ofs) = G_NewString(value);
					break;

				case F_VECTOR:
					sscanf(value, "%f %f %f", &vec[0], &vec[1], &vec[2]);

					((float *)(b + f->ofs))[0] = vec[0];
					((float *)(b + f->ofs))[1] = vec[1];
					((float *)(b + f->ofs))[2] = vec[2];
					break;

				case F_VECTOR4:
					sscanf(value, "%f %f %f %f", &vec4[0], &vec4[1], &vec4[2], &vec4[3]);

					((float *)(b + f->ofs))[0] = vec4[0];
					((float *)(b + f->ofs))[1] = vec4[1];
					((float *)(b + f->ofs))[2] = vec4[2];
					((float *)(b + f->ofs))[3] = vec4[3];
					break;

				case F_INT:
					*(int *)(b + f->ofs) = atoi(value);
					break;

				case F_FLOAT:
					*(float *)(b + f->ofs) = atof(value);
					break;

				case F_ANGLEHACK:
					v = atof(value);
					((float *)(b + f->ofs))[0] = 0;
					((float *)(b + f->ofs))[1] = v;
					((float *)(b + f->ofs))[2] = 0;
					break;

				case F_MOVEDIRHACK:
					v = atof(value);
					angles[0] = 0;
					angles[1] = v;
					angles[2] = 0;

					G_SetMovedir(angles, vec);

					((float *)(b + f->ofs))[0] = vec[0];
					((float *)(b + f->ofs))[1] = vec[1];
					((float *)(b + f->ofs))[2] = vec[2];
					break;

				case F_ROTATIONHACK:
					MatrixIdentity(rotation);
					p = (char *)value;
					for(i = 0; i < 9; i++)
					{
						token = Com_Parse(&p);
						rotation[i] = atof(token);
					}
					MatrixToAngles(rotation, vec);
					((float *)(b + f->ofs))[0] = vec[0];
					((float *)(b + f->ofs))[1] = vec[1];
					((float *)(b + f->ofs))[2] = vec[2];
					break;

				default:
				case F_IGNORE:
					break;
			}

			return;
		}
	}
}




/*
===================
G_SpawnGEntityFromSpawnVars

Spawn an entity and fill in all of the level fields from
level.spawnVars[], then call the class specfic spawn function
===================
*/
void G_SpawnGEntityFromSpawnVars(void)
{
	int             i;
	gentity_t      *ent;

	// get the next free entity
	ent = G_Spawn();

	for(i = 0; i < level.numSpawnVars; i++)
		G_ParseField(level.spawnVars[i][0], level.spawnVars[i][1], ent);

	G_SpawnInt("notq3a", "0", &i);

	if(i)
	{
		G_FreeEntity(ent);
		return;
	}

	// move editor origin to pos
	VectorCopy(ent->s.origin, ent->s.pos.trBase);
	VectorCopy(ent->s.origin, ent->r.currentOrigin);

	// if we didn't get a classname, don't bother spawning anything
	if(!G_CallSpawn(ent))
		G_FreeEntity(ent);
}



/*
====================
G_AddSpawnVarToken
====================
*/
char           *G_AddSpawnVarToken(const char *string)
{
	int             l;
	char           *dest;

	l = strlen(string);
	if(level.numSpawnVarChars + l + 1 > MAX_SPAWN_VARS_CHARS)
		G_Error("G_AddSpawnVarToken: MAX_SPAWN_CHARS");

	dest = level.spawnVarChars + level.numSpawnVarChars;
	memcpy(dest, string, l + 1);

	level.numSpawnVarChars += l + 1;

	return dest;
}

/*
====================
G_ParseSpawnVars

Parses a brace bounded set of key / value pairs out of the
level's entity strings into level.spawnVars[]

This does not actually spawn an entity.
====================
*/
qboolean G_ParseSpawnVars(void)
{
	char            keyname[MAX_TOKEN_CHARS];
	char            com_token[MAX_TOKEN_CHARS];

	level.numSpawnVars = 0;
	level.numSpawnVarChars = 0;

	// parse the opening brace
	if(!trap_GetEntityToken(com_token, sizeof(com_token)))
	{
		// end of spawn string
		return qfalse;
	}

	if(com_token[0] != '{')
		G_Error("G_ParseSpawnVars: found %s when expecting {", com_token);

	// go through all the key / value pairs
	while(1)
	{
		// parse key
		if(!trap_GetEntityToken(keyname, sizeof(keyname)))
			G_Error("G_ParseSpawnVars: EOF without closing brace");

		if(keyname[0] == '}')
			break;

		// parse value
		if(!trap_GetEntityToken(com_token, sizeof(com_token)))
			G_Error("G_ParseSpawnVars: EOF without closing brace");

		if(com_token[0] == '}')
			G_Error("G_ParseSpawnVars: closing brace without data");

		if(level.numSpawnVars == MAX_SPAWN_VARS)
			G_Error("G_ParseSpawnVars: MAX_SPAWN_VARS");

		level.spawnVars[level.numSpawnVars][0] = G_AddSpawnVarToken(keyname);
		level.spawnVars[level.numSpawnVars][1] = G_AddSpawnVarToken(com_token);
		level.numSpawnVars++;
	}

	return qtrue;
}



/*QUAKED worldspawn (0 0 0) ?

Every map should have exactly one worldspawn.
"music"   music wav file
"gravity" 800 is default gravity
"message" Text to print during connection process
*/
void SP_worldspawn(void)
{
	char           *s;

	G_SpawnString("classname", "", &s);

	if(Q_stricmp(s, "worldspawn"))
		G_Error("SP_worldspawn: The first entity isn't 'worldspawn'");

	// make some data visible to connecting client
	trap_SetConfigstring(CS_GAME_VERSION, GAME_VERSION);

	trap_SetConfigstring(CS_LEVEL_START_TIME, va("%i", level.startTime));

	G_SpawnString("music", "", &s);
	trap_SetConfigstring(CS_MUSIC, s);

	G_SpawnString("message", "", &s);
	trap_SetConfigstring(CS_MESSAGE, s);	// map specific message

	trap_SetConfigstring(CS_MOTD, g_motd.string);	// message of the day

	G_SpawnString("gravity", "800", &s);
	trap_Cvar_Set("g_gravity", s);

	G_SpawnString("humanBuildPoints", DEFAULT_HUMAN_BUILDPOINTS, &s);
	trap_Cvar_Set("g_humanBuildPoints", s);

	G_SpawnString("humanMaxStage", "2", &s);
	trap_Cvar_Set("g_humanMaxStage", s);

	G_SpawnString("humanStage2Threshold", g_humanStage2Threshold.string, &s);
	trap_Cvar_Set("g_humanStage2Threshold", s);

	G_SpawnString("humanStage3Threshold", g_humanStage3Threshold.string, &s);
	trap_Cvar_Set("g_humanStage3Threshold", s);

	G_SpawnString("alienBuildPoints", DEFAULT_ALIEN_BUILDPOINTS, &s);
	trap_Cvar_Set("g_alienBuildPoints", s);

	G_SpawnString("alienMaxStage", "2", &s);
	trap_Cvar_Set("g_alienMaxStage", s);

	G_SpawnString("alienStage2Threshold", g_alienStage2Threshold.string, &s);
	trap_Cvar_Set("g_alienStage2Threshold", s);

	G_SpawnString("alienStage3Threshold", g_alienStage3Threshold.string, &s);
	trap_Cvar_Set("g_alienStage3Threshold", s);

	G_SpawnString("enableDust", "0", &s);
	trap_Cvar_Set("g_enableDust", s);

	G_SpawnString("enableBreath", "0", &s);
	trap_Cvar_Set("g_enableBreath", s);

	G_SpawnString("disabledEquipment", "", &s);
	trap_Cvar_Set("g_disabledEquipment", s);

	G_SpawnString("disabledClasses", "", &s);
	trap_Cvar_Set("g_disabledClasses", s);

	G_SpawnString("disabledBuildables", "", &s);
	trap_Cvar_Set("g_disabledBuildables", s);

	g_entities[ENTITYNUM_WORLD].s.number = ENTITYNUM_WORLD;
	g_entities[ENTITYNUM_WORLD].classname = "worldspawn";

	// see if we want a warmup time
	trap_SetConfigstring(CS_WARMUP, "");
	if(g_restarted.integer)
	{
		trap_Cvar_Set("g_restarted", "0");
		level.warmupTime = 0;
	}
	else if(g_doWarmup.integer)
	{
		// Turn it on
		level.warmupTime = -1;
		trap_SetConfigstring(CS_WARMUP, va("%i", level.warmupTime));
		G_LogPrintf("Warmup:\n");
	}

}


/*
==============
G_SpawnEntitiesFromString

Parses textual entity definitions out of an entstring and spawns gentities.
==============
*/
void G_SpawnEntitiesFromString(void)
{
	// allow calls to G_Spawn*()
	level.spawning = qtrue;
	level.numSpawnVars = 0;

	// the worldspawn is not an actual entity, but it still
	// has a "spawn" function to perform any global setup
	// needed by a level (setting configstrings or cvars, etc)
	if(!G_ParseSpawnVars())
		G_Error("SpawnEntities: no entities");

	SP_worldspawn();

	// parse ents
	while(G_ParseSpawnVars())
		G_SpawnGEntityFromSpawnVars();

	level.spawning = qfalse;	// any future calls to G_Spawn*() will be errors
}
