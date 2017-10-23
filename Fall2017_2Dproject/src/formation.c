#include "formation.h"

Formation formation_detect(Graph * graph)
{
	GraphNode * hor_iter = NULL;
	GraphNode * ver_iter = NULL;
	Formation f = Formation_None;

	if (!graph)
	{
		slog("Error: cannot check formations of a null graph");
		return Formation_None;
	}
	hor_iter = graph->head;
	ver_iter = graph->head;

	while (ver_iter != NULL)
	{
		while (hor_iter != NULL)
		{
			if (hor_iter->data != NULL)
			{

			}
		}
	}

	return f;
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
