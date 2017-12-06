#include "entity_s.h"
#include "simple_logger.h"
#include "gf2d_sprite.h"
#include "think_functions.h"

typedef struct
{
	Uint32 maxEntities;
	Entity * entityList;
	Uint64 increment;
}EntityManager;

//static variables prevents any other file from accessing the variable
//similar to private in this case
static EntityManager entityManager = { 0, NULL, 0 };

Vector4D transparent = { 255, 255, 255, 122 };

struct think_function think_table[] =
{
	{move, "move"},
	{ NULL, NULL }
};

/**
 * @brief Entity system clean-up function
 */
void entitySystemClose()
{
	int i;
	//entityDeleteAll();
	if (entityManager.entityList != NULL)
	{
		for (i = 0; i < entityManager.maxEntities; i++)
		{
			entityFree(&entityManager.entityList[i]);
		}
		free(entityManager.entityList);
	}
	//entityManager.entityList = NULL;
	//entityManager.maxEntities = 0;
	memset(&entityManager, 0, sizeof(EntityManager));
	slog("Entity system closed.");
}

void entitySystemInit(Uint32 max)
{
	if (max <= 0)
	{
		slog("Cannot initialize an entity manager for zero or negative entities!");
		return;
	}
	memset(&entityManager, 0, sizeof(EntityManager));

	entityManager.entityList = (Entity *)malloc(sizeof(Entity) * max);
	if (!entityManager.entityList)
	{
		slog("Could not allocate memory for the entity list");
		entitySystemClose();
		return;
	}
	memset(entityManager.entityList, 0, sizeof(Entity) * max);
	entityManager.maxEntities = max;

	slog("Entity system initialized");
	//on exit clean-up stuff goes here
	atexit(entitySystemClose);
}

Entity * entityNew()
{
	int i;

	for (i = 0; i < entityManager.maxEntities; i++)
	{
		if (entityManager.entityList[i].inUse == 0)
		{
			slog("Found a suitible spot for an entity at (%i)", i);
			memset(&entityManager.entityList[i], 0, sizeof(Entity));
			entityManager.entityList[i].id = entityManager.increment++;
			entityManager.entityList[i].inUse = 1;
			vector2d_set(entityManager.entityList[i].scale, 1, 1);
			vector2d_set(entityManager.entityList[i].pathToDraw, -1, -1);
			entityManager.entityList[i].pathColor = COLOR_NONE;
			//entityManager.entityList[i].actor.color = vector4d(1, 1, 1, 1);
			return &entityManager.entityList[i];
		}
	}

	slog("Error: Out of entity addresses!");
	return NULL;
}

void entityDelete(Entity * thingThatDies)
{
	if (!thingThatDies)
	{
		slog("Error: Cannot delete an entity that does not exist!");
		return;
	}
	/*if (thingThatDies->mySprite != NULL)
	{
		if (thingThatDies->mySprite->texture != NULL)
		{
			slog("destroying texture");
			SDL_DestroyTexture(thingThatDies->mySprite->texture);
		}
	}*/
	if (thingThatDies->inUse)
	{
		thingThatDies->inUse = 0;
	}
	//slog("Deleting a thing...");
	memset(thingThatDies, 0, sizeof(Entity));
	entityFree(thingThatDies);
	//slog("Deleted!");
}

void entityFree(Entity * e)
{
	//int i;
	if (!e)
	{
		slog("Error: Cannot free something that doesn't exist!");
		return;
	}

	//function pointer to a free functions
	//sound free
	//free actor
	//particless

	//slog("Freeing something from memory.");
	e->inUse = 0;
	//memset(e, 0, sizeof(Entity));
}

void entityDeleteAll()
{
	int i = 0;

	for (i = 0; i < entityManager.maxEntities; i++)
	{
		entityDelete(&entityManager.entityList[i]);
	}
}

