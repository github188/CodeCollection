/* $Id: miniupnpc.c,v 1.140 2015/10/08 16:15:47 nanard Exp $ */
/* Project : miniupnp
 * Web : http://miniupnp.free.fr/
 * Author : Thomas BERNARD
 * copyright (c) 2005-2015 Thomas Bernard
 * This software is subjet to the conditions detailed in the
 * provided LICENSE file. */
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#ifdef _WIN32
/* Win32 Specific includes and defines */
#include <winsock2.h>
#include <ws2tcpip.h>
#include <io.h>
#include <iphlpapi.h>
#define snprintf _snprintf
#define strdup _strdup
#ifndef strncasecmp
#if defined(_MSC_VER) && (_MSC_VER >= 1400)
#define strncasecmp _memicmp
#else /* defined(_MSC_VER) && (_MSC_VER >= 1400) */
#define strncasecmp memicmp
#endif /* defined(_MSC_VER) && (_MSC_VER >= 1400) */
#endif /* #ifndef strncasecmp */
#define MAXHOSTNAMELEN 64
#else /* #ifdef _WIN32 */
/* Standard POSIX includes */
#include <unistd.h>
#if defined(__amigaos__) && !defined(__amigaos4__)
/* Amiga OS 3 specific stuff */
#define socklen_t int
#else
#include <sys/select.h>
#endif
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/param.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <net/if.h>
#if !defined(__amigaos__) && !defined(__amigaos4__)
#include <poll.h>
#endif
#include <strings.h>
#include <errno.h>
#define closesocket close
#endif /* #else _WIN32 */
#ifdef __GNU__
#define MAXHOSTNAMELEN 64
#endif


#include "miniupnpc.h"
#include "minissdpc.h"
#include "miniwget.h"
#include "minisoap.h"
#include "minixml.h"
#include "upnpcommands.h"
#include "connecthostport.h"

/* compare the begining of a string with a constant string */
#define COMPARE(str, cstr) (0==memcmp(str, cstr, sizeof(cstr) - 1))

#ifndef MAXHOSTNAMELEN
#define MAXHOSTNAMELEN 64
#endif

#define SOAPPREFIX "s"
#define SERVICEPREFIX "u"
#define SERVICEPREFIX2 'u'

/* root description parsing */
MINIUPNP_LIBSPEC void parserootdesc(const char * buffer, int bufsize, struct IGDdatas * data)
{
	struct xmlparser parser;
	/* xmlparser object */
	parser.xmlstart = buffer;
	parser.xmlsize = bufsize;
	parser.data = data;
	parser.starteltfunc = IGDstartelt;
	parser.endeltfunc = IGDendelt;
	parser.datafunc = IGDdata;
	parser.attfunc = 0;
	parsexml(&parser);
#ifdef DEBUG
	printIGD(data);
#endif
}

/* simpleUPnPcommand2 :
 * not so simple !
 * return values :
 *   pointer - OK
 *   NULL - error */
