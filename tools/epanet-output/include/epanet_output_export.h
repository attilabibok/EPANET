
#ifndef DLLEXPORT_H
#define DLLEXPORT_H

#ifdef SHARED_EXPORTS_BUILT_AS_STATIC
#  define DLLEXPORT
#  define EPANET_OUTPUT_NO_EXPORT
#else
#  ifndef DLLEXPORT
#    ifdef epanet_output_EXPORTS
        /* We are building this library */
#      define DLLEXPORT __declspec(dllexport)
#    else
        /* We are using this library */
#      define DLLEXPORT __declspec(dllimport)
#    endif
#  endif

#  ifndef EPANET_OUTPUT_NO_EXPORT
#    define EPANET_OUTPUT_NO_EXPORT 
#  endif
#endif

#ifndef EPANET_OUTPUT_DEPRECATED
#  define EPANET_OUTPUT_DEPRECATED __declspec(deprecated)
#endif

#ifndef EPANET_OUTPUT_DEPRECATED_EXPORT
#  define EPANET_OUTPUT_DEPRECATED_EXPORT DLLEXPORT EPANET_OUTPUT_DEPRECATED
#endif

#ifndef EPANET_OUTPUT_DEPRECATED_NO_EXPORT
#  define EPANET_OUTPUT_DEPRECATED_NO_EXPORT EPANET_OUTPUT_NO_EXPORT EPANET_OUTPUT_DEPRECATED
#endif

#if 0 /* DEFINE_NO_DEPRECATED */
#  ifndef EPANET_OUTPUT_NO_DEPRECATED
#    define EPANET_OUTPUT_NO_DEPRECATED
#  endif
#endif

#endif /* DLLEXPORT_H */
