//Written By Rhezaagit

#include <processthreadsapi.h>
#include <string.h>
#include <windows.h>
#include <dirent.h>
#include <stdio.h>
#include "vorbis_data.h"

//log so you can see what error 
void LogMod(const char* text) {
    FILE* f = fopen("SimpleAsiLoader.log", "a");
    if (f) {
        fprintf(f, "%s\n", text);
        fclose(f);
    }
}

//Iterate Every asi and dll file to run loadlibraryA
void LoadAsi(const char* folderPath) {
    DIR *dr = opendir(folderPath);
    if (!dr) {
        CreateDirectoryA(folderPath, NULL);
        return;
    }

    struct dirent *de;
    char fullPath[512];

    while ((de = readdir(dr)) != NULL) {
        if (de->d_name[0] == '.') continue;

        if (strstr(de->d_name, ".asi") || strstr(de->d_name, ".dll")) {
            
            snprintf(fullPath, sizeof(fullPath), "%s%s", folderPath, de->d_name);

			char msg[600];
			//the magic is LoadLibraryA
            if (LoadLibraryA(fullPath)) { // See This The only thing you need to just Load Asi
                sprintf(msg, "Success Loading: %s", fullPath);
                LogMod(msg);
            } else {
            	sprintf(msg, "Failed Loading: %s", fullPath);
                LogMod(msg);
            }
        }
    }
    closedir(dr);
}

//Dont Think To Much About This Just create The Original vorbisFileHook.dll That GTA SA Needed
void CreateOriginal() {
    FILE* check = fopen("vorbisFileHook.dll", "rb");
    if (check) {
        fclose(check);
        return;    
    }

    FILE* f = fopen("vorbisFileHook.dll", "wb");
    if (f) {
        fwrite(vorbisHooked_dll, 1, vorbisHooked_dll_len, f);
        fclose(f);
        ExitProcess(0); // the gta still not load the dll yet so you need run it again
    }
}

void ResetLog() {
	FILE* f = fopen("SimpleAsiLoader.log", "w");
	if (f) {
		fwrite(NULL, 1, 0, f);
	}
}

//The Main Function start When The Loader Begin 
BOOL WINAPI DllMain(HINSTANCE hinstDLL, DWORD fdwReason, LPVOID lpvReserved) { //sont think to much bout the paramater it just decoration
    if (fdwReason == DLL_PROCESS_ATTACH) {
    	ResetLog();
    
        CreateOriginal();
        LoadAsi("./scripts/");
    }
    return TRUE;
}