char * simpleUPnPcommand2(int s, const char * url, const char * service,
		       const char * action, struct UPNParg * args,
		       int * bufsize, const char * httpversion)
{
	char hostname[MAXHOSTNAMELEN+1];
	unsigned short port = 0;
	char * path;
	char soapact[128];
	char soapbody[2048];
	char * buf;
    int n;

	*bufsize = 0;
	snprintf(soapact, sizeof(soapact), "%s#%s", service, action);
	if(args==NULL)
	{
		/*soapbodylen = */snprintf(soapbody, sizeof(soapbody),
						"<?xml version=\"1.0\"?>\r\n"
	    	              "<" SOAPPREFIX ":Envelope "
						  "xmlns:" SOAPPREFIX "=\"http://schemas.xmlsoap.org/soap/envelope/\" "
						  SOAPPREFIX ":encodingStyle=\"http://schemas.xmlsoap.org/soap/encoding/\">"
						  "<" SOAPPREFIX ":Body>"
						  "<" SERVICEPREFIX ":%s xmlns:" SERVICEPREFIX "=\"%s\">"
						  "</" SERVICEPREFIX ":%s>"
						  "</" SOAPPREFIX ":Body></" SOAPPREFIX ":Envelope>"
					 	  "\r\n", action, service, action);
	}
	else
	{
		char * p;
		const char * pe, * pv;
		int soapbodylen;
		soapbodylen = snprintf(soapbody, sizeof(soapbody),
						"<?xml version=\"1.0\"?>\r\n"
	    	            "<" SOAPPREFIX ":Envelope "
						"xmlns:" SOAPPREFIX "=\"http://schemas.xmlsoap.org/soap/envelope/\" "
						SOAPPREFIX ":encodingStyle=\"http://schemas.xmlsoap.org/soap/encoding/\">"
						"<" SOAPPREFIX ":Body>"
						"<" SERVICEPREFIX ":%s xmlns:" SERVICEPREFIX "=\"%s\">",
						action, service);
		p = soapbody + soapbodylen;
		while(args->elt)
		{
			/* check that we are never overflowing the string... */
			if(soapbody + sizeof(soapbody) <= p + 100)
			{
				/* we keep a margin of at least 100 bytes */
				return NULL;
			}
			*(p++) = '<';
			pe = args->elt;
			while(*pe)
				*(p++) = *(pe++);
			*(p++) = '>';
			if((pv = args->val))
			{
				while(*pv)
					*(p++) = *(pv++);
			}
			*(p++) = '<';
			*(p++) = '/';
			pe = args->elt;
			while(*pe)
				*(p++) = *(pe++);
			*(p++) = '>';
			args++;
		}
		*(p++) = '<';
		*(p++) = '/';
		*(p++) = SERVICEPREFIX2;
		*(p++) = ':';
		pe = action;
		while(*pe)
			*(p++) = *(pe++);
		strncpy(p, "></" SOAPPREFIX ":Body></" SOAPPREFIX ":Envelope>\r\n",
		        soapbody + sizeof(soapbody) - p);
	}
	if(!parseURL(url, hostname, &port, &path, NULL)) return NULL;
	if(s < 0) {
		s = connecthostport(hostname, port, 0);
		if(s < 0) {
			/* failed to connect */
			return NULL;
		}
	}

	n = soapPostSubmit(s, path, hostname, port, soapact, soapbody, httpversion);
	if(n<=0) {
#ifdef DEBUG
		printf("Error sending SOAP request\n");
#endif
		closesocket(s);
		return NULL;
	}

	buf = getHTTPResponse(s, bufsize);
#ifdef DEBUG
	if(*bufsize > 0 && buf)
	{
		printf("SOAP Response :\n%.*s\n", *bufsize, buf);
	}
#endif
	closesocket(s);
	return buf;
}

/* simpleUPnPcommand :
 * not so simple !
 * return values :
 *   pointer - OK
 *   NULL    - error */
char * simpleUPnPcommand(int s, const char * url, const char * service,
		       const char * action, struct UPNParg * args,
		       int * bufsize)
{
	char * buf;

#if 1
	buf = simpleUPnPcommand2(s, url, service, action, args, bufsize, "1.1");
#else
	buf = simpleUPnPcommand2(s, url, service, action, args, bufsize, "1.0");
	if (!buf || *bufsize == 0)
	{
#if DEBUG
	    printf("Error or no result from SOAP request; retrying with HTTP/1.1\n");
#endif
		buf = simpleUPnPcommand2(s, url, service, action, args, bufsize, "1.1");
	}
#endif
	return buf;
}

/* upnpDiscoverDevices() :
 * return a chained list of all devices found or NULL if
 * no devices was found.
 * It is up to the caller to free the chained list
 * delay is in millisecond (poll).
 * UDA v1.1 says :
 *   The TTL for the IP packet SHOULD default to 2 and
 *   SHOULD be configurable. */
