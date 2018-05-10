/****************************************************************************
 * WiiMC
 * Tantric 2009-2012
 *
 * http.cpp
 *
 * HTTP operations
 * Written by dhewg/bushing, modified by Tantric
 ***************************************************************************/

#include <malloc.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <ogcsys.h>
#include <network.h>
#include <ogc/lwp_watchdog.h>
#include <sys/types.h>
#include <sys/errno.h>
#include <fcntl.h>
#include <ctype.h>


#include "menu.h"
#include "http.h"
#include "wiimc.h"

//#define http_malloc malloc
//#define http_free free
//#define http_memalign memalign

#include "mem2_manager.h"
#define http_malloc(x) mem2_malloc(x, MEM2_OTHER)
#define http_free(x) mem2_free(x, MEM2_OTHER)
#define http_memalign(x,y) mem2_memalign(x,y, MEM2_OTHER)

#define MAX_SIZE (1024*1024*15)

extern "C" {
#include "mplayer/stream/url.h"
}

#define TCP_CONNECT_TIMEOUT 	13  // 13 secs to make a connection
#define TCP_SEND_SIZE 			(32 * 1024)
#define TCP_RECV_SIZE 			(32 * 1024)
#define TCP_BLOCK_RECV_TIMEOUT 	15000 // 15 secs to receive
#define TCP_BLOCK_SEND_TIMEOUT 	15000 // 15 secs to send
#define TCP_BLOCK_SIZE 			2048
#define HTTP_TIMEOUT 			35000 // 35 secs to get an http response
#define IOS_O_NONBLOCK			0x04

static int _httoi(const char *value)
{
	struct CHexMap
	{
		char chr;
		int value;
	};
	const int HexMapL = 16;
	CHexMap HexMap[HexMapL] =
	{
		{'0', 0}, {'1', 1},
		{'2', 2}, {'3', 3},
		{'4', 4}, {'5', 5},
		{'6', 6}, {'7', 7},
		{'8', 8}, {'9', 9},
		{'A', 10}, {'B', 11},
		{'C', 12}, {'D', 13},
		{'E', 14}, {'F', 15}
	};

	char *s = (char *)value;
	int result = 0;
	if (*s == '0' && toupper(*(s + 1)) == 'X') s += 2;
	bool firsttime = true;
	while (*s != '\0')
	{
		bool found = false;
		for (int i = 0; i < HexMapL; i++)
		{
			if (toupper(*s) == HexMap[i].chr)
			{
				if (!firsttime) result <<= 4;
				result |= HexMap[i].value;
				found = true;
				break;
			}
		}
		if (!found) break;
		s++;
		firsttime = false;
	}

	return result;
}

static s32 tcp_socket(void)
{
	s32 s, res;

	s = net_socket(PF_INET, SOCK_STREAM, IPPROTO_IP);
	if (s < 0)
		return s;

	// Switch off Nagle with TCP_NODELAY
	u32 nodelay = 1;
	net_setsockopt(s,IPPROTO_TCP,TCP_NODELAY,&nodelay,sizeof(nodelay));

	res = net_fcntl(s, F_GETFL, 0);
	if (res < 0)
	{
		net_close(s);
		return res;
	}

	res = net_fcntl(s, F_SETFL, res | IOS_O_NONBLOCK);
	if (res < 0)
	{
		net_close(s);
		return res;
	}

	return s;
}

static s32 tcp_connect(char *host, const u16 port)
{
	struct hostent *hp;
	struct sockaddr_in sa;
	struct in_addr val;
	s32 s, res;
	u64 t1;

	s = tcp_socket();
	if (s < 0)
		return s;

	memset(&sa, 0, sizeof(struct sockaddr_in));
	sa.sin_family= PF_INET;
	sa.sin_len = sizeof(struct sockaddr_in);
	sa.sin_port= htons(port);

	if(strlen(host) < 16 && inet_aton(host, &val))
	{
		sa.sin_addr.s_addr = val.s_addr;
	}
	else
	{
		hp = net_gethostbyname (host);
		if (!hp || !(hp->h_addrtype == PF_INET))
			return errno;

		memcpy((char *) &sa.sin_addr, hp->h_addr_list[0], hp->h_length);
	}

	t1=ticks_to_secs(gettime());
	do 
	{
		res = net_connect(s,(struct sockaddr*) &sa, sizeof (sa));
		if(ticks_to_secs(gettime())-t1 > TCP_CONNECT_TIMEOUT) break; 
		usleep(500);
	} while(res != -EISCONN);

	if(res != -EISCONN)
	{		
		net_close(s);
		return -1;
	}

	return s;
}