void entityUpdate(Entity * self)
{
	if (!(self))
	{
		slog("Cannot update an entity that does not exist");
		return;
	}
	if (!(self)->inUse)
	{
		slog("Cannot update an entity that is not in use");
		return;
	}
	
	vector2d_add((self)->velocity, (self)->velocity, (self)->acceleration);

	if ((self)->boundingBox != NULL)
	{
		(self)->boundingBox->x = (self)->position.x;
		(self)->boundingBox->y = (self)->position.y;
	}

	if ((self)->update != NULL)
	{
		(self)->update((self));
	}

	switch (self->currentState)
	{
	case ES_Dead:
		self->position.y++;
		self->rotation.z += 10;
		if (self->position.y > 600)
		{
			entityDelete(self);
		}
		break;
	default:
		break;
	}
}

void entityUpdateAll()
{
	int i;
	for (i = 0; i < entityManager.maxEntities; i++)
	{
		if (entityManager.entityList[i].inUse == 1)
		{
			entityUpdate(&entityManager.entityList[i]);
		}
	}
}

void entityDraw(Entity * self)
{
	if (!self)
	{
		slog("Cannot draw an entity that does not exist");
		return;
	}
	if (!self->inUse)
	{
		slog("Cannot draw an entity that is not in use");
		return;
	}
	if (!self->mySprite)
	{
		slog("Cannot draw an entity with no sprite...%d", self->id);
		return;
	}
	gf2d_sprite_draw(
		self->mySprite, 
		self->position, 
		&self->scale, 
		&self->scaleCenter, 
		&self->rotation, 
		&self->flip, 
		NULL, 
		self->currentFrame
	);

	if (self->instrumentSprite)
	{
		gf2d_sprite_draw(
			self->instrumentSprite,
			self->position,
			&self->scale,
			&self->scaleCenter,
			&self->rotation,
			&self->flip,
			NULL,
			0
		);

		//slog("position x (%f) y (%f)", self->position.x, self->position.y);
	}

	//health bar; only draws when not at max health
	if (self->currMorale < self->statMorale)
	{
		self->healthBar.x = self->position.x;
		self->healthBar.y = (self->position.y + self->mySprite->frame_h * self->scale.y) - (self->mySprite->frame_h * self->scale.y / 4);
		self->healthBar.w = (self->currMorale * self->mySprite->frame_w * self->scale.x) / self->statMorale;
		self->healthBar.h = self->mySprite->frame_h / 4 * self->scale.y;
		draw_filled_rect(self->healthBar, COLOR_RED);
	}

	//drawing the transparent next selected position
	if (self->nextPosition != self->currentPosition)
	{
		gf2d_sprite_draw(
			self->mySprite,
			vector2d(self->nextPosition % 18 * 64, self->nextPosition / 18 * 64),
			&self->scale,
			&self->scaleCenter,
			&self->rotation,
			&self->flip,
			&transparent,
			0
		);
		self->pathToDraw.x = self->nextPosition % 18 * 64 + 32;
		self->pathToDraw.y = self->nextPosition / 18 * 64 + 32;
	}

	if (!vector2d_equal(self->pathToDraw, vector2d(-1, -1)))
	{
		gf2d_draw_line(vector2d(self->position.x + 32, self->position.y + 32), self->pathToDraw, self->pathColor);
	}
}

void entityDrawAll()
{
	int i;
	for (i = 0; i < entityManager.maxEntities; i++)
	{
		if (entityManager.entityList[i].inUse)
		{
			entityDraw(&entityManager.entityList[i]);
		}
	}
}

void entityIncrementCurrentFrame(Entity * self)
{
	if (!self)
	{
		slog("Error: Cannot increment frame on a non-exisiting entity");
		return;
	}
	if (!self->inUse)
	{
		slog("Error: Cannot increment frame on an entity not in use");
		return;
	}
	if (!self->mySprite)
	{
		slog("Error: Cannot increment frame on an entity with no sprite");
		return;
	}

	self->currentFrame = self->currentFrame + 0.05f;
	//slog("%f", self->currentFrame);
	if (self->currentFrame < self->minFrame)
	{
		self->currentFrame = self->minFrame;
	}
	if (self->currentFrame > self->maxFrame)
	{
		self->currentFrame = self->minFrame;
	}
}

