#include "baffa1_config.h"
#include "baffa1_utils.h"


void BAFFA1_CONFIG::io_file(const char *dir, const char *reg, BAFFA1_BYTE value) {
	char line[255];
	sprintf(line, "         \t\t\t\t%s REG\t %s \t= %02x\n", dir, reg, value);
	printf("%s", line);

	//FILE *fa
	//save_to_log(NULL, fa, line);
}
