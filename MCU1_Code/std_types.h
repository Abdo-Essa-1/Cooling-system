#ifndef STD_TYPES_H_
#define STD_TYPES_H_

/*Boolean Datatype*/
typedef unsigned char boolean;

/*Boolean Values*/
#ifndef FALSE
#define FALSE   (0u)
#endif

#ifndef TRUE
#define TRUE (1u)
#endif


#define LOGIC_LOW (0u)
#define LOGIC_HIGH (1u)

#define NULL_PTR ((void *) 0)

typedef unsigned char uint8;
typedef unsigned short uint16;
typedef unsigned int uint32;
typedef unsigned long long uint64;
typedef signed char sint8;
typedef signed short sint16;
typedef signed int sint32;
typedef signed long long sint64;
typedef float float32;
typedef double float64;

#endif /* STD_TYPES_H_ */
