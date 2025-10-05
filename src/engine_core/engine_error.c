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
	case EPERM: 	 		c = "Operation not permitted."; break;
	case ENOENT: 	 		c = "No such file or directory."; break;
	case ESRCH: 	 		c = "No such process."; break;
	case EINTR: 	 		c = "Interrupted system call."; break;
	case EIO: 	 			c = "Input/output error."; break;
	case ENXIO: 	 		c = "No such device or address."; break;
	case E2BIG: 	 		c = "Argument list too long."; break;
	case ENOEXEC: 	 		c = "Exec format error."; break;
	case EBADF: 	 		c = "Bad file descriptor."; break;
	case ECHILD: 	 		c = "No child processes."; break;
	case EAGAIN: 	 		c = "Resource temporarily unavailable."; break;
	case ENOMEM: 	 		c = "Cannot allocate memory."; break;
	case EACCES: 	 		c = "Permission denied."; break;
	case EFAULT: 	 		c = "Bad address."; break;
	//case ENOTBLK: 	 		c = "Block device required."; break;
	case EBUSY: 	 		c = "Device or resource busy."; break;
	case EEXIST: 	 		c = "File exists."; break;
	case EXDEV: 	 		c = "Invalid cross-device link."; break;
	case ENODEV: 	 		c = "No such device."; break;
	case ENOTDIR: 	 		c = "Not a directory."; break;
	case EISDIR: 	 		c = "Is a directory."; break;
	case EINVAL: 	 		c = "Invalid argument."; break;
	case ENFILE: 	 		c = "Too many open files in system."; break;
	case EMFILE: 	 		c = "Too many open files."; break;
	case ENOTTY: 	 		c = "Inappropriate ioctl for device."; break;
	case ETXTBSY: 	 		c = "Text file busy."; break;
	case EFBIG: 	 		c = "File too large."; break;
	case ENOSPC: 	 		c = "No space left on device."; break;
	case ESPIPE: 	 		c = "Illegal seek."; break;
	case EROFS: 	 		c = "Read-only file system."; break;
	case EMLINK: 	 		c = "Too many links."; break;
	case EPIPE: 	 		c = "Broken pipe."; break;
	case EDOM: 	 			c = "Numerical argument out of domain."; break;
	case ERANGE: 	 		c = "Numerical result out of range."; break;
	case EDEADLK: 	 		c = "Resource deadlock avoided."; break;
	case ENAMETOOLONG: 	 	c = "File name too long."; break;
	case ENOLCK: 	 		c = "No locks available."; break;
	case ENOSYS: 	 		c = "Function not implemented."; break;
	case ENOTEMPTY: 	 	c = "Directory not empty."; break;
	case ELOOP: 	 		c = "Too many levels of symbolic links."; break;
	case ENOMSG: 	 		c = "No message of desired type."; break;
	case EIDRM: 	 		c = "Identifier removed."; break;
	//case ECHRNG: 	 		c = "Channel number out of range."; break;
	//case EL2NSYNC: 	 		c = "Level 2 not synchronized."; break;
	//case EL3HLT: 	 		c = "Level 3 halted."; break;
	//case EL3RST: 	 		c = "Level 3 reset."; break;
	//case ELNRNG: 	 		c = "Link number out of range."; break;
	//case EUNATCH: 	 		c = "Protocol driver not attached."; break;
	//case ENOCSI: 	 		c = "No CSI structure available."; break;
	//case EL2HLT: 	 		c = "Level 2 halted."; break;
	//case EBADE: 	 		c = "Invalid exchange."; break;
	//case EBADR: 	 		c = "Invalid request descriptor."; break;
	//case EXFULL: 	 		c = "Exchange full."; break;
	//case ENOANO: 	 		c = "No anode."; break;
	//case EBADRQC: 	 		c = "Invalid request code."; break;
	//case EBADSLT: 	 		c = "Invalid slot."; break;
	//case EBFONT: 	 		c = "Bad font file format."; break;
	case ENOSTR: 	 		c = "Device not a stream."; break;
	case ENODATA: 	 		c = "No data available."; break;
	case ETIME: 	 		c = "Timer expired."; break;
	case ENOSR: 	 		c = "Out of streams resources."; break;
	//case ENONET: 	 		c = "Machine is not on the network."; break;
	//case ENOPKG: 	 		c = "Package not installed."; break;
	//case EREMOTE: 	 		c = "Object is remote."; break;
	case ENOLINK: 	 		c = "Link has been severed."; break;
	//case EADV: 	 			c = "Advertise error."; break;
	//case ESRMNT: 	 		c = "Srmount error."; break;
	//case ECOMM: 	 		c = "Communication error on send."; break;
	case EPROTO: 	 		c = "Protocol error."; break;
	//case EMULTIHOP: 	 	c = "Multihop attempted."; break;
	//case EDOTDOT: 	 		c = "RFS specific error."; break;
	case EBADMSG: 	 		c = "Bad message."; break;
	case EOVERFLOW: 	 	c = "Value too large for defined data type."; break;
	//case ENOTUNIQ: 	 		c = "Name not unique on network."; break;
	//case EBADFD: 	 		c = "File descriptor in bad state."; break;
	//case EREMCHG: 	 		c = "Remote address changed."; break;
	//case ELIBACC: 	 		c = "Can not access a needed shared library."; break;
	//case ELIBBAD: 	 		c = "Accessing a corrupted shared library."; break;
	//case ELIBSCN: 	 		c = "lib section in a.out corrupted."; break;
	//case ELIBMAX: 	 		c = "Attempting to link in too many shared libraries."; break;
	//case ELIBEXEC: 	 		c = "Cannot exec a shared library directly."; break;
	case EILSEQ: 	 		c = "Invalid or incomplete multibyte or wide character."; break;
	//case ERESTART: 	 		c = "Interrupted system call should be restarted."; break;
	//case ESTRPIPE: 	 		c = "Streams pipe error."; break;
	//case EUSERS: 	 		c = "Too many users."; break;
	case ENOTSOCK: 	 		c = "Socket operation on non-socket."; break;
	case EDESTADDRREQ: 	 	c = "Destination address required."; break;
	case EMSGSIZE: 	 		c = "Message too long."; break;
	case EPROTOTYPE: 	 	c = "Protocol wrong type for socket."; break;
	case ENOPROTOOPT: 	 	c = "Protocol not available."; break;
	case EPROTONOSUPPORT: 	c = "Protocol not supported."; break;
	//case ESOCKTNOSUPPORT: 	c = "Socket type not supported."; break;
	case EOPNOTSUPP: 	 	c = "Operation not supported."; break;
	//case EPFNOSUPPORT: 	 	c = "Protocol family not supported."; break;
	case EAFNOSUPPORT: 	 	c = "Address family not supported by protocol."; break;
	case EADDRINUSE: 	 	c = "Address already in use."; break;
	case EADDRNOTAVAIL: 	c = "Cannot assign requested address."; break;
	case ENETDOWN: 	 		c = "Network is down."; break;
	case ENETUNREACH: 	 	c = "Network is unreachable."; break;
	case ENETRESET: 	 	c = "Network dropped connection on reset."; break;
	case ECONNABORTED: 	 	c = "Software caused connection abort."; break;
	case ECONNRESET: 	 	c = "Connection reset by peer."; break;
	case ENOBUFS: 	 		c = "No buffer space available."; break;
	case EISCONN: 	 		c = "Transport endpoint is already connected."; break;
	case ENOTCONN: 	 		c = "Transport endpoint is not connected."; break;
	//case ESHUTDOWN: 	 	c = "Cannot send after transport endpoint shutdown."; break;
	//case ETOOMANYREFS: 	 	c = "Too many references: cannot splice."; break;
	case ETIMEDOUT: 	 	c = "Connection timed out."; break;
	case ECONNREFUSED: 	 	c = "Connection refused."; break;
	//case EHOSTDOWN: 	 	c = "Host is down."; break;
	case EHOSTUNREACH: 	 	c = "No route to host."; break;
	case EALREADY: 	 		c = "Operation already in progress."; break;
	case EINPROGRESS: 	 	c = "Operation now in progress."; break;
	//case ESTALE: 	 		c = "Stale file handle."; break;
	//case EUCLEAN: 	 		c = "Structure needs cleaning."; break;
	//case ENOTNAM: 	 		c = "Not a XENIX named type file."; break;
	//case ENAVAIL: 	 		c = "No XENIX semaphores available."; break;
	//case EISNAM: 	 		c = "Is a named type file."; break;
	//case EREMOTEIO: 	 	c = "Remote I/O error."; break;
	//case EDQUOT: 	 		c = "Disk quota exceeded."; break;
	//case ENOMEDIUM: 	 	c = "No medium found."; break;
	//case EMEDIUMTYPE: 	 	c = "Wrong medium type."; break;
	case ECANCELED: 	 	c = "Operation canceled."; break;
	//case ENOKEY: 	 		c = "Required key not available."; break;
	//case EKEYEXPIRED: 	 	c = "Key has expired."; break;
	//case EKEYREVOKED: 	 	c = "Key has been revoked."; break;
	//case EKEYREJECTED: 	 	c = "Key was rejected by service."; break;
	case EOWNERDEAD: 	 	c = "Owner died."; break;
	case ENOTRECOVERABLE: 	c = "State not recoverable."; break;
	//case ERFKILL: 	 		c = "Operation not possible due to RF-kill."; break;
	//case EHWPOISON: 	 	c = "Memory page has hardware error."; break;
	case ENOTSUP: 	 		c = "Not supported parameter or option."; break;
	//case ENOMEDIUM: 	 	c = "Missing media."; break;
	//case EILSEQ: 	 		c = "Invalid multi-byte sequence."; break;
	//case EOVERFLOW: 	 	c = "Value too large."; break;
	//case ECANCELED: 	 	c = "Asynchronous operation stopped before normal completion."; break;
	//case ENOTRECOVERABLE: 	c = "State not recoverable."; break;
	//case EOWNERDEAD: 	 	c = "Previous owner died."; break;
	//case ESTRPIPE: 	 		c = "Streams pipe error ."; break;
	
	// 	   
	// Engine Errors:
	//
	
	case ERROR_GENERIC:		c = "Generic program failure encountered."; break;
	case ERROR_BADPOINTER:	c = "Null pointer was encountered where it should have had a value."; break;
	case ERROR_BADVALUE:	c = "Encountered a nonsensical value."; break;
	case ERROR_IO_FILETOBIG:c = "Encountered error while reading file. File was too big to fit within buffer."; break;
	default:	            c = "Undefined error code.";
	}
	
	printf("%s\n", c);
}
#endif