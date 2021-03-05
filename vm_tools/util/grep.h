#ifdef __cplusplus
extern "C" {
#endif
//int	grep(char*, FILE*, char*);
int	match(char*, char*);
int	matchhere(char*, char*);
int	matchstar(int, char*, char*);
char	*get_matchp(void);

#ifdef __cplusplus
}
#endif
