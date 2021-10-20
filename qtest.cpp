/*
 * Copyright 2000-2021, Hare Team. All rights reserved.
 * Distributed under the terms of the MIT License.
 */
#include <string.h>
#include <OS.h>
#include <Directory.h>
#include <Entry.h>
#include <File.h>
#include <FindDirectory.h>
#include <Path.h>
#include <Query.h>
#include <Volume.h>
#include <VolumeRoster.h>
#include <Debug.h>
#include <String.h>


int32 queryForExecutable(const char*, BVolume*, char*);

int
main(int argc, char* argv[])
{
	char executable[B_FILE_NAME_LENGTH];
	char path[B_FILE_NAME_LENGTH];
	char volumename[B_FILE_NAME_LENGTH];
	BVolumeRoster volumeRoster;
    BVolume bootVol;
    volumeRoster.GetBootVolume(&bootVol);
	bootVol.GetName(volumename);
	dev_t bootvolumeid = dev_for_path("/boot");
	
	strcpy(executable,argv[1]);
	find_directory(B_SYSTEM_BIN_DIRECTORY, bootvolumeid, false, path, B_FILE_NAME_LENGTH);
	printf("arg = %s\n", argv[1]);
    printf("B_SYSTEM_BIN_DIRECTORY = %s\n", path);
    printf("Volume name = %s\n", volumename);
    
	return queryForExecutable(executable, &bootVol, path);
}


int32
queryForExecutable(const char* executable, BVolume* volume, char* path)
{
	BQuery query;
	BString executablePath;
	if(query.SetVolume(volume) != B_OK)
	{
		printf("1");
		return B_ERROR;
	}

	BString predicate("((name==\"");
	predicate += executable;
	predicate += "\")&&(BEOS:TYPE==\"application/x-vnd.*\"))";
	
	
	
	query.SetPredicate(predicate.String());
		printf("Query => %s\n", predicate.String());
	if(query.Fetch() != B_OK)
	{
		printf("Query fetch failed\n");
		return B_ERROR;
	}
	BEntry entry;
	while(query.GetNextEntry(&entry) == B_OK)
	{
		if(entry.InitCheck() != B_OK)
		{
			printf("4");
			return B_ERROR;
		}

		BPath entryPath;
		entry.GetPath(&entryPath);
		if(entryPath.InitCheck() != B_OK)
		{
			printf("5");
			return B_ERROR;
		}
		printf("Path => %s\n", entryPath.Path());
		executablePath = entryPath.Path();
		if(executablePath != entryPath.Path())
		{
			printf("6");
			return B_ERROR;
		}
		printf("Path for %s is %s\n",executable,executablePath.String());
	}	
	return B_OK;
}