void entityIncrementCurrentFrameAll()
{
	int i;
	for (i = 0; i < entityManager.maxEntities; i++)
	{
		if (entityManager.entityList[i].inUse)
		{
			entityIncrementCurrentFrame(&entityManager.entityList[i]);
		}
	}
}

//Entity * entityLoadFromFile(FILE * file, Entity * new_entity)
Entity * entityLoadFromFile(char * filename, Entity * new_entity)
{
	//char buffer[512];
	char buffer[512];
	char * physBuffer = "";
	char holder[512];
	PHYSFS_File * file;
	int n = 0;

	if (!filename)
	{
		slog("Error: Filename was null");
		return NULL;
	}
	if (!PHYSFS_exists(filename))
	{
		slog("Error: could not find file with name (%s)", filename);
	}
	if (!new_entity)
	{
		slog("Error: new_entity was null");
		return NULL;
	}
	//rewind(file);

	file = PHYSFS_openRead(filename);
	physBuffer = (char *)malloc(PHYSFS_fileLength(file));
	memset(physBuffer, 0, PHYSFS_fileLength(file));
	PHYSFS_readBytes(file, physBuffer, PHYSFS_fileLength(file));
	PHYSFS_close(file);
	
	/* Note to self!
	   Order matters here. Please strcmp variables
	   in the order they appear in the entity_s
	   struct. Thanks Future Matt ~<3 Why don't
	   you text me back anymore */
	//while (fscanf(file, "%s", buffer) != EOF)
	while (sscanf(physBuffer, " %s\n%n", buffer, &n) == 1)
	{
		if (buffer[0] == '~')
		{
			break;
		}
		physBuffer += n;
		if (strcmp(buffer, "name:") == 0)
		{
			//fscanf(file, "%s", buffer);
			sscanf(physBuffer, " %s\n%n", buffer, &n);
			physBuffer += n;
			strncpy(new_entity->name, buffer, MAX_CHARS);
			//fscanf(file, "%s", buffer);
			slog("name is (%s)", new_entity->name);
			//slog("2 name is (%s)", buffer);
			continue;
		}
		if (strcmp(buffer, "fav_thing:") == 0)
		{
			//fscanf(file, "%s", buffer);
			sscanf(physBuffer, " %s\n%n", buffer, &n);
			physBuffer += n;
			strncpy(new_entity->favoriteThing, buffer, MAX_CHARS);
			//fscanf(file, "%s", buffer);
			slog("favThing is (%s)", new_entity->favoriteThing);
			continue;
		}
		if (strcmp(buffer, "sprite:") == 0)
		{
			//fscanf(file, "%s", buffer);
			sscanf(physBuffer, " %s\n%n", buffer, &n);
			physBuffer += n;
			new_entity->mySprite = gf2d_sprite_load_all(buffer, 32, 32, 2);
			new_entity->rotation.x = new_entity->mySprite->frame_w / 2;
			new_entity->rotation.y = new_entity->mySprite->frame_h / 2;
			continue;
		}
		if (strcmp(buffer, "Instrument:") == 0)
		{
			//fscanf(file, "%s", holder);
			sscanf(physBuffer, " %s\n%n", holder, &n);
			physBuffer += n;
			if (strcmp(holder, "flute") == 0)
			{
				new_entity->myInstrument = Instrument_Flute;
				new_entity->pathColor = COLOR_BLUE_LIGHT;
			}
			else if (strcmp(holder, "clarinet") == 0)
			{
				new_entity->myInstrument = Instrument_Clarinet;
				new_entity->pathColor = COLOR_PURPLE;
			}
			else if (strcmp(holder, "altosax") == 0)
			{
				new_entity->myInstrument = Instrument_Alto_Saxophone;
				new_entity->pathColor = COLOR_YELLOW;
			}
			else if (strcmp(holder, "tenorsax") == 0)
			{
				new_entity->myInstrument = Instrument_Tenor_Saxophone;
				new_entity->pathColor = COLOR_ORANGE;
			}
			else if (strcmp(holder, "trumpet") == 0)
			{
				new_entity->myInstrument = Instrument_Trumpet;
				new_entity->pathColor = COLOR_RED;
			}
			else if (strcmp(holder, "baritone") == 0)
			{
				new_entity->myInstrument = Instrument_Baritone;
				new_entity->pathColor = COLOR_BLUE;
			}
			else if (strcmp(holder, "tuba") == 0)
			{
				new_entity->myInstrument = Instrument_Tuba;
				new_entity->pathColor = COLOR_GREEN;
			}
			else if (strcmp(holder, "snaredrum") == 0)
			{
				new_entity->myInstrument = Instrument_Snare_Drum;
				new_entity->pathColor = COLOR_BLACK;
			}
			else if (strcmp(holder, "bassdrum") == 0)
				new_entity->myInstrument = Instrument_Bass_Drum;
			else if (strcmp(holder, "cgflag") == 0)
				new_entity->myInstrument = Instrument_Colorguard_Flag;
			else if (strcmp(holder, "cgrifle") == 0)
				new_entity->myInstrument = Instrument_Colorguard_Rifle;
			else if (strcmp(holder, "pmarimba") == 0)
				new_entity->myInstrument = Instrument_Pit_Marimba;
			else if (strcmp(holder, "pxylophone") == 0)
				new_entity->myInstrument = Instrument_Pit_Xylophone;
			else if (strcmp(holder, "pguitar") == 0)
				new_entity->myInstrument = Instrument_Pit_Guitar;
			else if (strcmp(holder, "psynth") == 0)
				new_entity->myInstrument = Instrument_Pit_Synthesizer;
			else if (strcmp(holder, "pgong") == 0)
				new_entity->myInstrument = Instrument_Pit_Gong;
			else
				new_entity->myInstrument = Instrument_Unassigned;
			slog("instrument (%d)", new_entity->myInstrument);
			continue;
		}
		if (strcmp(buffer, "instrumentSprite:") == 0)
		{
			//fscanf(file, "%s", buffer);
			sscanf(physBuffer, " %s\n%n", buffer, &n);
			physBuffer += n;
			strncpy(new_entity->instrumentSpriteFilePath, buffer, MAX_FILEPATH_CHARS);
			slog("instrument sprite file (%s)", new_entity->instrumentSpriteFilePath);
		}
		if (strcmp(buffer, "statMarching:") == 0)
		{
			//fscanf(file, "%i", &new_entity->statMarching);
			sscanf(physBuffer, " %i\n%n", &new_entity->statMarching, &n);
			physBuffer += n;
			//fscanf(file, "%s", buffer);
			slog("stat marching (%i)", new_entity->statMarching);
			continue;
		}
		if (strcmp(buffer, "statMusic:") == 0)
		{
			//fscanf(file, "%i", &new_entity->statMusic);
			sscanf(physBuffer, " %i\n%n", &new_entity->statMusic, &n);
			physBuffer += n;
			slog("stat music (%i)", new_entity->statMusic);
			continue;
		}
		if (strcmp(buffer, "statMorale:") == 0)
		{
			//fscanf(file, "%i", &new_entity->statMorale);
			sscanf(physBuffer, " %i\n%n", &new_entity->statMorale, &n);
			physBuffer += n;
			new_entity->currMorale = new_entity->statMorale;
			slog("stat morale (%i)", new_entity->statMorale);
			continue;
		}
		if (strcmp(buffer, "statMotivation:") == 0)
		{
			//fscanf(file, "%i", &new_entity->statMotivation);
			sscanf(physBuffer, " %i\n%n", &new_entity->statMotivation, &n);
			physBuffer += n;
			slog("stat motivation (%i)", new_entity->statMotivation);
			continue;
		}
		if (strcmp(buffer, "sectionLeader:") == 0)
		{
			//fscanf(file, "%i", &new_entity->isSectionLeader);
			sscanf(physBuffer, " %i\n%n", &new_entity->isSectionLeader, &n);
			physBuffer += n;
			slog("is section leader (%i)", new_entity->isSectionLeader);
			continue;
		}

		//fscanf(file, "%s", buffer);
		//fgets(buffer, sizeof(buffer), file);
		//fscanf(file, "%s", holder);
		//slog("%s, %s", buffer, holder);
	}

	//fscanf(file, "name: %s", buffer);
	//slog("%s", buffer);
	//fscanf(file, "favThing: %s", buffer);
	//slog("%s", buffer);
	return new_entity;
}

