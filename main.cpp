#include <iostream>
#include <filesystem>
#include <windows.h>
#include <stdio.h>
#include <Lmcons.h>
#include <string>

using namespace std;

int Delete(const char* cPath) {
	int nFolderPathLen = strlen(cPath);

	TCHAR* pszFrom = new TCHAR[nFolderPathLen + 2];
	strcpy(pszFrom, cPath);

	pszFrom[nFolderPathLen] = 0; pszFrom[++nFolderPathLen] = 0;

	SHFILEOPSTRUCT stSHFileOpStruct = { 0 };
	stSHFileOpStruct.wFunc = FO_DELETE;
	stSHFileOpStruct.pFrom = pszFrom;
	stSHFileOpStruct.fFlags = FOF_SILENT | FOF_NOCONFIRMATION | FOF_NOERRORUI | FOF_NOCONFIRMMKDIR | FOF_SILENT;
	stSHFileOpStruct.fAnyOperationsAborted = FALSE;
	int nFileDeleteOprnRet = SHFileOperation(&stSHFileOpStruct);

	delete[] pszFrom;

	if (0 != nFileDeleteOprnRet)
		return 1;

	return 0;
}

int ExecuteDelete(const string& sPath) {
	vector<string> FilesPath; vector<string> FileName;
	for (const auto& entry : filesystem::directory_iterator(sPath)) {
		FilesPath.push_back(entry.path().string());
		FileName.push_back(entry.path().filename().string());
	}

	if (FilesPath.size() > 0) {
		for (int i = 0; i < FilesPath.size(); i++) {
			cout << "Deleting:" << " " << FileName[i] << "\n";
			Delete((LPCSTR&)FilesPath[i]);
		}

		cout << endl; cout << "~ Path [" << sPath << "] ~";
		cout << endl; cout << "~ Deleted [" << FilesPath.size() << "]" << " " << "files ~";
	} else cout << "~ No files had been found ~";
}

int main() {
	TCHAR username[UNLEN + 1]; DWORD size = UNLEN + 1;
	GetUserName((TCHAR*)username, &size);
	
	string sPath = "C:\\Users\\" + std::string(username) + "\\AppData\\Local\\Temp";
	ExecuteDelete(sPath);

        getchar();
}
