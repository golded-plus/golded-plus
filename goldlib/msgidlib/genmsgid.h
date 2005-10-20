#ifndef __GENMSGID_H_
#define __GENMSGID_H_

#ifdef __cplusplus
extern "C" {
#endif

uint32_t GenMsgId(char *seqdir, uint32_t max_outrun);
uint32_t GenMsgIdEx(char *seqdir, uint32_t max_outrun, uint32_t (*altGenMsgId)(void), char **errstr);
uint32_t oldGenMsgId(void);

#ifdef __cplusplus
}
#endif

#endif /* __GENMSGID_H_ */