//void entityLoadAllFromFile(FILE * file, TileMap * map/*, Graph ** graph*/)
void entityLoadAllFromFile(char * filename, TileMap * map)
{
	PHYSFS_File * file;
	Entity * currNew = NULL;
	char buffer[512];
	char * physBuffer;
	char * entityBuffer;
	//PHYSFS_File * entityFile = NULL;
	int i = 0, n = 0;
	if (!filename)
	{
		slog("Error: filename was null");
		return;
	}
	if (!PHYSFS_exists(filename))
	{
		slog("Error: could not find file with name (%s)", filename);
		return;
	}

	file = PHYSFS_openRead(filename);
	physBuffer = (char *)malloc(PHYSFS_fileLength(file));
	memset(physBuffer, 0, PHYSFS_fileLength(file));
	PHYSFS_readBytes(file, physBuffer, PHYSFS_fileLength(file));
	PHYSFS_close(file);

	//while (fscanf(file, "%s", buffer) != EOF)
	while (sscanf(physBuffer, " %s\n%n", buffer, &n) == 1)
	{
		//entityFile = fopen(buffer, "r");
		//if (!entityFile)
		physBuffer += n;
		if (!PHYSFS_exists(buffer))
		{
			slog("bad filename (%s)", buffer);
			//fclose(entityFile);
		}
		else
		{
			//entityFile = PHYSFS_openRead(buffer);
			//entityBuffer = (char *)malloc(PHYSFS_fileLength(entityFile));
			//memset(entityBuffer, 0, PHYSFS_fileLength(entityFile));
			//PHYSFS_readBytes(entityFile, entityBuffer, PHYSFS_fileLength(entityFile));
			//PHYSFS_close(file);
			if (buffer[0] == '~')
			{
				break;
			}

			currNew = entityNew();
			//entityLoadFromFile(entityFile, currNew);
			entityLoadFromFile(buffer, currNew);
			//fclose(entityFile);
			currNew->inUse = 1;
			currNew->instrumentSprite = gf2d_sprite_load_all(&currNew->instrumentSpriteFilePath, 32, 32, 1);
			currNew->boundingBox = rect_new(currNew->position.x, currNew->position.y, 64, 64);
			currNew->scale = vector2d(2, 2);
			currNew->currentFrame = 0;
			currNew->minFrame = 0;
			currNew->maxFrame = 2;
			while (1)
			{
				i = random_int(0, map->width * map->height);
				if (map->space[i] != 0)
				{
					slog("Hey that spot is taken! (%i) Try again...", i);
					continue;
				}
				currNew->currentPosition = i;
				currNew->nextPosition = i;
				currNew->position = vector2d((i % 18) * 64, (i / 18) * 64);
				currNew->boundingBox = rect_new(currNew->position.x, currNew->position.y, 64, 64);
				map->space[i] = 1;
				/*if (graph != NULL)
				{
					graph_update_node(graph, i % 18, i / 18, currNew, sizeof(Entity));
				}*/
				break;
			}
		}
	}
}

