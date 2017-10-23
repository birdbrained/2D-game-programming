#include "formation.h"

int formation_rectangle(GraphNode * n)
{
	GraphNode * start = n;
	int stepsToRight = 0, stepsToLeft = 0, stepsDown = 0, stepsUp = 0;
	if (n == NULL)
	{
		slog("Error: could not detect rectangle from a null node");
		return -1;
	}

	while (n->right_node != NULL && n->right_node->data != NULL)
	{
		n = n->right_node;
		stepsToRight++;
	}
	while (n->down_node != NULL && n->down_node->data != NULL)
	{
		n = n->down_node;
		stepsDown++;
	}
	while (n->left_node != NULL && n->left_node->data != NULL)
	{
		n = n->left_node;
		stepsToLeft++;
	}
	while (n->up_node != NULL && n->up_node->data != NULL)
	{
		n = n->up_node;
		stepsUp++;
	}

	if (n == start)
	{
		if (stepsToRight > 0 && stepsToLeft > 0 && stepsDown > 0 && stepsUp > 0)
		{
			return 100;
		}
	}
	
	return 0;
}

int formation_detect(Graph ** graph)
{
	GraphNode * hor_iter = NULL;
	GraphNode * ver_iter = NULL;
	Instrument inst = Instrument_Unassigned;
	Formation f = Formation_None;
	int numInstrInARow = 0;
	int numInstrToCheck = 0;
	int numInRow = 0;
	int baseScore = 0;
	int multiplier = 1;
	int checkedRectangle = 0;
	int temp = 0;

	if (!graph)
	{
		slog("Error: cannot check formations of a null graph");
		return 0;
	}
	hor_iter = (*graph)->head;
	ver_iter = (*graph)->head;

	while (ver_iter != NULL)
	{
		while (hor_iter != NULL)
		{
			if (hor_iter->data != NULL && hor_iter->traversed != 1)
			{
				if (inst == Instrument_Unassigned)
				{
					inst = ((Entity *)hor_iter->data)->myInstrument;
					numInstrInARow++;
				}
				else if (((Entity *)hor_iter->data)->myInstrument == inst)
				{
					numInstrInARow++;
				}
				else
				{
					inst = ((Entity *)hor_iter->data)->myInstrument;
					numInstrInARow = 1;
				}
				numInRow++;
				hor_iter->traversed = 1;
				if (checkedRectangle == 0)
				{
					temp = formation_rectangle(hor_iter);
					if (temp > 0)
					{
						baseScore += temp;
						checkedRectangle++;
						temp = 0;
					}
				}
			}
			else if (hor_iter->data == NULL)
			{
				baseScore = baseScore + (numInRow * 10);
				numInRow = 0;
				numInstrInARow = 0;
				inst = Instrument_Unassigned;
			}
			switch (inst)
			{
			case Instrument_Snare_Drum:
				if (numInstrInARow == (*graph)->numSnareDrums)
				{
					soundAdjustVolume(soundFindByInstrument(inst), MIX_MAX_VOLUME / 4);
					multiplier *= 2;
				}
				break;
			case Instrument_Flute:
				if (numInstrInARow == (*graph)->numFlutes)
				{
					soundAdjustVolume(soundFindByInstrument(inst), MIX_MAX_VOLUME / 4);
					multiplier *= 2;
				}
				break;
			case Instrument_Trumpet:
				if (numInstrInARow == (*graph)->numTrumpets)
				{
					soundAdjustVolume(soundFindByInstrument(inst), MIX_MAX_VOLUME / 4);
					multiplier *= 2;
				}
				break;
			case Instrument_Alto_Saxophone:
				if (numInstrInARow == (*graph)->numAltoSaxes)
				{
					soundAdjustVolume(soundFindByInstrument(inst), MIX_MAX_VOLUME / 4);
					multiplier *= 2;
				}
				break;
			case Instrument_Baritone:
				if (numInstrInARow == (*graph)->numBaritones)
				{
					soundAdjustVolume(soundFindByInstrument(inst), MIX_MAX_VOLUME / 4);
					multiplier *= 2;
				}
				break;
			default:
				break;
			}
			hor_iter = hor_iter->right_node;
		}
		numInstrInARow = 0;
		inst = Instrument_Unassigned;
		ver_iter = ver_iter->down_node;
		hor_iter = ver_iter;
	}

	slog("base score (%i) multiplier (%i) score (%i)", baseScore, multiplier, baseScore * multiplier);
	return baseScore * multiplier;
}

char * formation_to_string(Formation f)
{
	char buffer[FORM_CHAR_LEN];

	switch (f)
	{
	case Formation_Line:
		strncpy(buffer, "Line", FORM_CHAR_LEN);
		break;
	case Formation_Rectangle:
		strncpy(buffer, "Rectangle", FORM_CHAR_LEN);
		break;
	default:
		strncpy(buffer, "No formation", FORM_CHAR_LEN);
		break;
	}

	return buffer;
}
