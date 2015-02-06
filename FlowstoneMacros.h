/* 
 * File:   FlowstoneMacros.h
 * Author: Thomas
 *
 * Created on 06 February 2015, 19:04
 */

#ifndef FLOWSTONEMACROS_H
#define	FLOWSTONEMACROS_H

#define GETFLOAT(p) *((float*)&p)
#define GETBOOL(p) *((bool*)&p)
#define GETINT(p) p
#define GETSTRING(p) *((char**)&p)
#define GETFLOATARRAY(p) p ? ((float*)p+1) : 0
#define GETINTARRAY(p) p ? ((int*)p+1) : 0
#define GETSTRINGARRAY(p) p ? ((char**)p+1) : 0
#define GETARRAYSIZE(p) p ? *((int*)p) : 0
#define GETFRAME(p) p ? ((float*)p+1) : 0
#define GETFRAMESIZE(p) p ? *((int*)p) : 0
#define GETBITMAPWIDTH(p) p ? *((int*)p) : 0
#define GETBITMAPHEIGHT(p) p ? *((int*)p+1) : 0
#define GETBITMAPCHANNELS(p) p ? *((int*)p+2) : 0
#define GETBITMAPDATA(p) p ? ((BYTE*)p+12) : 0
#define GETBITMAPBYTES(p) p ? *((int*)p) * *((int*)p+1) * *((int*)p+2) : 0
#define NEWINTARRAY(p,n) if(n>0) { *((int**)&p)=new int[n+1]; ((int*)p)[0]=n; }
#define NEWFLOATARRAY(p,n) if(n>0) { *((float**)&p)=new float[n+1]; ((int*)p)[0]=n; }
#define NEWSTRINGARRAY(p,n) if(n>0) { *((char***)&p)=new char*[n+1]; ((int*)p)[0]=n; }
#define DELETESTRING(p) if(p) { delete *((char**)&p); p=0; }
#define DELETEINTARRAY(p) if(p) { delete *((int**)&p); p=0; }
#define DELETEFLOATARRAY(p) if(p) { delete *((float**)&p); p=0; }
#define DELETESTRINGARRAY(p) if(p) { for( int j=0; j<*((int*)p); j++ ) { if( ((char**)p+1)[j] ) delete ((char**)p+1)[j]; } delete *((char***)&p); p=0; }

#endif	/* FLOWSTONEMACROS_H */