static int tcp_readln(const s32 s, char *buf, const u16 max_length)
{
	s32 res = -1;
	s32 ret;
	u64 start_time = gettime();
	u16 c = 0;

	while (c < max_length)
	{
		if (ticks_to_millisecs(diff_ticks(start_time, gettime())) > HTTP_TIMEOUT)
			break;

		ret = net_read(s, &buf[c], 1);

		if (ret == -EAGAIN)
		{
			usleep(20 * 1000);
			continue;
		}

		if (ret <= 0)
			break;

		if (c > 0 && buf[c - 1] == '\r' && buf[c] == '\n')
		{
			res = 0;
			buf[c-1] = 0;
			break;
		}
		c++;
		start_time = gettime();
		usleep(100);
	}
	return res;
}

static u32 tcp_read(const s32 s, u8 *buffer, const u32 length)
{
	char *p;
	u32 left, block, received, step=0;
	u64 t;
	s32 res;

	p = (char *)buffer;
	left = length;
	received = 0;

	t = gettime();
	while (left)
	{
		if (ticks_to_millisecs(diff_ticks(t, gettime()))
				> TCP_BLOCK_RECV_TIMEOUT)
		{
			break;
		}

		block = left;
		if (block > TCP_RECV_SIZE)
			block = TCP_RECV_SIZE;

		res = net_read(s, p, block);

		if (res == -EAGAIN)
		{
			usleep(20 * 1000);
			continue;
		}

		if(res<=0) break; 

		received += res;
		left -= res;
		p += res;
		usleep(1000);

		if ((received / TCP_BLOCK_SIZE) > step)
		{
			t = gettime ();
			step++;
		}
	}
	return received;
}

static u32 tcp_write(const s32 s, const u8 *buffer, const u32 length)
{
	const u8 *p;
	u32 left, block, sent, step=0;
	s64 t;
	s32 res;

	p = buffer;
	left = length;
	sent = 0;

	t = gettime();
	while (left)
	{
		if (ticks_to_millisecs(diff_ticks(t, gettime()))
				> TCP_BLOCK_SEND_TIMEOUT)
		{
			break;
		}

		block = left;
		if (block > TCP_SEND_SIZE)
			block = TCP_SEND_SIZE;

		res = net_write(s, p, block);

		if ((res == 0) || (res == -56))
		{
			usleep(20 * 1000);
			continue;
		}

		if (res < 0)
			break;

		sent += res;
		left -= res;
		p += res;
		usleep(100);

		if ((sent / TCP_BLOCK_SIZE) > step)
		{
			t = gettime ();
			step++;
		}
	}

	return left == 0;
}

/****************************************************************************
 * http_request
 * Retrieves the specified URL, and stores it in the specified file or buffer
 ***************************************************************************/
