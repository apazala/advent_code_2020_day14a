#include <cstdio>
#include <cinttypes>
#include <unordered_map>
#include <cstring>

using namespace std;


int main() {
	const int64_t ALLONES = (1ull << 36) - 1;
	FILE *fp;
	char line[128];
	char *valuestr;
	char *memdirstr;
	char *endptr;
	int64_t memdir;
	int64_t result;
	int64_t value;
	int64_t maskones = 0;
	int64_t maskzeros = ALLONES;
	unordered_map<int64_t, int64_t> memmap;

	fp = fopen("input.txt", "r");

	if (!fp) {
		fprintf(stderr, "Can't open file\n");
		return 1;
	}

	result = 0;
	while (fgets(line, sizeof(line), fp)) {
		valuestr = strchr(line, '=');
		if (!valuestr) break; 

		valuestr++;

		if (strncmp(line, "mask", 4) == 0) {
			//change mask
			while ((*valuestr) < '0')
				valuestr++;


			maskones = 0;
			maskzeros = ALLONES;
			for (int i = 35; i >= 0; i--, valuestr++) {
				//xor to toggle i-th bit
				switch (*valuestr)
				{
				case '0':
					maskzeros ^= (1ull << i);
					break;

				case '1':
					maskones ^= (1ull << i);
					break;
				}
			}
		}
		else if (memdirstr = strchr(line, '[')) {
			memdirstr++;
			memdir = strtol(memdirstr, &endptr, 10);
			if (*endptr == ']') {
				value = strtol(valuestr, &endptr, 10);

				value |= maskones;
				value &= maskzeros;

				result += value;

				auto it = memmap.emplace(memdir, value);
				if (!it.second) {
					//delete old value and update
					result -= it.first->second;
					it.first->second = value;
				}

			}
		}
	}

	printf("Answer: %" PRId64 "\n", result);

	fclose(fp);

	return 0;
}