Entity * entityCheckCollisionInAll(int mx, int my)
{
	int i = 0;

	for (i = 0; i < entityManager.maxEntities; i++)
	{
		if (entityManager.entityList[i].inUse > 0)
		{
			if (point_in_rect(mx, my, entityManager.entityList[i].boundingBox))
			{
				return &entityManager.entityList[i];
			}
		}
	}

	return NULL;
}

char * entityGetInstrumentName(Entity * e)
{
	char buffer[512];
	if (!e)
	{
		slog("Error: could not get instrument from entity because entity was null");
		return NULL;
	}

	switch (e->myInstrument)
	{
	case Instrument_Flute:
		strncpy(buffer, "Flute", 512);
		break;
	case Instrument_Clarinet:
		strncpy(buffer, "Clarinet", 512);
		break;
	case Instrument_Alto_Saxophone:
		strncpy(buffer, "Alto Saxophone", 512);
		break;
	case Instrument_Tenor_Saxophone:
		strncpy(buffer, "Tenor Saxophone", 512);
		break;
	case Instrument_Trumpet:
		strncpy(buffer, "Trumpet", 512);
		break;
	case Instrument_Baritone:
		strncpy(buffer, "Baritone", 512);
		break;
	case Instrument_Tuba:
		strncpy(buffer, "Tuba", 512);
		break;
	case Instrument_Snare_Drum:
		strncpy(buffer, "Snare Drum", 512);
		break;
	case Instrument_Bass_Drum:
		strncpy(buffer, "Bass Drum", 512);
		break;
	case Instrument_Pit_Marimba:
		strncpy(buffer, "Marimba", 512);
		break;
	case Instrument_Pit_Xylophone:
		strncpy(buffer, "Xylophone", 512);
		break;
	case Instrument_Pit_Guitar:
		strncpy(buffer, "Guitar", 512);
		break;
	case Instrument_Pit_Synthesizer:
		strncpy(buffer, "Synthesizer", 512);
		break;
	case Instrument_Pit_Gong:
		strncpy(buffer, "Gong", 512);
		break;
	case Instrument_Colorguard_Flag:
		strncpy(buffer, "Flag Twirler", 512);
		break;
	case Instrument_Colorguard_Rifle:
		strncpy(buffer, "Rifle Tosser", 512);
		break;
	default:
		strncpy(buffer, "I have no idea, man...", 512);
		break;
	}

	return buffer;
}

