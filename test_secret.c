#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <sys/syscall.h>

#define __NR_set_secret 472
#define __NR_get_secret 473

int main(void)
{
	const char *msg = "minha-senha-123";
	char out[256] = {0};
	long ret;

	ret = syscall(__NR_set_secret, msg, strlen(msg) + 1);
	printf("set_secret ret = %ld (errno=%d)\n", ret, errno);

	ret = syscall(__NR_get_secret, out, sizeof(out));
	printf("get_secret ret = %ld, segredo = \"%s\"\n", ret, out);

	if (strcmp(msg, out) == 0)
		printf("OK: segredo recuperado corretamente!\n");
	else
		printf("ERRO: segredo diferente\n");

	/* teste de limite: len > 256 deve retornar EINVAL */
	ret = syscall(__NR_set_secret, msg, 300);
	printf("set_secret com len=300: ret=%ld errno=%d (esperado EINVAL=22)\n", ret, errno);

	return 0;
}