MINIUPNP_LIBSPEC struct UPNPDev *
upnpDiscoverDevices(const char * const deviceTypes[],
                    int delay, const char * multicastif,
                    const char * minissdpdsock, int localport,
                    int ipv6, unsigned char ttl,
                    int * error,
                    int searchalltypes)
{
	struct UPNPDev * tmp;
	struct UPNPDev * devlist = 0;
#if !defined(_WIN32) && !defined(__amigaos__) && !defined(__amigaos4__)
	int deviceIndex;
#endif /* !defined(_WIN32) && !defined(__amigaos__) && !defined(__amigaos4__) */

	if(error)
		*error = UPNPDISCOVER_UNKNOWN_ERROR;
#if !defined(_WIN32) && !defined(__amigaos__) && !defined(__amigaos4__)
	/* first try to get infos from minissdpd ! */
	if(!minissdpdsock)
		minissdpdsock = "/var/run/minissdpd.sock";
	for(deviceIndex = 0; deviceTypes[deviceIndex]; deviceIndex++) {
		struct UPNPDev * minissdpd_devlist;
		int only_rootdevice = 1;
		minissdpd_devlist = getDevicesFromMiniSSDPD(deviceTypes[deviceIndex],
		                                            minissdpdsock, 0);
		if(minissdpd_devlist) {
#ifdef DEBUG
			printf("returned by MiniSSDPD: %s\t%s\n",
			       minissdpd_devlist->st, minissdpd_devlist->descURL);
#endif /* DEBUG */
			if(!strstr(minissdpd_devlist->st, "rootdevice"))
				only_rootdevice = 0;
			for(tmp = minissdpd_devlist; tmp->pNext != NULL; tmp = tmp->pNext) {
#ifdef DEBUG
				printf("returned by MiniSSDPD: %s\t%s\n",
				       tmp->pNext->st, tmp->pNext->descURL);
#endif /* DEBUG */
				if(!strstr(tmp->st, "rootdevice"))
					only_rootdevice = 0;
			}
			tmp->pNext = devlist;
			devlist = minissdpd_devlist;
			if(!searchalltypes && !only_rootdevice)
				break;
		}
	}
	for(tmp = devlist; tmp != NULL; tmp = tmp->pNext) {
		/* We return what we have found if it was not only a rootdevice */
		if(!strstr(tmp->st, "rootdevice")) {
			if(error)
				*error = UPNPDISCOVER_SUCCESS;
			return devlist;
		}
	}
#endif	/* !defined(_WIN32) && !defined(__amigaos__) && !defined(__amigaos4__) */

	/* direct discovery if minissdpd responses are not sufficient */
	{
		struct UPNPDev * discovered_devlist;
		discovered_devlist = ssdpDiscoverDevices(deviceTypes, delay, multicastif, localport,
		                                         ipv6, ttl, error, searchalltypes);
		if(devlist == NULL)
			devlist = discovered_devlist;
		else {
			for(tmp = devlist; tmp->pNext != NULL; tmp = tmp->pNext);
			tmp->pNext = discovered_devlist;
		}
	}
	return devlist;
}

/* upnpDiscover() Discover IGD device */
MINIUPNP_LIBSPEC struct UPNPDev *
upnpDiscover(int delay, const char * multicastif,
             const char * minissdpdsock, int localport,
             int ipv6, unsigned char ttl,
             int * error)
{
	static const char * const deviceList[] = {
#if 0
		"urn:schemas-upnp-org:device:InternetGatewayDevice:2",
		"urn:schemas-upnp-org:service:WANIPConnection:2",
#endif
		"urn:schemas-upnp-org:device:InternetGatewayDevice:1",
		"urn:schemas-upnp-org:service:WANIPConnection:1",
		"urn:schemas-upnp-org:service:WANPPPConnection:1",
		"upnp:rootdevice",
		/*"ssdp:all",*/
		0
	};
	return upnpDiscoverDevices(deviceList,
	                           delay, multicastif, minissdpdsock, localport,
	                           ipv6, ttl, error, 0);
}