int entityDamage(Entity * self, int damage)
{
	if (!self)
	{
		slog("Error: cannot damage a null entity");
		return -1;
	}

	self->currMorale -= damage;
	if (self->currMorale <= 0)
	{
		//killing blow!
		self->currMorale = 0;
		self->currentState = ES_Dead;
		self->currentFrame = 2;
		self->minFrame = 2;
		self->maxFrame = 2;
		return 1;
	}
	//damaged, but not a kill
	return 0;
}

void entityDamageAll(int damage)
{
	int i = 0;

	for (i = 0; i < entityManager.maxEntities; i++)
	{
		if (entityManager.entityList[i].inUse > 0)
		{
			entityDamage(&entityManager.entityList[i], damage);
		}
	}
}

void entityUpdateGraphPositionAll(Graph ** graph)
{
	int i = 0;

	for (i = 0; i < entityManager.maxEntities; i++)
	{
		if (entityManager.entityList[i].inUse > 0)
		{
			if (entityManager.entityList[i].myInstrument != Instrument_Unassigned)
			{
				graph_update_node(graph, entityManager.entityList[i].currentPosition % 18, entityManager.entityList[i].currentPosition / 18, &entityManager.entityList[i], sizeof(Entity));
				switch (entityManager.entityList[i].myInstrument)
				{
				case Instrument_Snare_Drum:
					(*graph)->numSnareDrums++;
					break;
				case Instrument_Flute:
					(*graph)->numFlutes++;
					break;
				case Instrument_Trumpet:
					(*graph)->numTrumpets++;
					break;
				case Instrument_Alto_Saxophone:
					(*graph)->numAltoSaxes++;
					break;
				case Instrument_Baritone:
					(*graph)->numBaritones++;
					break;
				default:
					(*graph)->numOtherInstruments++;
					break;
				}
			}
		}
	}
}

void entityUpdatePosition(Entity * self, TileMap * map)
{
	if (!self)
	{
		return;
	}
	if (self->currentState == ES_Dead)
	{
		return;
	}

	if (self->nextPosition != self->currentPosition)
	{
		self->currentPosition = self->nextPosition;
		self->position.x = self->currentPosition % map->width * map->tilemapSprite->frame_w + map->xPos;
		self->position.y = self->currentPosition / map->width * map->tilemapSprite->frame_h + map->yPos;
		self->pathToDraw.x = -1;
		self->pathToDraw.y = -1;
	}
}

void entityUpdatePositionAll(TileMap * map)
{
	int i = 0;

	for (i = 0; i < entityManager.maxEntities; i++)
	{
		if (entityManager.entityList[i].inUse > 0)
		{
			entityUpdatePosition(&entityManager.entityList[i], map);
		}
	}
}
