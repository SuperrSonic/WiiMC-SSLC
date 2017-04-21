/****************************************************************************
 * WiiMC
 * Tantric 2009-2012
 *
 * networkop.h
 * Network and SMB support routines
 ****************************************************************************/

#ifndef _NETWORKOP_H_
#define _NETWORKOP_H_

struct SMBSettings {
	char	ip[81];
	char	share[81];
	char	user[21];
	char	pwd[15];
	char	displayname[41];
};

struct FTPSettings {
	char	ip[81];
	char	folder[81];
	char	user[21];
	char	pwd[21];
	int		port;
	int		passive;
	char	displayname[41];
};

void UpdateCheck();
bool DownloadUpdate();
void StartNetworkThread();
bool InitializeNetwork(bool silent);
bool ConnectShare (int num, bool silent);
void CloseShare(int num);
void ReconnectShare(int num, bool silent);
bool ConnectFTP(int num, bool silent);
void CloseFTP(int num);

extern bool updateFound;
extern char wiiIP[16];
#endif