/* upnpDiscoverAll() Discover all UPnP devices */
MINIUPNP_LIBSPEC struct UPNPDev *
upnpDiscoverAll(int delay, const char * multicastif,
                const char * minissdpdsock, int localport,
                int ipv6, unsigned char ttl,
                int * error)
{
	static const char * const deviceList[] = {
		/*"upnp:rootdevice",*/
		"ssdp:all",
		0
	};
	return upnpDiscoverDevices(deviceList,
	                           delay, multicastif, minissdpdsock, localport,
	                           ipv6, ttl, error, 0);
}

/* upnpDiscoverDevice() Discover a specific device */
MINIUPNP_LIBSPEC struct UPNPDev *
upnpDiscoverDevice(const char * device, int delay, const char * multicastif,
                const char * minissdpdsock, int localport,
                int ipv6, unsigned char ttl,
                int * error)
{
	const char * const deviceList[] = {
		device,
		0
	};
	return upnpDiscoverDevices(deviceList,
	                           delay, multicastif, minissdpdsock, localport,
	                           ipv6, ttl, error, 0);
}

static char *
build_absolute_url(const char * baseurl, const char * descURL,
                   const char * url, unsigned int scope_id)
{
	int l, n;
	char * s;
	const char * base;
	char * p;
#if defined(IF_NAMESIZE) && !defined(_WIN32)
	char ifname[IF_NAMESIZE];
#else /* defined(IF_NAMESIZE) && !defined(_WIN32) */
	char scope_str[8];
#endif	/* defined(IF_NAMESIZE) && !defined(_WIN32) */

	if(  (url[0] == 'h')
	   &&(url[1] == 't')
	   &&(url[2] == 't')
	   &&(url[3] == 'p')
	   &&(url[4] == ':')
	   &&(url[5] == '/')
	   &&(url[6] == '/'))
		return strdup(url);
	base = (baseurl[0] == '\0') ? descURL : baseurl;
	n = strlen(base);
	if(n > 7) {
		p = strchr(base + 7, '/');
		if(p)
			n = p - base;
	}
	l = n + strlen(url) + 1;
	if(url[0] != '/')
		l++;
	if(scope_id != 0) {
#if defined(IF_NAMESIZE) && !defined(_WIN32)
		if(if_indextoname(scope_id, ifname)) {
			l += 3 + strlen(ifname);	/* 3 == strlen(%25) */
		}
#else /* defined(IF_NAMESIZE) && !defined(_WIN32) */
		/* under windows, scope is numerical */
		l += 3 + snprintf(scope_str, sizeof(scope_str), "%u", scope_id);
#endif /* defined(IF_NAMESIZE) && !defined(_WIN32) */
	}
	s = malloc(l);
	if(s == NULL) return NULL;
	memcpy(s, base, n);
	if(scope_id != 0) {
		s[n] = '\0';
		if(0 == memcmp(s, "http://[fe80:", 13)) {
			/* this is a linklocal IPv6 address */
			p = strchr(s, ']');
			if(p) {
				/* insert %25<scope> into URL */
#if defined(IF_NAMESIZE) && !defined(_WIN32)
				memmove(p + 3 + strlen(ifname), p, strlen(p) + 1);
				memcpy(p, "%25", 3);
				memcpy(p + 3, ifname, strlen(ifname));
				n += 3 + strlen(ifname);
#else /* defined(IF_NAMESIZE) && !defined(_WIN32) */
				memmove(p + 3 + strlen(scope_str), p, strlen(p) + 1);
				memcpy(p, "%25", 3);
				memcpy(p + 3, scope_str, strlen(scope_str));
				n += 3 + strlen(scope_str);
#endif /* defined(IF_NAMESIZE) && !defined(_WIN32) */
			}
		}
	}
	if(url[0] != '/')
		s[n++] = '/';
	memcpy(s + n, url, l - n);
	return s;
}

/* Prepare the Urls for usage...
 */
