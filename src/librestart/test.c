#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

int main()

{
	struct stat buffer;
	int status = xstat("xstat.s", &buffer);
	return status;
}
