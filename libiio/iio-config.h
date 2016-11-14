#ifndef IIO_CONFIG_H
#define IIO_CONFIG_H

#define LIBIIO_VERSION_MAJOR	0
#define LIBIIO_VERSION_MINOR	7
#define LIBIIO_VERSION_GIT	"775b9f6"

/* #undef WITH_LOCAL_BACKEND */
#define WITH_XML_BACKEND
/* #undef WITH_NETWORK_BACKEND */
/* #undef WITH_USB_BACKEND */
#define WITH_SERIAL_BACKEND

/* #undef WITH_NETWORK_GET_BUFFER */
/* #undef WITH_NETWORK_EVENTFD */
/* #undef HAS_PIPE2 */
/* #undef HAS_STRDUP */
/* #undef HAS_STRERROR_R */
/* #undef HAS_IPV6 */
/* #undef HAVE_AVAHI */
#define NO_THREADS

#endif /* IIO_CONFIG_H */

