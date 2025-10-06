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

	case EPERM:					"Operation not permitted."; break;
	case ENOENT:				"No such file or directory."; break;
	case ESRCH:					"No such process."; break;
	case EINTR:					"Interrupted system call."; break;
	case EIO:					"Input/output error."; break;
	case ENXIO:					"No such device or address."; break;
	case E2BIG:					"Argument list too long."; break;
	case ENOEXEC:				"Exec format error."; break;
	case EBADF:					"Bad file descriptor."; break;
	case ECHILD:				"No child processes."; break;
	case EAGAIN:				"Resource temporarily unavailable."; break;
	case ENOMEM:				"Cannot allocate memory."; break;
	case EACCES:				"Permission denied."; break;
	case EFAULT:				"Bad address."; break;
	case EBUSY:					"Device or resource busy."; break;
	case EEXIST:				"File exists."; break;
	case EXDEV:					"Invalid cross-device link."; break;
	case ENODEV:				"No such device."; break;
	case ENOTDIR:				"Not a directory."; break;
	case EISDIR:				"Is a directory."; break;
	case EINVAL:				"Invalid argument."; break;
	case ENFILE:				"Too many open files in system."; break;
	case EMFILE:				"Too many open files."; break;
	case ENOTTY:				"Inappropriate ioctl for device."; break;
	case ETXTBSY:				"Text file busy."; break;
	case EFBIG:					"File too large."; break;
	case ENOSPC:				"No space left on device."; break;
	case ESPIPE:				"Illegal seek."; break;
	case EROFS:					"Read-only file system."; break;
	case EMLINK:				"Too many links."; break;
	case EPIPE:					"Broken pipe."; break;
	case EDOM:					"Numerical argument out of domain."; break;
	case ERANGE:				"Numerical result out of range."; break;
	case EDEADLK:				"Resource deadlock avoided."; break;
	case ENAMETOOLONG:			"File name too long."; break;
	case ENOLCK:				"No locks available."; break;
	case ENOSYS:				"Function not implemented."; break;
	case ENOTEMPTY:				"Directory not empty."; break;
	case ELOOP	:				"Too many levels of symbolic links."; break;
	case ENOMSG:				"No message of desired type."; break;
	case EIDRM	:				"Identifier removed."; break;
	case ENOSTR:				"Device not a stream."; break;
	case ENODATA:				"No data available."; break;
	case ETIME	:				"Timer expired."; break;
	case ENOSR	:				"Out of streams resources."; break;
	case ENOLINK:				"Link has been severed."; break;
	case EPROTO:				"Protocol error."; break;
	case EBADMSG:				"Bad message."; break;
	case EOVERFLOW:				"Value too large for defined data type."; break;
	case EILSEQ:				"Invalid or incomplete multi-byte or wide character."; break;
	case ENOTSOCK:				"Socket operation on non-socket."; break;
	case EDESTADDRREQ:			"Destination address required."; break;
	case EMSGSIZE:				"Message too long."; break;
	case EPROTOTYPE:			"Protocol wrong type for socket."; break;
	case ENOPROTOOPT:			"Protocol not available."; break;
	case EPROTONOSUPPORT:		"Protocol not supported."; break;
	//case EOPNOTSUPP:			"Operation not supported."; break;
	case EAFNOSUPPORT:			"Address family not supported by protocol."; break;
	case EADDRINUSE:			"Address already in use."; break;
	case EADDRNOTAVAIL:			"Cannot assign requested address."; break;
	case ENETDOWN:				"Network is down."; break;
	case ENETUNREACH:			"Network is unreachable."; break;
	case ENETRESET:				"Network dropped connection on reset."; break;
	case ECONNABORTED:			"Software caused connection abort."; break;
	case ECONNRESET:			"Connection reset by peer."; break;
	case ENOBUFS:				"No buffer space available."; break;
	case EISCONN:				"Transport endpoint is already connected."; break;
	case ENOTCONN:				"Transport endpoint is not connected."; break;
	case ETIMEDOUT:				"Connection timed out."; break;
	case ECONNREFUSED:			"Connection refused."; break;
	case EHOSTUNREACH:			"No route to host."; break;
	case EALREADY:				"Operation already in progress."; break;
	case EINPROGRESS:			"Operation now in progress."; break;
	case ECANCELED:				"Operation canceled."; break;
	case EOWNERDEAD:			"Owner died."; break;
	case ENOTRECOVERABLE:		"State not recoverable."; break;
	case ENOTSUP:				"Not supported parameter or option."; break;
	// Engine Errors:
	// 	   
	//
	case ERROR_GENERIC:						"Generic program failure encountered."; break;
	case ERROR_BADPOINTER:					"Null pointer was encountered where it should have had a value."; break;
	case ERROR_BADVALUE:					"Encountered a nonsensical value."; break;
	case ERROR_IO_FILETOOBIG:				"Encountered error while reading file. File was too big to fit within buffer."; break;
	case ERROR_OBJECT_MISSING_PARENT:		"Object was initialized without a parent."; break;
	case ERROR_OBJECT_SELF_PARENT:			"Object was initialized to be parented to itself. Object will be discarded."; break;
	case ERROR_OBJECT_NULL_OBJECT:			"Object was null while initializing."; break;
	case ERROR_OBJECT_NAMETOBIG:			"Object name was too big to fit in the object header."; break;
	case ERROR_STRING_INVALIDSIZE:			"Tried to initialize a String with invalid size."; break;
	case ERROR_STRING_DSTTOOSMALL:			"Tried writing to a String which is too small to fit the contents."; break;
	
	default: c = " ..."; break;
	}
	
	printf("ERROR: %s\n", c);
}
#endif

#undef Case
