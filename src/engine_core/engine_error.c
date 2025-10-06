#include "engine_core/engine_error.h"
#ifdef ENGINE_DEBUG
#include "stdio.h"

#define case(e, str) case e: c = str; break;

void internal_Engine_log_errorcode(ecode errorcode) {

	// Zero value means no error.
	if (!errorcode) {
		return;
	}

	char* c = NULL;

	switch (errorcode) {
	// Standard Errors:

	case(EPERM,					"Operation not permitted.")
	case(ENOENT,				"No such file or directory.")
	case(ESRCH,					"No such process.")
	case(EINTR,					"Interrupted system call.")
	case(EIO,					"Input/output error.")
	case(ENXIO,					"No such device or address.")
	case(E2BIG,					"Argument list too long.")
	case(ENOEXEC,				"Exec format error.")
	case(EBADF,					"Bad file descriptor.")
	case(ECHILD,				"No child processes.")
	case(EAGAIN,				"Resource temporarily unavailable.")
	case(ENOMEM,				"Cannot allocate memory.")
	case(EACCES,				"Permission denied.")
	case(EFAULT,				"Bad address.")
	case(EBUSY,					"Device or resource busy.")
	case(EEXIST,				"File exists.")
	case(EXDEV,					"Invalid cross-device link.")
	case(ENODEV,				"No such device.")
	case(ENOTDIR,				"Not a directory.")
	case(EISDIR,				"Is a directory.")
	case(EINVAL,				"Invalid argument.")
	case(ENFILE,				"Too many open files in system.")
	case(EMFILE,				"Too many open files.")
	case(ENOTTY,				"Inappropriate ioctl for device.")
	case(ETXTBSY,				"Text file busy.")
	case(EFBIG,					"File too large.")
	case(ENOSPC,				"No space left on device.")
	case(ESPIPE,				"Illegal seek.")
	case(EROFS,					"Read-only file system.")
	case(EMLINK,				"Too many links.")
	case(EPIPE,					"Broken pipe.")
	case(EDOM,					"Numerical argument out of domain.")
	case(ERANGE,				"Numerical result out of range.")
	case(EDEADLK,				"Resource deadlock avoided.")
	case(ENAMETOOLONG,			"File name too long.")
	case(ENOLCK,				"No locks available.")
	case(ENOSYS,				"Function not implemented.")
	case(ENOTEMPTY,				"Directory not empty.")
	case(ELOOP	,				"Too many levels of symbolic links.")
	case(ENOMSG,				"No message of desired type.")
	case(EIDRM	,				"Identifier removed.")
	case(ENOSTR,				"Device not a stream.")
	case(ENODATA,				"No data available.")
	case(ETIME	,				"Timer expired.")
	case(ENOSR	,				"Out of streams resources.")
	case(ENOLINK,				"Link has been severed.")
	case(EPROTO,				"Protocol error.")
	case(EBADMSG,				"Bad message.")
	case(EOVERFLOW,				"Value too large for defined data type.")
	case(EILSEQ,				"Invalid or incomplete multi-byte or wide character.")
	case(ENOTSOCK,				"Socket operation on non-socket.")
	case(EDESTADDRREQ,			"Destination address required.")
	case(EMSGSIZE,				"Message too long.")
	case(EPROTOTYPE,			"Protocol wrong type for socket.")
	case(ENOPROTOOPT,			"Protocol not available.")
	case(EPROTONOSUPPORT,		"Protocol not supported.")
	case(EOPNOTSUPP,			"Operation not supported.")
	case(EAFNOSUPPORT,			"Address family not supported by protocol.")
	case(EADDRINUSE,			"Address already in use.")
	case(EADDRNOTAVAIL,			"Cannot assign requested address.")
	case(ENETDOWN,				"Network is down.")
	case(ENETUNREACH,			"Network is unreachable.")
	case(ENETRESET,				"Network dropped connection on reset.")
	case(ECONNABORTED,			"Software caused connection abort.")
	case(ECONNRESET,			"Connection reset by peer.")
	case(ENOBUFS,				"No buffer space available.")
	case(EISCONN,				"Transport endpoint is already connected.")
	case(ENOTCONN,				"Transport endpoint is not connected.")
	case(ETIMEDOUT,				"Connection timed out.")
	case(ECONNREFUSED,			"Connection refused.")
	case(EHOSTUNREACH,			"No route to host.")
	case(EALREADY,				"Operation already in progress.")
	case(EINPROGRESS,			"Operation now in progress.")
	case(ECANCELED,				"Operation canceled.")
	case(EOWNERDEAD,			"Owner died.")
	case(ENOTRECOVERABLE,		"State not recoverable.")
	case(ENOTSUP,				"Not supported parameter or option.")
	// Engine Errors:
	// 	   
	//
	case(ERROR_GENERIC,						"Generic program failure encountered.")
	case(ERROR_BADPOINTER,					"Null pointer was encountered where it should have had a value.")
	case(ERROR_BADVALUE,					"Encountered a nonsensical value.")
	case(ERROR_IO_FILETOOBIG,				"Encountered error while reading file. File was too big to fit within buffer.")
	case(ERROR_OBJECT_MISSING_PARENT,		"Object was initialized without a parent.")
	case(ERROR_OBJECT_SELF_PARENT,			"Object was initialized to be parented to itself. Object will be discarded.")
	case(ERROR_OBJECT_NULL_OBJECT,			"Object was null while initializing.")
	case(ERROR_OBJECT_NAMETOBIG,			"Object name was too big to fit in the object header.")
	case(ERROR_STRING_INVALIDSIZE,			"Tried to initialize a String with invalid size.")
	case(ERROR_STRING_DSTTOOSMALL,			"Tried writing to a String which is too small to fit the contents.")
	
	default: c = " ..."; break;
	}
	
	printf("ERROR: %s\n", c);
}
#endif

#undef case