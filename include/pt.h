 #ifndef __PT_H__
 #define __PT_H__

#include "error.h"

/*This pt is a stripdown of Adam Dunkels'
 * the lc state is staticly scoped to the PT_INIT() living scope.
 * same cautions:
 * 			local variables don't preserve across PT_WAIT or PT_YIELD
 * 			user switch structure should not across any PT_* macros
 */
 
/*Return value of a pt thread: 
 *			0 for success; 
 *			negtive for custom errors;
 *			positive but <PT_WAITING for custom uses
 */ 

#define PT_WAITING	126
#define PT_YIELDED	127

#define PT_LC_INITVAL 1

#define CHECKOUT(c,err,out) if(c){ret=err;goto out;}
#define CHECKRES(f,out)	if((ret=(f))<0)goto out

typedef unsigned short lc_t;

#define WRAP(x)		do{ x }while(0)

#define PT(fname,args...) 	char fname(lc_t * _lc, args)

#define PT_V(fname)			char fname(lc_t * _lc)

#define PT_INIT(lc)		lc=PT_LC_INITVAL

#define PT_DECLARE(lc)	lc_t PT_INIT(lc)

#define PT_B()				char ret=SUCCESS;						\
							switch(*_lc){								\
								case PT_LC_INITVAL:

#define PT_WAIT_(condition)			*_lc = __LINE__; case __LINE__:		\
									if(!(condition))return PT_WAITING;	\
									 
#define PT_YIELD_					ret=PT_YIELDED;						\
									*_lc = __LINE__; case __LINE__:		\
									if(ret==PT_YIELDED) return ret;		\
									
#define PT_YWAIT_(condition)		ret=PT_YIELDED;						\
									*_lc = __LINE__; case __LINE__:		\
									if(ret==PT_YIELDED)	return ret;		\
									if(!(condition)) return PT_WAITING;	\
							
#define PT_RESET_					PT_INIT(*_lc); return PT_YIELDED;

#define PT_E(out)					default: 	break;						\
							}											\
							out: PT_INIT(*_lc); return ret;

#define PT_WAIT(condition)	WRAP(PT_WAIT_(condition))
#define PT_YIELD()			WRAP(PT_YIELD_)
#define PT_YWAIT(condition)	WRAP(PT_YWAIT_(condition))
#define PT_RESET			WRAP(PT_RESET_)

#define PT_CALL(name,args...)	WRAP(PT_WAIT_(name(_lc+sizeof(lc_t),args)<PT_WAITING))
#define PT_CALLV(name)			WRAP(PT_WAIT_(name(_lc+sizeof(lc_t))<PT_WAITING))

#define PT_CALL_OUT(out,name, args...)	WRAP(PT_WAIT_((ret=name(_lc+sizeof(lc_t),args))<PT_WAITING);\
												if(ret<0)goto out;)
#define PT_CALLV_OUT(out,name, args...)	WRAP(PT_WAIT_((ret=name(_lc+sizeof(lc_t)))<PT_WAITING);\
												if(ret<0)goto out;)

#define PT_NEST_CALL(name,args...)	return name(_lc,args); 

#define PT_LOCK(lock) 	WRAP(PT_WAIT_(!lock);lock=1;)
#define PT_UNLOCK(lock)	WRAP(lock=0;)

#endif