MINIUPNP_LIBSPEC void
GetUPNPUrls(struct UPNPUrls * urls, struct IGDdatas * data,
            const char * descURL, unsigned int scope_id)
{
	/* strdup descURL */
	urls->rootdescURL = strdup(descURL);

	/* get description of WANIPConnection */
	urls->ipcondescURL = build_absolute_url(data->urlbase, descURL,
	                                        data->first.scpdurl, scope_id);
	urls->controlURL = build_absolute_url(data->urlbase, descURL,
	                                      data->first.controlurl, scope_id);
	urls->controlURL_CIF = build_absolute_url(data->urlbase, descURL,
	                                          data->CIF.controlurl, scope_id);
	urls->controlURL_6FC = build_absolute_url(data->urlbase, descURL,
	                                          data->IPv6FC.controlurl, scope_id);

#ifdef DEBUG
	printf("urls->ipcondescURL='%s'\n", urls->ipcondescURL);
	printf("urls->controlURL='%s'\n", urls->controlURL);
	printf("urls->controlURL_CIF='%s'\n", urls->controlURL_CIF);
	printf("urls->controlURL_6FC='%s'\n", urls->controlURL_6FC);
#endif
}

MINIUPNP_LIBSPEC void
FreeUPNPUrls(struct UPNPUrls * urls)
{
	if(!urls)
		return;
	free(urls->controlURL);
	urls->controlURL = 0;
	free(urls->ipcondescURL);
	urls->ipcondescURL = 0;
	free(urls->controlURL_CIF);
	urls->controlURL_CIF = 0;
	free(urls->controlURL_6FC);
	urls->controlURL_6FC = 0;
	free(urls->rootdescURL);
	urls->rootdescURL = 0;
}

int
UPNPIGD_IsConnected(struct UPNPUrls * urls, struct IGDdatas * data)
{
	char status[64];
	unsigned int uptime;
	status[0] = '\0';
	UPNP_GetStatusInfo(urls->controlURL, data->first.servicetype,
	                   status, &uptime, NULL);
	if(0 == strcmp("Connected", status))
		return 1;
	else if(0 == strcmp("Up", status))	/* Also accept "Up" */
		return 1;
	else
		return 0;
}


/* UPNP_GetValidIGD() :
 * return values :
 *    -1 = Internal error
 *     0 = NO IGD found
 *     1 = A valid connected IGD has been found
 *     2 = A valid IGD has been found but it reported as
 *         not connected
 *     3 = an UPnP device has been found but was not recognized as an IGD
 *
 * In any positive non zero return case, the urls and data structures
 * passed as parameters are set. Donc forget to call FreeUPNPUrls(urls) to
 * free allocated memory.
 */
