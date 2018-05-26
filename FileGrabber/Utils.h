#include <string.h>
#include <windows.h>
#include <sstream>
#include <vector>
#include <iostream>
#include "zip.h"
#include "dirent.h"

using namespace std;

void ZipFolder(string name, string path);
extern vector<string> ListDirectory(string path, string mask);
extern char *GetTempPath();
extern BOOL FileExists(char *path);