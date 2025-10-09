#include "engine_core/engine_error.h"
#ifdef ENGINE_DEBUG
#include "stdio.h"

void internal_Engine_log_errorcode(ecode errorcode) {

	// Zero value means no error.
	if (!errorcode) {
		return;
	}

	char* c = NULL;

	switch (errorcode) {
	// Standard Errors:
	case EPERM:					c = "Operation not permitted."; break;
	case ENOENT:				c = "No such file or directory."; break;
	case ESRCH:					c = "No such process."; break;
	case EINTR:					c = "Interrupted system call."; break;
	case EIO:					c = "Input/output error."; break;
	case ENXIO:					c = "No such device or address."; break;
	case E2BIG:					c = "Argument list too long."; break;
	case ENOEXEC:				c = "Exec format error."; break;
	case EBADF:					c = "Bad file descriptor."; break;
	case ECHILD:				c = "No child processes."; break;
	case EAGAIN:				c = "Resource temporarily unavailable."; break;
	case ENOMEM:				c = "Cannot allocate memory."; break;
	case EACCES:				c = "Permission denied."; break;
	case EFAULT:				c = "Bad address."; break;
	case EBUSY:					c = "Device or resource busy."; break;
	case EEXIST:				c = "File exists."; break;
	case EXDEV:					c = "Invalid cross-device link."; break;
	case ENODEV:				c = "No such device."; break;
	case ENOTDIR:				c = "Not a directory."; break;
	case EISDIR:				c = "Is a directory."; break;
	case EINVAL:				c = "Invalid argument."; break;
	case ENFILE:				c = "Too many open files in system."; break;
	case EMFILE:				c = "Too many open files."; break;
	case ENOTTY:				c = "Inappropriate ioctl for device."; break;
	case ETXTBSY:				c = "Text file busy."; break;
	case EFBIG:					c = "File too large."; break;
	case ENOSPC:				c = "No space left on device."; break;
	case ESPIPE:				c = "Illegal seek."; break;
	case EROFS:					c = "Read-only file system."; break;
	case EMLINK:				c = "Too many links."; break;
	case EPIPE:					c = "Broken pipe."; break;
	case EDOM:					c = "Numerical argument out of domain."; break;
	case ERANGE:				c = "Numerical result out of range."; break;
	case EDEADLK:				c = "Resource deadlock avoided."; break;
	case ENAMETOOLONG:			c = "File name too long."; break;
	case ENOLCK:				c = "No locks available."; break;
	case ENOSYS:				c = "Function not implemented."; break;
	case ENOTEMPTY:				c = "Directory not empty."; break;
	case ELOOP	:				c = "Too many levels of symbolic links."; break;
	case ENOMSG:				c = "No message of desired type."; break;
	case EIDRM	:				c = "Identifier removed."; break;
	case ENOSTR:				c = "Device not a stream."; break;
	case ENODATA:				c = "No data available."; break;
	case ETIME	:				c = "Timer expired."; break;
	case ENOSR	:				c = "Out of streams resources."; break;
	case ENOLINK:				c = "Link has been severed."; break;
	case EPROTO:				c = "Protocol error."; break;
	case EBADMSG:				c = "Bad message."; break;
	case EOVERFLOW:				c = "Value too large for defined data type."; break;
	case EILSEQ:				c = "Invalid or incomplete multi-byte or wide character."; break;
	case ENOTSOCK:				c = "Socket operation on non-socket."; break;
	case EDESTADDRREQ:			c = "Destination address required."; break;
	case EMSGSIZE:				c = "Message too long."; break;
	case EPROTOTYPE:			c = "Protocol wrong type for socket."; break;
	case ENOPROTOOPT:			c = "Protocol not available."; break;
	case EPROTONOSUPPORT:		c = "Protocol not supported."; break;
	//case EOPNOTSUPP:			c = "Operation not supported."; break;
	case EAFNOSUPPORT:			c = "Address family not supported by protocol."; break;
	case EADDRINUSE:			c = "Address already in use."; break;
	case EADDRNOTAVAIL:			c = "Cannot assign requested address."; break;
	case ENETDOWN:				c = "Network is down."; break;
	case ENETUNREACH:			c = "Network is unreachable."; break;
	case ENETRESET:				c = "Network dropped connection on reset."; break;
	case ECONNABORTED:			c = "Software caused connection abort."; break;
	case ECONNRESET:			c = "Connection reset by peer."; break;
	case ENOBUFS:				c = "No buffer space available."; break;
	case EISCONN:				c = "Transport endpoint is already connected."; break;
	case ENOTCONN:				c = "Transport endpoint is not connected."; break;
	case ETIMEDOUT:				c = "Connection timed out."; break;
	case ECONNREFUSED:			c = "Connection refused."; break;
	case EHOSTUNREACH:			c = "No route to host."; break;
	case EALREADY:				c = "Operation already in progress."; break;
	case EINPROGRESS:			c = "Operation now in progress."; break;
	case ECANCELED:				c = "Operation canceled."; break;
	case EOWNERDEAD:			c = "Owner died."; break;
	case ENOTRECOVERABLE:		c = "State not recoverable."; break;
	case ENOTSUP:				c = "Not supported parameter or option."; break;
	// Engine Errors:
	// 	   
	//
    case ERROR_INTSIZE:                     c = "size of int was not expected value."; break;
    case ERROR_GENERIC:						c = "Generic program failure encountered."; break;
	case ERROR_BADPOINTER:					c = "Null pointer was encountered where it should have had a value."; break;
	case ERROR_BADVALUE:					c = "Encountered a nonsensical value."; break;
	case ERROR_IO_FILETOOBIG:				c = "Encountered error while reading file. File was too big to fit within buffer."; break;
	case ERROR_OBJECT_MISSING_PARENT:		c = "Object was initialized without a parent."; break;
	case ERROR_OBJECT_SELF_PARENT:			c = "Object was initialized to be parented to itself. Object will be discarded."; break;
	case ERROR_OBJECT_NULL_OBJECT:			c = "Object was null while initializing."; break;
	case ERROR_OBJECT_NAMETOBIG:			c = "Object name was too big to fit in the object header."; break;
	case ERROR_STRING_INVALIDSIZE:			c = "Tried to initialize a String with invalid size."; break;
	case ERROR_STRING_DSTTOOSMALL:			c = "Tried writing to a String which is too small to fit the contents."; break;
	
	default: c = " ..."; break;
	}
	
	Engine_log("ERROR: %s\n", c);
}
#endif
