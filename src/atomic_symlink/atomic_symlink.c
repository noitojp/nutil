#include <sys/types.h>
#include <linux/limits.h>
#include <errno.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <libgen.h>
#include <stdio.h>

static void usage(char *cmd);

int main(int argc, char **argv)
{
	int rc = EXIT_SUCCESS;
	char tmp_path[PATH_MAX + 1];

	if( argc != 3 ){
		usage(argv[0]);
		rc = EXIT_FAILURE;
		goto finally;
	}

	snprintf(tmp_path,sizeof(tmp_path),"%s.%ld.%d",argv[2],time(NULL),getpid());
	if( symlink(argv[1],tmp_path) < 0 ){
		fprintf(stderr,"ERROR: can't create symlink %s to %s\n",argv[2],argv[1]);
		rc = EXIT_FAILURE;
		goto finally;
	}

	if( rename(tmp_path,argv[2]) < 0 ){
		unlink(tmp_path);
		fprintf(stderr,"ERROR: can't create symlink %s to %s\n",argv[2],argv[1]);
		rc = EXIT_FAILURE;
		goto finally;
	}

 finally:
	return(rc);
}

static void usage(char *cmd)
{
	fprintf(stderr,"usage: %s TARGET LINK_NAME\n",basename(cmd));
}
