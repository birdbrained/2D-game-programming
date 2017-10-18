#include "band.h"

Band * band_init(Uint32 numMembers)
{
	Band * new_band;
	new_band = (Band *)malloc(sizeof(Band) * numMembers);
	if (!new_band)
	{
		return NULL;
	}
	memset(new_band, 0, sizeof(Band) * numMembers);
	return new_band;
}

Band * band_load_from_file(FILE * file)
{
	Band * new_band = NULL;
	Uint32 numMembers = 0;
	char buffer[512];
	int i = 0;
	FILE * entityFile = NULL;
	if (!file)
	{
		slog("Error: could not open file");
		return NULL;
	}
	rewind(file);

	while (fscanf(file, "%s", buffer) != EOF)
	{
		if (strcmp(buffer, "num:") == 0)
		{
			fscanf(file, "%i", &numMembers);
			new_band = band_init(numMembers);
		}
		if (strcmp(buffer, "members:") == 0)
		{
			if (new_band != NULL)
			{
				for (i = 0; i < numMembers; i++)
				{
					if (&new_band->myBand[i] == NULL)
					{
						fscanf(file, "%s", buffer);
						if (buffer == EOF)
						{
							slog("end of file");
							return new_band;
						}
						entityFile = fopen(buffer, "r");
						if (!entityFile)
						{
							slog("error opening file");
							fclose(entityFile);
							break;
						}
						entityLoadFromFile(entityFile, entityNew(&new_band->myBand[i]));
						fclose(entityFile);
					}
				}
			}
		}
	}

	return new_band;
}
