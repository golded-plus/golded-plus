#ifndef __GENMSGID_H_
#define __GENMSGID_H_

#ifdef __cplusplus
extern "C" {
#endif

ulong GenMsgId(char *seqdir, ulong max_outrun);
ulong GenMsgIdEx(char *seqdir, ulong max_outrun, ulong (*altGenMsgId)(void), char **errstr);
ulong oldGenMsgId(void);

#ifdef __cplusplus
}
#endif

#endif /* __GENMSGID_H_ */
