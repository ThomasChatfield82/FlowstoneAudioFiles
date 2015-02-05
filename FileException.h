/* 
 * File:   FileException.h
 * Author: Thomas
 *
 * Created on 13 January 2015, 20:19
 */

#ifndef FILEEXCEPTION_H
#define	FILEEXCEPTION_H

class FileException
{
    public:
    char message[30]; // buffer for messages
    FileException(char *msg)
    { strcpy(message, msg); }
    
    char* getMessage()
    {
        return message;
    }
};

#endif	/* FILEEXCEPTION_H */