MINIUPNP_LIBSPEC int
UPNP_GetValidIGD(struct UPNPDev * devlist,
                 struct UPNPUrls * urls,
				 struct IGDdatas * data,
				 char * lanaddr, int lanaddrlen)
{
	struct xml_desc {
		char * xml;
		int size;
		int is_igd;
	} * desc = NULL;
	struct UPNPDev * dev;
	int ndev = 0;
	int i;
	int state = -1; /* state 1 : IGD connected. State 2 : IGD. State 3 : anything */
	int n_igd = 0;
	char extIpAddr[16];
	if(!devlist)
	{
#ifdef DEBUG
		printf("Empty devlist\n");
#endif
		return 0;
	}
	/* counting total number of devices in the list */
	for(dev = devlist; dev; dev = dev->pNext)
		ndev++;
	if(ndev > 0)
	{
		desc = calloc(ndev, sizeof(struct xml_desc));
		if(!desc)
			return -1; /* memory allocation error */
	}
	/* Step 1 : downloading descriptions and testing type */
	for(dev = devlist, i = 0; dev; dev = dev->pNext, i++)
	{
		/* we should choose an internet gateway device.
		 * with st == urn:schemas-upnp-org:device:InternetGatewayDevice:1 */
		desc[i].xml = miniwget_getaddr(dev->descURL, &(desc[i].size),
		                               lanaddr, lanaddrlen,
		                               dev->scope_id);
#ifdef DEBUG
		if(!desc[i].xml)
		{
			printf("error getting XML description %s\n", dev->descURL);
		}
#endif
		if(desc[i].xml)
		{
			memset(data, 0, sizeof(struct IGDdatas));
			memset(urls, 0, sizeof(struct UPNPUrls));
			parserootdesc(desc[i].xml, desc[i].size, data);
			if(COMPARE(data->CIF.servicetype,
			           "urn:schemas-upnp-org:service:WANCommonInterfaceConfig:"))
			{
				desc[i].is_igd = 1;
				n_igd++;
			}
		}
	}
	/* iterate the list to find a device depending on state */
	for(state = 1; state <= 3; state++)
	{
		for(dev = devlist, i = 0; dev; dev = dev->pNext, i++)
		{
			if(desc[i].xml)
			{
				memset(data, 0, sizeof(struct IGDdatas));
				memset(urls, 0, sizeof(struct UPNPUrls));
				parserootdesc(desc[i].xml, desc[i].size, data);
				if(desc[i].is_igd || state >= 3 )
				{
				  GetUPNPUrls(urls, data, dev->descURL, dev->scope_id);

				  /* in state 2 and 3 we dont test if device is connected ! */
				  if(state >= 2)
				    goto free_and_return;
#ifdef DEBUG
				  printf("UPNPIGD_IsConnected(%s) = %d\n",
				     urls->controlURL,
			         UPNPIGD_IsConnected(urls, data));
#endif
				  /* checks that status is connected AND there is a external IP address assigned */
				  if(UPNPIGD_IsConnected(urls, data)
				     && (UPNP_GetExternalIPAddress(urls->controlURL,  data->first.servicetype, extIpAddr) == 0))
					goto free_and_return;
				  FreeUPNPUrls(urls);
				  if(data->second.servicetype[0] != '\0') {
#ifdef DEBUG
				    printf("We tried %s, now we try %s !\n",
				           data->first.servicetype, data->second.servicetype);
#endif
				    /* swaping WANPPPConnection and WANIPConnection ! */
				    memcpy(&data->tmp, &data->first, sizeof(struct IGDdatas_service));
				    memcpy(&data->first, &data->second, sizeof(struct IGDdatas_service));
				    memcpy(&data->second, &data->tmp, sizeof(struct IGDdatas_service));
				    GetUPNPUrls(urls, data, dev->descURL, dev->scope_id);
#ifdef DEBUG
				    printf("UPNPIGD_IsConnected(%s) = %d\n",
				       urls->controlURL,
			           UPNPIGD_IsConnected(urls, data));
#endif
				    if(UPNPIGD_IsConnected(urls, data)
				       && (UPNP_GetExternalIPAddress(urls->controlURL,  data->first.servicetype, extIpAddr) == 0))
					  goto free_and_return;
				    FreeUPNPUrls(urls);
				  }
				}
				memset(data, 0, sizeof(struct IGDdatas));
			}
		}
	}
	state = 0;
free_and_return:
	if(desc) {
		for(i = 0; i < ndev; i++) {
			if(desc[i].xml) {
				free(desc[i].xml);
			}
		}
		free(desc);
	}
	return state;
}

/* UPNP_GetIGDFromUrl()
 * Used when skipping the discovery process.
 * return value :
 *   0 - Not ok
 *   1 - OK */
int
UPNP_GetIGDFromUrl(const char * rootdescurl,
                   struct UPNPUrls * urls,
                   struct IGDdatas * data,
                   char * lanaddr, int lanaddrlen)
{
	char * descXML;
	int descXMLsize = 0;
	descXML = miniwget_getaddr(rootdescurl, &descXMLsize,
	   	                       lanaddr, lanaddrlen, 0);
	if(descXML) {
		memset(data, 0, sizeof(struct IGDdatas));
		memset(urls, 0, sizeof(struct UPNPUrls));
		parserootdesc(descXML, descXMLsize, data);
		free(descXML);
		descXML = NULL;
		GetUPNPUrls(urls, data, rootdescurl, 0);
		return 1;
	} else {
		return 0;
	}
}