static u32 http_request(char *url, FILE *hfile, char *buffer, u32 maxsize, bool silent, int total_redirects)
{
	u32 res = 0;
	char *http_host=NULL;
	char *http_path=NULL;
	u16 http_port;
	URL_t *tmpurl;

	u32 http_status = 404;
	u32 sizeread = 0, content_length = 0;

	int linecount;

	if(maxsize > MAX_SIZE)
		return 0;

	if (url == NULL || (hfile == NULL && buffer == NULL))
		return 0;

	tmpurl = url_new(url);

	if (tmpurl == NULL)
		return 0;

	if(strlen(tmpurl->hostname) >= 1024 || strlen(tmpurl->file) >= 1024)
	{
		url_free(tmpurl);
		return 0;
	}
	http_host = (char*) malloc(1024);
	http_path = (char*) malloc(1024);

	strcpy(http_host, tmpurl->hostname);
	strcpy(http_path, tmpurl->file);
	http_port = tmpurl->port;

	if(http_port == 0)
		http_port = 80;

	url_free(tmpurl);

	int s = tcp_connect(http_host, http_port);

	if (s < 0)
	{
		free(http_path);
		free(http_host);
		return 0;
	}
	char *request = NULL;
	char *r;

	request = (char*) malloc(1024);
	r = request;

	r += sprintf(r, "GET %s HTTP/1.1\r\n", http_path);
	r += sprintf(r, "Host: %s\r\n", http_host);
	r += sprintf(r, "User-Agent: %s/%s (IOS%d)\r\n", APPNAME, APPVERSION, IOS_GetVersion());
	r += sprintf(r, "Cache-Control: no-cache\r\n\r\n");

	res = tcp_write(s, (u8 *) request, strlen(request));

	free(request);
	free(http_path);
	free(http_host);

	if(res == 0) // can't send request to server
	{
		net_close(s);
		return 0;
	}

	char *line = NULL;
	char *redirect = NULL;
	char encoding[128] = { 0 };
	bool chunked = false;
	redirect = (char*) malloc(1024);
	line = (char*) malloc(1055);
	for (linecount = 0; linecount < 32; linecount++)
	{
		memset(line,0,1055);
		if (tcp_readln(s, line, 1054) != 0)
		{
			http_status = 404;
			break;
		}
		if (strlen(line) < 1)
			break;

		sscanf(line, "HTTP/1.%*u %u", &http_status);
		sscanf(line, "Content-Length: %u", &content_length);
		sscanf(line, "Location: %s", redirect);
		sscanf(line, "Transfer-Encoding: %s", encoding);		
	}

	if (http_status != 200)
	{
		free(line);
		net_close(s);

		if((http_status == 301 || http_status == 302) && redirect[0] != 0 && total_redirects < 5)  //only 5 redirects allowed
		{
			u32 ret = http_request(redirect, hfile, buffer, maxsize, silent, total_redirects+1);
			free(redirect);
			return ret; 
		}
		return 0;
	}
	free(redirect);

	// length unknown - just read as much as we can
	if(content_length == 0)
	{
		content_length = maxsize;
	}
	else if (content_length > maxsize)
	{
		free(line);
		net_close(s);
		return 0;
	}

	chunked = !(strcmp("chunked",encoding));
	if (buffer != NULL)
	{
		if(!silent)
			ShowAction("Downloading...");

		if(chunked)
		{
			u32 ret;
			do
			{
				if (tcp_readln(s, line, 1054) != 0) // read chunk size
				{
					http_status = 404;
					free(line);
					net_close(s);
					return sizeread;
				}

				content_length = _httoi(line);

				if(content_length > 0)
				{
					if(sizeread+content_length > maxsize) 
						break;

					ret = tcp_read(s, (u8 *)buffer+sizeread, content_length);
					if(ret<=0) break;
					sizeread += ret;
				}
				if (tcp_readln(s, line, 1054) != 0)  // read \r\n (chunk boundary)
				{
					http_status = 404;
					free(line);
					net_close(s);
					return sizeread;
				}
			} while(content_length > 0);
			content_length = sizeread;
		}
		else 
		{
			free(line);
			sizeread = tcp_read(s, (u8 *)buffer, content_length);
		}
		if(!silent)
			CancelAction();
	}
	else
	{
		// read into file
		u32 bufSize = (1024 * 32);
		u32 bytesLeft = content_length;
		u32 readSize;
		free(line);

		if(!silent)
			ShowProgress("Downloading...", 0, content_length);
		u8 * fbuffer = (u8 *) http_malloc(bufSize);
		if(fbuffer)
		{
			while (bytesLeft > 0)
			{
				if (bytesLeft < bufSize)
					readSize = bytesLeft;
				else
					readSize = bufSize;

				res = tcp_read(s, fbuffer, readSize);
				if (!res)
					break;

				sizeread += res;
				bytesLeft -= res;

				res = fwrite(fbuffer, 1, res, hfile);
				if (!res)
					break;

				if(!silent)
					ShowProgress("Downloading...", (content_length - bytesLeft), content_length);
			}
			http_free(fbuffer);
		}
		if(!silent)
			CancelAction();
	}

	net_close(s);

	if (content_length < maxsize && sizeread != content_length)
	{
		return 0;
	}

	return sizeread;
}

u32 http_request(char *url, FILE *hfile, char *buffer, u32 maxsize, bool silent)
{
	return http_request(url, hfile, buffer, maxsize, silent, 0);
}
